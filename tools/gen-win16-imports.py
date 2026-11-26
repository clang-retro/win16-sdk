#!/usr/bin/env python3
"""Write the linker import directives for the Windows 3.1 API.

Reads Wine's .spec files for the 16-bit system DLLs (dlls/<dll>/<dll>.spec
in the Wine source tree, which lists every entry point with its ordinal)
and prints one wlink directive per entry:

    import MESSAGEBOX USER.1

Pascal functions are named in upper case, as clang names __pascal
functions, and so are the stubs (functions Wine does not implement, which
are Pascal functions in the Windows API). C functions (varargs, cdecl)
get the C underscore (_wsprintf).

    gen-win16-imports.py <wine-src>/dlls > lib/windows.lnk

The lists for older Windows versions come from the import library of
that version's SDK instead (SLIBW.LIB in the Windows 1.x and 2.x SDKs,
LIBW.LIB in the 3.0 SDK): its import definitions give exactly the
functions, and the ordinals, that version has. The names are spelled as
for Wine's entries (_wsprintf for the C function wsprintf); the others are
Pascal functions, in upper case.

    gen-win16-imports.py --from-lib "Windows 2.03 SDK/LIB/SLIBW.LIB" \
        <wine-src>/dlls > lib/windows200.lnk
"""
import argparse
import os
import re
import struct
import sys

# The import module name and Wine's spec file for each DLL.
MODULES = [
    ("KERNEL", "krnl386.exe16/krnl386.exe16.spec"),
    ("USER", "user.exe16/user.exe16.spec"),
    ("GDI", "gdi.exe16/gdi.exe16.spec"),
    ("KEYBOARD", "keyboard.drv16/keyboard.drv16.spec"),
    ("SOUND", "sound.drv16/sound.drv16.spec"),
    ("SHELL", "shell.dll16/shell.dll16.spec"),
    ("COMMDLG", "commdlg.dll16/commdlg.dll16.spec"),
    ("TOOLHELP", "toolhelp.dll16/toolhelp.dll16.spec"),
    ("MMSYSTEM", "mmsystem.dll16/mmsystem.dll16.spec"),
    ("VER", "ver.dll16/ver.dll16.spec"),
    ("LZEXPAND", "lzexpand.dll16/lzexpand.dll16.spec"),
    ("DDEML", "ddeml.dll16/ddeml.dll16.spec"),
    ("STRESS", "stress.dll16/stress.dll16.spec"),
    ("WINSOCK", "winsock.dll16/winsock.dll16.spec"),
    # OLE 1.0 (Windows 3.1), OLE 2.0, Video for Windows and WinG: DLLs that
    # come with Windows 3.1 or that programs install with them.
    ("OLECLI", "olecli.dll16/olecli.dll16.spec"),
    ("OLESVR", "olesvr.dll16/olesvr.dll16.spec"),
    ("COMPOBJ", "compobj.dll16/compobj.dll16.spec"),
    ("STORAGE", "storage.dll16/storage.dll16.spec"),
    ("OLE2", "ole2.dll16/ole2.dll16.spec"),
    ("OLE2DISP", "ole2disp.dll16/ole2disp.dll16.spec"),
    ("OLE2NLS", "ole2nls.dll16/ole2nls.dll16.spec"),
    ("TYPELIB", "typelib.dll16/typelib.dll16.spec"),
    ("MSVIDEO", "msvideo.dll16/msvideo.dll16.spec"),
    ("AVIFILE", "avifile.dll16/avifile.dll16.spec"),
    ("WING", "wing.dll16/wing.dll16.spec"),
]

# Modules whose Wine spec has entries of Wine's own from ordinal 500 up
# (the methods of its objects).
WINE_METHODS_FROM_500 = {"COMPOBJ", "STORAGE", "OLE2"}

# Wine's name of an entry where it is not the name that Windows exports.
RENAMED = {
    ("STORAGE", "STGCREATEDOCFILEA"): "STGCREATEDOCFILE",
    ("AVIFILE", "_AVISAVE"): "_AVISave",
    # OLE 2 functions with the names of OLE 1 functions (OLECLI). ole2.h
    # gives them these names with an assembler label.
    ("OLE2", "OLECREATE"): "OLE2_OLECREATE",
    ("OLE2", "OLECREATEFROMFILE"): "OLE2_OLECREATEFROMFILE",
    ("OLE2", "OLEDRAW"): "OLE2_OLEDRAW",
    ("OLE2", "OLESAVETOSTREAM"): "OLE2_OLESAVETOSTREAM",
    ("OLE2", "OLELOADFROMSTREAM"): "OLE2_OLELOADFROMSTREAM",
}

# Entry points of the OLE, video and WinG DLLs that each DLL defines for
# itself, which a DLL must not import from another.
OWN_ENTRIES = {"LIBMAIN", "DLLENTRYPOINT", "DLLGETCLASSOBJECT",
               "DLLCANUNLOADNOW"}
NEW_MODULES = {"OLECLI", "OLESVR", "COMPOBJ", "STORAGE", "OLE2", "OLE2DISP",
               "OLE2NLS", "TYPELIB", "MSVIDEO", "AVIFILE", "WING"}

# DLLs that Wine has no 16-bit spec for. Their functions are imported by
# name (Pascal functions export their names in upper case), which needs no
# ordinal: Simple MAPI (MAPI.DLL, Windows for Workgroups and Microsoft
# Mail) and ODBC 2.0 (ODBC.DLL).
BY_NAME = [
    ("MAPI", ["MAPILogon", "MAPILogoff", "MAPISendMail", "MAPISendDocuments",
              "MAPIFindNext", "MAPIReadMail", "MAPISaveMail",
              "MAPIDeleteMail", "MAPIFreeBuffer", "MAPIAddress",
              "MAPIDetails", "MAPIResolveName"]),
    ("ODBC", ["SQLAllocConnect", "SQLAllocEnv", "SQLAllocStmt",
              "SQLBindCol", "SQLCancel", "SQLColAttributes", "SQLConnect",
              "SQLDescribeCol", "SQLDisconnect", "SQLError",
              "SQLExecDirect", "SQLExecute", "SQLFetch", "SQLFreeConnect",
              "SQLFreeEnv", "SQLFreeStmt", "SQLGetCursorName",
              "SQLNumResultCols", "SQLPrepare", "SQLRowCount",
              "SQLSetCursorName", "SQLSetParam", "SQLTransact",
              "SQLColumns", "SQLDriverConnect", "SQLGetConnectOption",
              "SQLGetData", "SQLGetFunctions", "SQLGetInfo",
              "SQLGetStmtOption", "SQLGetTypeInfo", "SQLParamData",
              "SQLPutData", "SQLSetConnectOption", "SQLSetStmtOption",
              "SQLSpecialColumns", "SQLStatistics", "SQLTables",
              "SQLBrowseConnect", "SQLColumnPrivileges", "SQLDataSources",
              "SQLDescribeParam", "SQLExtendedFetch", "SQLForeignKeys",
              "SQLMoreResults", "SQLNativeSql", "SQLNumParams",
              "SQLParamOptions", "SQLPrimaryKeys", "SQLProcedureColumns",
              "SQLProcedures", "SQLSetPos", "SQLSetScrollOptions",
              "SQLTablePrivileges", "SQLDrivers", "SQLBindParameter"]),
]

# DLLs that Wine has no 16-bit spec for, with the ordinals of the import
# library of their SDK: Pen Windows (PENWIN.DLL), from PENWIN.LIB of the
# Windows 3.1 SDK (Pen Windows 1.0) and of the Win32 SDK's 16-bit
# libraries (Pen Services 2.0). The two versions give each function the
# same ordinal, except RemovePenDataStrokes, which is in EXACT_NAME.
BY_ORDINAL = [
    ("PENWIN", [
        ("HeditWndProc", 5), ("InitRC", 10), ("Recognize", 11),
        ("RecognizeData", 12), ("BoundingRectFromPoints", 13),
        ("InstallRecognizer", 14), ("UninstallRecognizer", 15),
        ("TrainInk", 16), ("TrainContext", 17), ("ResultsFunction", 100),
        ("PostVirtualMouseEvent", 101), ("PostVirtualKeyEvent", 102),
        ("AtomicVirtualEvent", 104), ("GetPenAppFlags", 109),
        ("IsPenAware", 110), ("RegisterPenApp", 111),
        ("FRegisterVirtualKeyBoard", 112), ("SetPenAppFlags", 112),
        ("DWMouseHook", 113), ("SetRecogHook", 114), ("SetPenHook", 115),
        ("EditSubWndProc", 116), ("GetSymbolMaxLength", 121),
        ("GetSymbolCount", 122), ("FirstSymbolFromGraph", 123),
        ("EnumSymbols", 124), ("SymbolToCharacter", 125),
        ("CharacterToSymbol", 126), ("DPtoTP", 131), ("TPtoDP", 132),
        ("IsPenEvent", 135), ("EndPenCollection", 137),
        ("GetPenHwData", 138), ("GetPenHwEventData", 139),
        ("GetPenAsyncState", 144), ("SetGlobalRC", 150),
        ("GetGlobalRC", 151), ("SetInkProperties", 160),
        ("GetInkProperties", 161), ("EnableInking", 162),
        ("UpdateInking", 163), ("ProcessWriting", 170),
        ("PenInputWndProc", 171), ("CorrectWriting", 172),
        ("EmulatePen", 173), ("CorrectionDlgProc", 175),
        ("PictureWndProc", 176), ("DwWrKbdHook", 178),
        ("ArrowTimerProc", 181), ("ArrowControlProc", 182),
        ("RecSetPenEndEvents", 190), ("FSetPenCollectionMode", 191),
        ("ListenWndProc", 192), ("SaveScreen", 193), ("RestoreScreen", 194),
        ("AddPenEvent", 195), ("ProcessPenEvent", 196), ("SystemEntry", 197),
        ("SaveMEI", 198), ("GetMEI", 199), ("UpdatePenInfo", 207),
        ("CreatePenDataEx", 208), ("DestroyPenData", 209),
        ("CreatePenData", 210), ("GetPenDataInfo", 211),
        ("AddPointsPenData", 212), ("BeginEnumStrokes", 213),
        ("DrawPenData", 214), ("MetricScalePenData", 215),
        ("OffsetPenData", 216), ("EndEnumStrokes", 217),
        ("DuplicatePenData", 218), ("GetPenDataStroke", 219),
        ("StandardYieldFunction", 220), ("GetPenDataAttributes", 220),
        ("GetPointsFromPendata", 221), ("GetPointsFromPenData", 221),
        ("ResizePenData", 222), ("CompactPenData", 223),
        ("InsertPenData", 224), ("GetStrokeAttributes", 225),
        ("SetStrokeAttributes", 226), ("GetStrokeTableAttributes", 227),
        ("SetStrokeTableAttributes", 228), ("DrawPenDataEx", 229),
        ("InsertPenDataPoints", 234), ("InsertPenDataStroke", 235),
        ("ExtractPenDataPoints", 236), ("HitTestPenData", 238),
        ("CompressPenData", 240), ("TrimPenData", 241),
        ("RedisplayPenData", 242), ("ExtractPenDataStrokes", 243),
        ("PenDataFromBuffer", 247), ("PenDataToBuffer", 248),
        ("ShowKeyboard", 250), ("ECPenWndProc", 300), ("ECDrawInk", 301),
        ("ECDrawRule", 302), ("NotifyHHSystemChanged", 303),
        ("HwndCreateDinkie", 310), ("FDestroyDinkie", 311),
        ("FModifyDinkie", 312), ("DefPenWindowProc", 350),
        ("DoDefaultPenInput", 351), ("DriverProc", 401),
        ("GetVersionPenWin", 402), ("ShowGestureEditor", 412),
        ("GeaDlgProc", 413), ("PropHook", 414), ("ExecuteGesture", 418),
        ("DictionarySearch", 420), ("GetPenMiscInfo", 421),
        ("SetPenMiscInfo", 422), ("BoxEditWndProc", 501),
        ("EnumFontProc", 502), ("CreatePenDataRegion", 581),
        ("StartPenInput", 600), ("GetPenInput", 601), ("PeekPenInput", 602),
        ("StopPenInput", 603), ("StartInking", 610), ("StopInking", 611),
        ("TargetPoints", 625), ("AddInksetInterval", 630),
        ("CreateInkset", 631), ("DestroyInkset", 632),
        ("GetInksetInterval", 633), ("GetInksetIntervalCount", 634),
        ("CreateCompatibleHRC", 1000), ("DestroyHRC", 1001),
        ("ProcessHRC", 1002), ("GetHRECFromHRC", 1003),
        ("AddPenInputHRC", 1010), ("EndPenInputHRC", 1011),
        ("AddPenDataHRC", 1012), ("CreatePenDataHRC", 1013),
        ("SetAlphabetHRC", 1020), ("GetAlphabetHRC", 1021),
        ("SetAlphabetPriorityHRC", 1022), ("GetAlphabetPriorityHRC", 1023),
        ("EnableGestureSetHRC", 1024), ("EnableSystemDictionaryHRC", 1026),
        ("SetMaxResultsHRC", 1030), ("GetMaxResultsHRC", 1031),
        ("SetInternationalHRC", 1032), ("GetInternationalHRC", 1033),
        ("SetGuideHRC", 1040), ("GetGuideHRC", 1041),
        ("SetBoxAlphabetHRC", 1045), ("SetWordlistHRC", 1046),
        ("GetWordlistHRC", 1047), ("SetWordlistCoercionHRC", 1048),
        ("GetWordlistCoercionHRC", 1049), ("GetResultsHRC", 1050),
        ("GetBoxResultsHRC", 1051), ("DestroyHRCRESULT", 1062),
        ("GetSymbolsHRCRESULT", 1063), ("GetSymbolCountHRCRESULT", 1064),
        ("GetBoxMappingHRCRESULT", 1065), ("GetHotspotsHRCRESULT", 1066),
        ("CreateInksetHRCRESULT", 1068),
        ("GetAlternateWordsHRCRESULT", 1069), ("CreateHWL", 1070),
        ("DestroyHWL", 1071), ("AddWordsHWL", 1072), ("ReadHWL", 1073),
        ("WriteHWL", 1074), ("ConfigHREC", 1080),
        ("SetResultsHookHREC", 1081), ("UnhookResultsHookHREC", 1082),
        ("TrainHREC", 1083), ("FT_PWFTHKTHKCONNECTIONDATA", 2000),
        ("PWCTHKCONNECTIONDATASL", 2001), ("IMTDispatchProcIE", 2101),
        ("IMTDispatchProcPM", 2102)]),
]

# Functions imported by name, with the name spelled as the DLL exports
# it: the conversions of the Win32s Universal Thunk (WIN32S16.DLL, from
# W32SUT16.LIB of the Win32 SDK), and RemovePenDataStrokes, which is
# ordinal 243 in Pen Windows 1.0 and 237 in Pen Services 2.0.
EXACT_NAME = [
    ("PENWIN", ["REMOVEPENDATASTROKES"]),
    ("WIN32S16", ["UTLinearToSelectorOffset", "UTSelectorOffsetToLinear"]),
]

ENTRY = re.compile(r"^\s*(\d+)\s+(\w+)((?:\s+-[\w=]+)*)\s+([\w@?$]+)")


def lib_imports(path):
    """(name, module, ordinal) for the OMF import definitions (COMENT class
    0xA0, subtype 1) of a library, by ordinal only."""
    data = open(path, "rb").read()
    out = []
    i = page = 0
    while i + 3 <= len(data):
        kind = data[i]
        (n,) = struct.unpack_from("<H", data, i + 1)
        rec = data[i + 3:i + 3 + n]
        if kind == 0xF0 and i == 0:  # library header: the page size
            page = n + 3
            i = page
            continue
        if kind == 0xF1:  # library end
            break
        if kind == 0x88 and rec[1:3] == b"\xa0\x01" and rec[3]:
            p = 4
            name = rec[p + 1:p + 1 + rec[p]].decode("latin-1")
            p += 1 + rec[p]
            module = rec[p + 1:p + 1 + rec[p]].decode("latin-1")
            p += 1 + rec[p]
            (ordinal,) = struct.unpack_from("<H", rec, p)
            out.append((name, module.upper(), ordinal))
        i += 3 + n
        if kind in (0x8A, 0x8B) and page:  # module end: next page
            i = (i + page - 1) // page * page
    return out


def from_lib(dlls, libs):
    # Wine's spelling of each symbol, by its upper-case form.
    spelling = {}
    for module, spec in MODULES:
        path = os.path.join(dlls, spec)
        if os.path.exists(path):
            for sym in spec_symbols(path):
                spelling[sym.upper()] = sym
    print("# Imports of the 16-bit Windows API, generated by "
          "tools/gen-win16-imports.py")
    print("# from the import library of the SDK: "
          + ", ".join(os.path.basename(l) for l in libs)
          + ". The linker keeps only")
    print("# the imports a program uses.")
    imports = []
    for lib in libs:
        imports += lib_imports(lib)
    order = [m for m, _ in MODULES]
    imports.sort(key=lambda e: (order.index(e[1]) if e[1] in order
                                else len(order), e[1], e[2]))
    seen = set()
    module = None
    for name, m, ordinal in imports:
        sym = spelling.get(name.upper(), name.upper())
        if sym in seen or sym == "WEP":
            continue
        seen.add(sym)
        if m != module:
            module = m
            print(f"# {module}")
        print(f"import {sym} {m}.{ordinal}")


def spec_symbols(path):
    for line in open(path):
        m = ENTRY.match(line)
        if not m:
            continue
        _, kind, _, name = m.groups()
        if kind == "pascal":
            yield name.upper()
        elif kind in ("varargs", "cdecl"):
            yield name if name.startswith("_") else "_" + name
        elif kind in ("equate", "variable", "extern"):
            yield name


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--from-lib", metavar="LIB", action="append",
                    help="take the imports of an SDK import library")
    ap.add_argument("dlls", help="Wine's dlls directory")
    args = ap.parse_args()
    dlls = args.dlls
    if args.from_lib:
        return from_lib(dlls, args.from_lib)
    print("# Imports of the 16-bit Windows API, generated by "
          "tools/gen-win16-imports.py")
    print("# from Wine's .spec files. The linker keeps only the imports a "
          "program uses.")
    entries = []
    for module, spec in MODULES:
        path = os.path.join(dlls, spec)
        if not os.path.exists(path):
            continue
        for line in open(path):
            m = ENTRY.match(line)
            if not m:
                continue
            ordinal, kind, _flags, name = m.groups()
            # Wine's private entries; in KERNEL, USER and GDI every
            # ordinal from 1000 up is Wine's own (MMSYSTEM and WINSOCK
            # have real ones there, such as mmioOpen at 1210).
            if name.startswith("__wine"):
                continue
            if int(ordinal) >= 1000 and module in ("KERNEL", "USER", "GDI"):
                continue
            if int(ordinal) >= 500 and module in WINE_METHODS_FROM_500:
                continue
            # C++ members of Microsoft's own classes, and Wine's second
            # names for one entry (in the OLE DLLs).
            if "?" in name or (name.endswith("_dup") and
                               module in WINE_METHODS_FROM_500):
                continue
            # Wine has no code for a stub, but Windows has the function:
            # a program that calls one loads, and works on Windows.
            if kind in ("pascal", "stub"):
                sym = name.upper()
            elif kind in ("varargs", "cdecl"):
                sym = name if name.startswith("_") else "_" + name
            elif kind in ("equate", "variable", "extern"):
                sym = name
            else:
                continue
            sym = RENAMED.get((module, sym.upper()), sym)
            # Every DLL has its own WEP, which its startup code exports.
            if sym != "WEP" and not (module in NEW_MODULES and
                                     sym in OWN_ENTRIES):
                entries.append((module, sym, ordinal))
    # A function that KERNEL has too came there later (the registry
    # functions moved from SHELL to KERNEL in Windows 95): import it from
    # the other module, where Windows 3.1 has it.
    others = {sym for module, sym, _ in entries if module != "KERNEL"}
    seen = set()
    module = None
    for m, sym, ordinal in entries:
        if sym in seen or (m == "KERNEL" and sym in others):
            continue
        seen.add(sym)
        if m != module:
            module = m
            print(f"# {module}")
        print(f"import {sym} {m}.{ordinal}")
    for m, names in BY_NAME:
        print(f"# {m} (by name)")
        for name in names:
            print(f"import {name.upper()} {m}.{name.upper()}")
    for m, entries in BY_ORDINAL:
        print(f"# {m}")
        for name, ordinal in entries:
            # A driver defines its own DriverProc, which PENWIN.DLL (a
            # driver too) exports.
            if name.upper() not in seen and name != "DriverProc":
                seen.add(name.upper())
                print(f"import {name.upper()} {m}.{ordinal}")
    for m, names in EXACT_NAME:
        print(f"# {m} (by name)")
        for name in names:
            print(f"import {name.upper()} {m}.{name}")

if __name__ == "__main__":
    main()
