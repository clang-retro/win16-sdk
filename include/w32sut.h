/* w32sut.h: the Universal Thunk of Win32s, which lets a Win32 program
 * on Windows 3.1 call a 16-bit DLL and the DLL call back into it.
 * Written for clang-retro from the published API; AGPL-3.0-only, see the
 * win16-sdk LICENSE.
 *
 * The header serves both halves. The 16-bit DLL defines W32SUT_16 (the
 * default for a 16-bit target) and the Win32 program W32SUT_32 (the
 * default for a 32-bit target).
 *
 * The Win32 program calls UTRegister (KERNEL32) with the name of the
 * 16-bit DLL and the names of two of its exports. Win32s then calls the
 * init export,
 *     DWORD FAR PASCAL UTInit(UT16CBPROC lpfnUT16Callback, LPVOID lpBuf);
 * and each call through the thunk that UTRegister gave goes to the proc
 * export,
 *     DWORD FAR PASCAL UTProc(LPVOID lpBuf, DWORD dwUserDefined);
 * The translation list (a NULL-terminated array of addresses of
 * pointers in the buffer) tells Win32s which pointers to convert
 * between 0:32 and 16:16 addresses. */
#ifndef _INC_W32SUT
#define _INC_W32SUT

#if !defined(W32SUT_16) && !defined(W32SUT_32)
#if defined(_WIN32) || defined(__i386__) || defined(__x86_64__)
#define W32SUT_32
#else
#define W32SUT_16
#endif
#endif

#if defined(W32SUT_16) && defined(W32SUT_32)
#error W32SUT_16 and W32SUT_32 cannot be defined together
#endif

#include <windows.h>

#ifndef RC_INVOKED
#ifdef __cplusplus
extern "C" {
#endif

#ifdef W32SUT_32
/* The thunk that UTRegister gives: a call to the 16-bit proc export. */
typedef DWORD(WINAPI *UT32PROC)(LPVOID lpBuff, DWORD dwUserDefined,
                                LPVOID *lpTranslationList);

BOOL WINAPI UTRegister(HANDLE hModule, LPCSTR lpsz16BitDLL,
                       LPCSTR lpszInitName, LPCSTR lpszProcName,
                       UT32PROC *ppfn32Thunk, FARPROC pfnUT32Callback,
                       LPVOID lpBuff);
VOID WINAPI UTUnRegister(HANDLE hModule);
#endif

#ifdef W32SUT_16
/* The callback into the Win32 program that the init export gets. */
typedef DWORD(FAR PASCAL *UT16CBPROC)(LPVOID lpBuff, DWORD dwUserDefined,
                                      LPVOID FAR *lpTranslationList);

/* The address conversions of WIN32S16.DLL. */
LPVOID WINAPI UTLinearToSelectorOffset(LPBYTE lpByte);
LPVOID WINAPI UTSelectorOffsetToLinear(LPBYTE lpByte);
#endif

#ifdef __cplusplus
}
#endif
#endif /* RC_INVOKED */

#endif /* _INC_W32SUT */
