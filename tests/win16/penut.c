/* penut.c: a DLL that calls Pen Windows, the Win32s Universal Thunk and
 * the generic thunks. Wine has none of PENWIN.DLL, WIN32S16.DLL and the
 * Win32 side of the thunks, so the test only builds the DLL; run-tests.sh
 * checks the modules and names that it imports. */
#include <windows.h>
#include <penwin.h>
#include <w32sut.h>
#include <wownt16.h>

static UT16CBPROC callback;
static HPENDATA pendata;

/** Keep the Win32 callback that Win32s gives to the init export. */
DWORD __export FAR PASCAL UTInit(UT16CBPROC lpfn, LPVOID lpBuf) {
  (void)lpBuf;
  callback = lpfn;
  return 1;
}

/** Do one call through the thunk: use each API once. */
DWORD __export FAR PASCAL UTProc(LPVOID lpBuf, DWORD dwFunc) {
  RC rc;
  DWORD k32, fn;
  switch (dwFunc) {
  case 0: /* Pen Windows 1.0: a recognition context with the defaults. */
    if (!GetVersionPenWin())
      return 0;
    InitRC(0, &rc);
    return (DWORD)ProcessWriting(0, &rc);
  case 1: /* Pen Windows 1.0 and 2.0 at different ordinals: by name. */
    return (DWORD)RemovePenDataStrokes(pendata, 0, 1);
  case 2: /* The Universal Thunk's address conversions. */
    return (DWORD)UTSelectorOffsetToLinear((LPBYTE)lpBuf) +
           (DWORD)UTLinearToSelectorOffset((LPBYTE)lpBuf);
  case 3: /* A generic thunk to GetTickCount in KERNEL32. */
    k32 = LoadLibraryEx32W("KERNEL32.DLL", 0, 0);
    fn = GetProcAddress32W(k32, "GetTickCount");
    fn = CallProcEx32W(0, 0, fn);
    FreeLibrary32W(k32);
    return fn + GetVDMPointer32W(lpBuf, 1);
  default: /* Back into the Win32 program. */
    return callback ? callback(lpBuf, dwFunc, NULL) : 0;
  }
}
