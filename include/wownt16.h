/* wownt16.h: the generic thunks, with which a 16-bit program or DLL calls
 * a 32-bit DLL on Windows NT (WOW) and Windows 95 and later. Written for
 * clang-retro from the published API; AGPL-3.0-only, see the win16-sdk
 * LICENSE.
 *
 * LoadLibraryEx32W loads the 32-bit DLL, GetProcAddress32W finds a
 * function in it and CallProcEx32W calls the function. GetVDMPointer32W
 * converts a 16:16 pointer for the 32-bit code. The functions are in
 * KERNEL from Windows NT 3.5 and Windows 95 on. */
#ifndef _INC_WOWNT16
#define _INC_WOWNT16
#define _WOWNT16_

#include <windows.h>

/* The calling convention of the 32-bit function, in CallProcEx32W's
 * second argument with the bits of the arguments to convert. */
#define CPEX_DEST_STDCALL 0x00000000L
#define CPEX_DEST_CDECL 0x80000000L

#ifndef RC_INVOKED
#ifdef __cplusplus
extern "C" {
#endif

DWORD WINAPI GetVDMPointer32W(LPVOID vp, UINT fMode);
DWORD WINAPI LoadLibraryEx32W(LPCSTR lpszLibFile, DWORD hFile,
                              DWORD dwFlags);
DWORD WINAPI GetProcAddress32W(DWORD hModule, LPCSTR lpszProc);
DWORD WINAPI FreeLibrary32W(DWORD hLibModule);
/* CallProcEx32W(nParams, fAddressConvert, lpProcAddress32, ...): bit n
 * of fAddressConvert converts argument n from a 16:16 pointer. */
DWORD FAR CDECL CallProcEx32W(DWORD nParams, DWORD fAddressConvert,
                              DWORD lpProcAddress32, ...);

#ifdef __cplusplus
}
#endif
#endif /* RC_INVOKED */

#endif /* _INC_WOWNT16 */
