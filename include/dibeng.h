/* dibeng.h: the DIB engine (DIBENG.DLL) of Windows 95, 98 and Me, for
 * display minidrivers.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 * Copyright (C) clang-retro contributors.
 *
 * Written for clang-retro from the published documentation of the DIB
 * engine (the display driver chapters of the Windows 95 DDK
 * documentation). No text of a Microsoft header or include file is used.
 * tools/check-ddk.py compares the values with a DDK that you have; see
 * the README.
 *
 * The DIB engine draws into a framebuffer that is laid out as a DIB. A
 * display minidriver is a 16-bit DLL named DISPLAY. It exports the GDI
 * display driver functions (BitBlt @1 to ReEnable @31, Inquire @101 to
 * CheckCursor @104, GetDriverResourceID @450, UserRepaintDisable @500 and
 * ValidateMode @700). It sets the video mode in Enable, and sends almost
 * all other calls to the DIB engine. Link it with
 * -Wl,@<sysroot>/lib/win16/dibeng.lnk, which imports the functions of
 * the engine by ordinal. tests/win16/display.c is an example. */
#ifndef _DIBENG_H
#define _DIBENG_H
#include <gdidefs.h>

#pragma pack(push, 1)

#define VER_DIBENG 0x400   /* DIBENGINE.deVersion */
#define TYPE_DIBENG 0x5250 /* DIBENGINE.deType: the letters "RP" */

/* The PDEVICE of a surface that the DIB engine draws on: the screen, or
 * a device bitmap. It is also the header of each bitmap that the engine
 * makes. */
typedef struct tagDIBENGINE {
  WORD deType;            /* TYPE_DIBENG */
  WORD deWidth;           /* width, in pixels */
  WORD deHeight;          /* height, in pixels */
  WORD deWidthBytes;      /* bytes of each scan line */
  BYTE dePlanes;          /* 1 */
  BYTE deBitsPixel;       /* 1, 4, 8, 16, 24 or 32 */
  DWORD deReserved1;
  DWORD deDeltaScan;      /* bytes from one scan line to the next */
  LPBYTE delpPDevice;     /* the PDEVICE that owns this surface */
  DWORD deBitsOffset;     /* offset of the bits in their segment */
  WORD deBitsSelector;    /* selector of the bits */
  WORD deFlags;           /* the flags below */
  WORD deVersion;         /* VER_DIBENG */
  LPBITMAPINFO deBitmapInfo; /* the BITMAPINFO of the surface */
  void (WINAPI *deBeginAccess)(void); /* before the engine draws */
  void (WINAPI *deEndAccess)(void);   /* after the engine draws */
  DWORD deDriverReserved; /* for the minidriver */
} DIBENGINE, FAR *LPDIBENGINE;

/* DIBENGINE.deFlags. */
#define MINIDRIVER 0x0001      /* the screen of a minidriver */
#define PALETTIZED 0x0002      /* the colours go through a palette */
#define SELECTEDDIB 0x0004     /* a DIB section selected into a DC */
#define OFFSCREEN 0x0008       /* in off-screen video memory */
#define DISABLED 0x0010        /* the screen is disabled */
#define BUSY 0x0010            /* the engine is drawing */
#define NOT_FRAMEBUFFER 0x0020 /* no linear access to the bits */
#define FIVE6FIVE 0x0040       /* 16 bits per pixel, as 5-6-5 */
#define NON64KBANK 0x0080      /* banks are not 64 KB in size */
#define HWCURSOR 0x0100        /* the hardware draws the cursor */
#define DITHER256 0x0200       /* dither to the 256-colour palette */
#define CTCHANGE 0x0400        /* the colour table changed */
#define VGADITHER 0x0800       /* dither to the 16 VGA colours */
#define PALETTE_XLAT 0x1000    /* translate through the palette */
#define BANKEDSCAN 0x2000      /* a scan line can cross a bank */
#define BANKEDVRAM 0x4000      /* video memory is in banks */
#define VRAM 0x8000            /* the bits are in video memory */
#define BUSY_BIT 4             /* the bit number of BUSY */

/* Bits of the screen-access flags that the engine and the minidriver
 * share: the framebuffer is being accessed, or the cursor is excluded. */
#define FB_ACCESS 0x0001
#define CURSOREXCLUDE 0x0008

/* An entry in the colour table of a surface: an RGBQUAD with flags in
 * the last byte. */
typedef struct tagDIBColorEntry {
  BYTE dceBlue;
  BYTE dceGreen;
  BYTE dceRed;
  BYTE dceFlags; /* MAPTOWHITE, NONSTATIC */
} DIBColorEntry;
#define MAPTOWHITE 0x01 /* the colour maps to white on a mono surface */
#define NONSTATIC 0x80  /* an entry that an application can change */

/* The flags of a brush that the engine realized. */
#define COLORSOLID 0x01
#define MONOSOLID 0x02
#define PATTERNMONO 0x04
#define MONOVALID 0x08
#define MASKVALID 0x10
#define PRIVATEDATA 0x20
#define BRUSH40 0x40
#define DIBENGBRUSH 0x80

/* The DIB engine's functions that a minidriver calls itself. The others
 * (DIB_BitBlt, DIB_Output, ...) take the arguments of the GDI function of
 * the same name and are reached through DIBENG_LINK below. */
UINT WINAPI DIB_Enable(LPVOID lpDevice, UINT style, LPSTR lpDeviceType,
                       LPSTR lpOutputFile, LPVOID lpStuff);
UINT WINAPI DIB_Disable(LPVOID lpDevice);
void WINAPI DIB_BeginAccess(void); /* for deBeginAccess and deEndAccess */
void WINAPI DIB_EndAccess(void);
/* Fills in a DIBENGINE at lpDevice for the bits at lpBits, as lpInfo
 * describes them; flags are the deFlags (MINIDRIVER, PALETTIZED, VRAM,
 * ...). The engine returns the size of the surface in EAX, or 0 if it
 * failed, which C cannot see: call DIBEngCreatePDevice instead. */
void WINAPI CreateDIBPDevice(LPBITMAPINFOHEADER lpInfo, LPVOID lpDevice,
                             LPVOID lpBits, WORD flags);

/* CreateDIBPDevice, returning its result: the bytes of the surface, or 0
 * if it failed. */
__attribute__((naked, unused)) static DWORD WINAPI
DIBEngCreatePDevice(LPBITMAPINFOHEADER lpInfo, LPVOID lpDevice,
                    LPVOID lpBits, WORD flags) {
  __asm__("pushw %bp\n\tmovw %sp, %bp\n\t"
          "pushw 18(%bp)\n\tpushw 16(%bp)\n\tpushw 14(%bp)\n\t"
          "pushw 12(%bp)\n\tpushw 10(%bp)\n\tpushw 8(%bp)\n\t"
          "pushw 6(%bp)\n\t"
          "lcallw $CREATEDIBPDEVICE@SEG, $CREATEDIBPDEVICE\n\t"
          ".byte 0x66\n\tpushw %ax\n\tpopw %ax\n\tpopw %dx\n\t" /* push eax */
          "popw %bp\n\tlretw $14");
}

/* DIBENG_LINK(Name, DIB_Name) defines the exported function Name that
 * jumps to the DIB engine's DIB_Name, with the arguments GDI passed.
 * DIBENG_LINK_EXT(Name, DIB_NameExt, var) also passes the DWORD var (in
 * DGROUP: the minidriver's PDEVICE, for the DIB_*Ext functions) as a last
 * argument, and DIBENG_LINK_EXTW the WORD var. Give both names in upper
 * case, as the assembler sees __pascal functions:
 * DIBENG_LINK(BITBLT, DIB_BITBLT), which also keeps them apart from the
 * GDI functions of windows.h. Export each at its ordinal with a linker
 * directive (export BITBLT.1). */
#define DIBENG_LINK(name, target)                                           \
  _DIBENG_LINK_X(_dibeng_##name, #name, "", #target)
#define DIBENG_LINK_EXT(name, target, var)                                  \
  _DIBENG_LINK_X(_dibeng_##name, #name,                                     \
                 _DIBENG_DS "pushw %es:" #var "+2\n\t"                      \
                 "pushw %es:" #var "\n\t" _DIBENG_RET, #target)
#define DIBENG_LINK_EXTW(name, target, var)                                 \
  _DIBENG_LINK_X(_dibeng_##name, #name,                                     \
                 _DIBENG_DS "pushw %es:" #var "\n\t" _DIBENG_RET, #target)
/* The C name is _dibeng_NAME and the assembler name NAME, and neither is
 * macro-expanded: windows.h has macros such as EXTTEXTOUT. */
#define _DIBENG_LINK_X(cname, name, push, target)                           \
  __attribute__((naked)) void WINAPI cname(void) __asm__(name);             \
  __attribute__((naked)) void WINAPI cname(void) {                          \
    __asm__(push "ljmpw *%cs:1f\n1:\n\t.long " target);                     \
  }
/* Take the far return address into CX and DX (which the GDI functions
 * need not keep) and set ES = DGROUP: push ds; pop ax; nop is what
 * Windows patches into mov ax, DGROUP in the exported functions of a
 * DLL. Then push the extra argument, and the return address above it. */
#define _DIBENG_DS                                                          \
  "pushw %ds\n\tpopw %ax\n\tnop\n\tmovw %ax, %es\n\t"                       \
  "popw %cx\n\tpopw %dx\n\t"
#define _DIBENG_RET "pushw %dx\n\tpushw %cx\n\t"

/* Int 2Fh functions between the display driver and the display VxD
 * (VDD). The driver calls STOP_IO_TRAP in Enable, when it takes the
 * hardware, and START_IO_TRAP in Disable; the screen switch and critical
 * section ones are notifications its own int 2Fh handler may get. */
#define STOP_IO_TRAP 0x4000
#define SCREEN_SWITCH_OUT 0x4001
#define SCREEN_SWITCH_IN 0x4002
#define ENTER_CRIT_REG 0x4003
#define EXIT_CRIT_REG 0x4004
#define START_IO_TRAP 0x4007

/* ValidateMode: can the driver run the mode? */
typedef struct tagDISPVALMODE {
  UINT dvmSize;           /* sizeof(DISPVALMODE) */
  UINT dvmBpp;
  int dvmXRes;
  int dvmYRes;
} DISPVALMODE, FAR *LPDISPVALMODE;
#define VALMODE_YES 0
#define VALMODE_NO_WRONGDRV 1
#define VALMODE_NO_NOMEM 2
#define VALMODE_NO_NODAC 3
#define VALMODE_NO_UNKNOWN 4

#pragma pack(pop)
#endif
