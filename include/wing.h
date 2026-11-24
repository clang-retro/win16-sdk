/* wing.h: WinG 1.0, fast device-independent bitmaps for games (WING.DLL,
 * which programs install with them on Windows 3.1). Written for clang-dos
 * from the published API; AGPL-3.0-only, see the win16-sdk LICENSE.
 *
 * A WinG DC selects a WinGBitmap, a DIB whose bits the program writes
 * directly (WinGGetDIBPointer), then copies to the screen with WinGBitBlt or
 * WinGStretchBlt. */
#ifndef _INC_WING
#define _INC_WING

#include <windows.h>

#ifndef RC_INVOKED
#ifdef __cplusplus
extern "C" {
#endif

/* The dither patterns of WinGCreateHalftoneBrush. */
typedef enum WING_DITHER_TYPE {
  WING_DISPERSED_4x4,
  WING_DISPERSED_8x8,
  WING_CLUSTERED_4x4
} WING_DITHER_TYPE;

#if WINVER >= 0x030A
HDC WINAPI WinGCreateDC(void);
BOOL WINAPI WinGRecommendDIBFormat(BITMAPINFO FAR *);
HBITMAP WINAPI WinGCreateBitmap(HDC, BITMAPINFO const FAR *, void FAR *FAR *);
void FAR *WINAPI WinGGetDIBPointer(HBITMAP, BITMAPINFO FAR *);
UINT WINAPI WinGGetDIBColorTable(HDC, UINT, UINT, RGBQUAD FAR *);
UINT WINAPI WinGSetDIBColorTable(HDC, UINT, UINT, RGBQUAD const FAR *);
HPALETTE WINAPI WinGCreateHalftonePalette(void);
HBRUSH WINAPI WinGCreateHalftoneBrush(HDC, COLORREF, WING_DITHER_TYPE);
BOOL WINAPI WinGBitBlt(HDC, int, int, int, int, HDC, int, int);
BOOL WINAPI WinGStretchBlt(HDC, int, int, int, int, HDC, int, int, int, int);
#endif

#ifdef __cplusplus
}
#endif
#endif /* RC_INVOKED */

#endif /* _INC_WING */
