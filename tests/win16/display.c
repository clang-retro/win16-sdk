/* DISPLAY.DRV: the shape of a display minidriver for Windows 95, 98 and
 * Me, built on the DIB engine (dibeng.h). It runs the VGA's 320x200 mode
 * with 256 colours (mode 13h, at A000:0) as a palettized framebuffer:
 * Enable sets the mode and has the DIB engine make the PDEVICE, SetPalette
 * programs the DAC, and every drawing function goes to the DIB engine.
 * It is a starting point, not a driver to install: Windows wants 640x480
 * at least, and a real one finds its adapter's modes and linear
 * framebuffer, works with its mini-VDD (win32-sdk's vxd/minivdd.c) and
 * draws with its accelerator. Wine has no DIB engine to load it, so the
 * test only checks the NE file: module name, description, imports and
 * the exports at their ordinals (display.lnk). Build it with
 *   clang --target=i8086-pc-win164.00 -shared -mwindows-prologue=all
 *     display.c -o DISPLAY.DRV -Wl,@<sysroot>/lib/win16/dibeng.lnk
 *     -Wl,@display.lnk
 * -mwindows-prologue=all gives the exported functions the DLL entry
 * sequence; with __export instead, the linker would export them twice. */
#include <dibeng.h>
#include <string.h>

#define WIDTH 320
#define HEIGHT 200
#define DPI 96

/* The stubs that send the GDI functions to the DIB engine. The names are
 * the assembler ones, upper case as __pascal makes them, which also keeps
 * them apart from the GDI functions of windows.h (BitBlt, ...). */

/* What the stubs below pass to the DIB_*Ext functions: the PDEVICE GDI
 * enabled, and whether the screen is palettized. The assembler names
 * are what DIBENG_LINK_EXT refers to. */
LPVOID lpDriverPDevice __asm__("lpDriverPDevice");
WORD wPalettized __asm__("wPalettized") = 1;
static WORD wEnabled, wDIBPdevSize;

DIBENG_LINK(BITBLT, DIB_BITBLT)
DIBENG_LINK(COLORINFO, DIB_COLORINFO)
DIBENG_LINK(CONTROL, DIB_CONTROL)
DIBENG_LINK(ENUMDFONTS, DIB_ENUMDFONTS)
DIBENG_LINK_EXT(ENUMOBJ, DIB_ENUMOBJEXT, lpDriverPDevice)
DIBENG_LINK(OUTPUT, DIB_OUTPUT)
DIBENG_LINK(PIXEL, DIB_PIXEL)
DIBENG_LINK_EXT(REALIZEOBJECT, DIB_REALIZEOBJECTEXT, lpDriverPDevice)
DIBENG_LINK(STRBLT, DIB_STRBLT)
DIBENG_LINK(SCANLR, DIB_SCANLR)
DIBENG_LINK(DEVICEMODE, DIB_DEVICEMODE)
DIBENG_LINK(EXTTEXTOUT, DIB_EXTTEXTOUT)
DIBENG_LINK(GETCHARWIDTH, DIB_GETCHARWIDTH)
DIBENG_LINK(DEVICEBITMAP, DIB_DEVICEBITMAP)
DIBENG_LINK(FASTBORDER, DIB_FASTBORDER)
DIBENG_LINK(SETATTRIBUTE, DIB_SETATTRIBUTE)
DIBENG_LINK_EXTW(DIBBLT, DIB_DIBBLTEXT, wPalettized)
DIBENG_LINK(CREATEDIBITMAP, DIB_CREATEDIBITMAP)
DIBENG_LINK(DIBTODEVICE, DIB_DIBTODEVICE)
DIBENG_LINK_EXT(GETPALETTE, DIB_GETPALETTEEXT, lpDriverPDevice)
DIBENG_LINK_EXT(SETPALETTETRANSLATE, DIB_SETPALETTETRANSLATEEXT, lpDriverPDevice)
DIBENG_LINK_EXT(GETPALETTETRANSLATE, DIB_GETPALETTETRANSLATEEXT, lpDriverPDevice)
DIBENG_LINK_EXT(UPDATECOLORS, DIB_UPDATECOLORSEXT, lpDriverPDevice)
DIBENG_LINK(STRETCHBLT, DIB_STRETCHBLT)
DIBENG_LINK(STRETCHDIBITS, DIB_STRETCHDIBITS)
DIBENG_LINK(SELECTBITMAP, DIB_SELECTBITMAP)
DIBENG_LINK(BITMAPBITS, DIB_BITMAPBITS)
DIBENG_LINK(INQUIRE, DIB_INQUIRE)
DIBENG_LINK_EXT(SETCURSOR, DIB_SETCURSOREXT, lpDriverPDevice)
DIBENG_LINK_EXT(MOVECURSOR, DIB_MOVECURSOREXT, lpDriverPDevice)

void WINAPI DIB_CheckCursorExt(LPVOID lpDevice);
void WINAPI DIB_SetPaletteExt(UINT start, UINT count, LPVOID colors,
                              LPVOID lpDevice);

/* GDI calls this on every timer tick: draw the cursor if it moved. */
void WINAPI CheckCursor(void) {
  if (wEnabled)
    DIB_CheckCursorExt(lpDriverPDevice);
}

static void out8(WORD port, BYTE v) {
  __asm__ volatile("outb %0, %1" : : "a"(v), "Nd"(port));
}

static void program_dac(UINT start, UINT count) {
  LPBITMAPINFO info =
      (LPBITMAPINFO)((LPBYTE)lpDriverPDevice + wDIBPdevSize);
  DIBColorEntry FAR *c = (DIBColorEntry FAR *)info->bmiColors + start;
  out8(0x3C8, (BYTE)start);
  for (; count; count--, c++) {
    out8(0x3C9, c->dceRed >> 2);
    out8(0x3C9, c->dceGreen >> 2);
    out8(0x3C9, c->dceBlue >> 2);
  }
}

/* The engine keeps the colour table; the driver sets the hardware. */
void WINAPI SetPalette(UINT start, UINT count, LPVOID colors) {
  DIB_SetPaletteExt(start, count, colors, lpDriverPDevice);
  if (wEnabled)
    program_dac(start, count);
}

/* The 20 colours Windows reserves in a 256-colour palette. */
static const BYTE syscolors[20][3] = {
    {0, 0, 0},       {0x80, 0, 0},    {0, 0x80, 0},    {0x80, 0x80, 0},
    {0, 0, 0x80},    {0x80, 0, 0x80}, {0, 0x80, 0x80}, {0xC0, 0xC0, 0xC0},
    {0xC0, 0xDC, 0xC0}, {0xA6, 0xCA, 0xF0},
    {0xFF, 0xFB, 0xF0}, {0xA0, 0xA0, 0xA4},
    {0x80, 0x80, 0x80}, {0xFF, 0, 0},    {0, 0xFF, 0},    {0xFF, 0xFF, 0},
    {0, 0, 0xFF},    {0xFF, 0, 0xFF}, {0, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}};

static void inquire(LPGDIINFO gi) {
  static const POINT win[5] = {{254, 254}, {2540, 2540}, {100, 100},
                               {1000, 1000}, {1440, 1440}};
  POINT FAR *p = &gi->dpMLoWin;
  int i;
  gi->dpVersion = 0x400;
  gi->dpTechnology = DT_RASDISPLAY;
  gi->dpHorzSize = WIDTH * 254L / (DPI * 10);
  gi->dpVertSize = HEIGHT * 254L / (DPI * 10);
  gi->dpHorzRes = WIDTH;
  gi->dpVertRes = HEIGHT;
  gi->dpBitsPixel = 8;
  gi->dpPlanes = 1;
  gi->dpNumBrushes = -1;
  gi->dpNumPens = 16;
  gi->dpNumFonts = 0;
  gi->dpNumColors = 20;
  /* The extents of the mapping modes at DPI dots per inch. */
  for (i = 0; i < 5; i++) {
    p[2 * i] = win[i];
    p[2 * i + 1].x = DPI;
    p[2 * i + 1].y = -DPI;
  }
  gi->dpLogPixelsX = gi->dpLogPixelsY = DPI;
  gi->dpDCManage = DC_IgnoreDFNP;
  gi->dpCaps1 |= C1_BYTE_PACKED | C1_COLORCURSOR | C1_SLOW_CARD;
  gi->dpNumPalReg = 256;
  gi->dpPalReserved = 20;
  gi->dpColorRes = 18;
  gi->dpRaster |= RC_PALETTE | RC_DIBTODEV | RC_SAVEBITMAP;
  /* GDI allocates the PDEVICE: the engine's part, then the screen's
   * BITMAPINFOHEADER and colour table. */
  wDIBPdevSize = gi->dpDEVICEsize;
  gi->dpDEVICEsize += sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
}

UINT WINAPI Enable(LPVOID lpDevice, UINT style, LPSTR lpDeviceType,
                            LPSTR lpOutputFile, LPVOID lpStuff) {
  LPDIBENGINE pdev = lpDevice;
  LPBITMAPINFO info;
  WORD screen;
  int i;
  if (style & InquireInfo) {
    /* The engine fills in what it can draw, the driver the rest. */
    DIB_Enable(lpDevice, style, lpDeviceType, lpOutputFile, lpStuff);
    inquire((LPGDIINFO)lpDevice);
    return sizeof(GDIINFO);
  }
  /* Take the hardware from the VDD and set the mode. */
  __asm__ volatile("int $0x2f" : : "a"(STOP_IO_TRAP) : "memory");
  __asm__ volatile("int $0x10" : : "a"(0x13) : "bx", "cx", "dx", "memory");
  DIB_Enable(lpDevice, style, lpDeviceType, lpOutputFile, lpStuff);
  info = (LPBITMAPINFO)((LPBYTE)lpDevice + wDIBPdevSize);
  memset(&info->bmiHeader, 0, sizeof(info->bmiHeader));
  info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  info->bmiHeader.biWidth = WIDTH;
  info->bmiHeader.biHeight = HEIGHT;
  info->bmiHeader.biPlanes = 1;
  info->bmiHeader.biBitCount = 8;
  for (i = 0; i < 256; i++) {
    const BYTE *c = syscolors[i < 10 ? i : i >= 246 ? i - 236 : 0];
    DIBColorEntry FAR *e = (DIBColorEntry FAR *)&info->bmiColors[i];
    e->dceRed = c[0];
    e->dceGreen = c[1];
    e->dceBlue = c[2];
    e->dceFlags = i < 10 || i >= 246 ? 0 : NONSTATIC;
  }
  /* KERNEL's __A000H is a selector for the VGA's memory. */
  __asm__("movw $__A000H, %0" : "=r"(screen));
  if (!DIBEngCreatePDevice(&info->bmiHeader, lpDevice, MAKELP(screen, 0),
                           MINIDRIVER | PALETTIZED))
    return 0;
  pdev->deBeginAccess = DIB_BeginAccess;
  pdev->deEndAccess = DIB_EndAccess;
  lpDriverPDevice = lpDevice;
  wEnabled = 1;
  program_dac(0, 256);
  return 1;
}

UINT WINAPI Disable(LPVOID lpDevice) {
  wEnabled = 0;
  DIB_Disable(lpDevice);
  /* Back to text mode, and give the hardware back to the VDD. */
  __asm__ volatile("int $0x10" : : "a"(0x03) : "bx", "cx", "dx", "memory");
  __asm__ volatile("int $0x2f" : : "a"(START_IO_TRAP) : "memory");
  return 1;
}

/* A mode change without a restart: only drivers that set C1_REINIT_ABLE
 * get it. */
UINT WINAPI ReEnable(LPVOID lpDevice, LPGDIINFO lpInfo) {
  (void)lpDevice;
  (void)lpInfo;
  return 0;
}

/* USER asks for its resources (cursors, icons, bitmaps, fonts) by these
 * ids; a driver for large displays maps them to its own. */
DWORD WINAPI GetDriverResourceID(WORD id, LPSTR type) {
  (void)type;
  return id;
}

/* USER stops drawing while a full-screen DOS box or DirectDraw has the
 * screen. */
void WINAPI UserRepaintDisable(BOOL disable) { (void)disable; }

/* The Display control panel asks which modes it may offer. */
UINT WINAPI ValidateMode(LPDISPVALMODE mode) {
  if (mode->dvmXRes == WIDTH && mode->dvmYRes == HEIGHT && mode->dvmBpp == 8)
    return VALMODE_YES;
  return VALMODE_NO_UNKNOWN;
}
