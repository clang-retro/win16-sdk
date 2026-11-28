/* gdidefs.h: the interface between GDI and a device driver (display or
 * printer) of Windows 3.1, 95, 98 and Me.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 * Copyright (C) clang-retro contributors.
 *
 * Written for clang-retro from the published documentation of the
 * driver interface (the Windows 3.1 "Device Driver Adaptation Guide" and
 * the display driver chapters of the Windows 95 DDK documentation). No
 * text of a Microsoft header or include file is used. tools/check-ddk.py
 * compares the values with a DDK that you have; see the README.
 *
 * GDI calls the driver's Enable function twice. With InquireInfo, the
 * driver fills a GDIINFO that tells GDI what the device is and what it
 * can draw. With EnableDevice, the driver fills its PDEVICE. GDI then
 * gives a DRAWMODE to each output function. Structures that windows.h
 * has (RECT, POINT, BITMAPINFOHEADER, LOGPEN and the others) are not
 * declared again here. */
#ifndef _GDIDEFS_H
#define _GDIDEFS_H
#include <windows.h>

#pragma pack(push, 1)

/* The "style" argument of Enable. */
#define EnableDevice 0x0000 /* fill in the PDEVICE */
#define InquireInfo 0x0001  /* fill in the GDIINFO */
#define InfoContext 0x8000  /* an information context: no output */

/* dpTechnology: the type of device. windows.h has most of them, for
 * GetDeviceCaps(TECHNOLOGY). */
#ifndef DT_PLOTTER
#define DT_PLOTTER 0
#endif
#ifndef DT_RASDISPLAY
#define DT_RASDISPLAY 1
#endif
#ifndef DT_RASPRINTER
#define DT_RASPRINTER 2
#endif
#ifndef DT_RASCAMERA
#define DT_RASCAMERA 3
#endif
#ifndef DT_CHARSTREAM
#define DT_CHARSTREAM 4
#endif
#ifndef DT_METAFILE
#define DT_METAFILE 5
#endif
#ifndef DT_DISPFILE
#define DT_DISPFILE 6
#endif
#define DT_JUMBO 11 /* a device that needs more than 64 KB of PDEVICE */

/* dpCurves: the curves that the driver draws itself. */
#ifndef CC_NONE
#define CC_NONE 0x0000
#endif
#ifndef CC_CIRCLES
#define CC_CIRCLES 0x0001
#endif
#ifndef CC_PIE
#define CC_PIE 0x0002
#endif
#ifndef CC_CHORD
#define CC_CHORD 0x0004
#endif
#ifndef CC_ELLIPSES
#define CC_ELLIPSES 0x0008
#endif
#ifndef CC_WIDE
#define CC_WIDE 0x0010
#endif
#ifndef CC_STYLED
#define CC_STYLED 0x0020
#endif
#ifndef CC_WIDESTYLED
#define CC_WIDESTYLED 0x0040
#endif
#ifndef CC_INTERIORS
#define CC_INTERIORS 0x0080
#endif
#ifndef CC_ROUNDRECT
#define CC_ROUNDRECT 0x0100
#endif
#define CC_POLYBEZIER 0x0200

/* dpLines: the lines that the driver draws itself. */
#ifndef LC_NONE
#define LC_NONE 0x0000
#endif
#define LC_POLYSCANLINE 0x0001
#ifndef LC_POLYLINE
#define LC_POLYLINE 0x0002
#endif
#ifndef LC_MARKER
#define LC_MARKER 0x0004
#endif
#ifndef LC_POLYMARKER
#define LC_POLYMARKER 0x0008
#endif
#ifndef LC_WIDE
#define LC_WIDE 0x0010
#endif
#ifndef LC_STYLED
#define LC_STYLED 0x0020
#endif
#ifndef LC_WIDESTYLED
#define LC_WIDESTYLED 0x0040
#endif
#ifndef LC_INTERIORS
#define LC_INTERIORS 0x0080
#endif

/* dpPolygonals: the polygons that the driver draws itself. */
#ifndef PC_NONE
#define PC_NONE 0x0000
#endif
#define PC_ALTPOLYGON 0x0001
#ifndef PC_POLYGON
#define PC_POLYGON 0x0001
#endif
#ifndef PC_RECTANGLE
#define PC_RECTANGLE 0x0002
#endif
#ifndef PC_WINDPOLYGON
#define PC_WINDPOLYGON 0x0004
#endif
#define PC_TRAPEZOID 0x0004
#ifndef PC_SCANLINE
#define PC_SCANLINE 0x0008
#endif
#ifndef PC_WIDE
#define PC_WIDE 0x0010
#endif
#ifndef PC_STYLED
#define PC_STYLED 0x0020
#endif
#ifndef PC_WIDESTYLED
#define PC_WIDESTYLED 0x0040
#endif
#ifndef PC_INTERIORS
#define PC_INTERIORS 0x0080
#endif
#define PC_POLYPOLYGON 0x0100
#define PC_PATHS 0x0200

/* dpClip: the clipping that the driver does itself. */
#ifndef CP_NONE
#define CP_NONE 0x0000
#endif
#ifndef CP_RECTANGLE
#define CP_RECTANGLE 0x0001
#endif
#ifndef CP_REGION
#define CP_REGION 0x0002
#endif
#define CP_REGION32 0x0004

/* dpText: the text operations that the driver does itself. */
#ifndef TC_OP_CHARACTER
#define TC_OP_CHARACTER 0x0001
#endif
#ifndef TC_OP_STROKE
#define TC_OP_STROKE 0x0002
#endif
#ifndef TC_CP_STROKE
#define TC_CP_STROKE 0x0004
#endif
#ifndef TC_CR_90
#define TC_CR_90 0x0008
#endif
#ifndef TC_CR_ANY
#define TC_CR_ANY 0x0010
#endif
#ifndef TC_SF_X_YINDEP
#define TC_SF_X_YINDEP 0x0020
#endif
#ifndef TC_SA_DOUBLE
#define TC_SA_DOUBLE 0x0040
#endif
#ifndef TC_SA_INTEGER
#define TC_SA_INTEGER 0x0080
#endif
#ifndef TC_SA_CONTIN
#define TC_SA_CONTIN 0x0100
#endif
#ifndef TC_EA_DOUBLE
#define TC_EA_DOUBLE 0x0200
#endif
#ifndef TC_IA_ABLE
#define TC_IA_ABLE 0x0400
#endif
#ifndef TC_UA_ABLE
#define TC_UA_ABLE 0x0800
#endif
#ifndef TC_SO_ABLE
#define TC_SO_ABLE 0x1000
#endif
#ifndef TC_RA_ABLE
#define TC_RA_ABLE 0x2000
#endif
#ifndef TC_VA_ABLE
#define TC_VA_ABLE 0x4000
#endif
#ifndef TC_RESERVED
#define TC_RESERVED 0x8000
#endif

/* dpRaster: the raster operations of the driver. */
#ifndef RC_NONE
#define RC_NONE 0x0000
#endif
#ifndef RC_BITBLT
#define RC_BITBLT 0x0001
#endif
#ifndef RC_BANDING
#define RC_BANDING 0x0002
#endif
#ifndef RC_SCALING
#define RC_SCALING 0x0004
#endif
#ifndef RC_BITMAP64
#define RC_BITMAP64 0x0008
#endif
#define RC_GDI20_OUTPUT 0x0010
#define RC_GDI20_STATE 0x0020
#define RC_SAVEBITMAP 0x0040
#ifndef RC_DI_BITMAP
#define RC_DI_BITMAP 0x0080
#endif
#ifndef RC_PALETTE
#define RC_PALETTE 0x0100
#endif
#ifndef RC_DIBTODEV
#define RC_DIBTODEV 0x0200
#endif
#ifndef RC_BIGFONT
#define RC_BIGFONT 0x0400
#endif
#ifndef RC_STRETCHBLT
#define RC_STRETCHBLT 0x0800
#endif
#ifndef RC_FLOODFILL
#define RC_FLOODFILL 0x1000
#endif
#ifndef RC_STRETCHDIB
#define RC_STRETCHDIB 0x2000
#endif
#define RC_OP_DX_OUTPUT 0x4000
#define RC_DEVBITS 0x8000

/* dpDCManage: how GDI makes PDEVICEs for device contexts. */
#define DC_SPDevice 0x0001   /* one PDEVICE for each output file name */
#define DC_1PDevice 0x0002   /* one PDEVICE for all device contexts */
#define DC_IgnoreDFNP 0x0004 /* ignore the output file name */

/* dpCaps1: more capabilities (Windows 3.1 and later). */
#define C1_TRANSPARENT 0x0001   /* transparent background mode */
#define TC_TT_ABLE 0x0002       /* the driver draws TrueType glyphs */
#define C1_TT_CR_ANY 0x0004     /* it rotates TrueType text */
#define C1_EMF_COMPLIANT 0x0008 /* it can play back enhanced metafiles */
#define C1_DIBENGINE 0x0010     /* it is built on the DIB engine */
#define C1_GAMMA_RAMP 0x0020    /* it supports gamma ramps */
#define C1_ICM 0x0040           /* it does image colour matching */
#define C1_REINIT_ABLE 0x0080   /* ReEnable can change the mode */
#define C1_GLYPH_INDEX 0x0100   /* ExtTextOut takes glyph indexes */
#define C1_BIT_PACKED 0x0200    /* it takes bit-packed glyphs */
#define C1_BYTE_PACKED 0x0400   /* it takes byte-packed glyphs */
#define C1_COLORCURSOR 0x0800   /* it draws colour cursors */
#define C1_CMYK_ABLE 0x1000     /* it takes CMYK colours */
#define C1_SLOW_CARD 0x2000     /* drawing to the card is slow */

/* The "style" argument of Output: the figure to draw. */
#define OS_POLYBEZIER 1
#define OS_ARC 3
#define OS_SCANLINES 4
#define OS_POLYSCANLINE 5
#define OS_RECTANGLE 6
#define OS_ELLIPSE 7
#define OS_MARKER 8
#define OS_POLYLINE 18
#define OS_WINDPOLYGON 20
#define OS_TRAPEZOID 20
#define OS_ALTPOLYGON 22
#define OS_POLYGON 22
#define OS_PIE 23
#define OS_POLYMARKER 24
#define OS_CHORD 39
#define OS_CIRCLE 55
#define OS_POLYPOLYGON 0x4000 /* added to a polygon style */
#define OS_BEGINNSCAN 80
#define OS_ENDNSCAN 81

/* DRAWMODE.bkMode. */
#define BKMODE_TRANSPARENT 1
#define BKMODE_OPAQUE 2
#define BKMODE_LEVEL1 3
#define BKMODE_LEVEL2 4
#define BKMODE_LEVEL3 5
#define BKMODE_TRANSLATE 6

/* The "style" of RealizeObject and EnumObj: the kind of object. */
#ifndef OBJ_PEN
#define OBJ_PEN 1
#endif
#ifndef OBJ_BRUSH
#define OBJ_BRUSH 2
#endif
#ifndef OBJ_FONT
#define OBJ_FONT 3
#endif

/* What Enable(InquireInfo) returns: the description of the device. The
 * dpM, dpE and dpTw pairs are the window and viewport extents of the
 * MM_LOMETRIC, MM_HIMETRIC, MM_LOENGLISH, MM_HIENGLISH and MM_TWIPS
 * mapping modes. */
typedef struct tagGDIINFO {
  short dpVersion;      /* the driver interface version: 0x30A, 0x400 */
  short dpTechnology;   /* DT_* */
  short dpHorzSize;     /* width of the display, in millimetres */
  short dpVertSize;     /* height, in millimetres */
  short dpHorzRes;      /* width, in pixels */
  short dpVertRes;      /* height, in pixels */
  short dpBitsPixel;    /* bits of each pixel in each plane */
  short dpPlanes;       /* colour planes */
  short dpNumBrushes;   /* device brushes; -1 for more than 32767 */
  short dpNumPens;      /* device pens */
  short futureuse;
  short dpNumFonts;     /* device fonts */
  short dpNumColors;    /* entries in the device's colour table */
  unsigned short dpDEVICEsize; /* bytes of the PDEVICE */
  unsigned short dpCurves;     /* CC_* */
  unsigned short dpLines;      /* LC_* */
  unsigned short dpPolygonals; /* PC_* */
  unsigned short dpText;       /* TC_* */
  unsigned short dpClip;       /* CP_* */
  unsigned short dpRaster;     /* RC_* */
  short dpAspectX;      /* relative width of a pixel */
  short dpAspectY;      /* relative height of a pixel */
  short dpAspectXY;     /* relative diagonal of a pixel */
  short dpStyleLen;     /* length of a styled line segment */
  POINT dpMLoWin;       /* MM_LOMETRIC */
  POINT dpMLoVpt;
  POINT dpMHiWin;       /* MM_HIMETRIC */
  POINT dpMHiVpt;
  POINT dpELoWin;       /* MM_LOENGLISH */
  POINT dpELoVpt;
  POINT dpEHiWin;       /* MM_HIENGLISH */
  POINT dpEHiVpt;
  POINT dpTwpWin;       /* MM_TWIPS */
  POINT dpTwpVpt;
  short dpLogPixelsX;   /* pixels for each logical inch, across */
  short dpLogPixelsY;   /* pixels for each logical inch, down */
  short dpDCManage;     /* DC_* */
  short dpCaps1;        /* C1_* */
  short futureuse4;
  short futureuse5;
  short futureuse6;
  short futureuse7;
  short dpNumPalReg;    /* palette entries (RC_PALETTE) */
  short dpPalReserved;  /* reserved (static) palette entries */
  short dpColorRes;     /* bits of colour of the DAC */
} GDIINFO, FAR *LPGDIINFO;

/* Far East capabilities of a driver: it draws DBCS TrueType text, or it
 * supports WIFE, the font interface of the Far East versions. */
#define FEC_TT_DBCS 0x0020
#define FEC_WIFE_ABLE 0x0080

/* What GDI gives to each output function: the state of the device
 * context that changes how a figure or text is drawn. */
typedef struct tagDRAWMODE {
  short Rop2;           /* the binary raster operation (R2_*) */
  short bkMode;         /* BKMODE_* */
  DWORD bkColor;        /* the background colour, as a physical colour */
  DWORD TextColor;      /* the text colour, as a physical colour */
  short TBreakExtra;    /* pixels to add to the break characters */
  short BreakExtra;     /* pixels for each break character */
  short BreakErr;       /* the running error of the justification */
  short BreakRem;       /* the remainder of TBreakExtra */
  short BreakCount;     /* break characters in the string */
  short CharExtra;      /* pixels to add to each character */
  DWORD LbkColor;       /* the background colour, as a logical colour */
  DWORD LTextColor;     /* the text colour, as a logical colour */
  DWORD ICMCXform;      /* the colour transform of ICM (Windows 95) */
  short StretchBltMode; /* the stretching mode (Windows 95) */
  DWORD eMiterLimit;    /* the miter limit, as a float (Windows 95) */
} DRAWMODE, FAR *LPDRAWMODE;

#pragma pack(pop)
#endif
