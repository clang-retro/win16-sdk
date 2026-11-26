/* penwin.h: Pen Windows (PENWIN.DLL), Windows for Pen Computing 1.0 on
 * Windows 3.1, and with PENVER 0x0200 the 16-bit API of Pen Services 2.0.
 * Written for clang-retro; AGPL-3.0-only, see the win16-sdk LICENSE.
 * The types and constants come from mingw-w64's penwin.h, which is in the
 * public domain (see NOTICE.md).
 *
 * PENVER (default 0x0100) is the Pen Windows version that the program
 * needs. The functions of Pen Windows 1.0 are always declared. The
 * functions that only Pen Services 2.0 has need PENVER 0x0200. A pointer
 * in a structure or a parameter is a far pointer, as Pen Windows expects
 * in every memory model. */
#ifndef _INC_PENWIN
#define _INC_PENWIN

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PENVER
#define PENVER 0x0100
#endif

/* A rectangle with 32-bit coordinates, which the 16-bit windows.h lacks. */
#ifndef _RECTL_DEFINED
#define _RECTL_DEFINED
typedef struct tagRECTL {
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECTL, FAR *LPRECTL;
#endif


#ifndef NOPENAPPS
#ifndef RC_INVOKED
#include <skbapi.h>
#endif
#endif

#ifdef NOPENCTL
#define NOPENBEDIT
#define NOPENIEDIT
#endif

#ifdef NOPENRES
#define NOPENBMP
#define NOPENCURS
#endif

#ifndef NOPENALC

#define ALC_DEFAULT (0x00000000L)
#define ALC_LCALPHA (0x00000001L)
#define ALC_UCALPHA (0x00000002L)
#define ALC_NUMERIC (0x00000004L)
#define ALC_PUNC (0x00000008L)
#define ALC_MATH (0x00000010L)
#define ALC_MONETARY (0x00000020L)
#define ALC_OTHER (0x00000040L)
#define ALC_ASCII (0x00000080L)
#define ALC_WHITE (0x00000100L)
#define ALC_NONPRINT (0x00000200L)
#define ALC_DBCS (0x00000400L)
#define ALC_JIS1 (0x00000800L)
#define ALC_GESTURE (0x00004000L)
#define ALC_USEBITMAP (0x00008000L)
#define ALC_HIRAGANA (0x00010000L)
#define ALC_KATAKANA (0x00020000L)
#define ALC_KANJI (0x00040000L)
#define ALC_GLOBALPRIORITY (0x10000000L)
#define ALC_OEM (0x0FF80000L)
#if PENVER >= 0x0200
#define ALC_RESERVED (0xE0003000L)
#else
#define ALC_RESERVED (0xF0003800L)
#endif
#define ALC_NOPRIORITY (0x00000000L)

#define ALC_ALPHA (ALC_LCALPHA | ALC_UCALPHA)
#define ALC_ALPHANUMERIC (ALC_ALPHA | ALC_NUMERIC)
#define ALC_SYSMINIMUM (ALC_ALPHANUMERIC | ALC_PUNC | ALC_WHITE | ALC_GESTURE)
#if PENVER >= 0x0200
#define ALC_ALL (ALC_SYSMINIMUM | ALC_MATH | ALC_MONETARY | ALC_OTHER | ALC_NONPRINT)
#else
#define ALC_ALL (0x000043FFL)
#endif
#define ALC_KANJISYSMINIMUM (ALC_SYSMINIMUM | ALC_HIRAGANA | ALC_KATAKANA | ALC_JIS1)
#define ALC_KANJIALL (ALC_ALL | ALC_HIRAGANA | ALC_KATAKANA | ALC_KANJI)
#endif

#ifndef NOPENBEDIT

#define BXS_NONE 0x0000U
#define BXS_RECT 0x0001U
#define BXS_BOXCROSS 0x0004U
#if PENVER >= 0x0200
#define BXS_MASK 0x0007U
#else
#define BXS_ENDTEXTMARK 0x0002U
#define BXS_MASK 0x0003U
#endif
#endif

#ifndef NOPENBMP

#define OBM_SKBBTNUP 32767
#define OBM_SKBBTNDOWN 32766
#define OBM_SKBBTNDISABLED 32765

#define OBM_ZENBTNUP 32764
#define OBM_ZENBTNDOWN 32763
#define OBM_ZENBTNDISABLED 32762

#define OBM_HANBTNUP 32761
#define OBM_HANBTNDOWN 32760
#define OBM_HANBTNDISABLED 32759

#define OBM_KKCBTNUP 32758
#define OBM_KKCBTNDOWN 32757
#define OBM_KKCBTNDISABLED 32756

#define OBM_SIPBTNUP 32755
#define OBM_SIPBTNDOWN 32754
#define OBM_SIPBTNDISABLED 32753

#define OBM_PTYBTNUP 32752
#define OBM_PTYBTNDOWN 32751
#define OBM_PTYBTNDISABLED 32750
#endif

#ifndef NOPENCURS

#define IDC_PEN MAKEINTRESOURCE(32631)

#define IDC_ALTSELECT MAKEINTRESOURCE(32501)
#endif

#ifndef NOPENHRC

#define SYV_NULL (0x00000000L)
#define SYV_UNKNOWN (0x00000001L)
#define SYV_EMPTY (0x00000003L)
#define SYV_BEGINOR (0x00000010L)
#define SYV_ENDOR (0x00000011L)
#define SYV_OR (0x00000012L)
#define SYV_SOFTNEWLINE (0x00000020L)
#define SYV_SPACENULL (0x00010000L)

#define SYV_SELECTFIRST (0x0002FFC0L)
#define SYV_LASSO (0x0002FFC1L)
#define SYV_SELECTLEFT (0x0002FFC2L)
#define SYV_SELECTRIGHT (0x0002FFC3L)
#define SYV_SELECTLAST (0x0002FFCFL)

#define SYV_CLEARCHAR (0x0002FFD2L)
#define SYV_HELP (0x0002FFD3L)
#define SYV_KKCONVERT (0x0002FFD4L)
#define SYV_CLEAR (0x0002FFD5L)
#define SYV_INSERT (0x0002FFD6L)
#define SYV_CONTEXT (0x0002FFD7L)
#define SYV_EXTENDSELECT (0x0002FFD8L)
#define SYV_UNDO (0x0002FFD9L)
#define SYV_COPY (0x0002FFDAL)
#define SYV_CUT (0x0002FFDBL)
#define SYV_PASTE (0x0002FFDCL)
#define SYV_CLEARWORD (0x0002FFDDL)
#define SYV_USER (0x0002FFDEL)
#define SYV_CORRECT (0x0002FFDFL)

#define SYV_BACKSPACE (0x00020008L)
#define SYV_TAB (0x00020009L)
#define SYV_RETURN (0x0002000DL)
#define SYV_SPACE (0x00020020L)

#define SYV_APPGESTUREMASK (0x00020000L)
#define SYV_CIRCLEUPA (0x000224B6L)
#define SYV_CIRCLEUPZ (0x000224CFL)
#define SYV_CIRCLELOA (0x000224D0L)
#define SYV_CIRCLELOZ (0x000224E9L)

#define SYV_SHAPELINE (0x00040001L)
#define SYV_SHAPEELLIPSE (0x00040002L)
#define SYV_SHAPERECT (0x00040003L)
#define SYV_SHAPEMIN SYV_SHAPELINE
#define SYV_SHAPEMAX SYV_SHAPERECT

#define SYVHI_SPECIAL 0
#define SYVHI_ANSI 1
#define SYVHI_GESTURE 2
#define SYVHI_KANJI 3
#define SYVHI_SHAPE 4
#define SYVHI_UNICODE 5
#define SYVHI_VKEY 6
#endif

#ifndef NOPENIEDIT

#define IEM_UNDO 1
#define IEM_CUT 2
#define IEM_COPY 3
#define IEM_PASTE 4
#define IEM_CLEAR 5
#define IEM_SELECTALL 6
#define IEM_ERASE 7
#define IEM_PROPERTIES 8
#define IEM_LASSO 9
#define IEM_RESIZE 10

#define IEM_USER 100

#define IES_BORDER 0x0001
#define IES_HSCROLL 0x0002
#define IES_VSCROLL 0x0004
#define IES_OWNERDRAW 0x0008
#endif

#ifndef RC_INVOKED

#ifndef NOPENDATA

#define AI_CBSTROKE 0xFFFF

#define AI_SKIPUPSTROKES 0x0001

#define CMPD_COMPRESS 0x0001
#define CMPD_DECOMPRESS 0x0002

#define CPDR_BOX 1
#define CPDR_LASSO 2

#define CPD_DEFAULT 0x047F
#define CPD_USERBYTE 0x0100
#define CPD_USERWORD 0x0200
#define CPD_USERDWORD 0x0300
#define CPD_TIME 0x0400

#define DPD_HDCPEN 0x0001
#define DPD_DRAWSEL 0x0002

#define EPDP_REMOVE 0x0001

#define EPDS_SELECT 1
#define EPDS_STROKEINDEX 2
#define EPDS_USER 3
#define EPDS_PENTIP 4
#define EPDS_TIPCOLOR 5
#define EPDS_TIPWIDTH 6
#define EPDS_TIPNIB 7
#define EPDS_INKSET 8

#define EPDS_EQ 0x0000
#define EPDS_LT 0x0010
#define EPDS_GT 0x0020
#define EPDS_NOT 0x0040
#define EPDS_NE 0x0040
#define EPDS_GTE 0x0050
#define EPDS_LTE 0x0060

#define EPDS_REMOVE 0x8000

#define GPA_MAXLEN 1
#define GPA_POINTS 2
#define GPA_PDTS 3
#define GPA_RATE 4
#define GPA_RECTBOUND 5
#define GPA_RECTBOUNDINK 6
#define GPA_SIZE 7
#define GPA_STROKES 8
#define GPA_TIME 9
#define GPA_USER 10
#define GPA_VERSION 11

#define GSA_PENTIP 1
#define GSA_PENTIPCLASS 2
#define GSA_USER 3
#define GSA_USERCLASS 4
#define GSA_TIME 5
#define GSA_SIZE 6
#define GSA_SELECT 7
#define GSA_DOWN 8
#define GSA_RECTBOUND 9

#define GSA_PENTIPTABLE 10
#define GSA_SIZETABLE 11
#define GSA_USERTABLE 12

#ifndef IX_END
#define IX_END 0xFFFF
#endif

#define PENTIP_NIBDEFAULT ((BYTE)0)
#define PENTIP_HEIGHTDEFAULT ((BYTE)0)
#define PENTIP_OPAQUE ((BYTE)0xFF)
#define PENTIP_HILITE ((BYTE)0x80)
#define PENTIP_TRANSPARENT ((BYTE)0)

#define PDR_NOHIT 3
#define PDR_HIT 2
#define PDR_OK 1
#define PDR_CANCEL 0

#define PDR_ERROR (-1)
#define PDR_PNDTERR (-2)
#define PDR_VERSIONERR (-3)
#define PDR_COMPRESSED (-4)
#define PDR_STRKINDEXERR (-5)
#define PDR_PNTINDEXERR (-6)
#define PDR_MEMERR (-7)
#define PDR_INKSETERR (-8)
#define PDR_ABORT (-9)
#define PDR_NA (-10)

#define PDR_USERDATAERR (-16)
#define PDR_SCALINGERR (-17)
#define PDR_TIMESTAMPERR (-18)
#define PDR_OEMDATAERR (-19)
#define PDR_SCTERR (-20)

#define PDTS_LOMETRIC 0
#define PDTS_HIMETRIC 1
#define PDTS_HIENGLISH 2
#define PDTS_STANDARDSCALE 2
#define PDTS_DISPLAY 3
#define PDTS_ARBITRARY 4
#define PDTS_SCALEMASK 0x000F

#define PDTT_DEFAULT 0x0000
#define PDTT_PENINFO 0x0100
#define PDTT_UPPOINTS 0x0200
#define PDTT_OEMDATA 0x0400
#define PDTT_COLLINEAR 0x0800
#define PDTT_COLINEAR 0x0800
#define PDTT_DECOMPRESS 0x4000
#define PDTT_COMPRESS 0x8000
#define PDTT_ALL 0x0F00

#define PHW_NONE 0x0000
#define PHW_PRESSURE 0x0001
#define PHW_HEIGHT 0x0002
#define PHW_ANGLEXY 0x0004
#define PHW_ANGLEZ 0x0008
#define PHW_BARRELROTATION 0x0010
#define PHW_OEMSPECIFIC 0x0020
#define PHW_PDK 0x0040
#define PHW_ALL 0x007F

#define PDTS_COMPRESS2NDDERIV 0x0010
#define PDTS_COMPRESSMETHOD 0x00F0
#define PDTS_NOPENINFO 0x0100
#define PDTS_NOUPPOINTS 0x0200
#define PDTS_NOOEMDATA 0x0400
#define PDTS_NOCOLLINEAR 0x0800
#define PDTS_NOCOLINEAR 0x0800
#define PDTS_NOTICK 0x1000
#define PDTS_NOUSER 0x2000
#define PDTS_NOEMPTYSTROKES 0x4000
#define PDTS_COMPRESSED 0x8000

#define SSA_PENTIP 1
#define SSA_PENTIPCLASS 2
#define SSA_USER 3
#define SSA_USERCLASS 4
#define SSA_TIME 5
#define SSA_SELECT 6
#define SSA_DOWN 7

#define SSA_PENTIPTABLE 8
#define SSA_USERTABLE 9

#define TIP_ERASECOLOR 1

#define TPD_RECALCSIZE 0x0000
#define TPD_USER 0x0080
#define TPD_TIME 0x0100
#define TPD_UPPOINTS 0x0200
#define TPD_COLLINEAR 0x0400
#define TPD_COLINEAR 0x0400
#define TPD_PENINFO 0x0800
#define TPD_PHW 0x1000
#define TPD_OEMDATA 0x1000
#define TPD_EMPTYSTROKES 0x2000
#define TPD_EVERYTHING 0x3FFF
#endif

#ifndef NOPENDICT

#define cbDictPathMax 255
#define DIRQ_QUERY 1
#define DIRQ_DESCRIPTION 2
#define DIRQ_CONFIGURE 3
#define DIRQ_OPEN 4
#define DIRQ_CLOSE 5
#define DIRQ_SETWORDLISTS 6
#define DIRQ_STRING 7
#define DIRQ_SUGGEST 8
#define DIRQ_ADD 9
#define DIRQ_DELETE 10
#define DIRQ_FLUSH 11
#define DIRQ_RCCHANGE 12
#define DIRQ_SYMBOLGRAPH 13
#define DIRQ_INIT 14
#define DIRQ_CLEANUP 15
#define DIRQ_COPYRIGHT 16
#define DIRQ_USER 4096
#endif

#ifndef NOPENDRIVER

#define BITPENUP 0x8000

#define DRV_SetPenDriverEntryPoints DRV_RESERVED+1
#define DRV_SetEntryPoints DRV_RESERVED+1
#define DRV_RemovePenDriverEntryPoints DRV_RESERVED+2
#define DRV_RemoveEntryPoints DRV_RESERVED+2
#define DRV_SetPenSamplingRate DRV_RESERVED+3
#define DRV_SetPenSamplingDist DRV_RESERVED+4
#define DRV_GetName DRV_RESERVED+5
#define DRV_GetVersion DRV_RESERVED+6
#define DRV_GetPenInfo DRV_RESERVED+7
#define DRV_PenPlayStart DRV_RESERVED+8
#define DRV_PenPlayBack DRV_RESERVED+9
#define DRV_PenPlayStop DRV_RESERVED+10
#define DRV_GetCalibration DRV_RESERVED+11
#define DRV_SetCalibration DRV_RESERVED+12
#define DRV_Reserved1 DRV_RESERVED+13
#define DRV_Reserved2 DRV_RESERVED+14
#define DRV_Query DRV_RESERVED+15
#define DRV_GetPenSamplingRate DRV_RESERVED+16
#define DRV_Calibrate DRV_RESERVED+17

#define PLAY_VERSION_10_DATA 0
#define PLAY_VERSION_20_DATA 1

#define DRV_FAILURE 0x00000000
#define DRV_SUCCESS 0x00000001
#define DRV_BADPARAM1 0xFFFFFFFF
#define DRV_BADPARAM2 0xFFFFFFFE
#define DRV_BADSTRUCT 0xFFFFFFFD

#define PENREG_DEFAULT 0x00000002
#define PENREG_WILLHANDLEMOUSE 0x00000001

#define MAXOEMDATAWORDS 6

#define PCM_PENUP (0x00000001L)
#define PCM_RANGE (0x00000002L)
#define PCM_INVERT (0x00000020L)
#define PCM_RECTEXCLUDE (0x00002000L)
#define PCM_RECTBOUND (0x00004000L)
#define PCM_TIMEOUT (0x00008000L)

#define PCM_RGNBOUND (0x00010000L)
#define PCM_RGNEXCLUDE (0x00020000L)
#define PCM_DOPOLLING (0x00040000L)
#define PCM_TAPNHOLD (0x00080000L)
#define PCM_ADDDEFAULTS RC_LDEFAULTFLAGS

#define PDC_INTEGRATED (0x00000001L)
#define PDC_PROXIMITY (0x00000002L)
#define PDC_RANGE (0x00000004L)
#define PDC_INVERT (0x00000008L)
#define PDC_RELATIVE (0x00000010L)
#define PDC_BARREL1 (0x00000020L)
#define PDC_BARREL2 (0x00000040L)
#define PDC_BARREL3 (0x00000080L)

#define PDK_NULL 0x0000
#define PDK_UP 0x0000
#define PDK_DOWN 0x0001
#define PDK_BARREL1 0x0002
#define PDK_BARREL2 0x0004
#define PDK_BARREL3 0x0008
#define PDK_SWITCHES 0x000f
#define PDK_TRANSITION 0x0010
#define PDK_UNUSED10 0x0020
#define PDK_UNUSED20 0x0040
#define PDK_INVERTED 0x0080
#define PDK_PENIDMASK 0x0F00
#define PDK_UNUSED1000 0x1000
#define PDK_INKSTOPPED 0x2000
#define PDK_OUTOFRANGE 0x4000
#define PDK_DRIVER 0x8000

#define PDK_TIPMASK 0x0001

#define PDT_NULL 0
#define PDT_PRESSURE 1
#define PDT_HEIGHT 2
#define PDT_ANGLEXY 3
#define PDT_ANGLEZ 4
#define PDT_BARRELROTATION 5
#define PDT_OEMSPECIFIC 16

#define PID_CURRENT (UINT)(-1)

#define REC_OEM (-1024)
#define REC_LANGUAGE (-48)
#define REC_GUIDE (-47)
#define REC_PARAMERROR (-46)
#define REC_INVALIDREF (-45)
#define REC_RECTEXCLUDE (-44)
#define REC_RECTBOUND (-43)
#define REC_PCM (-42)
#define REC_RESULTMODE (-41)
#define REC_HWND (-40)
#define REC_ALC (-39)
#define REC_ERRORLEVEL (-38)
#define REC_CLVERIFY (-37)
#define REC_DICT (-36)
#define REC_HREC (-35)
#define REC_BADEVENTREF (-33)
#define REC_NOCOLLECTION (-32)
#define REC_DEBUG (-32)
#define REC_POINTEREVENT (-31)
#define REC_BADHPENDATA (-9)
#define REC_OOM (-8)
#define REC_NOINPUT (-7)
#define REC_NOTABLET (-6)
#define REC_BUSY (-5)
#define REC_BUFFERTOOSMALL (-4)
#define REC_ABORT (-3)
#define REC_NA (-2)
#define REC_OVERFLOW (-1)
#define REC_OK 0
#define REC_TERMBOUND 1
#define REC_TERMEX 2
#define REC_TERMPENUP 3
#define REC_TERMRANGE 4
#define REC_TERMTIMEOUT 5
#define REC_DONE 6
#define REC_TERMOEM 512
#endif

#ifndef NOPENHRC

#define GRH_ALL 0
#define GRH_GESTURE 1
#define GRH_NONGESTURE 2

#define GST_SEL (0x00000001L)
#define GST_CLIP (0x00000002L)
#define GST_WHITE (0x00000004L)
#define GST_EDIT (0x00000010L)
#define GST_SYS (0x00000017L)
#define GST_CIRCLELO (0x00000100L)
#define GST_CIRCLEUP (0x00000200L)
#define GST_CIRCLE (0x00000300L)
#define GST_ALL (0x00000317L)

#define HRCR_NORESULTS 4
#define HRCR_COMPLETE 3
#define HRCR_GESTURE 2
#define HRCR_OK 1
#define HRCR_INCOMPLETE 0
#define HRCR_ERROR (-1)
#define HRCR_MEMERR (-2)
#define HRCR_INVALIDGUIDE (-3)
#define HRCR_INVALIDPNDT (-4)
#define HRCR_UNSUPPORTED (-5)
#define HRCR_CONFLICT (-6)
#define HRCR_HOOKED (-8)

#define HWL_SYSTEM ((HWL)1)

#define ISR_ERROR (-1)
#define ISR_BADINKSET (-2)
#define ISR_BADINDEX (-3)

#ifndef IX_END
#define IX_END 0xFFFF
#endif

#define MAXHOTSPOT 8

#define PH_MAX (0xFFFFFFFFL)
#define PH_DEFAULT (0xFFFFFFFEL)
#define PH_MIN (0xFFFFFFFDL)

#define RHH_STD 0
#define RHH_BOX 1

#define SCH_NONE 0
#define SCH_ADVISE 1
#define SCH_FORCE 2

#define SCIM_INSERT 0
#define SCIM_OVERWRITE 1

#define SRH_HOOKALL (HREC)1

#define SSH_RD 1
#define SSH_RU 2
#define SSH_LD 3
#define SSH_LU 4
#define SSH_DL 5
#define SSH_DR 6
#define SSH_UL 7
#define SSH_UR 8

#define SIH_ALLANSICHAR 1

#define TH_QUERY 0
#define TH_FORCE 1
#define TH_SUGGEST 2

#define TRAIN_NONE 0x0000
#define TRAIN_DEFAULT 0x0001
#define TRAIN_CUSTOM 0x0002
#define TRAIN_BOTH (TRAIN_DEFAULT | TRAIN_CUSTOM)

#define TRAIN_SAVE 0
#define TRAIN_REVERT 1
#define TRAIN_RESET 2

#define WCR_RECOGNAME 0
#define WCR_QUERY 1
#define WCR_CONFIGDIALOG 2
#define WCR_DEFAULT 3
#define WCR_RCCHANGE 4
#define WCR_VERSION 5
#define WCR_TRAIN 6
#define WCR_TRAINSAVE 7
#define WCR_TRAINMAX 8
#define WCR_TRAINDIRTY 9
#define WCR_TRAINCUSTOM 10
#define WCR_QUERYLANGUAGE 11
#define WCR_USERCHANGE 12

#define WCR_PWVERSION 13
#define WCR_GETALCPRIORITY 14
#define WCR_SETALCPRIORITY 15
#define WCR_GETANSISTATE 16
#define WCR_SETANSISTATE 17
#define WCR_GETHAND 18
#define WCR_SETHAND 19
#define WCR_GETDIRECTION 20
#define WCR_SETDIRECTION 21
#define WCR_INITRECOGNIZER 22
#define WCR_CLOSERECOGNIZER 23

#define WCR_PRIVATE 1024

#define CRUC_NOTIFY 0
#define CRUC_REMOVE 1

#define WLT_STRING 0
#define WLT_STRINGTABLE 1
#define WLT_EMPTY 2
#define WLT_WORDLIST 3
#endif

#ifndef NOPENIEDIT

#define IEB_DEFAULT 0
#define IEB_BRUSH 1
#define IEB_BIT_UL 2
#define IEB_BIT_CENTER 3
#define IEB_BIT_TILE 4
#define IEB_BIT_STRETCH 5
#define IEB_OWNERDRAW 6

#define IEDO_NONE 0x0000
#define IEDO_FAST 0x0001
#define IEDO_SAVEUPSTROKES 0x0002
#define IEDO_RESERVED 0xFFFC

#define IEI_MOVE 0x0001
#define IEI_RESIZE 0x0002
#define IEI_CROP 0x0004
#define IEI_DISCARD 0x0008
#define IEI_RESERVED 0xFFF0

#define IEGI_ALL 0x0000
#define IEGI_SELECTION 0x0001

#define IEMODE_READY 0
#define IEMODE_ERASE 1
#define IEMODE_LASSO 2

#define IEN_NULL 0x0000
#define IEN_PDEVENT 0x0001
#define IEN_PAINT 0x0002
#define IEN_FOCUS 0x0004
#define IEN_SCROLL 0x0008
#define IEN_EDIT 0x0010
#define IEN_PROPERTIES 0x0020
#define IEN_RESERVED 0xFF80

#define IER_OK 0
#define IER_NO 0
#define IER_YES 1
#define IER_ERROR (-1)
#define IER_PARAMERR (-2)
#define IER_OWNERDRAW (-3)
#define IER_SECURITY (-4)
#define IER_SELECTION (-5)
#define IER_SCALE (-6)
#define IER_MEMERR (-7)
#define IER_NOCOMMAND (-8)
#define IER_NOGESTURE (-9)
#define IER_NOPDEVENT (-10)
#define IER_NOTINPAINT (-11)
#define IER_PENDATA (-12)

#define IEREC_NONE 0x0000
#define IEREC_GESTURE 0x0001
#define IEREC_ALL (IEREC_GESTURE)
#define IEREC_RESERVED 0xFFFE

#define IESEC_NOCOPY 0x0001
#define IESEC_NOCUT 0x0002
#define IESEC_NOPASTE 0x0004
#define IESEC_NOUNDO 0x0008
#define IESEC_NOINK 0x0010
#define IESEC_NOERASE 0x0020
#define IESEC_NOGET 0x0040
#define IESEC_NOSET 0x0080
#define IESEC_RESERVED 0xFF00

#define IESF_ALL 0x0001
#define IESF_SELECTION 0x0002
#define IESF_STROKE 0x0004

#define IESF_TIPCOLOR 0x0008
#define IESF_TIPWIDTH 0x0010
#define IESF_PENTIP (IESF_TIPCOLOR|IESF_TIPWIDTH)

#define IESI_REPLACE 0x0000
#define IESI_APPEND 0x0001

#define IN_PDEVENT ((IEN_PDEVENT<<8)|0)
#define IN_ERASEBKGND ((IEN_NULL<<8)|1)
#define IN_PREPAINT ((IEN_PAINT<<8)|2)
#define IN_PAINT ((IEN_NULL<<8)|3)
#define IN_POSTPAINT ((IEN_PAINT<<8)|4)
#define IN_MODECHANGED ((IEN_EDIT<<8)|5)
#define IN_CHANGE ((IEN_EDIT<<8)|6)
#define IN_UPDATE ((IEN_EDIT<<8)|7)
#define IN_SETFOCUS ((IEN_FOCUS<<8)|8)
#define IN_KILLFOCUS ((IEN_FOCUS<<8)|9)
#define IN_MEMERR ((IEN_NULL<<8)|10)
#define IN_HSCROLL ((IEN_SCROLL<<8)|11)
#define IN_VSCROLL ((IEN_SCROLL<<8)|12)
#define IN_GESTURE ((IEN_EDIT<<8)|13)
#define IN_COMMAND ((IEN_EDIT<<8)|14)
#define IN_CLOSE ((IEN_NULL<<8)|15)
#define IN_PROPERTIES ((IEN_PROPERTIES<<8)|16)
#endif

#ifndef NOPENINKPUT

#define LRET_DONE (1L)
#define LRET_ABORT ((-1L))
#define LRET_HRC ((-2L))
#define LRET_HPENDATA ((-3L))
#define LRET_PRIVATE ((-4L))

#define PCMR_OK 0
#define PCMR_ALREADYCOLLECTING (-1)
#define PCMR_INVALIDCOLLECTION (-2)
#define PCMR_EVENTLOCK (-3)
#define PCMR_INVALID_PACKETID (-4)
#define PCMR_TERMTIMEOUT (-5)
#define PCMR_TERMRANGE (-6)
#define PCMR_TERMPENUP (-7)
#define PCMR_TERMEX (-8)
#define PCMR_TERMBOUND (-9)
#define PCMR_APPTERMINATED (-10)
#define PCMR_TAP (-11)
#define PCMR_SELECT (-12)
#define PCMR_OVERFLOW (-13)
#define PCMR_ERROR (-14)
#define PCMR_DISPLAYERR (-15)
#define PCMR_TERMINVERT (-16)

#define PII_INKCLIPRECT 0x0001
#define PII_INKSTOPRECT 0x0002
#define PII_INKCLIPRGN 0x0004
#define PII_INKSTOPRGN 0x0008
#define PII_INKPENTIP 0x0010
#define PII_SAVEBACKGROUND 0x0020
#define PII_CLIPSTOP 0x0040

#define PIT_RGNBOUND 0x0001
#define PIT_RGNEXCLUDE 0x0002
#define PIT_TIMEOUT 0x0004
#define PIT_TAPNHOLD 0x0008
#endif

#ifndef NOPENMISC

#define CL_NULL 0
#define CL_MINIMUM 1
#define CL_MAXIMUM 100
#define cwRcReservedMax 8
#define ENUM_MINIMUM 1
#define ENUM_MAXIMUM 4096

#define HKP_SETHOOK 0
#define HKP_UNHOOK 0xFFFF

#define HWR_RESULTS 0
#define HWR_APPWIDE 1

#define iSycNull (-1)
#define LPDFNULL ((LPDF)NULL)
#define MAXDICTIONARIES 16
#define wPntAll (UINT)0xFFFF
#define cbRcLanguageMax 44
#define cbRcUserMax 32
#define cbRcrgbfAlcMax 32
#define RC_WDEFAULT 0xffff
#define RC_LDEFAULT (0xffffffffL)
#define RC_WDEFAULTFLAGS 0x8000
#define RC_LDEFAULTFLAGS (0x80000000L)

#define CWR_REPLACECR 0x0001
#define CWR_STRIPCR CWR_REPLACECR
#define CWR_STRIPLF 0x0002
#define CWR_REPLACETAB 0x0004
#define CWR_STRIPTAB CWR_REPLACETAB
#define CWR_SINGLELINEEDIT (CWR_REPLACECR|CWR_STRIPLF|CWR_REPLACETAB)
#define CWR_INSERT 0x0008
#define CWR_TITLE 0x0010
#define CWR_SIMPLE 0x0040
#define CWR_HEDIT 0x0080
#define CWR_KEYBOARD 0x0100
#define CWR_BOXES 0x0200

#define CWRK_DEFAULT 0
#define CWRK_BASIC 1
#define CWRK_FULL 2
#define CWRK_NUMPAD 3
#define CWRK_TELPAD 4



#define INKWIDTH_MINIMUM 0
#define INKWIDTH_MAXIMUM 15

#define PMI_RCCHANGE 0

#define PMI_BEDIT 1
#define PMI_CXTABLET 3
#define PMI_CYTABLET 4
#define PMI_PENTIP 6
#define PMI_ENABLEFLAGS 7
#define PMI_TIMEOUT 8
#define PMI_TIMEOUTGEST 9
#define PMI_TIMEOUTSEL 10
#define PMI_SYSFLAGS 11
#define PMI_INDEXFROMRGB 12
#define PMI_RGBFROMINDEX 13
#define PMI_SYSREC 14
#define PMI_TICKREF 15

#define PMI_SAVE 0x1000


#define PWE_AUTOWRITE 0x0001
#define PWE_ACTIONHANDLES 0x0002
#define PWE_INPUTCURSOR 0x0004
#define PWE_LENS 0x0008

#define PWF_RC1 0x0001
#define PWF_PEN 0x0004
#define PWF_INKDISPLAY 0x0008
#define PWF_RECOGNIZER 0x0010
#define PWF_BEDIT 0x0100
#define PWF_HEDIT 0x0200
#define PWF_IEDIT 0x0400
#define PWF_ENHANCED 0x1000
#define PWF_FULL PWF_RC1|PWF_PEN|PWF_INKDISPLAY|PWF_RECOGNIZER| PWF_BEDIT|PWF_HEDIT |PWF_IEDIT|PWF_ENHANCED

#define RPA_DEFAULT 0x0001
#define RPA_HEDIT 0x0001
#define RPA_KANJIFIXEDBEDIT 0x0002
#define RPA_DBCSPRIORITY 0x0004
#define RPA_SBCSPRIORITY 0x0008

#define PMIR_OK (0L)
#define PMIR_INDEX ((-1L))
#define PMIR_VALUE ((-2L))
#define PMIR_INVALIDBOXEDITINFO ((-3L))
#define PMIR_INIERROR ((-4L))
#define PMIR_ERROR ((-5L))
#define PMIR_NA ((-6L))

#endif

#ifndef NOPENRC1

#define GGRC_OK 0
#define GGRC_DICTBUFTOOSMALL 1
#define GGRC_PARAMERROR 2
#define GGRC_NA 3

#define RCD_DEFAULT 0
#define RCD_LR 1
#define RCD_RL 2
#define RCD_TB 3
#define RCD_BT 4

#define RCIP_ALLANSICHAR 0x0001
#define RCIP_MASK 0x0001

#define RCO_NOPOINTEREVENT (0x00000001L)
#define RCO_SAVEALLDATA (0x00000002L)
#define RCO_SAVEHPENDATA (0x00000004L)
#define RCO_NOFLASHUNKNOWN (0x00000008L)
#define RCO_TABLETCOORD (0x00000010L)
#define RCO_NOSPACEBREAK (0x00000020L)
#define RCO_NOHIDECURSOR (0x00000040L)
#define RCO_NOHOOK (0x00000080L)
#define RCO_BOXED (0x00000100L)
#define RCO_SUGGEST (0x00000200L)
#define RCO_DISABLEGESMAP (0x00000400L)
#define RCO_NOFLASHCURSOR (0x00000800L)
#define RCO_BOXCROSS (0x00001000L)
#define RCO_COLDRECOG (0x00008000L)
#define RCO_SAVEBACKGROUND (0x00010000L)
#define RCO_DODEFAULT (0x00020000L)

#define RCOR_NORMAL 1
#define RCOR_RIGHT 2
#define RCOR_UPSIDEDOWN 3
#define RCOR_LEFT 4

#define RCP_LEFTHAND 0x0001
#define RCP_MAPCHAR 0x0004

#define RCRT_DEFAULT 0x0000
#define RCRT_UNIDENTIFIED 0x0001
#define RCRT_GESTURE 0x0002
#define RCRT_NOSYMBOLMATCH 0x0004
#define RCRT_PRIVATE 0x4000
#define RCRT_NORECOG 0x8000
#define RCRT_ALREADYPROCESSED 0x0008
#define RCRT_GESTURETRANSLATED 0x0010
#define RCRT_GESTURETOKEYS 0x0020

#define RRM_STROKE 0
#define RRM_SYMBOL 1
#define RRM_WORD 2
#define RRM_NEWLINE 3
#define RRM_COMPLETE 16

#define SGRC_OK 0x0000
#define SGRC_USER 0x0001
#define SGRC_PARAMERROR 0x0002
#define SGRC_RC 0x0004
#define SGRC_RECOGNIZER 0x0008
#define SGRC_DICTIONARY 0x0010
#define SGRC_INIFILE 0x0020
#define SGRC_NA 0x8000
#endif

#ifndef NOPENTARGET

#define TPT_CLOSEST 0x0001
#define TPT_INTERSECTINK 0x0002
#define TPT_TEXTUAL 0x0004
#define TPT_DEFAULT (TPT_TEXTUAL | TPT_INTERSECTINK | TPT_CLOSEST)
#endif

#ifndef NOPENVIRTEVENT

#define VWM_MOUSEMOVE 0x0001
#define VWM_MOUSELEFTDOWN 0x0002
#define VWM_MOUSELEFTUP 0x0004
#define VWM_MOUSERIGHTDOWN 0x0008
#define VWM_MOUSERIGHTUP 0x0010
#endif
#endif

#ifndef NOPENMSGS

#ifndef NOPENRC1
#define WM_RCRESULT (WM_PENWINFIRST+1)
#define WM_HOOKRCRESULT (WM_PENWINFIRST+2)
#endif

#define WM_PENMISCINFO (WM_PENWINFIRST+3)
#define WM_GLOBALRCCHANGE (WM_PENWINFIRST+3)

#ifndef NOPENAPPS
#define WM_SKB (WM_PENWINFIRST+4)
#endif

#define WM_PENCTL (WM_PENWINFIRST+5)
#define WM_HEDITCTL (WM_PENWINFIRST+5)

#define HE_GETUNDERLINE 7
#define HE_SETUNDERLINE 8
#define HE_GETINKHANDLE 9
#define HE_SETINKMODE 10
#define HE_STOPINKMODE 11
#define HE_DEFAULTFONT 13
#define HE_CHARPOSITION 14
#define HE_CHAROFFSET 15
#define HE_GETBOXLAYOUT 20
#define HE_SETBOXLAYOUT 21
#define HE_ENABLEALTLIST 40
#define HE_SHOWALTLIST 41
#define HE_HIDEALTLIST 42
#ifndef JAPAN
#define HE_GETLENSTYPE 43
#define HE_SETLENSTYPE 44
#endif


#define HEP_NORECOG 0
#define HEP_RECOG 1
#define HEP_WAITFORTAP 2

#define HN_ENDREC 4
#define HN_DELAYEDRECOGFAIL 5
#define HN_RESULT 20
#define HN_BEGINDIALOG 40

#define HN_ENDDIALOG 41

#ifndef NOPENIEDIT

#define IE_GETMODIFY (EM_GETMODIFY)
#define IE_SETMODIFY (EM_SETMODIFY)
#define IE_CANUNDO (EM_CANUNDO)
#define IE_UNDO (EM_UNDO)
#define IE_EMPTYUNDOBUFFER (EM_EMPTYUNDOBUFFER)

#define IE_MSGFIRST (WM_USER+150)

#define IE_GETINK (IE_MSGFIRST+0)
#define IE_SETINK (IE_MSGFIRST+1)
#define IE_GETPENTIP (IE_MSGFIRST+2)
#define IE_SETPENTIP (IE_MSGFIRST+3)
#define IE_GETERASERTIP (IE_MSGFIRST+4)
#define IE_SETERASERTIP (IE_MSGFIRST+5)
#define IE_GETBKGND (IE_MSGFIRST+6)
#define IE_SETBKGND (IE_MSGFIRST+7)
#define IE_GETGRIDORIGIN (IE_MSGFIRST+8)
#define IE_SETGRIDORIGIN (IE_MSGFIRST+9)
#define IE_GETGRIDPEN (IE_MSGFIRST+10)
#define IE_SETGRIDPEN (IE_MSGFIRST+11)
#define IE_GETGRIDSIZE (IE_MSGFIRST+12)
#define IE_SETGRIDSIZE (IE_MSGFIRST+13)
#define IE_GETMODE (IE_MSGFIRST+14)
#define IE_SETMODE (IE_MSGFIRST+15)
#define IE_GETINKRECT (IE_MSGFIRST+16)

#define IE_GETAPPDATA (IE_MSGFIRST+34)
#define IE_SETAPPDATA (IE_MSGFIRST+35)
#define IE_GETDRAWOPTS (IE_MSGFIRST+36)
#define IE_SETDRAWOPTS (IE_MSGFIRST+37)
#define IE_GETFORMAT (IE_MSGFIRST+38)
#define IE_SETFORMAT (IE_MSGFIRST+39)
#define IE_GETINKINPUT (IE_MSGFIRST+40)
#define IE_SETINKINPUT (IE_MSGFIRST+41)
#define IE_GETNOTIFY (IE_MSGFIRST+42)
#define IE_SETNOTIFY (IE_MSGFIRST+43)
#define IE_GETRECOG (IE_MSGFIRST+44)
#define IE_SETRECOG (IE_MSGFIRST+45)
#define IE_GETSECURITY (IE_MSGFIRST+46)
#define IE_SETSECURITY (IE_MSGFIRST+47)
#define IE_GETSEL (IE_MSGFIRST+48)
#define IE_SETSEL (IE_MSGFIRST+49)
#define IE_DOCOMMAND (IE_MSGFIRST+50)
#define IE_GETCOMMAND (IE_MSGFIRST+51)
#define IE_GETCOUNT (IE_MSGFIRST+52)
#define IE_GETGESTURE (IE_MSGFIRST+53)
#define IE_GETMENU (IE_MSGFIRST+54)
#define IE_GETPAINTDC (IE_MSGFIRST+55)
#define IE_GETPDEVENT (IE_MSGFIRST+56)
#define IE_GETSELCOUNT (IE_MSGFIRST+57)
#define IE_GETSELITEMS (IE_MSGFIRST+58)
#define IE_GETSTYLE (IE_MSGFIRST+59)
#endif

#ifndef NOPENHEDIT

#define CIH_NOGDMSG 0x0001
#define CIH_NOACTIONHANDLE 0x0002
#define CIH_NOEDITTEXT 0x0004
#define CIH_NOFLASHCURSOR 0x0008
#endif

#ifndef NOPENBEDIT

#define HEAL_DEFAULT (-1L)

#define BEI_FACESIZE 32
#define BEIF_BOXCROSS 0x0001

#define BESC_DEFAULT 0
#define BESC_ROMANFIXED 1
#define BESC_KANJIFIXED 2
#define BESC_USERDEFINED 3

#define CIB_NOGDMSG 0x0001
#define CIB_NOACTIONHANDLE 0x0002
#define CIB_NOFLASHCURSOR 0x0004

#define BXD_CELLWIDTH 12
#define BXD_CELLHEIGHT 16
#define BXD_BASEHEIGHT 13
#define BXD_BASEHORZ 0
#define BXD_MIDFROMBASE 0
#define BXD_CUSPHEIGHT 2
#define BXD_ENDCUSPHEIGHT 4

#define BXDK_CELLWIDTH 32
#define BXDK_CELLHEIGHT 32
#define BXDK_BASEHEIGHT 28
#define BXDK_BASEHORZ 0
#define BXDK_MIDFROMBASE 0
#define BXDK_CUSPHEIGHT 28
#define BXDK_ENDCUSPHEIGHT 10
#endif

#define WM_PENMISC (WM_PENWINFIRST+6)

#define PMSC_BEDITCHANGE 1
#define PMSC_GETPCMINFO 5
#define PMSC_SETPCMINFO 6
#define PMSC_GETINKINGINFO 7
#define PMSC_SETINKINGINFO 8
#define PMSC_GETHRC 9
#define PMSC_SETHRC 10
#define PMSC_GETSYMBOLCOUNT 11
#define PMSC_GETSYMBOLS 12
#define PMSC_SETSYMBOLS 13
#define PMSC_LOADPW 15
#define PMSC_INKSTOP 16

#define PMSCL_UNLOADED (0L)
#define PMSCL_LOADED (1L)
#define PMSCL_UNLOADING (2L)

#define WM_CTLINIT (WM_PENWINFIRST+7)

#define CTLINIT_HEDIT 1
#define CTLINIT_BEDIT 7
#define CTLINIT_IEDIT 9
#define CTLINIT_MAX 10

#define WM_PENEVENT (WM_PENWINFIRST+8)

#define PE_PENDOWN 1
#define PE_PENUP 2
#define PE_PENMOVE 3
#define PE_TERMINATING 4
#define PE_TERMINATED 5
#define PE_BUFFERWARNING 6
#define PE_BEGININPUT 7
#define PE_SETTARGETS 8
#define PE_BEGINDATA 9
#define PE_MOREDATA 10
#define PE_ENDDATA 11
#define PE_GETPCMINFO 12
#define PE_GETINKINGINFO 13
#define PE_ENDINPUT 14

#define PE_RESULT 15
#endif

/* The messages and values of the Pen Windows 1.0 controls, which
 * Pen Services 2.0 also accepts. */
#define HE_GETRC 3
#define HE_SETRC 4
#define HE_GETINFLATE 5
#define HE_SETINFLATE 6
#define HE_GETRCRESULTCODE 12
#define HE_GETRCRESULT 22
#define HE_SETIMEDEFAULT 36
#define HE_GETIMEDEFAULT 37
#define HN_RCRESULT 20
#define CBN_ENDREC 16
#define CBN_DELAYEDRECOGFAIL 17
#define CBN_RCRESULT 18
#define PDTS_SCALEMAX 0x0003
#define PEN_NOINKWIDTH 0
#define PMI_IMECOLOR 2

#ifndef RC_INVOKED

#ifndef NOPENDRIVER

#define FPenUpX(x) ((BOOL)(((x) & BITPENUP)!=0))
#define GetWEventRef() (LOWORD(GetMessageExtraInfo()))
#endif

#ifndef NOPENALC

#define MpAlcB(lprc,i) ((lprc)->rgbfAlc[((i) & 0xff) >> 3])
#define MpIbf(i) ((BYTE)(1 << ((i) & 7)))
#define SetAlcBitAnsi(lprc,i) do {MpAlcB(lprc,i) |= MpIbf(i);} while (0)
#define ResetAlcBitAnsi(lprc,i) do {MpAlcB(lprc,i) &= ~MpIbf(i);} while (0)
#define IsAlcBitAnsi(lprc,i) ((MpAlcB(lprc,i) & MpIbf(i))!=0)
#endif

#ifndef NOPENDATA

#define DrawPenDataFmt(hdc,lprect,hpndt) DrawPenDataEx(hdc,lprect,hpndt,0,IX_END,0,IX_END,NULL,NULL,0)
#endif

#ifndef NOPENHRC

#define dwDiffAT(at1,at2) (1000*((at2).sec - (at1).sec) - (DWORD)(at1).ms + (DWORD)(at2).ms)
#define FLTAbsTime(at1,at2) ((at1).sec < (at2).sec || ((at1).sec==(at2).sec && (at1).ms < (at2).ms))
#define FLTEAbsTime(at1,at2) ((at1).sec < (at2).sec || ((at1).sec==(at2).sec && (at1).ms <= (at2).ms))
#define FEQAbsTime(at1,at2) ((at1).sec==(at2).sec && (at1).ms==(at2).ms)
#define FAbsTimeInInterval(at,lpi) (FLTEAbsTime((lpi)->atBegin,at) && FLTEAbsTime(at,(lpi)->atEnd))
#define FIntervalInInterval(lpiT,lpiS) (FLTEAbsTime((lpiS)->atBegin,(lpiT)->atBegin) && FLTEAbsTime((lpiT)->atEnd,(lpiS)->atEnd))
#define FIntervalXInterval(lpiT,lpiS) (!(FLTAbsTime((lpiT)->atEnd,(lpiS)->atBegin) || FLTAbsTime((lpiS)->atEnd,(lpiT)->atBegin)))
#define dwDurInterval(lpi) dwDiffAT((lpi)->atBegin,(lpi)->atEnd)
#define MakeAbsTime(lpat,sec,ms) do { (lpat)->sec = sec + ((ms) / 1000); (lpat)->ms = (ms) % 1000; } while (0)

#define FIsSpecial(syv) (HIWORD((syv))==SYVHI_SPECIAL)
#define FIsAnsi(syv) (HIWORD((syv))==SYVHI_ANSI)
#define FIsGesture(syv) (HIWORD((syv))==SYVHI_GESTURE)
#define FIsKanji(syv) (HIWORD((syv))==SYVHI_KANJI)
#define FIsShape(syv) (HIWORD((syv))==SYVHI_SHAPE)
#define FIsUniCode(syv) (HIWORD((syv))==SYVHI_UNICODE)
#define FIsVKey(syv) (HIWORD((syv))==SYVHI_VKEY)

#define ChSyvToAnsi(syv) ((BYTE) (LOBYTE(LOWORD((syv)))))
#define WSyvToKanji(syv) ((WORD) (LOWORD((syv))))
#define SyvCharacterToSymbol(c) ((LONG)(unsigned char)(c) | 0x00010000)
#define SyvKanjiToSymbol(c) ((LONG)(UINT)(c) | 0x00030000)

#define FIsSelectGesture(syv) ((syv) >= SYVSELECTFIRST && (syv) <= SYVSELECTLAST)

#define FIsStdGesture(syv) (FIsSelectGesture(syv) || (syv)==SYV_CLEAR || (syv)==SYV_HELP || (syv)==SYV_EXTENDSELECT || (syv)==SYV_UNDO || (syv)==SYV_COPY || (syv)==SYV_CUT || (syv)==SYV_PASTE || (syv)==SYV_CLEARWORD || (syv)==SYV_KKCONVERT || (syv)==SYV_USER || (syv)==SYV_CORRECT)

#define FIsAnsiGesture(syv) ((syv)==SYV_BACKSPACE || (syv)==SYV_TAB || (syv)==SYV_RETURN || (syv)==SYV_SPACE)
#endif

#ifndef NOPENINKPUT
#define SubPenMsgFromWpLp(wp,lp) (LOWORD(wp))
#define EventRefFromWpLp(wp,lp) (HIWORD(wp))
#define TerminationFromWpLp(wp,lp) ((int)HIWORD(wp))
#define HpcmFromWpLp(wp,lp) ((HPCM)(lp))
#endif

#ifndef NOPENTARGET
#define HwndFromHtrg(htrg) ((HWND)(DWORD)(htrg))
#define HtrgFromHwnd(hwnd) ((HTRG)(UINT)(hwnd))
#endif

  typedef LONG ALC;
  typedef int CL;
  typedef UINT HKP;
  typedef int REC;
  typedef LONG SYV;


  DECLARE_HANDLE32(HTRG);
  DECLARE_HANDLE(HPCM);
  DECLARE_HANDLE(HPENDATA);
  DECLARE_HANDLE(HREC);

  typedef ALC FAR *LPALC;
  typedef LPVOID LPOEM;
  typedef SYV FAR *LPSYV;
  typedef HPENDATA FAR *LPHPENDATA;

  typedef int (CALLBACK *ENUMPROC)(LPSYV,int,VOID FAR *);
  typedef int (CALLBACK *LPDF)(int,LPVOID,LPVOID,int,DWORD,DWORD);
  typedef BOOL (CALLBACK *RCYIELDPROC)(VOID);

  typedef struct tagABSTIME {
    DWORD sec;
    UINT ms;
  } ABSTIME, FAR *LPABSTIME;

#ifndef NOPENHEDIT
  typedef struct tagCTLINITHEDIT {
    DWORD cbSize;
    HWND hwnd;
    int id;
    DWORD dwFlags;
    DWORD dwReserved;
  } CTLINITHEDIT, FAR *LPCTLINITHEDIT;
#endif

#ifndef NOPENBEDIT

  typedef struct tagBOXLAYOUT {
    int cyCusp;
    int cyEndCusp;
    UINT style;
    DWORD dwReserved1;
    DWORD dwReserved2;
    DWORD dwReserved3;
  } BOXLAYOUT, FAR *LPBOXLAYOUT;

  typedef struct tagCTLINITBEDIT {
    DWORD cbSize;
    HWND hwnd;
    int id;
    WORD wSizeCategory;
    WORD wFlags;
    DWORD dwReserved;
  } CTLINITBEDIT, FAR *LPCTLINITBEDIT;

  typedef struct tagBOXEDITINFO {
    int cxBox;
    int cyBox;
    int cxBase;
    int cyBase;
    int cyMid;
    BOXLAYOUT boxlayout;
    UINT wFlags;
    BYTE szFaceName[BEI_FACESIZE];
    UINT wFontHeight;
    UINT rgwReserved[8];
  } BOXEDITINFO, FAR *LPBOXEDITINFO;
#endif

#ifndef NOPENCTL

  typedef struct tagRECTOFS {
    int dLeft;
    int dTop;
    int dRight;
    int dBottom;
  } RECTOFS, FAR *LPRECTOFS;
#endif

#ifndef NOPENDATA
  typedef struct tagPENDATAHEADER {
    UINT wVersion;
    UINT cbSizeUsed;
    UINT cStrokes;
    UINT cPnt;
    UINT cPntStrokeMax;
    RECT rectBound;
    UINT wPndts;
    int nInkWidth;
    DWORD rgbInk;
  } PENDATAHEADER, FAR *LPPENDATAHEADER, FAR *LPPENDATA;

  typedef struct tagSTROKEINFO {
    UINT cPnt;
    UINT cbPnts;
    UINT wPdk;
    DWORD dwTick;
  } STROKEINFO, FAR *LPSTROKEINFO;

  typedef struct tagPENTIP {
    DWORD cbSize;
    BYTE btype;
    BYTE bwidth;
    BYTE bheight;
    BYTE bOpacity;
    COLORREF rgb;
    DWORD dwFlags;
    DWORD dwReserved;
  } PENTIP, FAR *LPPENTIP;

  typedef BOOL (CALLBACK *ANIMATEPROC)(HPENDATA,UINT,UINT,UINT FAR *,LPARAM);

  typedef struct tagANIMATEINFO {
    DWORD cbSize;
    UINT uSpeedPct;
    UINT uPeriodCB;
    UINT fuFlags;
    LPARAM lParam;
    DWORD dwReserved;
  } ANIMATEINFO, FAR *LPANIMATEINFO;
#endif

#ifndef NOPENDRIVER
  typedef struct tagOEMPENINFO {
    UINT wPdt;
    UINT wValueMax;
    UINT wDistinct;
  } OEMPENINFO, FAR *LPOEMPENINFO;

  typedef struct tagPENPACKET {
    UINT wTabletX;
    UINT wTabletY;
    UINT wPDK;
    UINT rgwOemData[MAXOEMDATAWORDS];
  } PENPACKET, FAR *LPPENPACKET;

  typedef struct tagOEM_PENPACKET {
    UINT wTabletX;
    UINT wTabletY;
    UINT wPDK;
    UINT rgwOemData[MAXOEMDATAWORDS];
    DWORD dwTime;
  } OEM_PENPACKET, FAR *LPOEM_PENPACKET;

  typedef struct tagPENINFO {
    UINT cxRawWidth;
    UINT cyRawHeight;
    UINT wDistinctWidth;
    UINT wDistinctHeight;
    int nSamplingRate;
    int nSamplingDist;
    LONG lPdc;
    int cPens;
    int cbOemData;
    OEMPENINFO rgoempeninfo[MAXOEMDATAWORDS];
    UINT rgwReserved[7];
    UINT fuOEM;
  } PENINFO, FAR *LPPENINFO;

  typedef struct tagCALBSTRUCT {
    int wOffsetX;
    int wOffsetY;
    int wDistinctWidth;
    int wDistinctHeight;
  } CALBSTRUCT, FAR *LPCALBSTRUCT;

  typedef BOOL (CALLBACK *LPFNRAWHOOK)(LPPENPACKET);
#endif

#ifndef NOPENHRC
  DECLARE_HANDLE32(HRC);
  DECLARE_HANDLE32(HRCRESULT);
  DECLARE_HANDLE32(HWL);
  DECLARE_HANDLE32(HRECHOOK);

  typedef HRC FAR *LPHRC;
  typedef HRCRESULT FAR *LPHRCRESULT;
  typedef HWL FAR *LPHWL;

  typedef BOOL (CALLBACK *HRCRESULTHOOKPROC)(HREC,HRC,UINT,UINT,UINT,LPVOID);

  DECLARE_HANDLE(HINKSET);
  typedef HINKSET FAR *LPHINKSET;

  typedef struct tagINTERVAL {
    ABSTIME atBegin;
    ABSTIME atEnd;
  } INTERVAL, FAR *LPINTERVAL;

  typedef struct tagBOXRESULTS {
    UINT indxBox;
    HINKSET hinksetBox;
    SYV rgSyv[1];
  } BOXRESULTS, FAR *LPBOXRESULTS;

  typedef struct tagGUIDE {
    int xOrigin;
    int yOrigin;
    int cxBox;
    int cyBox;
    int cxBase;
    int cyBase;
    int cHorzBox;
    int cVertBox;
    int cyMid;
  } GUIDE, FAR *LPGUIDE;
#endif

#ifndef NOPENIEDIT
  typedef struct tagCTLINITIEDIT {
    DWORD cbSize;
    HWND hwnd;
    int id;
    WORD ieb;
    WORD iedo;
    WORD iei;
    WORD ien;
    WORD ierec;
    WORD ies;
    WORD iesec;
    WORD pdts;
    HPENDATA hpndt;
    HGDIOBJ hgdiobj;
    HPEN hpenGrid;
    POINT ptOrgGrid;
    WORD wVGrid;
    WORD wHGrid;
    DWORD dwApp;
    DWORD dwReserved;
  } CTLINITIEDIT, FAR *LPCTLINITIEDIT;

  typedef struct tagPDEVENT {
    DWORD cbSize;
    HWND hwnd;
    UINT wm;
    WPARAM wParam;
    LPARAM lParam;
    POINT pt;
    BOOL fPen;
    LONG lExInfo;
    DWORD dwReserved;
  } PDEVENT, FAR *LPPDEVENT;

  typedef struct tagSTRKFMT {
    DWORD cbSize;
    UINT iesf;
    UINT iStrk;
    PENTIP tip;
    DWORD dwUser;
    DWORD dwReserved;
  } STRKFMT, FAR *LPSTRKFMT;
#endif

#ifndef NOPENINKPUT

  typedef struct tagPCMINFO {
    DWORD cbSize;
    DWORD dwPcm;
    RECT rectBound;
    RECT rectExclude;
    HRGN hrgnBound;
    HRGN hrgnExclude;
    DWORD dwTimeout;
  } PCMINFO, FAR *LPPCMINFO;

  typedef struct tagINKINGINFO {
    DWORD cbSize;
    UINT wFlags;
    PENTIP tip;
    RECT rectClip;
    RECT rectInkStop;
    HRGN hrgnClip;
    HRGN hrgnInkStop;
  } INKINGINFO, FAR *LPINKINGINFO;
#endif

#ifndef NOPENRC1

  typedef struct tagSYC {
    UINT wStrokeFirst;
    UINT wPntFirst;
    UINT wStrokeLast;
    UINT wPntLast;
    BOOL fLastSyc;
  } SYC, FAR *LPSYC;

  typedef struct tagSYE {
    SYV syv;
    LONG lRecogVal;
    CL cl;
    int iSyc;
  } SYE, FAR *LPSYE;

  typedef struct tagSYG {
    POINT rgpntHotSpots[MAXHOTSPOT];
    int cHotSpot;
    int nFirstBox;
    LONG lRecogVal;
    LPSYE lpsye;
    int cSye;
    LPSYC lpsyc;
    int cSyc;
  } SYG, FAR *LPSYG;

  typedef struct tagRC {
    HREC hrec;
    HWND hwnd;
    UINT wEventRef;
    UINT wRcPreferences;
    LONG lRcOptions;
    RCYIELDPROC lpfnYield;
    BYTE lpUser[cbRcUserMax];
    UINT wCountry;
    UINT wIntlPreferences;
    char lpLanguage[cbRcLanguageMax];
    LPDF rglpdf[MAXDICTIONARIES];
    UINT wTryDictionary;
    CL clErrorLevel;
    ALC alc;
    ALC alcPriority;
    BYTE rgbfAlc[cbRcrgbfAlcMax];
    UINT wResultMode;
    UINT wTimeOut;
    LONG lPcm;
    RECT rectBound;
    RECT rectExclude;
    GUIDE guide;
    UINT wRcOrient;
    UINT wRcDirect;
    int nInkWidth;
    COLORREF rgbInk;
    DWORD dwAppParam;
    DWORD dwDictParam;
    DWORD dwRecognizer;
    UINT rgwReserved[cwRcReservedMax];
  } RC, FAR *LPRC;

  typedef struct tagRCRESULT {
    SYG syg;
    UINT wResultsType;
    int cSyv;
    LPSYV lpsyv;
    HANDLE hSyv;
    int nBaseLine;
    int nMidLine;
    HPENDATA hpendata;
    RECT rectBoundInk;
    POINT pntEnd;
    LPRC lprc;
  } RCRESULT, FAR *LPRCRESULT;

  typedef int (CALLBACK *LPFUNCRESULTS)(LPRCRESULT,REC);
#endif

#ifndef NOPENTARGET

  typedef struct tagTARGET {
    DWORD dwFlags;
    DWORD idTarget;
    HTRG htrgTarget;
    RECTL rectBound;
    DWORD dwData;
    RECTL rectBoundInk;
    RECTL rectBoundLastInk;
  } TARGET, FAR *LPTARGET;

  typedef struct tagTARGINFO {
    DWORD cbSize;
    DWORD dwFlags;
    HTRG htrgOwner;
    WORD cTargets;
    WORD iTargetLast;
    TARGET rgTarget[1];
  } TARGINFO, FAR *LPTARGINFO;

  typedef struct tagINPPARAMS {
    DWORD cbSize;
    DWORD dwFlags;
    HPENDATA hpndt;
    TARGET target;
  } INPPARAMS, FAR *LPINPPARAMS;
#endif


#if PENVER >= 0x0200
  LRESULT CALLBACK DefPenWindowProc(HWND,UINT,WPARAM,LPARAM);
#endif

#ifndef NOPENAPPS
  BOOL WINAPI ShowKeyboard(HWND,UINT,LPPOINT,LPSKBINFO);
#endif

#ifndef NOPENDATA

#ifndef NOPENAPIFUN
  LPPENDATA WINAPI BeginEnumStrokes(HPENDATA);
#if PENVER >= 0x0200
  LPPENDATA WINAPI EndEnumStrokes(HPENDATA);
#endif
  HPENDATA WINAPI CompactPenData(HPENDATA,UINT);
  HPENDATA WINAPI CreatePenData(LPPENINFO,int,UINT,UINT);
  VOID WINAPI DrawPenData(HDC,LPRECT,HPENDATA);
  BOOL WINAPI GetPenDataStroke(LPPENDATA,UINT,LPPOINT FAR *,LPVOID FAR *,LPSTROKEINFO);
#endif
  HPENDATA WINAPI AddPointsPenData(HPENDATA,LPPOINT,LPVOID,LPSTROKEINFO);
#if PENVER >= 0x0200
  int WINAPI CompressPenData(HPENDATA,UINT,DWORD);
#endif
#if PENVER >= 0x0200
  HPENDATA WINAPI CreatePenDataEx(LPPENINFO,UINT,UINT,UINT);
#endif
#if PENVER >= 0x0200
  HRGN WINAPI CreatePenDataRegion(HPENDATA,UINT);
#endif
#if PENVER >= 0x0200
  BOOL WINAPI DestroyPenData(HPENDATA);
#endif
#if PENVER >= 0x0200
  int WINAPI DrawPenDataEx(HDC,LPRECT,HPENDATA,UINT,UINT,UINT,UINT,ANIMATEPROC,LPANIMATEINFO,UINT);
#endif
  HPENDATA WINAPI DuplicatePenData(HPENDATA,UINT);
#if PENVER >= 0x0200
  int WINAPI ExtractPenDataPoints(HPENDATA,UINT,UINT,UINT,LPPOINT,LPVOID,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI ExtractPenDataStrokes(HPENDATA,UINT,LPARAM,LPHPENDATA,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetPenDataAttributes(HPENDATA,LPVOID,UINT);
#endif
  BOOL WINAPI GetPenDataInfo(HPENDATA,LPPENDATAHEADER,LPPENINFO,DWORD);
#if PENVER >= 0x0200
  BOOL WINAPI GetPointsFromPenData(HPENDATA,UINT,UINT,UINT,LPPOINT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetStrokeAttributes(HPENDATA,UINT,LPVOID,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetStrokeTableAttributes(HPENDATA,UINT,LPVOID,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI HitTestPenData(HPENDATA,LPPOINT,UINT,UINT FAR *,UINT FAR *);
#endif
#if PENVER >= 0x0200
  int WINAPI InsertPenData(HPENDATA,HPENDATA,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI InsertPenDataPoints(HPENDATA,UINT,UINT,UINT,LPPOINT,LPVOID);
#endif
#if PENVER >= 0x0200
  int WINAPI InsertPenDataStroke(HPENDATA,UINT,LPPOINT,LPVOID,LPSTROKEINFO);
#endif
  BOOL WINAPI MetricScalePenData(HPENDATA,UINT);
  BOOL WINAPI OffsetPenData(HPENDATA,int,int);
#if PENVER >= 0x0200
  LONG WINAPI PenDataFromBuffer(LPHPENDATA,UINT,LPBYTE,LONG,LPDWORD);
#endif
#if PENVER >= 0x0200
  LONG WINAPI PenDataToBuffer(HPENDATA,LPBYTE,LONG,LPDWORD);
#endif
  BOOL WINAPI RedisplayPenData(HDC,HPENDATA,LPPOINT,LPPOINT,int,DWORD);
  int WINAPI RemovePenDataStrokes(HPENDATA,UINT,UINT);
  BOOL WINAPI ResizePenData(HPENDATA,LPRECT);
#if PENVER >= 0x0200
  int WINAPI SetStrokeAttributes(HPENDATA,UINT,LPARAM,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI SetStrokeTableAttributes(HPENDATA,UINT,LPARAM,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI TrimPenData(HPENDATA,DWORD,DWORD);
#endif
#endif

#ifndef NOPENDICT
  BOOL WINAPI DictionarySearch(LPRC,LPSYE,int,LPSYV,int);
#endif

#ifndef NOPENDRIVER

#ifndef NOPENAPIFUN
  BOOL WINAPI EndPenCollection(REC);
  REC WINAPI GetPenHwData(LPPOINT,LPVOID,int,UINT,LPSTROKEINFO);
  REC WINAPI GetPenHwEventData(UINT,UINT,LPPOINT,LPVOID,int,LPSTROKEINFO);
  BOOL WINAPI SetPenHook(HKP,LPFNRAWHOOK);
  VOID WINAPI UpdatePenInfo(LPPENINFO);
#endif
  BOOL WINAPI GetPenAsyncState(UINT);
  BOOL WINAPI IsPenEvent(UINT,LONG);
#endif

#ifndef NOPENHRC
#if PENVER >= 0x0200
  int WINAPI AddPenDataHRC(HRC,HPENDATA);
#endif
#if PENVER >= 0x0200
  int WINAPI AddPenInputHRC(HRC,LPPOINT,LPVOID,UINT,LPSTROKEINFO);
#endif
#if PENVER >= 0x0200
  int WINAPI AddWordsHWL(HWL,LPSTR,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI ConfigHREC(HREC,UINT,WPARAM,LPARAM);
#endif
#if PENVER >= 0x0200
  HRC WINAPI CreateCompatibleHRC(HRC,HREC);
#endif
#if PENVER >= 0x0200
  HWL WINAPI CreateHWL(HREC,LPSTR,UINT,DWORD);
#endif
#if PENVER >= 0x0200
  HINKSET WINAPI CreateInksetHRCRESULT(HRCRESULT,UINT,UINT);
#endif
#if PENVER >= 0x0200
  HPENDATA WINAPI CreatePenDataHRC(HRC);
#endif
#if PENVER >= 0x0200
  int WINAPI DestroyHRC(HRC);
#endif
#if PENVER >= 0x0200
  int WINAPI DestroyHRCRESULT(HRCRESULT);
#endif
#if PENVER >= 0x0200
  int WINAPI DestroyHWL(HWL);
#endif
#if PENVER >= 0x0200
  int WINAPI EnableGestureSetHRC(HRC,SYV,BOOL);
#endif
#if PENVER >= 0x0200
  int WINAPI EnableSystemDictionaryHRC(HRC,BOOL);
#endif
#if PENVER >= 0x0200
  int WINAPI EndPenInputHRC(HRC);
#endif
#if PENVER >= 0x0200
  int WINAPI GetAlphabetHRC(HRC,LPALC,LPBYTE);
#endif
#if PENVER >= 0x0200
  int WINAPI GetAlphabetPriorityHRC(HRC,LPALC,LPBYTE);
#endif
#if PENVER >= 0x0200
  int WINAPI GetAlternateWordsHRCRESULT(HRCRESULT,UINT,UINT,LPHRCRESULT,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetBoxMappingHRCRESULT(HRCRESULT,UINT,UINT,UINT FAR *);
#endif
#if PENVER >= 0x0200
  int WINAPI GetBoxResultsHRC(HRC,UINT,UINT,UINT,LPBOXRESULTS,BOOL);
#endif
#if PENVER >= 0x0200
  int WINAPI GetGuideHRC(HRC,LPGUIDE,UINT FAR *);
#endif
#if PENVER >= 0x0200
  int WINAPI GetHotspotsHRCRESULT(HRCRESULT,UINT,LPPOINT,UINT);
#endif
#if PENVER >= 0x0200
  HREC WINAPI GetHRECFromHRC(HRC);
#endif
#if PENVER >= 0x0200
  int WINAPI GetInternationalHRC(HRC,UINT FAR *,LPSTR,UINT FAR *,UINT FAR *);
#endif
#if PENVER >= 0x0200
  int WINAPI GetMaxResultsHRC(HRC);
#endif
#if PENVER >= 0x0200
  int WINAPI GetResultsHRC(HRC,UINT,LPHRCRESULT,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetSymbolCountHRCRESULT(HRCRESULT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetSymbolsHRCRESULT(HRCRESULT,UINT,LPSYV,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetWordlistHRC(HRC,LPHWL);
#endif
#if PENVER >= 0x0200
  int WINAPI GetWordlistCoercionHRC(HRC);
#endif
#if PENVER >= 0x0200
  int WINAPI ProcessHRC(HRC,DWORD);
#endif
#if PENVER >= 0x0200
  int WINAPI ReadHWL(HWL,HFILE);
#endif
#if PENVER >= 0x0200
  int WINAPI SetAlphabetHRC(HRC,ALC,LPBYTE);
#endif
#if PENVER >= 0x0200
  int WINAPI SetAlphabetPriorityHRC(HRC,ALC,LPBYTE);
#endif
#if PENVER >= 0x0200
  int WINAPI SetBoxAlphabetHRC(HRC,LPALC,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI SetGuideHRC(HRC,LPGUIDE,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI SetInternationalHRC(HRC,UINT,LPCSTR,UINT,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI SetMaxResultsHRC(HRC,UINT);
#endif
#if PENVER >= 0x0200
  HRECHOOK WINAPI SetResultsHookHREC(HREC,HRCRESULTHOOKPROC);
#endif
#if PENVER >= 0x0200
  int WINAPI SetWordlistCoercionHRC(HRC,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI SetWordlistHRC(HRC,HWL);
#endif
#if PENVER >= 0x0200
  int WINAPI TrainHREC(HREC,LPSYV,UINT,HPENDATA,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI UnhookResultsHookHREC(HREC,HRECHOOK);
#endif
#if PENVER >= 0x0200
  int WINAPI WriteHWL(HWL,HFILE);
#endif
  HREC WINAPI InstallRecognizer(LPSTR);
  VOID WINAPI UninstallRecognizer(HREC);
#if PENVER >= 0x0200
  BOOL WINAPI AddInksetInterval(HINKSET,LPINTERVAL);
#endif
#if PENVER >= 0x0200
  HINKSET WINAPI CreateInkset(UINT);
#endif
#if PENVER >= 0x0200
  BOOL WINAPI DestroyInkset(HINKSET);
#endif
#if PENVER >= 0x0200
  int WINAPI GetInksetInterval(HINKSET,UINT,LPINTERVAL);
#endif
#if PENVER >= 0x0200
  int WINAPI GetInksetIntervalCount(HINKSET);
#endif
  int WINAPI CharacterToSymbol(LPSTR,int,LPSYV);
  BOOL WINAPI SymbolToCharacter(LPSYV,int,LPSTR,LPINT);
#endif

#ifndef NOPENINKPUT
#if PENVER >= 0x0200
  int WINAPI DoDefaultPenInput(HWND,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI GetPenInput(HPCM,LPPOINT,LPVOID,UINT,UINT,LPSTROKEINFO);
#endif
#if PENVER >= 0x0200
  int WINAPI PeekPenInput(HPCM,UINT,LPPOINT,LPVOID,UINT);
#endif
#if PENVER >= 0x0200
  int WINAPI StartInking(HPCM,UINT,LPINKINGINFO);
#endif
#if PENVER >= 0x0200
  HPCM WINAPI StartPenInput(HWND,UINT,LPPCMINFO,LPINT);
#endif
#if PENVER >= 0x0200
  int WINAPI StopInking(HPCM);
#endif
#if PENVER >= 0x0200
  int WINAPI StopPenInput(HPCM,UINT,int);
#endif
#endif

#ifndef NOPENMISC
  VOID WINAPI BoundingRectFromPoints(LPPOINT,UINT,LPRECT);
  BOOL WINAPI DPtoTP(LPPOINT,int);
#if PENVER >= 0x0200
  UINT WINAPI GetPenAppFlags(VOID);
#endif
#if PENVER >= 0x0200
  VOID WINAPI SetPenAppFlags(UINT,UINT);
#endif
#if PENVER >= 0x0200
  LONG WINAPI GetPenMiscInfo(WPARAM,LPARAM);
#endif
  UINT WINAPI GetVersionPenWin(VOID);
#if PENVER >= 0x0200
  LONG WINAPI SetPenMiscInfo(WPARAM,LPARAM);
#endif
  BOOL WINAPI TPtoDP(LPPOINT,int);
  BOOL WINAPI CorrectWriting(HWND,LPSTR,UINT,LPVOID,DWORD,DWORD);
#endif

#ifndef NOPENRC1
  VOID WINAPI EmulatePen(BOOL);
  UINT WINAPI EnumSymbols(LPSYG,UINT,ENUMPROC,LPVOID);
  BOOL WINAPI ExecuteGesture(HWND,SYV,LPRCRESULT);
  VOID WINAPI FirstSymbolFromGraph(LPSYG,LPSYV,int,LPINT);
  UINT WINAPI GetGlobalRC(LPRC,LPSTR,LPSTR,int);
  int WINAPI GetSymbolCount(LPSYG);
  int WINAPI GetSymbolMaxLength(LPSYG);
  VOID WINAPI InitRC(HWND,LPRC);
  REC WINAPI ProcessWriting(HWND,LPRC);
  REC WINAPI Recognize(LPRC);
  REC WINAPI RecognizeData(LPRC,HPENDATA);
  UINT WINAPI SetGlobalRC(LPRC,LPSTR,LPSTR);
  BOOL WINAPI SetRecogHook(UINT,UINT,HWND);
  BOOL WINAPI TrainContext(LPRCRESULT,LPSYE,int,LPSYC,int);
  BOOL WINAPI TrainInk(LPRC,HPENDATA,LPSYV);

#endif

#ifndef NOPENTARGET
#if PENVER >= 0x0200
  int WINAPI TargetPoints(LPTARGINFO,LPPOINT,DWORD,UINT,LPSTROKEINFO);
#endif
#endif

#ifndef NOPENVIRTEVENT

  VOID WINAPI AtomicVirtualEvent(BOOL);
  VOID WINAPI PostVirtualKeyEvent(UINT,BOOL);
  VOID WINAPI PostVirtualMouseEvent(UINT,int,int);
#endif

#endif

#if PENVER < 0x0200
/* Pen Windows 1.0 has no DestroyPenData and EndEnumStrokes functions: a
 * pen data handle is a global memory handle. */
#define DestroyPenData(hpendata) (GlobalFree((HGLOBAL)(hpendata)) == NULL)
#define EndEnumStrokes(hpendata) GlobalUnlock((HGLOBAL)(hpendata))
#endif

#ifdef __cplusplus
}
#endif

#endif /* _INC_PENWIN */
