/* Loads HELLODRV.DRV (hellodrv.c) and goes through what OpenDriver,
 * SendDriverMessage and CloseDriver do with its DriverProc. */
#include <windows.h>
#include <stdio.h>

typedef LRESULT(WINAPI *DRIVERPROC)(DWORD, HDRVR, UINT, LPARAM, LPARAM);

int PASCAL WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {
  HINSTANCE lib = LoadLibrary("HELLODRV.DRV");
  HDRVR h = (HDRVR)1;
  DRIVERPROC proc;
  DWORD id;
  LRESULT r;
  int ok;
  if (lib < HINSTANCE_ERROR) {
    printf("LoadLibrary failed: %u\n", (UINT)lib);
    return 1;
  }
  proc = (DRIVERPROC)GetProcAddress(lib, "DriverProc");
  if (!proc) {
    printf("no DriverProc\n");
    return 1;
  }
  ok = proc(0, h, DRV_LOAD, 0, 0) == 1 && proc(0, h, DRV_ENABLE, 0, 0) == 1;
  id = (DWORD)proc(0, h, DRV_OPEN, 0, 0);
  r = proc(id, h, DRV_USER + 1, 14, 0);
  ok = ok && id == 0x1234 && r == 43;
  ok = ok && proc(id, h, DRV_CLOSE, 0, 0) == 1 &&
       proc(0, h, DRV_DISABLE, 0, 0) == 1 && proc(0, h, DRV_FREE, 0, 0) == 1;
  FreeLibrary(lib);
  printf("driver answered %ld for 14%s\n", (long)r, ok ? ": ALL PASSED" : "");
  return !ok;
}
