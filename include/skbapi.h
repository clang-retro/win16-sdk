/* skbapi.h: the screen keyboard of Pen Windows (ShowKeyboard in
 * PENWIN.DLL). Written for clang-retro from the published API;
 * AGPL-3.0-only, see the win16-sdk LICENSE. */
#ifndef _INC_SKBAPI
#define _INC_SKBAPI

#include <windows.h>

/* The commands of ShowKeyboard (wCommand). */
#define SKB_QUERY 0x0000
#define SKB_SHOW 0x0001
#define SKB_HIDE 0x0002
#define SKB_CENTER 0x0010
#define SKB_MOVE 0x0020
#define SKB_MINIMIZE 0x0040
#define SKB_FULL 0x0100
#define SKB_BASIC 0x0200
#define SKB_NUMPAD 0x0400

/* The changes that WM_SKB reports (wParam). */
#define SKN_CHANGED 1
#define SKN_POSCHANGED 1
#define SKN_PADCHANGED 2
#define SKN_MINCHANGED 4
#define SKN_VISCHANGED 8
#define SKN_TERMINATED 0xffff

#ifndef RC_INVOKED
/* The state of the screen keyboard. */
typedef struct tagSKBINFO {
  HWND hwnd;
  UINT nPad;
  BOOL fVisible;
  BOOL fMinimized;
  RECT rect;
  DWORD dwReserved;
} SKBINFO, FAR *LPSKBINFO;
#endif

#endif /* _INC_SKBAPI */
