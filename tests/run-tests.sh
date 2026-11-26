#!/bin/sh
# Build the 16-bit Windows tests for each memory model and optimization
# level, run them in Wine and report which fail. Wine runs a 16-bit program
# with the exit code 0 whatever it returns, so the tests print ALL PASSED.
#
#   BIN      directory with clang
#   SYSROOT  DOS sysroot with the Windows libraries (build-sysroot.sh)
#   LD       wlink or JWlink (default: found by clang on its own path)
#   WINE     wine (default: wine on PATH); WINEPREFIX as usual
#   MODELS   memory models (default: small medium compact large)
#   OPTS     optimization levels (default: O0 Os O2)
#   NO_CXX   set to skip the C++ test
#   NO_GUI   set to skip the test with a window, which needs a display
#            (DISPLAY, or xvfb-run on PATH)
#   VERSIONS Windows versions for the versioned triples, as in
#            i8086-pc-win162.03 (default: 1.01 2.03 3.00 3.10). Their
#            programs are checked with tools/ne-check.py: real-mode safe
#            before 3.10, protected mode only from 3.10. Wine runs them
#            reporting the closest Windows version it has (2.0, 3.0, 3.1);
#            it runs every program in protected mode.
: "${BIN:?set BIN}" "${SYSROOT:?set SYSROOT}"
WINE=${WINE:-wine}
WINEDEBUG=${WINEDEBUG:--all}
export WINEDEBUG
tests=$(cd "$(dirname "$0")" && pwd)/win16
scripts=$(cd "$(dirname "$0")/../tools" && pwd)
# Some tests are shared with the DOS ones in clang-dos/dos-sdk.
DOS_SDK=${DOS_SDK:-$tests/../../../dos-sdk}
[ -f "$DOS_SDK/tests/libc/nearfar.c" ] ||
  { echo "set DOS_SDK to a clang-dos/dos-sdk checkout" >&2; exit 1; }
dostests=$(cd "$DOS_SDK/tests" && pwd)
out=${WORK:-$PWD/build-win16-tests}
mkdir -p "$out"
cd "$out" || exit 1
ldflag=${LD:+--ld-path=$LD}
pass=0
fail=0

# Wine keeps the display its server started with, so with no display the
# whole run goes under xvfb-run.
if [ -z "$NO_GUI" ] && [ -z "$DISPLAY" ] && [ -z "$WIN16_TESTS_XVFB" ] &&
   command -v xvfb-run >/dev/null 2>&1; then
  # A Wine server left from a run without a display would be reused.
  wineserver -k 2>/dev/null
  WIN16_TESTS_XVFB=1 exec xvfb-run -a sh "$tests/../run-tests.sh" "$@"
fi
gui=
[ -n "$NO_GUI" ] || [ -z "$DISPLAY" ] || gui=env

run() {
  # run NAME COMMAND...: the output of a test, in NAME.out
  name=$1
  shift
  timeout 60 "$@" >"$name.out" 2>&1
  pattern="ALL PASSED"
  if grep -q -- "$pattern" "$name.out"; then
    pass=$((pass + 1))
  else
    fail=$((fail + 1))
    echo "FAIL $name"
    sed 's/^/    /' "$name.out" | head -20
  fi
}

build() {
  # build NAME COMMAND...
  name=$1
  shift
  if "$@" >"$name.log" 2>&1; then
    return 0
  fi
  fail=$((fail + 1))
  echo "FAIL $name (build)"
  sed 's/^/    /' "$name.log" | head -20
  return 1
}

for m in ${MODELS:-small medium compact large}; do
  for o in ${OPTS:-O0 Os O2}; do
    cc="$BIN/clang --target=i8086-pc-win16 -mcmodel=$m --sysroot=$SYSROOT $ldflag -$o"
    t=$m-$o
    build "hello-$t" $cc "$tests/hello.c" -o "hello-$t.exe" &&
      run "hello-$t" "$WINE" "hello-$t.exe" one two
    build "api-$t" $cc "$tests/api.c" -o "api-$t.exe" &&
      run "api-$t" "$WINE" "api-$t.exe" abc
    # The sizes of the structures in the headers (checked when it builds).
    build "sizes-$t" $cc "$tests/sizes.c" -o "sizes-$t.exe" &&
      run "sizes-$t" "$WINE" "sizes-$t.exe"
    # OLE 2: the task allocator, GUIDs, BSTRs and VARIANTs.
    build "ole-$t" $cc "$tests/ole.c" -o "ole-$t.exe" &&
      run "ole-$t" "$WINE" "ole-$t.exe"
    # OLE 2 from C++: interfaces as classes (far code and data only),
    # REFIID by reference, and the VARIANT union without a name.
    case $m in
      large | huge)
        build "olecpp-$t" $cc -x c++ "$tests/olecpp.cpp" -x none -o "olecpp-$t.exe" &&
          run "olecpp-$t" "$WINE" "olecpp-$t.exe"
        ;;
    esac
    # windowsx.h: the message crackers and forwarders, with and without
    # STRICT.
    build "windowsx-$t" $cc "$tests/windowsx.c" -o "windowsx-$t.exe" &&
      run "windowsx-$t" "$WINE" "windowsx-$t.exe"
    build "windowsxs-$t" $cc -DSTRICT "$tests/windowsx.c" -o "windowsxs-$t.exe" &&
      run "windowsxs-$t" "$WINE" "windowsxs-$t.exe"
    # The near and far heaps and string functions (GlobalAlloc and the
    # local heap), as C and as C++.
    build "nearfar-$t" $cc "$dostests/libc/nearfar.c" -o "nearfar-$t.exe" &&
      run "nearfar-$t" "$WINE" "nearfar-$t.exe"
    if [ -z "$NO_CXX" ] && [ -f "$SYSROOT/lib/win16/$m/libc++.a" ]; then
      build "nearfarxx-$t" "$BIN/clang++" --target=i8086-pc-win16 -mcmodel=$m \
          --sysroot="$SYSROOT" $ldflag -$o -x c++ "$dostests/libc/nearfar.c" \
          -o "nearfarxx-$t.exe" &&
        run "nearfarxx-$t" "$WINE" "nearfarxx-$t.exe"
    fi
    # getentropy and arc4random; two runs must get different seeds.
    if build "random-$t" $cc "$dostests/libc/random.c" -o "random-$t.exe"; then
      run "random-$t" "$WINE" "random-$t.exe"
      mv "random-$t.out" "random-$t.out1"
      run "random-$t" "$WINE" "random-$t.exe"
      if grep seed "random-$t.out1" >"random-$t.s1" &&
         grep seed "random-$t.out" >"random-$t.s2" &&
         ! cmp -s "random-$t.s1" "random-$t.s2"; then
        pass=$((pass + 1))
      else
        fail=$((fail + 1))
        echo "FAIL random-$t (the same seed twice)"
      fi
    fi
    if [ -z "$NO_CXX" ] && [ -f "$SYSROOT/lib/win16/$m/eh/libc++.a" ]; then
      cxx="$BIN/clang++ --target=i8086-pc-win16 -mcmodel=$m --sysroot=$SYSROOT $ldflag -$o"
      build "except-$t" $cxx -fexceptions "$dostests/cxx/except.cpp" -o "except-$t.exe" &&
        run "except-$t" "$WINE" "except-$t.exe"
    fi
    # A DLL, and a program that imports it. The DLL file name is its module
    # name, so it is the same for every build; each build has a directory.
    mkdir -p "dll-$t"
    if build "testdll-$t" $cc -shared "$tests/testdll.c" -o "dll-$t/TESTDLL.DLL" &&
       python3 "$scripts/win16-imports.py" "dll-$t/TESTDLL.DLL" >"dll-$t/testdll.lnk" &&
       build "dllapp-$t" $cc "$tests/dllapp.c" "-Wl,@dll-$t/testdll.lnk" -o "dll-$t/dllapp.exe"; then
      run "dll-$t" sh -c "cd dll-$t && '$WINE' dllapp.exe"
    fi
    # An installable driver (a DLL named .DRV that exports DriverProc).
    mkdir -p "drv-$t"
    if build "hellodrv-$t" $cc -shared "$tests/hellodrv.c" -o "drv-$t/HELLODRV.DRV" &&
       build "drvapp-$t" $cc "$tests/drvapp.c" -o "drv-$t/drvapp.exe"; then
      run "drv-$t" sh -c "cd drv-$t && '$WINE' drvapp.exe"
    fi
    # A C++ DLL with objects on its stack. Unoptimized, its libc++ code
    # does not fit in the one code segment of the compact model.
    if [ -z "$NO_CXX" ] && [ -f "$SYSROOT/lib/win16/$m/libc++.a" ] &&
       ! { [ "$o" = O0 ] && [ "$m" = compact ]; }; then
      mkdir -p "cxxdll-$t"
      cxx="$BIN/clang++ --target=i8086-pc-win16 -mcmodel=$m --sysroot=$SYSROOT $ldflag -$o"
      if build "cxxdll-$t" $cxx -shared "$tests/cxxdll.cpp" -o "cxxdll-$t/CXXDLL.DLL" &&
         python3 "$scripts/win16-imports.py" "cxxdll-$t/CXXDLL.DLL" >"cxxdll-$t/cxxdll.lnk" &&
         build "cxxdllapp-$t" $cc "$tests/cxxdllapp.c" "-Wl,@cxxdll-$t/cxxdll.lnk" -o "cxxdll-$t/app.exe"; then
        run "cxxdll-$t" sh -c "cd cxxdll-$t && '$WINE' app.exe"
      fi
    fi
    # C++ exceptions thrown and caught inside a DLL. Near-model DLLs use
    # the libraries of <model>/dll/eh, the others those of <model>/eh.
    ehlib=$SYSROOT/lib/win16/$m/eh/libc++.a
    case $m in small|medium) ehlib=$SYSROOT/lib/win16/$m/dll/eh/libc++.a ;; esac
    if [ -z "$NO_CXX" ] && [ -f "$ehlib" ]; then
      mkdir -p "ehdll-$t"
      cxx="$BIN/clang++ --target=i8086-pc-win16 -mcmodel=$m --sysroot=$SYSROOT $ldflag -$o"
      if build "ehdll-$t" $cxx -shared -fexceptions "$tests/ehdll.cpp" -o "ehdll-$t/EHDLL.DLL" &&
         python3 "$scripts/win16-imports.py" "ehdll-$t/EHDLL.DLL" >"ehdll-$t/ehdll.lnk" &&
         build "ehdllapp-$t" $cc "$tests/ehdllapp.c" "-Wl,@ehdll-$t/ehdll.lnk" -o "ehdll-$t/app.exe"; then
        run "ehdll-$t" sh -c "cd ehdll-$t && '$WINE' app.exe"
      fi
    fi
    # Resources, compiled by llvm-rc.
    if build "rc-$t" "$BIN/llvm-rc" -win16 -I "$SYSROOT/include" -I "$tests" \
         -FO "res-$t.res" "$tests/res.rc" &&
       build "res-$t" $cc -I "$tests" "$tests/res.c" "res-$t.res" -o "res-$t.exe"; then
      run "res-$t" $gui "$WINE" "res-$t.exe"
    fi
    if [ -n "$gui" ]; then
      build "gui-$t" $cc "$tests/gui.c" -o "gui-$t.exe" &&
        run "gui-$t" $gui "$WINE" "gui-$t.exe"
    fi
    # The QuickWin console window (-mquickwin). The program writes its
    # result to quickwin.out, because its standard output is the window.
    if [ -n "$gui" ] && [ -f "$SYSROOT/lib/win16/$m/quickwin.obj" ]; then
      mkdir -p "quickwin-$t"
      rm -f "quickwin-$t/quickwin.out"
      build "quickwin-$t" $cc -mquickwin "$tests/quickwin.c" \
          -o "quickwin-$t/qw.exe" &&
        run "quickwin-$t" $gui sh -c \
          "cd quickwin-$t && '$WINE' qw.exe; cat quickwin.out"
    fi
  done
done

# The versioned triples. ne-check.py checks the header and, for real mode,
# that no relocation needs a segment to stay where it was loaded.
check() {
  # check NAME FILE ARGS...: ne-check.py's report, in NAME.ne
  name=$1 file=$2
  shift 2
  if python3 "$scripts/ne-check.py" "$@" "$file" >"$name.ne" 2>&1; then
    return 0
  fi
  fail=$((fail + 1))
  echo "FAIL $name (ne-check)"
  sed 's/^/    /' "$name.ne" | head -20
  return 1
}

# wineversion VERSION: the Windows version Wine reports for it.
wineversion() {
  case $1 in
    1.* | 2.*) echo win20 ;;
    3.0*) echo win30 ;;
    *) echo win31 ;;
  esac
}

# winrun NAME VERSION EXE ARGS...: run EXE with Wine reporting VERSION.
winrun() {
  name=$1 ver=$2 exe=$3
  shift 3
  "$WINE" reg add "HKCU\\Software\\Wine\\AppDefaults\\$(basename "$exe")" \
    /v Version /d "$(wineversion "$ver")" /f >/dev/null 2>&1
  run "$name" "$WINE" "$exe" "$@"
}

for v in ${VERSIONS:-1.01 2.03 3.00 3.10}; do
  case $v in
    3.1* | [4-9].*) mode=--protmode ;;
    *) mode=--real-mode ;;
  esac
  for m in ${VERSION_MODELS:-small medium large}; do
    o=Os
    cc="$BIN/clang --target=i8086-pc-win16$v -mcmodel=$m --sysroot=$SYSROOT $ldflag -$o"
    t=win$v-$m
    build "hello-$t" $cc "$tests/hello.c" -o "hello-$t.exe" &&
      check "hello-$t" "hello-$t.exe" --version "$v" $mode &&
      winrun "hello-$t" "$v" "hello-$t.exe" one two
    build "api-$t" $cc "$tests/api.c" -o "api-$t.exe" &&
      check "api-$t" "api-$t.exe" --version "$v" $mode &&
      winrun "api-$t" "$v" "api-$t.exe" abc
    if [ -n "$gui" ]; then
      build "gui-$t" $cc "$tests/gui.c" -o "gui-$t.exe" &&
        check "gui-$t" "gui-$t.exe" --version "$v" $mode &&
        run "gui-$t" $gui "$WINE" "gui-$t.exe"
    fi
    # Resources for that version: before 3.0, menus have the old format.
    if build "rc-$t" "$BIN/llvm-rc" -win16-version "$v" -I "$SYSROOT/include" \
         -I "$tests" -FO "res-$t.res" "$tests/res.rc" &&
       build "res-$t" $cc -I "$tests" "$tests/res.c" "res-$t.res" -o "res-$t.exe" &&
       check "res-$t" "res-$t.exe" --version "$v" $mode; then
      winrun "res-$t" "$v" "res-$t.exe"
    fi
    # Before 3.0, Windows loads an imported module from MODULE.EXE.
    case $v in
      [12].*) dll=TESTDLL.EXE ;;
      *) dll=TESTDLL.DLL ;;
    esac
    mkdir -p "dll-$t"
    if build "testdll-$t" $cc -shared "$tests/testdll.c" -o "dll-$t/$dll" &&
       check "testdll-$t" "dll-$t/$dll" --version "$v" $mode &&
       python3 "$scripts/win16-imports.py" "dll-$t/$dll" >"dll-$t/testdll.lnk" &&
       build "dllapp-$t" $cc "$tests/dllapp.c" "-Wl,@dll-$t/testdll.lnk" -o "dll-$t/dllapp.exe" &&
       check "dllapp-$t" "dll-$t/dllapp.exe" --version "$v" $mode; then
      run "dll-$t" sh -c "cd dll-$t && '$WINE' dllapp.exe"
    fi
  done
done

# A display minidriver for Windows 95, 98 and Me, on the DIB engine
# (dibeng.h). Wine has no DIB engine, so the test checks the NE file: the
# header, and the display driver functions at their ordinals.
grep '^export' "$tests/display.lnk" |
  sed 's/^export \(.*\)\.\(.*\)/import \1 DISPLAY.\2/' >display.exports
for m in ${DISPLAY_MODELS:-small large}; do
  for o in O0 O2; do
    t=$m-$o
    mkdir -p "display-$t"
    if build "display-$t" "$BIN/clang" --target=i8086-pc-win164.00 -mcmodel=$m \
         --sysroot="$SYSROOT" $ldflag -$o -Wall -Wextra -Werror -shared \
         -mwindows-prologue=all "$tests/display.c" -o "display-$t/DISPLAY.DRV" \
         "-Wl,@$SYSROOT/lib/win16/dibeng.lnk" "-Wl,@$tests/display.lnk" &&
       check "display-$t" "display-$t/DISPLAY.DRV" --version 4.00 --protmode; then
      python3 "$scripts/win16-imports.py" "display-$t/DISPLAY.DRV" >"display-$t.exports"
      if cmp -s display.exports "display-$t.exports" &&
         grep -q "imports from: DIBENG, KERNEL" "display-$t.ne" &&
         ! grep "segment" "display-$t.ne" | grep -qv fixed; then
        pass=$((pass + 1))
      else
        fail=$((fail + 1))
        echo "FAIL display-$t (exports or segments)"
        diff display.exports "display-$t.exports" | head -20
        sed 's/^/    /' "display-$t.ne" | head -20
      fi
    fi
  done
done

# Pen Windows, the Win32s Universal Thunk and the generic thunks
# (penwin.h, w32sut.h, wownt16.h). Wine has no PENWIN.DLL, no WIN32S16.DLL
# and no Win32s, so the test checks the NE file: the modules it imports
# from, and the names it imports WIN32S16's functions and
# RemovePenDataStrokes by, which must have the case that the DLLs export.
for m in ${PEN_MODELS:-small medium compact large}; do
  for o in O0 O2; do
    t=$m-$o
    if build "penut-$t" "$BIN/clang" --target=i8086-pc-win16 -mcmodel=$m \
         --sysroot="$SYSROOT" $ldflag -$o -Wall -Wextra -Werror -shared \
         "$tests/penut.c" -o "PENUT-$t.DLL" &&
       check "penut-$t" "PENUT-$t.DLL" --protmode; then
      if grep -q "imports from: KERNEL, PENWIN, WIN32S16" "penut-$t.ne" &&
         grep -q -a "UTLinearToSelectorOffset" "PENUT-$t.DLL" &&
         grep -q -a "UTSelectorOffsetToLinear" "PENUT-$t.DLL" &&
         grep -q -a "REMOVEPENDATASTROKES" "PENUT-$t.DLL"; then
        pass=$((pass + 1))
      else
        fail=$((fail + 1))
        echo "FAIL penut-$t (imports)"
        sed 's/^/    /' "penut-$t.ne" | head -20
      fi
    fi
  done
done

echo "$pass passed, $fail failed"
[ "$fail" = 0 ]
