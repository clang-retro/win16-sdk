/* An installable driver (HELLODRV.DRV): a DLL that exports DriverProc,
 * which Windows 3.1's OpenDriver and SendDriverMessage call (USER loads
 * it with LoadLibrary and sends it DRV_LOAD, DRV_ENABLE and DRV_OPEN).
 * Multimedia drivers (MMSYSTEM's wave, MIDI, ... drivers) are the same
 * with more messages. drvapp.c loads it and calls DriverProc as USER
 * would: Wine's OpenDriver is a stub for 16-bit programs. */
#include <windows.h>

static int loads, enabled;

LRESULT __export WINAPI DriverProc(DWORD id, HDRVR hdrvr, UINT msg,
                                   LPARAM l1, LPARAM l2) {
  switch (msg) {
  case DRV_LOAD:
    loads++;
    return 1;
  case DRV_ENABLE:
    enabled = 1;
    return 1;
  case DRV_DISABLE:
    enabled = 0;
    return 1;
  case DRV_FREE:
  case DRV_CLOSE:
    return 1;
  case DRV_OPEN:
    return 0x1234; /* the id of this instance, passed back in id */
  case DRV_QUERYCONFIGURE:
    return 0;
  case DRV_USER + 1:
    /* Something only this driver does: 3 * l1 + 1, if open and enabled. */
    if (id != 0x1234 || !enabled)
      return -1;
    return l1 * 3 + loads;
  }
  return DefDriverProc(id, hdrvr, msg, l1, l2);
}
