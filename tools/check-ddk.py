#!/usr/bin/env python3
"""Compare the SDK's driver headers with the MASM include files of a DDK
that you have.

    check-ddk.py [--clang CLANG] [--target T] -I INCLUDE HEADER INC...

HEADER is an SDK header (for example gdidefs.h or dibeng.h), and each INC
is a DDK include file (GDIDEFS.INC, DIBENG.INC, VALMODE.INC ...). The
script reads the EQU constants and the STRUC layouts of the INC files,
and compares them with the macros and the structure layouts that clang
sees in HEADER. It prints each difference, and the names that only one
side has. The exit status is 1 if a value or a layout is different.

The DDK is used only for this check. Its files stay where they are, and
the script writes nothing: do not copy DDK files or this script's output
into the repository.
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile

SIZES = {"db": 1, "dw": 2, "dd": 4, "df": 6, "dq": 8, "dt": 10}


def masm_number(tok):
    """Return the value of one MASM number (1234, 0FFh, 101b), or None."""
    t = tok.lower()
    try:
        if re.fullmatch(r"[0-9][0-9a-f]*h", t):
            return int(t[:-1], 16)
        if re.fullmatch(r"[01]+b", t):
            return int(t[:-1], 2)
        if re.fullmatch(r"[0-9]+d?", t):
            return int(t.rstrip("d"))
    except ValueError:
        pass
    return None


def masm_eval(expr, known):
    """Evaluate a MASM constant expression with the names in KNOWN.

    Return None when the expression has a part that is not a number, a
    known name or one of + - * / ( ) shl shr or and xor not."""
    out = []
    for tok in re.findall(r"[A-Za-z_@$?][\w@$?]*|[0-9][0-9A-Fa-f]*[hHbBdD]?|\S", expr):
        low = tok.lower()
        ops = {"shl": "<<", "shr": ">>", "or": "|", "and": "&", "xor": "^", "not": "~"}
        if low in ops:
            out.append(ops[low])
        elif tok in "+-*/()":
            out.append("//" if tok == "/" else tok)
        elif masm_number(tok) is not None:
            out.append(str(masm_number(tok)))
        elif tok in known:
            out.append(str(known[tok]))
        else:
            return None
    try:
        return eval(" ".join(out), {"__builtins__": {}})  # noqa: S307
    except Exception:  # noqa: BLE001
        return None


def read_inc(paths):
    """Read the EQU constants and STRUC layouts of the INC files.

    Return (constants, structs): constants maps a name to its value, and
    structs maps a structure name to (size, [(field, offset)])."""
    consts, structs = {}, {}
    cur = None
    for path in paths:
        with open(path, encoding="latin-1") as f:
            lines = f.read().splitlines()
        for line in lines:
            line = line.split(";", 1)[0].strip()
            if not line:
                continue
            words = line.split()
            if len(words) >= 2 and words[1].lower() == "struc":
                cur = (words[0], [], [0])
                continue
            if cur and len(words) >= 2 and words[0] == cur[0] and words[1].lower() == "ends":
                structs[cur[0]] = (cur[2][0], cur[1])
                cur = None
                continue
            if cur:
                if words[0].lower() in SIZES or words[0] in structs:
                    words = [None] + words  # a field with no name
                if len(words) >= 2:
                    name, kind = words[0], words[1]
                    size = SIZES.get(kind.lower())
                    if size is None and kind in structs:
                        size = structs[kind][0]
                    if size is None:
                        continue
                    rest = " ".join(words[2:])
                    m = re.match(r"(\S+?)\s*dup\b", rest, re.I)
                    count = masm_eval(m.group(1), consts) if m else 1
                    if name:
                        cur[1].append((name, cur[2][0]))
                    cur[2][0] += size * (count or 1)
                continue
            m = re.match(r"([A-Za-z_@$?][\w@$?]*)\s+(?:equ|=)\s+(.+)$", line, re.I)
            if m:
                v = masm_eval(m.group(2), consts)
                if v is not None:
                    consts[m.group(1)] = v
    return consts, structs


def run_clang(args, clang, target, includes, source):
    """Run clang on SOURCE (a string) and return its standard output."""
    with tempfile.TemporaryDirectory() as d:
        src = os.path.join(d, "check.c")
        with open(src, "w") as f:
            f.write(source)
        cmd = [clang, "--target=" + target] + ["-I" + i for i in includes] + args + [src]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode:
            sys.exit(r.stderr)
        return r.stdout


def header_macros(clang, target, includes, header, names):
    """Return the values of the macros NAMES that HEADER defines."""
    src = "#include <%s>\n" % header
    src += "".join('"%s" = %s\n' % (n, n) for n in names)
    out = run_clang(["-E", "-P"], clang, target, includes, src)
    vals = {}
    for line in out.splitlines():
        m = re.match(r'"([^"]+)" = (.*)$', line)
        if not m or m.group(2) == m.group(1):
            continue
        expr = re.sub(r"\(\s*(?:unsigned\s+)?(?:WORD|DWORD|UINT|LONG|BYTE|int|long|short|char)\s*\)", "", m.group(2))
        expr = re.sub(r"(?<=[0-9a-fA-F])[uUlL]+\b", "", expr)
        try:
            vals[m.group(1)] = eval(expr, {"__builtins__": {}})  # noqa: S307
        except Exception:  # noqa: BLE001
            pass
    return vals


def header_layouts(clang, target, includes, header, names):
    """Return {type: (size, {field: offset})} for the types NAMES of HEADER."""
    src = "#include <%s>\n" % header
    src += "".join("int __sz_%s = sizeof(%s);\n" % (n, n) for n in names)
    out = run_clang(["-fsyntax-only", "-Xclang", "-fdump-record-layouts-simple"],
                    clang, target, includes, src)
    layouts, cur = {}, None
    for line in out.splitlines():
        m = re.match(r"Type: struct (\w+)", line)
        if m:
            cur = {"fields": {}, "size": None, "tag": m.group(1)}
            layouts[m.group(1)] = cur
        m = re.match(r"\s*Size:(\d+)", line)
        if m and cur:
            cur["size"] = int(m.group(1)) // 8
        if cur and "FieldOffsets: [" in line:
            cur["offsets"] = [int(x) // 8 for x in re.findall(r"\d+", line.split("[", 1)[1])]
    return layouts


def field_names(clang, target, includes, header, tag):
    """Return the field names of struct TAG in declaration order."""
    src = "#include <%s>\n" % header
    out = run_clang(["-fsyntax-only", "-Xclang", "-ast-dump", "-Xclang",
                     "-ast-dump-filter=" + tag], clang, target, includes, src)
    return re.findall(r"FieldDecl .*? (\w+) '", out)


def main():
    """Read the arguments, compare the two sides and print the result."""
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--clang", default="clang")
    ap.add_argument("--target", default="i8086-pc-win164.00")
    ap.add_argument("-I", dest="includes", action="append", default=[])
    ap.add_argument("header")
    ap.add_argument("inc", nargs="+")
    a = ap.parse_args()

    consts, structs = read_inc(a.inc)
    bad = 0
    vals = header_macros(a.clang, a.target, a.includes, a.header, sorted(consts))
    for name in sorted(consts):
        if name not in vals:
            continue
        if (vals[name] & 0xFFFFFFFF) != (consts[name] & 0xFFFFFFFF):
            print("value %s: SDK %#x, DDK %#x" % (name, vals[name], consts[name]))
            bad = 1
    print("%d constants compared" % len([n for n in consts if n in vals]))

    for sname, (size, fields) in sorted(structs.items()):
        src_names = None
        for cand in (sname, "tag" + sname):
            try:
                lay = header_layouts(a.clang, a.target, a.includes, a.header, [cand])
            except SystemExit:
                continue
            if lay:
                lay = list(lay.values())[-1]  # the outermost type comes last
                src_names = field_names(a.clang, a.target, a.includes, a.header, lay["tag"])
                break
        if not src_names:
            print("struct %s: not in the SDK header" % sname)
            continue
        offs = dict(zip(src_names, lay.get("offsets", [])))
        if lay["size"] != size:
            print("size %s: SDK %d, DDK %d" % (sname, lay["size"], size))
            bad = 1
        for fname, off in fields:
            if fname in offs and offs[fname] != off:
                print("offset %s.%s: SDK %d, DDK %d" % (sname, fname, offs[fname], off))
                bad = 1
        print("struct %s: %d of %d fields compared" % (sname, len([f for f, _ in fields if f in offs]), len(fields)))
    sys.exit(bad)


if __name__ == "__main__":
    main()
