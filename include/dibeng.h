/* dibeng.h: the DIB engine (DIBENG.DLL) of Windows 95, 98 and Me, which
 * draws into a framebuffer for display minidrivers. A minidriver is a
 * 16-bit DLL named DISPLAY that exports the GDI display driver functions
 * (BitBlt @1 ... ReEnable @31, Inquire @101 ... CheckCursor @104,
 * GetDriverResourceID @450, UserRepaintDisable @500, ValidateMode @700),
 * sets the mode in Enable and passes nearly everything else to the DIB
 * engine. Link it with -Wl,@<sysroot>/lib/win16/dibeng.lnk, which imports
 * the engine's functions by ordinal. The names, values and layouts are
 * those of the DDK's DIBENG.INC and VALMODE.INC. tests/win16/display.c is
 * a minidriver. */
#ifndef _DIBENG_H
#define _DIBENG_H
#include <gdidefs.h>

#pragma pack(push, 1)

#define VER_DIBENG 0x400
#define TYPE_DIBENG 0x5250  /* deType of a DIB engine PDEVICE ('RP') */

/* The PDEVICE (and the bitmap header) of a surface the DIB engine draws
 * on. A minidriver's PDEVICE starts with one. */
typedef struct tagDIBENGINE {
  WORD deType;            /* TYPE_DIBENG */
  WORD deWidth;
  WORD deHeight;
  WORD deWidthBytes;
  BYTE dePlanes;
  BYTE deBitsPixel;
  DWORD deReserved1;
  DWORD deDeltaScan;
  LPVOID delpPDevice;
  DWORD deBitsOffset;     /* the 16:32 address of the bits */
  WORD deBitsSelector;
  WORD deFlags;           /* below */
  WORD deVersion;         /* VER_DIBENG */
  LPBITMAPINFO deBitmapInfo;
  void (FAR PASCAL *deBeginAccess)(void); /* see DIB_BeginAccess */
  void (FAR PASCAL *deEndAccess)(void);
  DWORD deDriverReserved;
} DIBENGINE, FAR *LPDIBENGINE;

/* deFlags */
#define MINIDRIVER 0x0001
#define PALETTIZED 0x0002
#define SELECTEDDIB 0x0004
#define OFFSCREEN 0x0008
#define DISABLED 0x0010
#define BUSY 0x0010
#define NOT_FRAMEBUFFER 0x0020
#define FIVE6FIVE 0x0040
#define NON64KBANK 0x0080
#define HWCURSOR 0x0100
#define DITHER256 0x0200
#define CTCHANGE 0x0400
#define VGADITHER 0x0800
#define PALETTE_XLAT 0x1000
#define BANKEDSCAN 0x2000
#define BANKEDVRAM 0x4000
#define VRAM 0x8000
#define BUSY_BIT 4          /* the bit number of BUSY */

/* The flags of DIB_BeginAccess */
#define FB_ACCESS 0x0001
#define CURSOREXCLUDE 0x0008

/* A colour table entry of a palettized surface */
typedef struct tagDIBColorEntry {
  BYTE dceBlue;
  BYTE dceGreen;
  BYTE dceRed;
  BYTE dceFlags;          /* below */
} DIBColorEntry;
#define MAPTOWHITE 0x01
#define NONSTATIC 0x80

/* dp?BrushFlags of a DIB engine brush */
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
