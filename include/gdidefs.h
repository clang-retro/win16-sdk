/* gdidefs.h: the GDI device driver interface of Windows 3.1 and Windows
 * 95/98/Me, for display and printer drivers: what Enable returns in
 * GDIINFO, the capability bits in it, and the DRAWMODE GDI passes to the
 * output functions. The names, values and layouts are those of the
 * DDK's GDIDEFS.INC; GDI's own structures that windows.h already has
 * (RECT, POINT, BITMAPINFOHEADER, LOGPEN, ...) are not repeated. */
#ifndef _GDIDEFS_H
#define _GDIDEFS_H
#include <windows.h>

#pragma pack(push, 1)

/* The style of Enable */
#define EnableDevice 0  /* fill in the PDEVICE */
#define InquireInfo 1   /* fill in the GDIINFO */
#define InfoContext 0x8000 /* an information context, no output */

/* Device technologies (dpTechnology) */
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
#define DT_JUMBO 0xB

/* Curve capabilities (dpCurves) */
#ifndef CC_NONE
#define CC_NONE 0
#endif
#ifndef CC_CIRCLES
#define CC_CIRCLES 1
#endif
#ifndef CC_PIE
#define CC_PIE 2
#endif
#ifndef CC_CHORD
#define CC_CHORD 4
#endif
#ifndef CC_ELLIPSES
#define CC_ELLIPSES 8
#endif
#ifndef CC_WIDE
#define CC_WIDE 0x10
#endif
#ifndef CC_STYLED
#define CC_STYLED 0x20
#endif
#ifndef CC_WIDESTYLED
#define CC_WIDESTYLED 0x40
#endif
#ifndef CC_INTERIORS
#define CC_INTERIORS 0x80
#endif
#define CC_ROUNDRECT 0x100
#define CC_POLYBEZIER 0x200

/* Line capabilities (dpLines) */
#ifndef LC_NONE
#define LC_NONE 0
#endif
#define LC_POLYSCANLINE 1
#ifndef LC_POLYLINE
#define LC_POLYLINE 2
#endif
#ifndef LC_MARKER
#define LC_MARKER 4
#endif
#ifndef LC_POLYMARKER
#define LC_POLYMARKER 8
#endif
#ifndef LC_WIDE
#define LC_WIDE 0x10
#endif
#ifndef LC_STYLED
#define LC_STYLED 0x20
#endif
#ifndef LC_WIDESTYLED
#define LC_WIDESTYLED 0x40
#endif
#ifndef LC_INTERIORS
#define LC_INTERIORS 0x80
#endif

/* Polygon capabilities (dpPolygonals) */
#ifndef PC_NONE
#define PC_NONE 0
#endif
#define PC_ALTPOLYGON 1
#ifndef PC_POLYGON
#define PC_POLYGON 1
#endif
#ifndef PC_RECTANGLE
#define PC_RECTANGLE 2
#endif
#ifndef PC_WINDPOLYGON
#define PC_WINDPOLYGON 4
#endif
#define PC_TRAPEZOID 4
#ifndef PC_SCANLINE
#define PC_SCANLINE 8
#endif
#ifndef PC_WIDE
#define PC_WIDE 0x10
#endif
#ifndef PC_STYLED
#define PC_STYLED 0x20
#endif
#ifndef PC_WIDESTYLED
#define PC_WIDESTYLED 0x40
#endif
#ifndef PC_INTERIORS
#define PC_INTERIORS 0x80
#endif
#define PC_POLYPOLYGON 0x100
#define PC_PATHS 0x200

/* Clipping capabilities (dpClip) */
#ifndef CP_NONE
#define CP_NONE 0
#endif
#ifndef CP_RECTANGLE
#define CP_RECTANGLE 1
#endif
#ifndef CP_REGION
#define CP_REGION 2
#endif
#define CP_REGION32 4

/* Text capabilities (dpText) */
#ifndef TC_OP_CHARACTER
#define TC_OP_CHARACTER 1
#endif
#ifndef TC_OP_STROKE
#define TC_OP_STROKE 2
#endif
#ifndef TC_CP_STROKE
#define TC_CP_STROKE 4
#endif
#ifndef TC_CR_90
#define TC_CR_90 8
#endif
#ifndef TC_CR_ANY
#define TC_CR_ANY 0x10
#endif
#ifndef TC_SF_X_YINDEP
#define TC_SF_X_YINDEP 0x20
#endif
#ifndef TC_SA_DOUBLE
#define TC_SA_DOUBLE 0x40
#endif
#ifndef TC_SA_INTEGER
#define TC_SA_INTEGER 0x80
#endif
#ifndef TC_SA_CONTIN
#define TC_SA_CONTIN 0x100
#endif
#ifndef TC_EA_DOUBLE
#define TC_EA_DOUBLE 0x200
#endif
#ifndef TC_IA_ABLE
#define TC_IA_ABLE 0x400
#endif
#ifndef TC_UA_ABLE
#define TC_UA_ABLE 0x800
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

/* Raster capabilities (dpRaster) */
#ifndef RC_NONE
#define RC_NONE 0
#endif
#ifndef RC_BITBLT
#define RC_BITBLT 1
#endif
#ifndef RC_BANDING
#define RC_BANDING 2
#endif
#ifndef RC_SCALING
#define RC_SCALING 4
#endif
#ifndef RC_BITMAP64
#define RC_BITMAP64 8
#endif
#ifndef RC_GDI20_OUTPUT
#define RC_GDI20_OUTPUT 0x10
#endif
#define RC_GDI20_STATE 0x20
#define RC_SAVEBITMAP 0x40
#ifndef RC_DI_BITMAP
#define RC_DI_BITMAP 0x80
#endif
#ifndef RC_PALETTE
#define RC_PALETTE 0x100
#endif
#ifndef RC_DIBTODEV
#define RC_DIBTODEV 0x200
#endif
#ifndef RC_BIGFONT
#define RC_BIGFONT 0x400
#endif
#ifndef RC_STRETCHBLT
#define RC_STRETCHBLT 0x800
#endif
#ifndef RC_FLOODFILL
#define RC_FLOODFILL 0x1000
#endif
#ifndef RC_STRETCHDIB
#define RC_STRETCHDIB 0x2000
#endif
#define RC_OP_DX_OUTPUT 0x4000
#define RC_DEVBITS 0x8000

/* How GDI manages PDEVICEs (dpDCManage) */
#define DC_SPDevice 1
#define DC_1PDevice 2
#define DC_IgnoreDFNP 4

/* More capabilities (dpCaps1) */
#define C1_TRANSPARENT 1
#define C1_TT_CR_ANY 4
#define C1_EMF_COMPLIANT 8
#define C1_DIBENGINE 0x10
#define C1_GAMMA_RAMP 0x20
#define C1_ICM 0x40
#define C1_REINIT_ABLE 0x80
#define C1_GLYPH_INDEX 0x100
#define C1_BIT_PACKED 0x200
#define C1_BYTE_PACKED 0x400
#define C1_COLORCURSOR 0x800
#define C1_CMYK_ABLE 0x1000
#define C1_SLOW_CARD 0x2000

/* Output styles (the style of Output) */
#define OS_POLYBEZIER 1
#define OS_ARC 3
#define OS_SCANLINES 4
#define OS_POLYSCANLINE 5
#define OS_RECTANGLE 6
#define OS_ELLIPSE 7
#define OS_MARKER 8
#define OS_POLYLINE 0x12
#define OS_WINDPOLYGON 0x14
#define OS_TRAPEZOID 0x14
#define OS_ALTPOLYGON 0x16
#define OS_POLYGON 0x16
#define OS_PIE 0x17
#define OS_POLYMARKER 0x18
#define OS_CHORD 0x27
#define OS_CIRCLE 0x37
#define OS_POLYPOLYGON 0x4000
#define OS_BEGINNSCAN 0x50
#define OS_ENDNSCAN 0x51

/* Background modes in DRAWMODE.bkMode */
#define BKMODE_TRANSPARENT 1
#define BKMODE_OPAQUE 2
#define BKMODE_LEVEL1 3
#define BKMODE_LEVEL2 4
#define BKMODE_LEVEL3 5
#define BKMODE_TRANSLATE 6

/* Objects for RealizeObject and EnumObj */
#ifndef OBJ_PEN
#define OBJ_PEN 1
#endif
#ifndef OBJ_BRUSH
#define OBJ_BRUSH 2
#endif
#define OBJ_FONT 3

/* What Enable(InquireInfo) fills in: 110 bytes, the Windows 3.1 GDIINFO
 * with dpCaps1 and the fields after it, which Windows 3.1 drivers left
 * out. */
typedef struct tagGDIINFO {
  short dpVersion;        /* 0x400 for Windows 95 drivers, 0x30A for 3.1 */
  short dpTechnology;     /* DT_ */
  short dpHorzSize;       /* in millimetres */
  short dpVertSize;
  short dpHorzRes;        /* in pixels */
  short dpVertRes;
  short dpBitsPixel;
  short dpPlanes;
  short dpNumBrushes;
  short dpNumPens;
  short dpCapsFE;         /* FEC_ (far-east fonts) */
  short dpNumFonts;
  short dpNumColors;
  short dpDEVICEsize;     /* bytes GDI allocates for the PDEVICE */
  WORD dpCurves;          /* CC_ */
  WORD dpLines;           /* LC_ */
  WORD dpPolygonals;      /* PC_ */
  WORD dpText;            /* TC_ */
  WORD dpClip;            /* CP_ */
  WORD dpRaster;          /* RC_ */
  short dpAspectX;
  short dpAspectY;
  short dpAspectXY;
  short dpStyleLen;
  POINT dpMLoWin;         /* the mapping modes' window and viewport */
  POINT dpMLoVpt;         /* extents: MM_LOMETRIC, */
  POINT dpMHiWin;         /* MM_HIMETRIC, */
  POINT dpMHiVpt;
  POINT dpELoWin;         /* MM_LOENGLISH, */
  POINT dpELoVpt;
  POINT dpEHiWin;         /* MM_HIENGLISH, */
  POINT dpEHiVpt;
  POINT dpTwpWin;         /* MM_TWIPS */
  POINT dpTwpVpt;
  short dpLogPixelsX;     /* dots per inch */
  short dpLogPixelsY;
  WORD dpDCManage;        /* DC_ */
  WORD dpCaps1;           /* C1_ */
  long dpSpotSizeX;
  long dpSpotSizeY;
  short dpNumPalReg;      /* palette entries, if RC_PALETTE */
  short dpPalReserved;    /* of them, reserved for the system */
  short dpColorRes;       /* bits of colour of each entry */
} GDIINFO, FAR *LPGDIINFO;

#define FEC_TT_DBCS 0x20
#define FEC_WIFE_ABLE 0x80

/* The drawing state GDI passes to Output, StrBlt, ExtTextOut, ... */
typedef struct tagDRAWMODE {
  short Rop2;
  short bkMode;           /* BKMODE_ */
  DWORD bkColor;          /* physical colours */
  DWORD TextColor;
  short TBreakExtra;      /* text justification */
  short BreakExtra;
  short BreakErr;
  short BreakRem;
  short BreakCount;
  short CharExtra;
  DWORD LbkColor;         /* logical colours */
  DWORD LTextColor;
  /* Windows 95 and later: */
  DWORD ICMCXform;
  short StretchBltMode;
  DWORD eMiterLimit;
} DRAWMODE, FAR *LPDRAWMODE;

#pragma pack(pop)
#endif
