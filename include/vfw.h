/* vfw.h: Video for Windows 1.1 for 16-bit Windows: the video capture
 * driver interface and DrawDib (MSVIDEO.DLL), the Installable Compression
 * Manager (MSVIDEO.DLL), the MCIWnd window class (MSVIDEO.DLL) and the AVI
 * file functions (AVIFILE.DLL). Written for clang-dos from the published
 * API; AGPL-3.0-only, see the win16-sdk LICENSE.
 *
 * The AVI file and stream handles are OLE 2 interface pointers. This header
 * does not declare their method tables. Use the AVIFile* and AVIStream*
 * functions. */
#ifndef _INC_VFW
#define _INC_VFW

#include <windows.h>
#include <mmsystem.h>
#include <compobj.h>

#ifndef RC_INVOKED
#if WINVER >= 0x030A /* Windows 3.1 and later */
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

#define VFWAPI WINAPI
#define VFWAPIV FAR CDECL

/* VideoForWindowsVersion returns 0x010A for version 1.10. */
DWORD VFWAPI VideoForWindowsVersion(void);

/* ---- DrawDib: fast drawing of DIBs ---------------------------------- */

typedef WORD HDRAWDIB;

#define DDF_UPDATE 0x0002
#define DDF_SAME_HDC 0x0004
#define DDF_SAME_DRAW 0x0008
#define DDF_DONTDRAW 0x0010
#define DDF_ANIMATE 0x0020
#define DDF_BUFFER 0x0040
#define DDF_JUSTDRAWIT 0x0080
#define DDF_FULLSCREEN 0x0100
#define DDF_BACKGROUNDPAL 0x0200
#define DDF_NOTKEYFRAME 0x0400
#define DDF_HURRYUP 0x0800
#define DDF_HALFTONE 0x1000
#define DDF_PREROLL DDF_DONTDRAW
#define DDF_SAME_DIB DDF_SAME_DRAW
#define DDF_SAME_SIZE DDF_SAME_DRAW

#define PD_CAN_DRAW_DIB 0x0001
#define PD_CAN_STRETCHDIB 0x0002
#define PD_STRETCHDIB_1_1_OK 0x0004
#define PD_STRETCHDIB_1_2_OK 0x0008
#define PD_STRETCHDIB_1_N_OK 0x0010

HDRAWDIB VFWAPI DrawDibOpen(void);
BOOL VFWAPI DrawDibClose(HDRAWDIB hdd);
BOOL VFWAPI DrawDibBegin(HDRAWDIB hdd, HDC hdc, int dxDst, int dyDst,
                         LPBITMAPINFOHEADER lpbi, int dxSrc, int dySrc,
                         UINT wFlags);
BOOL VFWAPI DrawDibEnd(HDRAWDIB hdd);
BOOL VFWAPI DrawDibDraw(HDRAWDIB hdd, HDC hdc, int xDst, int yDst, int dxDst,
                        int dyDst, LPBITMAPINFOHEADER lpbi, LPVOID lpBits,
                        int xSrc, int ySrc, int dxSrc, int dySrc,
                        UINT wFlags);
HPALETTE VFWAPI DrawDibGetPalette(HDRAWDIB hdd);
BOOL VFWAPI DrawDibSetPalette(HDRAWDIB hdd, HPALETTE hpal);
UINT VFWAPI DrawDibRealize(HDRAWDIB hdd, HDC hdc, BOOL fBackground);
BOOL VFWAPI DrawDibProfileDisplay(LPBITMAPINFOHEADER lpbi);
BOOL VFWAPI DrawDibStart(HDRAWDIB hdd, LONG rate);
BOOL VFWAPI DrawDibStop(HDRAWDIB hdd);

/* DrawDibUpdate draws the last frame again from the DrawDib buffer. */
#define DrawDibUpdate(hdd, hdc, x, y)                                          \
  DrawDibDraw(hdd, hdc, x, y, 0, 0, NULL, NULL, 0, 0, 0, 0, DDF_UPDATE)

/* ---- The Installable Compression Manager ---------------------------- */

typedef WORD HIC;

#ifndef mmioFOURCC
#define mmioFOURCC(ch0, ch1, ch2, ch3)                                         \
  ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |                            \
   ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24))
#endif

#define ICTYPE_VIDEO mmioFOURCC('v', 'i', 'd', 'c')
#define ICTYPE_AUDIO mmioFOURCC('a', 'u', 'd', 'c')

#define ICERR_OK 0L
#define ICERR_DONTDRAW 1L
#define ICERR_NEWPALETTE 2L
#define ICERR_GOTOKEYFRAME 3L
#define ICERR_STOPDRAWING 4L
#define ICERR_UNSUPPORTED (-1L)
#define ICERR_BADFORMAT (-2L)
#define ICERR_MEMORY (-3L)
#define ICERR_INTERNAL (-4L)
#define ICERR_BADFLAGS (-5L)
#define ICERR_BADPARAM (-6L)
#define ICERR_BADSIZE (-7L)
#define ICERR_BADHANDLE (-8L)
#define ICERR_CANTUPDATE (-9L)
#define ICERR_ABORT (-10L)
#define ICERR_ERROR (-100L)
#define ICERR_BADBITDEPTH (-200L)
#define ICERR_BADIMAGESIZE (-201L)
#define ICERR_CUSTOM (-400L)

/* The modes of ICOpen. */
#define ICMODE_COMPRESS 1
#define ICMODE_DECOMPRESS 2
#define ICMODE_FASTDECOMPRESS 3
#define ICMODE_QUERY 4
#define ICMODE_FASTCOMPRESS 5
#define ICMODE_DRAW 8

/* The messages that ICSendMessage sends to a compressor. */
#define ICM_USER (DRV_USER + 0x0000)
#define ICM_RESERVED_LOW (DRV_USER + 0x1000)
#define ICM_RESERVED_HIGH (DRV_USER + 0x2000)
#define ICM_RESERVED ICM_RESERVED_LOW

#define ICM_GETSTATE (ICM_RESERVED + 0)
#define ICM_SETSTATE (ICM_RESERVED + 1)
#define ICM_GETINFO (ICM_RESERVED + 2)
#define ICM_CONFIGURE (ICM_RESERVED + 10)
#define ICM_ABOUT (ICM_RESERVED + 11)
#define ICM_GETDEFAULTQUALITY (ICM_RESERVED + 30)
#define ICM_GETQUALITY (ICM_RESERVED + 31)
#define ICM_SETQUALITY (ICM_RESERVED + 32)
#define ICM_SET (ICM_RESERVED + 40)
#define ICM_GET (ICM_RESERVED + 41)

#define ICM_COMPRESS_GET_FORMAT (ICM_USER + 4)
#define ICM_COMPRESS_GET_SIZE (ICM_USER + 5)
#define ICM_COMPRESS_QUERY (ICM_USER + 6)
#define ICM_COMPRESS_BEGIN (ICM_USER + 7)
#define ICM_COMPRESS (ICM_USER + 8)
#define ICM_COMPRESS_END (ICM_USER + 9)
#define ICM_DECOMPRESS_GET_FORMAT (ICM_USER + 10)
#define ICM_DECOMPRESS_QUERY (ICM_USER + 11)
#define ICM_DECOMPRESS_BEGIN (ICM_USER + 12)
#define ICM_DECOMPRESS (ICM_USER + 13)
#define ICM_DECOMPRESS_END (ICM_USER + 14)
#define ICM_DECOMPRESS_SET_PALETTE (ICM_USER + 29)
#define ICM_DECOMPRESS_GET_PALETTE (ICM_USER + 30)
#define ICM_DRAW_QUERY (ICM_USER + 31)
#define ICM_DRAW_BEGIN (ICM_USER + 15)
#define ICM_DRAW_GET_PALETTE (ICM_USER + 16)
#define ICM_DRAW_START (ICM_USER + 18)
#define ICM_DRAW_STOP (ICM_USER + 19)
#define ICM_DRAW_END (ICM_USER + 21)
#define ICM_DRAW_GETTIME (ICM_USER + 32)
#define ICM_DRAW (ICM_USER + 33)
#define ICM_DRAW_WINDOW (ICM_USER + 34)
#define ICM_DRAW_SETTIME (ICM_USER + 35)
#define ICM_DRAW_REALIZE (ICM_USER + 36)
#define ICM_DRAW_FLUSH (ICM_USER + 37)
#define ICM_DRAW_RENDERBUFFER (ICM_USER + 38)
#define ICM_DRAW_START_PLAY (ICM_USER + 39)
#define ICM_DRAW_STOP_PLAY (ICM_USER + 40)
#define ICM_DRAW_SUGGESTFORMAT (ICM_USER + 50)
#define ICM_DRAW_CHANGEPALETTE (ICM_USER + 51)
#define ICM_GETBUFFERSWANTED (ICM_USER + 41)
#define ICM_GETDEFAULTKEYFRAMERATE (ICM_USER + 42)
#define ICM_DECOMPRESSEX_BEGIN (ICM_USER + 60)
#define ICM_DECOMPRESSEX_QUERY (ICM_USER + 61)
#define ICM_DECOMPRESSEX (ICM_USER + 62)
#define ICM_DECOMPRESSEX_END (ICM_USER + 63)

/* The data of ICInfo and ICGetInfo. Win16 uses single-byte strings. */
typedef struct {
  DWORD dwSize;
  DWORD fccType;
  DWORD fccHandler;
  DWORD dwFlags;
  DWORD dwVersion;
  DWORD dwVersionICM;
  char szName[16];
  char szDescription[128];
  char szDriver[128];
} ICINFO;

#define VIDCF_QUALITY 0x0001
#define VIDCF_CRUNCH 0x0002
#define VIDCF_TEMPORAL 0x0004
#define VIDCF_COMPRESSFRAMES 0x0008
#define VIDCF_DRAW 0x0010
#define VIDCF_FASTTEMPORALC 0x0020
#define VIDCF_FASTTEMPORALD 0x0080

#define ICCOMPRESS_KEYFRAME 0x00000001L
#define ICDECOMPRESS_HURRYUP 0x80000000L
#define ICDECOMPRESS_UPDATE 0x40000000L
#define ICDECOMPRESS_PREROLL 0x20000000L
#define ICDECOMPRESS_NULLFRAME 0x10000000L
#define ICDECOMPRESS_NOTKEYFRAME 0x08000000L

#define ICDRAW_QUERY 0x00000001L
#define ICDRAW_FULLSCREEN 0x00000002L
#define ICDRAW_HDC 0x00000004L
#define ICDRAW_ANIMATE 0x00000008L
#define ICDRAW_CONTINUE 0x00000010L
#define ICDRAW_MEMORYDC 0x00000020L
#define ICDRAW_UPDATING 0x00000040L
#define ICDRAW_RENDER 0x00000080L
#define ICDRAW_BUFFER 0x00000100L
#define ICDRAW_HURRYUP 0x80000000L
#define ICDRAW_UPDATE 0x40000000L
#define ICDRAW_PREROLL 0x20000000L
#define ICDRAW_NULLFRAME 0x10000000L
#define ICDRAW_NOTKEYFRAME 0x08000000L

/* The flags of ICLocate. */
#define ICMODE_USE_COMPRESS ICMODE_COMPRESS

/* The callback of ICOpenFunction: the driver procedure of a compressor that
 * is in the program. */
typedef LRESULT(CALLBACK *ICDRIVERPROC)(DWORD dwDriverId, HDRVR hdrvr,
                                        UINT msg, LPARAM lParam1,
                                        LPARAM lParam2);

BOOL VFWAPI ICInfo(DWORD fccType, DWORD fccHandler, ICINFO FAR *lpicinfo);
BOOL VFWAPI ICInstall(DWORD fccType, DWORD fccHandler, LPARAM lParam,
                      LPSTR szDesc, UINT wFlags);
BOOL VFWAPI ICRemove(DWORD fccType, DWORD fccHandler, UINT wFlags);
HIC VFWAPI ICOpen(DWORD fccType, DWORD fccHandler, UINT wMode);
LRESULT VFWAPI ICClose(HIC hic);
LRESULT VFWAPI ICSendMessage(HIC hic, UINT msg, DWORD dw1, DWORD dw2);
HIC VFWAPI ICOpenFunction(DWORD fccType, DWORD fccHandler, UINT wMode,
                          FARPROC lpfnHandler);
LRESULT VFWAPIV ICMessage(HIC hic, UINT msg, UINT cb, ...);
LRESULT VFWAPI ICGetInfo(HIC hic, ICINFO FAR *picinfo, DWORD cb);
HIC VFWAPI ICLocate(DWORD fccType, DWORD fccHandler,
                    LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut,
                    WORD wFlags);
DWORD VFWAPIV ICCompress(HIC hic, DWORD dwFlags,
                         LPBITMAPINFOHEADER lpbiOutput, LPVOID lpData,
                         LPBITMAPINFOHEADER lpbiInput, LPVOID lpBits,
                         LPDWORD lpckid, LPDWORD lpdwFlags, LONG lFrameNum,
                         DWORD dwFrameSize, DWORD dwQuality,
                         LPBITMAPINFOHEADER lpbiPrev, LPVOID lpPrev);
DWORD VFWAPIV ICDecompress(HIC hic, DWORD dwFlags,
                           LPBITMAPINFOHEADER lpbiFormat, LPVOID lpData,
                           LPBITMAPINFOHEADER lpbi, LPVOID lpBits);
DWORD VFWAPIV ICDrawBegin(HIC hic, DWORD dwFlags, HPALETTE hpal, HWND hwnd,
                          HDC hdc, int xDst, int yDst, int dxDst, int dyDst,
                          LPBITMAPINFOHEADER lpbi, int xSrc, int ySrc,
                          int dxSrc, int dySrc, DWORD dwRate, DWORD dwScale);
DWORD VFWAPIV ICDraw(HIC hic, DWORD dwFlags, LPVOID lpFormat, LPVOID lpData,
                     DWORD cbData, LONG lTime);
HIC VFWAPI ICGetDisplayFormat(HIC hic, LPBITMAPINFOHEADER lpbiIn,
                              LPBITMAPINFOHEADER lpbiOut, int BitDepth,
                              int dx, int dy);

/* ICCompress and ICDecompress helpers that send one message. */
#define ICCompressGetFormat(hic, lpbiInput, lpbiOutput)                        \
  ICSendMessage(hic, ICM_COMPRESS_GET_FORMAT, (DWORD)(LPVOID)(lpbiInput),      \
                (DWORD)(LPVOID)(lpbiOutput))
#define ICCompressGetFormatSize(hic, lpbi)                                     \
  ICCompressGetFormat(hic, lpbi, NULL)
#define ICCompressBegin(hic, lpbiInput, lpbiOutput)                            \
  ICSendMessage(hic, ICM_COMPRESS_BEGIN, (DWORD)(LPVOID)(lpbiInput),           \
                (DWORD)(LPVOID)(lpbiOutput))
#define ICCompressGetSize(hic, lpbiInput, lpbiOutput)                          \
  ICSendMessage(hic, ICM_COMPRESS_GET_SIZE, (DWORD)(LPVOID)(lpbiInput),        \
                (DWORD)(LPVOID)(lpbiOutput))
#define ICCompressQuery(hic, lpbiInput, lpbiOutput)                            \
  ICSendMessage(hic, ICM_COMPRESS_QUERY, (DWORD)(LPVOID)(lpbiInput),           \
                (DWORD)(LPVOID)(lpbiOutput))
#define ICCompressEnd(hic) ICSendMessage(hic, ICM_COMPRESS_END, 0, 0)
#define ICDecompressGetFormat(hic, lpbiInput, lpbiOutput)                      \
  ((LONG)ICSendMessage(hic, ICM_DECOMPRESS_GET_FORMAT,                         \
                       (DWORD)(LPVOID)(lpbiInput),                             \
                       (DWORD)(LPVOID)(lpbiOutput)))
#define ICDecompressQuery(hic, lpbiInput, lpbiOutput)                          \
  ICSendMessage(hic, ICM_DECOMPRESS_QUERY, (DWORD)(LPVOID)(lpbiInput),         \
                (DWORD)(LPVOID)(lpbiOutput))
#define ICDecompressBegin(hic, lpbiInput, lpbiOutput)                          \
  ICSendMessage(hic, ICM_DECOMPRESS_BEGIN, (DWORD)(LPVOID)(lpbiInput),         \
                (DWORD)(LPVOID)(lpbiOutput))
#define ICDecompressEnd(hic) ICSendMessage(hic, ICM_DECOMPRESS_END, 0, 0)
#define ICDrawEnd(hic) ICSendMessage(hic, ICM_DRAW_END, 0, 0)
#define ICDrawStart(hic) ICSendMessage(hic, ICM_DRAW_START, 0, 0)
#define ICDrawStop(hic) ICSendMessage(hic, ICM_DRAW_STOP, 0, 0)
#define ICQueryAbout(hic)                                                      \
  (ICSendMessage(hic, ICM_ABOUT, (DWORD)-1, 0) == ICERR_OK)
#define ICAbout(hic, hwnd) ICSendMessage(hic, ICM_ABOUT, (UINT)(hwnd), 0)
#define ICQueryConfigure(hic)                                                  \
  (ICSendMessage(hic, ICM_CONFIGURE, (DWORD)-1, 0) == ICERR_OK)
#define ICConfigure(hic, hwnd) ICSendMessage(hic, ICM_CONFIGURE, (UINT)(hwnd), 0)
#define ICDecompressOpen(fccType, fccHandler, lpbiIn, lpbiOut)                 \
  ICLocate(fccType, fccHandler, lpbiIn, lpbiOut, ICMODE_DECOMPRESS)
#define ICDrawOpen(fccType, fccHandler, lpbiIn)                                \
  ICLocate(fccType, fccHandler, lpbiIn, NULL, ICMODE_DRAW)

/* ---- The MCIWnd window class ---------------------------------------- */

#define MCIWND_WINDOW_CLASS "MCIWndClass"

#define MCIWNDOPENF_NEW 0x0001

#define MCIWNDF_NOAUTOSIZEWINDOW 0x0001
#define MCIWNDF_NOPLAYBAR 0x0002
#define MCIWNDF_NOAUTOSIZEMOVIE 0x0004
#define MCIWNDF_NOMENU 0x0008
#define MCIWNDF_SHOWNAME 0x0010
#define MCIWNDF_SHOWPOS 0x0020
#define MCIWNDF_SHOWMODE 0x0040
#define MCIWNDF_SHOWALL 0x0070
#define MCIWNDF_NOTIFYMODE 0x0100
#define MCIWNDF_NOTIFYPOS 0x0200
#define MCIWNDF_NOTIFYSIZE 0x0400
#define MCIWNDF_NOTIFYERROR 0x1000
#define MCIWNDF_NOTIFYALL 0x1F00
#define MCIWNDF_NOTIFYMEDIA 0x0880
#define MCIWNDF_RECORD 0x2000
#define MCIWNDF_NOERRORDLG 0x4000
#define MCIWNDF_NOOPEN 0x8000

#define MCIWNDM_GETDEVICEID (WM_USER + 100)
#define MCIWNDM_GETSTART (WM_USER + 103)
#define MCIWNDM_GETLENGTH (WM_USER + 104)
#define MCIWNDM_GETEND (WM_USER + 105)
#define MCIWNDM_EJECT (WM_USER + 107)
#define MCIWNDM_SETZOOM (WM_USER + 108)
#define MCIWNDM_GETZOOM (WM_USER + 109)
#define MCIWNDM_SETVOLUME (WM_USER + 110)
#define MCIWNDM_GETVOLUME (WM_USER + 111)
#define MCIWNDM_SETSPEED (WM_USER + 112)
#define MCIWNDM_GETSPEED (WM_USER + 113)
#define MCIWNDM_SETREPEAT (WM_USER + 114)
#define MCIWNDM_GETREPEAT (WM_USER + 115)
#define MCIWNDM_REALIZE (WM_USER + 118)
#define MCIWNDM_VALIDATEMEDIA (WM_USER + 121)
#define MCIWNDM_PLAYFROM (WM_USER + 122)
#define MCIWNDM_PLAYTO (WM_USER + 123)
#define MCIWNDM_GETPALETTE (WM_USER + 126)
#define MCIWNDM_SETPALETTE (WM_USER + 127)
#define MCIWNDM_SETTIMERS (WM_USER + 129)
#define MCIWNDM_SETACTIVETIMER (WM_USER + 130)
#define MCIWNDM_SETINACTIVETIMER (WM_USER + 131)
#define MCIWNDM_GETACTIVETIMER (WM_USER + 132)
#define MCIWNDM_GETINACTIVETIMER (WM_USER + 133)
#define MCIWNDM_NEWDEVICE (WM_USER + 134)
#define MCIWNDM_CHANGESTYLES (WM_USER + 135)
#define MCIWNDM_GETSTYLES (WM_USER + 136)
#define MCIWNDM_GETALIAS (WM_USER + 137)
#define MCIWNDM_RETURNSTRING (WM_USER + 138)
#define MCIWNDM_PLAYREVERSE (WM_USER + 139)
#define MCIWNDM_GET_SOURCE (WM_USER + 140)
#define MCIWNDM_PUT_SOURCE (WM_USER + 141)
#define MCIWNDM_GET_DEST (WM_USER + 142)
#define MCIWNDM_PUT_DEST (WM_USER + 143)
#define MCIWNDM_CAN_PLAY (WM_USER + 144)
#define MCIWNDM_CAN_WINDOW (WM_USER + 145)
#define MCIWNDM_CAN_RECORD (WM_USER + 146)
#define MCIWNDM_CAN_SAVE (WM_USER + 147)
#define MCIWNDM_CAN_EJECT (WM_USER + 148)
#define MCIWNDM_CAN_CONFIG (WM_USER + 149)
#define MCIWNDM_PALETTEKICK (WM_USER + 150)
#define MCIWNDM_OPENINTERFACE (WM_USER + 151)
#define MCIWNDM_SETOWNER (WM_USER + 152)
#define MCIWNDM_SENDSTRING (WM_USER + 101)
#define MCIWNDM_GETPOSITION (WM_USER + 102)
#define MCIWNDM_GETMODE (WM_USER + 106)
#define MCIWNDM_SETTIMEFORMAT (WM_USER + 119)
#define MCIWNDM_GETTIMEFORMAT (WM_USER + 120)
#define MCIWNDM_GETFILENAME (WM_USER + 124)
#define MCIWNDM_GETDEVICE (WM_USER + 125)
#define MCIWNDM_GETERROR (WM_USER + 128)
#define MCIWNDM_OPEN (WM_USER + 153)

#define MCIWNDM_NOTIFYMODE (WM_USER + 200)
#define MCIWNDM_NOTIFYPOS (WM_USER + 201)
#define MCIWNDM_NOTIFYSIZE (WM_USER + 202)
#define MCIWNDM_NOTIFYMEDIA (WM_USER + 203)
#define MCIWNDM_NOTIFYERROR (WM_USER + 205)

#define MCIWND_START (-1)
#define MCIWND_END (-2)

/* MCIWndCreate makes an MCIWnd window and opens szFile in it. */
HWND VFWAPIV MCIWndCreate(HWND hwndParent, HINSTANCE hInstance,
                          DWORD dwStyle, LPCSTR szFile);
BOOL VFWAPIV MCIWndRegisterClass(void);

#define MCIWndSM SendMessage
#define MCIWndCanPlay(hwnd) (BOOL)MCIWndSM(hwnd, MCIWNDM_CAN_PLAY, 0, 0)
#define MCIWndCanRecord(hwnd) (BOOL)MCIWndSM(hwnd, MCIWNDM_CAN_RECORD, 0, 0)
#define MCIWndCanSave(hwnd) (BOOL)MCIWndSM(hwnd, MCIWNDM_CAN_SAVE, 0, 0)
#define MCIWndCanWindow(hwnd) (BOOL)MCIWndSM(hwnd, MCIWNDM_CAN_WINDOW, 0, 0)
#define MCIWndCanEject(hwnd) (BOOL)MCIWndSM(hwnd, MCIWNDM_CAN_EJECT, 0, 0)
#define MCIWndCanConfig(hwnd) (BOOL)MCIWndSM(hwnd, MCIWNDM_CAN_CONFIG, 0, 0)
#define MCIWndOpen(hwnd, sz, f)                                                \
  (LONG)MCIWndSM(hwnd, MCIWNDM_OPEN, (WPARAM)(UINT)(f), (LPARAM)(LPVOID)(sz))
#define MCIWndOpenDialog(hwnd) MCIWndOpen(hwnd, -1, 0)
#define MCIWndNew(hwnd, lp)                                                    \
  (LONG)MCIWndSM(hwnd, MCIWNDM_NEWDEVICE, 0, (LPARAM)(LPVOID)(lp))
#define MCIWndClose(hwnd) (LONG)MCIWndSM(hwnd, MCI_CLOSE, 0, 0)
#define MCIWndPlay(hwnd) (LONG)MCIWndSM(hwnd, MCI_PLAY, 0, 0)
#define MCIWndStop(hwnd) (LONG)MCIWndSM(hwnd, MCI_STOP, 0, 0)
#define MCIWndPause(hwnd) (LONG)MCIWndSM(hwnd, MCI_PAUSE, 0, 0)
#define MCIWndResume(hwnd) (LONG)MCIWndSM(hwnd, MCI_RESUME, 0, 0)
#define MCIWndSeek(hwnd, lPos) (LONG)MCIWndSM(hwnd, MCI_SEEK, 0, (LPARAM)(LONG)(lPos))
#define MCIWndHome(hwnd) MCIWndSeek(hwnd, MCIWND_START)
#define MCIWndEnd(hwnd) MCIWndSeek(hwnd, MCIWND_END)
#define MCIWndRecord(hwnd) (LONG)MCIWndSM(hwnd, MCI_RECORD, 0, 0)
#define MCIWndSave(hwnd, sz) (LONG)MCIWndSM(hwnd, MCI_SAVE, 0, (LPARAM)(LPVOID)(sz))
#define MCIWndGetPosition(hwnd) (LONG)MCIWndSM(hwnd, MCIWNDM_GETPOSITION, 0, 0)
#define MCIWndGetLength(hwnd) (LONG)MCIWndSM(hwnd, MCIWNDM_GETLENGTH, 0, 0)
#define MCIWndGetMode(hwnd, lp, len)                                           \
  (LONG)MCIWndSM(hwnd, MCIWNDM_GETMODE, (WPARAM)(UINT)(len), (LPARAM)(LPVOID)(lp))
#define MCIWndDestroy(hwnd) (LONG)MCIWndSM(hwnd, WM_CLOSE, 0, 0)

/* ---- AVI files and streams (AVIFILE.DLL) ---------------------------- */

#define streamtypeVIDEO mmioFOURCC('v', 'i', 'd', 's')
#define streamtypeAUDIO mmioFOURCC('a', 'u', 'd', 's')
#define streamtypeMIDI mmioFOURCC('m', 'i', 'd', 's')
#define streamtypeTEXT mmioFOURCC('t', 'x', 't', 's')

#define AVIIF_LIST 0x00000001L
#define AVIIF_KEYFRAME 0x00000010L
#define AVIIF_NOTIME 0x00000100L

#define AVIFILEINFO_HASINDEX 0x00000010
#define AVIFILEINFO_MUSTUSEINDEX 0x00000020
#define AVIFILEINFO_ISINTERLEAVED 0x00000100
#define AVIFILEINFO_WASCAPTUREFILE 0x00010000
#define AVIFILEINFO_COPYRIGHTED 0x00020000

#define AVIFILECAPS_CANREAD 0x00000001
#define AVIFILECAPS_CANWRITE 0x00000002
#define AVIFILECAPS_ALLKEYFRAMES 0x00000010
#define AVIFILECAPS_NOCOMPRESSION 0x00000020

#define AVISTREAMINFO_DISABLED 0x00000001
#define AVISTREAMINFO_FORMATCHANGES 0x00010000

#define AVISTREAMREAD_CONVENIENT (-1L)

/* The flags of AVIStreamFindSample. */
#define FIND_DIR 0x0000000FL
#define FIND_NEXT 0x00000001L
#define FIND_PREV 0x00000004L
#define FIND_TYPE 0x000000F0L
#define FIND_KEY 0x00000010L
#define FIND_ANY 0x00000020L
#define FIND_FORMAT 0x00000040L
#define FIND_RET 0x0000F000L
#define FIND_POS 0x00000000L
#define FIND_LENGTH 0x00001000L
#define FIND_OFFSET 0x00002000L
#define FIND_SIZE 0x00003000L
#define FIND_INDEX 0x00004000L

/* The flags of AVICOMPRESSOPTIONS. */
#define AVICOMPRESSF_INTERLEAVE 0x00000001
#define AVICOMPRESSF_DATARATE 0x00000002
#define AVICOMPRESSF_KEYFRAMES 0x00000004
#define AVICOMPRESSF_VALID 0x00000008

#define ICMF_CHOOSE_KEYFRAME 0x0001
#define ICMF_CHOOSE_DATARATE 0x0002
#define ICMF_CHOOSE_PREVIEW 0x0004
#define ICMF_CHOOSE_ALLCOMPRESSORS 0x0008

/* The AVI error codes. */
#define MAKE_AVIERR(error) MAKE_SCODE(SEVERITY_ERROR, FACILITY_ITF, 0x4000 + (error))
#define AVIERR_OK 0L
#define AVIERR_UNSUPPORTED MAKE_AVIERR(101)
#define AVIERR_BADFORMAT MAKE_AVIERR(102)
#define AVIERR_MEMORY MAKE_AVIERR(103)
#define AVIERR_INTERNAL MAKE_AVIERR(104)
#define AVIERR_BADFLAGS MAKE_AVIERR(105)
#define AVIERR_BADPARAM MAKE_AVIERR(106)
#define AVIERR_BADSIZE MAKE_AVIERR(107)
#define AVIERR_BADHANDLE MAKE_AVIERR(108)
#define AVIERR_FILEREAD MAKE_AVIERR(109)
#define AVIERR_FILEWRITE MAKE_AVIERR(110)
#define AVIERR_FILEOPEN MAKE_AVIERR(111)
#define AVIERR_COMPRESSOR MAKE_AVIERR(112)
#define AVIERR_NOCOMPRESSOR MAKE_AVIERR(113)
#define AVIERR_READONLY MAKE_AVIERR(114)
#define AVIERR_NODATA MAKE_AVIERR(115)
#define AVIERR_BUFFERTOOSMALL MAKE_AVIERR(116)
#define AVIERR_CANTCOMPRESS MAKE_AVIERR(117)
#define AVIERR_USERABORT MAKE_AVIERR(198)
#define AVIERR_ERROR MAKE_AVIERR(199)

/* The data of AVIFileInfo. */
typedef struct _AVIFILEINFO {
  DWORD dwMaxBytesPerSec;
  DWORD dwFlags;
  DWORD dwCaps;
  DWORD dwStreams;
  DWORD dwSuggestedBufferSize;
  DWORD dwWidth;
  DWORD dwHeight;
  DWORD dwScale;
  DWORD dwRate;
  DWORD dwLength;
  DWORD dwEditCount;
  char szFileType[64];
} AVIFILEINFO, FAR *LPAVIFILEINFO;

/* The data of AVIStreamInfo and AVIFileCreateStream. rcFrame is a 16-bit
 * RECT. */
typedef struct _AVISTREAMINFO {
  DWORD fccType;
  DWORD fccHandler;
  DWORD dwFlags;
  DWORD dwCaps;
  WORD wPriority;
  WORD wLanguage;
  DWORD dwScale;
  DWORD dwRate;
  DWORD dwStart;
  DWORD dwLength;
  DWORD dwInitialFrames;
  DWORD dwSuggestedBufferSize;
  DWORD dwQuality;
  DWORD dwSampleSize;
  RECT rcFrame;
  DWORD dwEditCount;
  DWORD dwFormatChangeCount;
  char szName[64];
} AVISTREAMINFO, FAR *LPAVISTREAMINFO;

/* The compression options of AVISaveOptions and AVIMakeCompressedStream. */
typedef struct {
  DWORD fccType;
  DWORD fccHandler;
  DWORD dwKeyFrameEvery;
  DWORD dwQuality;
  DWORD dwBytesPerSecond;
  DWORD dwFlags;
  LPVOID lpFormat;
  DWORD cbFormat;
  LPVOID lpParms;
  DWORD cbParms;
  DWORD dwInterleaveEvery;
} AVICOMPRESSOPTIONS, FAR *LPAVICOMPRESSOPTIONS;

/* The callback of AVISave: it returns TRUE to stop the save. */
typedef BOOL(FAR PASCAL *AVISAVECALLBACK)(int nPercent);

typedef struct IAVIFile IAVIFile;
typedef struct IAVIStream IAVIStream;
typedef struct IGetFrame IGetFrame;
typedef IAVIFile FAR *PAVIFILE;
typedef IAVIStream FAR *PAVISTREAM;
typedef IGetFrame FAR *PGETFRAME;

DEFINE_OLEGUID(IID_IAVIFile, 0x00020020, 0, 0);
DEFINE_OLEGUID(IID_IAVIStream, 0x00020021, 0, 0);
DEFINE_OLEGUID(IID_IGetFrame, 0x00020023, 0, 0);
DEFINE_OLEGUID(IID_IAVIEditStream, 0x00020024, 0, 0);
DEFINE_OLEGUID(CLSID_AVISimpleUnMarshal, 0x00020009, 0, 0);

void VFWAPI AVIFileInit(void);
void VFWAPI AVIFileExit(void);
HRESULT VFWAPI AVIFileOpen(PAVIFILE FAR *ppfile, LPCSTR szFile, UINT uMode,
                           CLSID FAR *lpHandler);
HRESULT VFWAPI AVIStreamOpenFromFile(PAVISTREAM FAR *ppavi, LPCSTR szFile,
                                     DWORD fccType, LONG lParam, UINT mode,
                                     CLSID FAR *pclsidHandler);
HRESULT VFWAPI AVIStreamCreate(PAVISTREAM FAR *ppavi, LONG lParam1,
                               LONG lParam2, CLSID FAR *pclsidHandler);
HRESULT VFWAPI AVIMakeCompressedStream(PAVISTREAM FAR *ppsCompressed,
                                       PAVISTREAM ppsSource,
                                       AVICOMPRESSOPTIONS FAR *lpOptions,
                                       CLSID FAR *pclsidHandler);
HRESULT VFWAPI AVIMakeFileFromStreams(PAVIFILE FAR *ppfile, int nStreams,
                                      PAVISTREAM FAR *papStreams);
LPVOID VFWAPI AVIStreamGetFrame(PGETFRAME pg, LONG lPos);
HRESULT VFWAPI AVIStreamGetFrameClose(PGETFRAME pg);
PGETFRAME VFWAPI AVIStreamGetFrameOpen(PAVISTREAM pavi,
                                       LPBITMAPINFOHEADER lpbiWanted);
HRESULT VFWAPIV AVISave(LPCSTR szFile, CLSID FAR *pclsidHandler,
                        AVISAVECALLBACK lpfnCallback, int nStreams,
                        PAVISTREAM pfile, LPAVICOMPRESSOPTIONS lpOptions,
                        ...);
HRESULT VFWAPI AVISaveV(LPCSTR szFile, CLSID FAR *pclsidHandler,
                        AVISAVECALLBACK lpfnCallback, int nStreams,
                        PAVISTREAM FAR *ppavi,
                        LPAVICOMPRESSOPTIONS FAR *plpOptions);
BOOL VFWAPI AVISaveOptions(HWND hwnd, UINT uiFlags, int nStreams,
                           PAVISTREAM FAR *ppavi,
                           LPAVICOMPRESSOPTIONS FAR *plpOptions);
HRESULT VFWAPI AVIBuildFilter(LPSTR lpszFilter, LONG cbFilter, BOOL fSaving);
HRESULT VFWAPI AVISaveOptionsFree(int nStreams,
                                  LPAVICOMPRESSOPTIONS FAR *plpOptions);
LONG VFWAPI AVIStreamStart(PAVISTREAM pavi);
LONG VFWAPI AVIStreamLength(PAVISTREAM pavi);
LONG VFWAPI AVIStreamTimeToSample(PAVISTREAM pavi, LONG lTime);
LONG VFWAPI AVIStreamSampleToTime(PAVISTREAM pavi, LONG lSample);
ULONG VFWAPI AVIFileAddRef(PAVIFILE pfile);
ULONG VFWAPI AVIFileRelease(PAVIFILE pfile);
HRESULT VFWAPI AVIFileInfo(PAVIFILE pfile, AVIFILEINFO FAR *pfi, LONG lSize);
HRESULT VFWAPI AVIFileGetStream(PAVIFILE pfile, PAVISTREAM FAR *ppavi,
                                DWORD fccType, LONG lParam);
HRESULT VFWAPI AVIFileCreateStream(PAVIFILE pfile, PAVISTREAM FAR *ppavi,
                                   AVISTREAMINFO FAR *psi);
HRESULT VFWAPI AVIFileWriteData(PAVIFILE pfile, DWORD ckid, LPVOID lpData,
                                LONG cbData);
HRESULT VFWAPI AVIFileReadData(PAVIFILE pfile, DWORD ckid, LPVOID lpData,
                               LONG FAR *lpcbData);
HRESULT VFWAPI AVIFileEndRecord(PAVIFILE pfile);
ULONG VFWAPI AVIStreamAddRef(PAVISTREAM pavi);
ULONG VFWAPI AVIStreamRelease(PAVISTREAM pavi);
HRESULT VFWAPI AVIStreamInfo(PAVISTREAM pavi, AVISTREAMINFO FAR *psi,
                             LONG lSize);
LONG VFWAPI AVIStreamFindSample(PAVISTREAM pavi, LONG lPos, LONG lFlags);
HRESULT VFWAPI AVIStreamReadFormat(PAVISTREAM pavi, LONG lPos,
                                   LPVOID lpFormat, LONG FAR *lpcbFormat);
HRESULT VFWAPI AVIStreamReadData(PAVISTREAM pavi, DWORD fcc, LPVOID lp,
                                 LONG FAR *lpcb);
HRESULT VFWAPI AVIStreamWriteData(PAVISTREAM pavi, DWORD fcc, LPVOID lp,
                                  LONG cb);
HRESULT VFWAPI AVIStreamRead(PAVISTREAM pavi, LONG lStart, LONG lSamples,
                             LPVOID lpBuffer, LONG cbBuffer,
                             LONG FAR *plBytes, LONG FAR *plSamples);
HRESULT VFWAPI AVIStreamWrite(PAVISTREAM pavi, LONG lStart, LONG lSamples,
                              LPVOID lpBuffer, LONG cbBuffer, DWORD dwFlags,
                              LONG FAR *plSampWritten,
                              LONG FAR *plBytesWritten);
HRESULT VFWAPI AVIStreamSetFormat(PAVISTREAM pavi, LONG lPos,
                                  LPVOID lpFormat, LONG cbFormat);
HRESULT VFWAPI AVIPutFileOnClipboard(PAVIFILE pf);
HRESULT VFWAPI AVIGetFromClipboard(PAVIFILE FAR *lppf);
HRESULT VFWAPI AVIClearClipboard(void);

#define AVIStreamFormatSize(pavi, lPos, plSize)                                \
  AVIStreamReadFormat(pavi, lPos, NULL, plSize)
#define AVIStreamEnd(pavi) (AVIStreamStart(pavi) + AVIStreamLength(pavi))
#define AVIStreamNextKeyFrame(pavi, l)                                         \
  AVIStreamFindSample(pavi, (l) + 1, FIND_NEXT | FIND_KEY)
#define AVIStreamPrevKeyFrame(pavi, l)                                         \
  AVIStreamFindSample(pavi, (l) - 1, FIND_PREV | FIND_KEY)
#define AVIStreamIsKeyFrame(pavi, l) (AVIStreamNearestKeyFrame(pavi, l) == (l))
#define AVIStreamNearestKeyFrame(pavi, l)                                      \
  AVIStreamFindSample(pavi, l, FIND_PREV | FIND_KEY)

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
#endif /* WINVER >= 0x030A */
#endif /* RC_INVOKED */

#endif /* _INC_VFW */
