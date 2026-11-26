/* vbapi.h: the Visual Basic custom control interface (VBX), for custom
 * controls for Visual Basic 1.0, 2.0 and 3.0 and for programs that call
 * the Visual Basic API from a DLL. Written for clang-retro from the
 * published interface; AGPL-3.0-only, see the win16-sdk LICENSE.
 *
 * A custom control is a DLL with the extension .VBX. It exports VBINITCC,
 * VBTERMCC and VBGETMODELINFO, and it registers one MODEL for each of its
 * controls with VBRegisterModel.
 *
 * The VB functions are not imports from a module. Visual Basic puts the
 * address of its dispatcher at offset 0x20 of its stack segment, and each
 * function is a stub that jumps there with the function number in BX.
 * The stubs are in vbapi.lib in the lib/win16 folder of the sysroot:
 *     clang --config=win31.cfg -shared ctl.c -o CTL.VBX \
 *         -L$SYSROOT/lib/win16 -lvbapi
 * The control runs on the stack of Visual Basic, as all DLL code does. */
#ifndef _INC_VBAPI
#define _INC_VBAPI

#include <windows.h>

#ifndef RC_INVOKED
#ifdef __cplusplus
extern "C" {
#endif
#pragma pack(push, 1)
#endif

/* Versions of Visual Basic, as VBGetVersion and MODEL.usVersion give
 * them. */
#define VB100_VERSION 0x0100
#define VB200_VERSION 0x0200
#define VB300_VERSION 0x0300
#ifndef VB_VERSION
#define VB_VERSION VB300_VERSION
#endif

#ifndef RC_INVOKED

/* General types. */
typedef signed short   SHORT;
typedef unsigned short USHORT;
typedef unsigned int   FSHORT;
typedef signed char    CHAR;
typedef unsigned char  ENUM;
typedef unsigned long  ULONG;
typedef unsigned long  FLONG;
typedef SHORT FAR     *LPSHORT;
typedef USHORT FAR    *LPUSHORT;
typedef USHORT         IPROP;
typedef HANDLE         HFORMFILE;
typedef USHORT         ERR;
typedef WORD           HPIC;
typedef HPIC FAR      *LPHPIC;
typedef LPSTR          HSZ;
typedef LPVOID         HLSTR;
typedef HLSTR FAR     *LPHLSTR;
typedef LPVOID         HAD;  /* A Basic array descriptor. */
typedef LPVOID         HCTL; /* A control. */

/* A window procedure and a control procedure, in the form of Visual
 * Basic. */
typedef LONG(FAR PASCAL *PFNWP)(HWND hwnd, USHORT msg, USHORT wp, LONG lp);
typedef LONG(FAR PASCAL *PCTLPROC)(HCTL hctl, HWND hwnd, USHORT msg, USHORT wp, LONG lp);

/* The Basic Currency type: a 64-bit integer, 10000 times the value. */
typedef struct {
  LONG lo;
  LONG hi;
} CURRENCY;

/* The value of a Variant, for VBGetVariantValue. */
typedef union {
  SHORT    i2;
  LONG     i4;
  float    r4;
  double   r8;
  CURRENCY cy;
  HLSTR    hlstr;
} VALUE;
typedef VALUE FAR *LPVAL;

/* A Basic Variant. Its contents are private; it has 16 bytes. */
typedef struct {
  char v[16];
} VARIANT;
typedef VARIANT FAR *LPVAR;

/* Variant types, as VBGetVariantType and the Basic VarType give them. */
#define VT_EMPTY    0
#define VT_NULL     1
#define VT_I2       2
#define VT_I4       3
#define VT_R4       4
#define VT_R8       5
#define VT_CURRENCY 6
#define VT_DATE     7
#define VT_STRING   8

/* A color: an RGB value, or with COLOR_DEFBITON set the index of a
 * system color. RGBCOLOR gives the RGB value of each. */
typedef unsigned long COLOR;
typedef COLOR FAR    *LPCOLOR;
#define COLOR_DEFBITON 0x80000000UL
#define RGBCOLOR(clr)                                                    \
  (((clr) & COLOR_DEFBITON) ? (GetSysColor((int)((clr) & 0xFFFF)))     \
                            : ((clr) & 0xFFFFFF))

/* The data of a property array access: the value and its index. */
typedef struct tagDATASTRUCT {
  LONG   data;
  USHORT cindex; /* The number of indexes (1). */
  struct {
    USHORT datatype; /* DT_SHORT. */
    LONG   data;
  } index[1];
} DATASTRUCT;
typedef DATASTRUCT FAR  *LPDATASTRUCT;
typedef DATASTRUCT NEAR *NPDATASTRUCT;

/* Types of Basic variables. */
#define ET_I2    1
#define ET_I4    2
#define ET_R4    3
#define ET_R8    4
#define ET_CY    5
#define ET_HLSTR 6
#define ET_SD    6
#define ET_FS    7

/* PROPINFO.fl: the data type (DT_) in the low byte, and flags. */
#define PF_datatype       0x000000FFL
#define PF_fPropArray     0x00000100L
#define PF_fSetData       0x00000200L
#define PF_fSetMsg        0x00000400L
#define PF_fNoShow        0x00000800L
#define PF_fNoRuntimeW    0x00001000L
#define PF_fGetData       0x00002000L
#define PF_fGetMsg        0x00004000L
#define PF_fSetCheck      0x00008000L
#define PF_fSaveData      0x00010000L
#define PF_fSaveMsg       0x00020000L
#define PF_fLoadDataOnly  0x20010000L
#define PF_fLoadMsgOnly   0x20020000L
#define PF_fGetHszMsg     0x00040000L
#define PF_fUpdateOnEdit  0x00080000L
#define PF_fEditable      0x00100000L
#define PF_fPreHwnd       0x00200000L
#define PF_fDefVal        0x00400000L
#define PF_fNoInitDef     0x00800000L
#define PF_fNoRuntimeR    0x02000000L
#define PF_fNoMultiSelect 0x04000000L

/* A property of a control. */
typedef struct tagPROPINFO {
  PSTR  npszName;
  FLONG fl;           /* PF_ flags and the DT_ type. */
  BYTE  offsetData;   /* The offset of the data in the control. */
  BYTE  infoData;     /* For a bit field: its bit and width, or 0. */
  LONG  dataDefault;  /* The default value, with PF_fDefVal. */
  PSTR  npszEnumList; /* For DT_ENUM: the names, each with a NUL, and
                       * an empty name at the end. */
  BYTE  enumMax;      /* For DT_ENUM: the largest value. */
} PROPINFO;
typedef PROPINFO FAR   *LPPROPINFO;
typedef PROPINFO NEAR  *NPPROPINFO;
typedef PROPINFO NEAR  *PPROPINFO;
typedef PPROPINFO FAR  *LPPROPLIST;
typedef PPROPINFO NEAR *NPPROPLIST;
typedef PPROPINFO NEAR *PPROPLIST;

/* The data types of properties. */
#define DT_HSZ     0x01
#define DT_SHORT   0x02
#define DT_LONG    0x03
#define DT_BOOL    0x04
#define DT_COLOR   0x05
#define DT_ENUM    0x06
#define DT_REAL    0x07
#define DT_XPOS    0x08 /* Twips; the _POS types subtract the origin. */
#define DT_XSIZE   0x09
#define DT_YPOS    0x0A
#define DT_YSIZE   0x0B
#define DT_PICTURE 0x0C
#define DT_HLSTR   0x0D
#define DT_OBJECT  0x20

/* EVENTINFO.fl. */
#define EF_fNoUnload 0x00000001L

/* An event of a control. */
typedef struct tagEVENTINFO {
  PSTR   npszName;     /* The suffix of the event procedure name. */
  USHORT cParms;       /* The number of parameters. */
  USHORT cwParms;      /* The size of the parameters in words. */
  PWORD  npParmTypes;  /* The ET_ type of each parameter. */
  PSTR   npszParmProf; /* The parameters as Basic text. */
  FLONG  fl;           /* EF_ flags. */
} EVENTINFO;
typedef EVENTINFO NEAR   *PEVENTINFO;
typedef PEVENTINFO NEAR  *PPEVENTINFO;
typedef EVENTINFO NEAR   *NPEVENTINFO;
typedef EVENTINFO FAR    *LPEVENTINFO;
typedef NPEVENTINFO FAR  *LPEVENTLIST;
typedef NPEVENTINFO NEAR *NPEVENTLIST;

/* The standard events. In an event list, PEVENTINFO_STD_x stands for
 * standard event x. */
#define IEVENT_STD_CLICK      0x0000
#define IEVENT_STD_DBLCLICK   0x0001
#define IEVENT_STD_DRAGDROP   0x0002
#define IEVENT_STD_DRAGOVER   0x0003
#define IEVENT_STD_GOTFOCUS   0x0004
#define IEVENT_STD_KEYDOWN    0x0005
#define IEVENT_STD_KEYPRESS   0x0006
#define IEVENT_STD_KEYUP      0x0007
#define IEVENT_STD_LOSTFOCUS  0x0008
#define IEVENT_STD_MOUSEDOWN  0x0009
#define IEVENT_STD_MOUSEMOVE  0x000A
#define IEVENT_STD_MOUSEUP    0x000B
#define IEVENT_STD_LINKERROR  0x000C
#define IEVENT_STD_LINKOPEN   0x000D
#define IEVENT_STD_LINKCLOSE  0x000E
#define IEVENT_STD_LINKNOTIFY 0x000F
#define IEVENT_STD_LINKCHANGE 0x0010
#define IEVENT_STD_NONE       0x0FFF
#define IEVENT_STD_LAST       0x0FFF

#define PEVENTINFO_STD_CLICK      ((PEVENTINFO)~IEVENT_STD_CLICK)
#define PEVENTINFO_STD_DBLCLICK   ((PEVENTINFO)~IEVENT_STD_DBLCLICK)
#define PEVENTINFO_STD_DRAGDROP   ((PEVENTINFO)~IEVENT_STD_DRAGDROP)
#define PEVENTINFO_STD_DRAGOVER   ((PEVENTINFO)~IEVENT_STD_DRAGOVER)
#define PEVENTINFO_STD_GOTFOCUS   ((PEVENTINFO)~IEVENT_STD_GOTFOCUS)
#define PEVENTINFO_STD_KEYDOWN    ((PEVENTINFO)~IEVENT_STD_KEYDOWN)
#define PEVENTINFO_STD_KEYPRESS   ((PEVENTINFO)~IEVENT_STD_KEYPRESS)
#define PEVENTINFO_STD_KEYUP      ((PEVENTINFO)~IEVENT_STD_KEYUP)
#define PEVENTINFO_STD_LOSTFOCUS  ((PEVENTINFO)~IEVENT_STD_LOSTFOCUS)
#define PEVENTINFO_STD_MOUSEDOWN  ((PEVENTINFO)~IEVENT_STD_MOUSEDOWN)
#define PEVENTINFO_STD_MOUSEMOVE  ((PEVENTINFO)~IEVENT_STD_MOUSEMOVE)
#define PEVENTINFO_STD_MOUSEUP    ((PEVENTINFO)~IEVENT_STD_MOUSEUP)
#define PEVENTINFO_STD_LINKERROR  ((PEVENTINFO)~IEVENT_STD_LINKERROR)
#define PEVENTINFO_STD_LINKOPEN   ((PEVENTINFO)~IEVENT_STD_LINKOPEN)
#define PEVENTINFO_STD_LINKCLOSE  ((PEVENTINFO)~IEVENT_STD_LINKCLOSE)
#define PEVENTINFO_STD_LINKNOTIFY ((PEVENTINFO)~IEVENT_STD_LINKNOTIFY)
#define PEVENTINFO_STD_LINKCHANGE ((PEVENTINFO)~IEVENT_STD_LINKCHANGE)
#define PEVENTINFO_STD_NONE       ((PEVENTINFO)~IEVENT_STD_NONE)
#define PEVENTINFO_STD_LAST       ((PEVENTINFO)~IEVENT_STD_LAST)

/* The standard properties. In a property list, PPROPINFO_STD_x stands
 * for standard property x. */
#define IPROP_STD_NAME           0x0000
#define IPROP_STD_CTLNAME        IPROP_STD_NAME
#define IPROP_STD_INDEX          0x0001
#define IPROP_STD_HWND           0x0002
#define IPROP_STD_BACKCOLOR      0x0003
#define IPROP_STD_FORECOLOR      0x0004
#define IPROP_STD_LEFT           0x0005
#define IPROP_STD_TOP            0x0006
#define IPROP_STD_WIDTH          0x0007
#define IPROP_STD_HEIGHT         0x0008
#define IPROP_STD_ENABLED        0x0009
#define IPROP_STD_VISIBLE        0x000A
#define IPROP_STD_MOUSEPOINTER   0x000B
#define IPROP_STD_CAPTION        0x000C
#define IPROP_STD_FONTNAME       0x000D
#define IPROP_STD_FONTBOLD       0x000E
#define IPROP_STD_FONTITALIC     0x000F
#define IPROP_STD_FONTSTRIKE     0x0010
#define IPROP_STD_FONTUNDER      0x0011
#define IPROP_STD_FONTSIZE       0x0012
#define IPROP_STD_TABINDEX       0x0013
#define IPROP_STD_PARENT         0x0014
#define IPROP_STD_DRAGMODE       0x0015
#define IPROP_STD_DRAGICON       0x0016
#define IPROP_STD_BORDERSTYLEOFF 0x0017
#define IPROP_STD_TABSTOP        0x0018
#define IPROP_STD_TAG            0x0019
#define IPROP_STD_TEXT           0x001B
#define IPROP_STD_BORDERSTYLEON  0x001C
#define IPROP_STD_CLIPCONTROLS   0x001D
#define IPROP_STD_NONE           0x001E
#define IPROP_STD_HELPCONTEXTID  0x001F
#define IPROP_STD_LINKMODE       0x0020
#define IPROP_STD_LINKITEM       0x0021
#define IPROP_STD_LINKTOPIC      0x0022
#define IPROP_STD_LINKTIMEOUT    0x0023
#define IPROP_STD_LEFTNORUN      0x0024
#define IPROP_STD_TOPNORUN       0x0025
#define IPROP_STD_ALIGN          0x0026
#define IPROP_STD_IMEMODE        0x0027
#define IPROP_STD_DATASOURCE     0x0028
#define IPROP_STD_DATAFIELD      0x0029
#define IPROP_STD_DATACHANGED    0x002A
#define IPROP_STD_LAST           0x0FFF

#define PPROPINFO_STD_NAME           ((PPROPINFO)~IPROP_STD_NAME)
#define PPROPINFO_STD_CTLNAME        ((PPROPINFO)~IPROP_STD_NAME)
#define PPROPINFO_STD_INDEX          ((PPROPINFO)~IPROP_STD_INDEX)
#define PPROPINFO_STD_HWND           ((PPROPINFO)~IPROP_STD_HWND)
#define PPROPINFO_STD_BACKCOLOR      ((PPROPINFO)~IPROP_STD_BACKCOLOR)
#define PPROPINFO_STD_FORECOLOR      ((PPROPINFO)~IPROP_STD_FORECOLOR)
#define PPROPINFO_STD_LEFT           ((PPROPINFO)~IPROP_STD_LEFT)
#define PPROPINFO_STD_TOP            ((PPROPINFO)~IPROP_STD_TOP)
#define PPROPINFO_STD_WIDTH          ((PPROPINFO)~IPROP_STD_WIDTH)
#define PPROPINFO_STD_HEIGHT         ((PPROPINFO)~IPROP_STD_HEIGHT)
#define PPROPINFO_STD_ENABLED        ((PPROPINFO)~IPROP_STD_ENABLED)
#define PPROPINFO_STD_VISIBLE        ((PPROPINFO)~IPROP_STD_VISIBLE)
#define PPROPINFO_STD_MOUSEPOINTER   ((PPROPINFO)~IPROP_STD_MOUSEPOINTER)
#define PPROPINFO_STD_CAPTION        ((PPROPINFO)~IPROP_STD_CAPTION)
#define PPROPINFO_STD_FONTNAME       ((PPROPINFO)~IPROP_STD_FONTNAME)
#define PPROPINFO_STD_FONTBOLD       ((PPROPINFO)~IPROP_STD_FONTBOLD)
#define PPROPINFO_STD_FONTITALIC     ((PPROPINFO)~IPROP_STD_FONTITALIC)
#define PPROPINFO_STD_FONTSTRIKE     ((PPROPINFO)~IPROP_STD_FONTSTRIKE)
#define PPROPINFO_STD_FONTUNDER      ((PPROPINFO)~IPROP_STD_FONTUNDER)
#define PPROPINFO_STD_FONTSIZE       ((PPROPINFO)~IPROP_STD_FONTSIZE)
#define PPROPINFO_STD_TABINDEX       ((PPROPINFO)~IPROP_STD_TABINDEX)
#define PPROPINFO_STD_PARENT         ((PPROPINFO)~IPROP_STD_PARENT)
#define PPROPINFO_STD_DRAGMODE       ((PPROPINFO)~IPROP_STD_DRAGMODE)
#define PPROPINFO_STD_DRAGICON       ((PPROPINFO)~IPROP_STD_DRAGICON)
#define PPROPINFO_STD_BORDERSTYLEOFF ((PPROPINFO)~IPROP_STD_BORDERSTYLEOFF)
#define PPROPINFO_STD_TABSTOP        ((PPROPINFO)~IPROP_STD_TABSTOP)
#define PPROPINFO_STD_TAG            ((PPROPINFO)~IPROP_STD_TAG)
#define PPROPINFO_STD_TEXT           ((PPROPINFO)~IPROP_STD_TEXT)
#define PPROPINFO_STD_BORDERSTYLEON  ((PPROPINFO)~IPROP_STD_BORDERSTYLEON)
#define PPROPINFO_STD_CLIPCONTROLS   ((PPROPINFO)~IPROP_STD_CLIPCONTROLS)
#define PPROPINFO_STD_NONE           ((PPROPINFO)~IPROP_STD_NONE)
#define PPROPINFO_STD_HELPCONTEXTID  ((PPROPINFO)~IPROP_STD_HELPCONTEXTID)
#define PPROPINFO_STD_LINKMODE       ((PPROPINFO)~IPROP_STD_LINKMODE)
#define PPROPINFO_STD_LINKITEM       ((PPROPINFO)~IPROP_STD_LINKITEM)
#define PPROPINFO_STD_LINKTOPIC      ((PPROPINFO)~IPROP_STD_LINKTOPIC)
#define PPROPINFO_STD_LINKTIMEOUT    ((PPROPINFO)~IPROP_STD_LINKTIMEOUT)
#define PPROPINFO_STD_LEFTNORUN      ((PPROPINFO)~IPROP_STD_LEFTNORUN)
#define PPROPINFO_STD_TOPNORUN       ((PPROPINFO)~IPROP_STD_TOPNORUN)
#define PPROPINFO_STD_ALIGN          ((PPROPINFO)~IPROP_STD_ALIGN)
#define PPROPINFO_STD_IMEMODE        ((PPROPINFO)~IPROP_STD_IMEMODE)
#define PPROPINFO_STD_DATASOURCE     ((PPROPINFO)~IPROP_STD_DATASOURCE)
#define PPROPINFO_STD_DATAFIELD      ((PPROPINFO)~IPROP_STD_DATAFIELD)
#define PPROPINFO_STD_DATACHANGED    ((PPROPINFO)~IPROP_STD_DATACHANGED)
#define PPROPINFO_STD_LAST           ((PPROPINFO)~IPROP_STD_LAST)

/* The parameter of VBM_DRAGOVER and VBM_DRAGDROP. */
typedef struct tagDRAGINFO {
  HCTL   hctl;
  POINT  pt;
  USHORT state; /* DRAG_STATE_, for VBM_DRAGOVER. */
} DRAGINFO;
typedef DRAGINFO NEAR *NPDRAGINFO;
typedef DRAGINFO FAR  *LPDRAGINFO;

#define DRAG_STATE_ENTER 0x00
#define DRAG_STATE_EXIT  0x01
#define DRAG_STATE_OVER  0x02

/* A control class: what VBRegisterModel registers. */
typedef struct tagMODEL {
  USHORT      usVersion;           /* VB_VERSION. */
  FLONG       fl;                  /* MODEL_ flags. */
  PCTLPROC    pctlproc;            /* The control procedure. */
  FSHORT      fsClassStyle;        /* The window class style. */
  FLONG       flWndStyle;          /* The window style. */
  USHORT      cbCtlExtra;          /* The size of the control data. */
  WORD        idBmpPalette;        /* The bitmap for the toolbox. */
  PSTR        npszDefCtlName;      /* The start of the default name. */
  PSTR        npszClassName;       /* The class name in Basic. */
  PSTR        npszParentClassName; /* A window class to subclass. */
  NPPROPLIST  npproplist;          /* The properties, NULL at the end. */
  NPEVENTLIST npeventlist;         /* The events, NULL at the end. */
  BYTE        nDefProp;            /* The default property. */
  BYTE        nDefEvent;           /* The default event. */
  BYTE        nValueProp;          /* The value property. */
  USHORT      usCtlVersion;        /* The version of the control (VB 3.0;
                                    * 1 and 2 are for VB 1.0 and 2.0). */
} MODEL;
typedef MODEL FAR *LPMODEL;

#define MODEL_fArrows      0x00000001L
#define MODEL_fFocusOk     0x00000002L
#define MODEL_fMnemonic    0x00000004L
#define MODEL_fChildrenOk  0x00000008L
#define MODEL_fInitMsg     0x00000010L
#define MODEL_fLoadMsg     0x00000020L
#define MODEL_fDesInteract 0x00000040L
#define MODEL_fInvisAtRun  0x00000080L
#define MODEL_fGraphical   0x00000100L

/* What VBGetModelInfo returns: the models of the VBX, NULL at the end. */
typedef struct tagMODELINFO {
  USHORT       usVersion;
  LPMODEL FAR *lplpmodel;
} MODELINFO;
typedef MODELINFO FAR *LPMODELINFO;

/* A picture. */
typedef struct tagPIC {
  BYTE picType; /* PICTYPE_. */
  union {
    struct {
      HBITMAP  hbitmap;
      HPALETTE hpal;
    } bmp;
    struct {
      HANDLE hmeta;
      int    xExt, yExt;
    } wmf;
    struct {
      HICON hicon;
    } icon;
  } picData;
  BYTE picReserved[4];
} PIC;
typedef PIC FAR *LPPIC;

#define PICTYPE_NONE     0
#define PICTYPE_BITMAP   1
#define PICTYPE_METAFILE 2
#define PICTYPE_ICON     3

#define HPIC_INVALID 0xFFFF
#define HPIC_NULL    0

/* What VBGetMode returns. */
#define MODE_DESIGN 1
#define MODE_RUN    2
#define MODE_BREAK  3

/* Flags for VBSetControlFlags. */
#define CTLFLG_HASPALETTE            0x00000001L
#define CTLFLG_USESPALETTE           0x00000002L
#define CTLFLG_GRAPHICALOPAQUE       0x00000004L
#define CTLFLG_GRAPHICALTRANSLUCENT  0x00000008L
#define CTLFLG_DATACHANGED           0x00000010L
#define CTLFLG_BOUNDDATASET          0x00000020L

/* The data of VBM_LINKGETDATA and VBM_LINKSETDATA. */
typedef struct tagVBLINKDATA {
  WORD   wReserved;
  DWORD  cb;
  HANDLE hData;
  DWORD  dwReserved;
} VBLINKDATA;
typedef VBLINKDATA FAR *LPLINKDATA;

/* Link (DDE) modes and values. */
#define LINKMODENONE         0
#define LINKMODEAUTO         1
#define LINKMODEMANUAL       2
#define LINKMODENOTIFY       3
#define LINKMODESERVERPOKE   1
#define LINKMODESERVERNOPOKE 2

#define VBDDEDATA VBLINKDATA
#define LPDDEDATA LPLINKDATA

#define MAX_EXEC           256
#define MAXLINKITEMNAME    256
#define ACCEPTSDATAFORMAT  1
#define SUPPLIESDATAFORMAT 2
#define LINKSRCASK         0
#define LINKSRCTELL        1
#define LINK_DATA_OK        0
#define LINK_DATA_OOM       1
#define LINK_DATA_FORMATBAD 2
#define LINK_DATA_SETFAILED 3

#define errInitOnNonServerDesk 6
#define errAllServerChUsed     7
#define errDataSettingFailed   8
#define errOutOfMemoryForLink  11

/* Bound controls: DATAACCESS.sAction for VBM_DATA_GET. */
#define DATA_FIELDVALUE      0
#define DATA_FIELDNAME       1
#define DATA_FIELDTYPE       2
#define DATA_FIELDSIZE       3
#define DATA_FIELDSCOUNT     4
#define DATA_FIELDCHUNK      5
#define DATA_FIELDPOSITION   6
#define DATA_FIELDATTRIBUTES 7
#define DATA_BOF             16
#define DATA_EOF             17
#define DATA_BOOKMARK        18
#define DATA_BOOKMARKABLE    19
#define DATA_UPDATABLE       20
#define DATA_RECORDCOUNT     21
#define DATA_LASTMODIFIED    22

/* DATAACCESS.sAction for VBM_DATA_AVAILABLE. */
#define DATA_MOVEFIRST        64
#define DATA_MOVENEXT         65
#define DATA_MOVEPREV         66
#define DATA_MOVELAST         67
#define DATA_FINDFIRST        68
#define DATA_FINDNEXT         69
#define DATA_FINDPREV         70
#define DATA_FINDLAST         71
#define DATA_DELETE           72
#define DATA_ADDNEW           73
#define DATA_REFRESH          74
#define DATA_ROLLBACK         76
#define DATA_CLOSE            77
#define DATA_DATAFIELDCHANGED 78
#define DATA_SAVEDATA         79
#define DATA_READDATA         80
#define DATA_UPDATE           81
#define DATA_UNLOAD           82

/* Field types, for DATA_FIELDTYPE. */
#define DATA_VT_BOOL     1
#define DATA_VT_VAR_BYTE 2
#define DATA_VT_INTEGER  3
#define DATA_VT_LONG     4
#define DATA_VT_CURRENCY 5
#define DATA_VT_SINGLE   6
#define DATA_VT_DOUBLE   7
#define DATA_VT_DATETIME 8
#define DATA_VT_TEXT     10
#define DATA_VT_BINARY   11
#define DATA_VT_MEMO     12

/* Bookmarks, for DATA_BOOKMARK. */
#define DATA_BOOKMARKFIRST   ((LONG)0x80000000L)
#define DATA_BOOKMARKPREV    (-1L)
#define DATA_BOOKMARKCURRENT 0L
#define DATA_BOOKMARKNEXT    1L
#define DATA_BOOKMARKLAST    ((LONG)0x7FFFFFFFL)

/* DATAACCESS.fs. */
#define DA_fNull 0x0001
#define DA_fBOF  0x0002
#define DA_fEOF  0x0004

/* The data of the messages between a bound control and a data control. */
typedef struct {
  USHORT usVersion;
  SHORT  sAction;         /* DATA_. */
  HCTL   hctlData;        /* The data control. */
  HCTL   hctlBound;       /* The bound control. */
  HSZ    hszDataField;    /* The field name, or NULL. */
  SHORT  sDataFieldIndex; /* The field index when there is no name. */
  USHORT usDataType;      /* The DT_ type for the data. */
  HLSTR  hlstrBookMark;
  FSHORT fs;              /* DA_ flags. */
  LONG   lData;
  ULONG  ulChunkOffset;
  ULONG  ulChunkNumBytes;
} DATAACCESS, FAR *LPDATAACCESS;

/* The control data of hctl, as a pointer to the structure type##. */
#define CONTROLDEREF(type, hctl) ((P##type)(VBDerefControl(hctl)))

#define VBAPI FAR PASCAL

/* General functions. */
USHORT  VBAPI VBGetMode(VOID);
USHORT  VBAPI VBGetVersion(VOID);

/* Controls. */
LPVOID  VBAPI VBDerefControl(HCTL hctl);
LONG    VBAPI VBDefControlProc(HCTL hctl, HWND hwnd, USHORT msg, USHORT wp, LONG lp);
BOOL    VBAPI VBRegisterModel(HANDLE hmod, LPMODEL lpmdl);
HWND    VBAPI VBGetControlHwnd(HCTL hctl);
HANDLE  VBAPI VBGetHInstance(VOID);
LPMODEL VBAPI VBGetControlModel(HCTL hctl);
LPSTR   VBAPI VBGetControlName(HCTL hctl, LPSTR lpszName);
HCTL    VBAPI VBGetHwndControl(HWND hwnd);
LONG    VBAPI VBSendControlMsg(HCTL hctl, USHORT msg, USHORT wp, LONG lp);
LONG    VBAPI VBSuperControlProc(HCTL hctl, USHORT msg, USHORT wp, LONG lp);
ERR     VBAPI VBRecreateControlHwnd(HCTL hctl);
VOID    VBAPI VBDirtyForm(HCTL hctl);
ERR     VBAPI VBSetErrorMessage(ERR err, LPSTR lpstr);
VOID    VBAPI VBGetAppTitle(LPSTR lpstr, USHORT cbMax);
int     VBAPI VBDialogBoxParam(HANDLE hinst, LPSTR pszTemplateName, FARPROC lpDialogFunc,
                               LONG lp);
ULONG   VBAPI VBSetControlFlags(HCTL hctl, ULONG mask, ULONG value);
HCTL    VBAPI VBGetCapture(VOID);
VOID    VBAPI VBSetCapture(HCTL hctl);
VOID    VBAPI VBReleaseCapture(VOID);
VOID    VBAPI VBMoveControl(HCTL hctl, LPRECT lprect, BOOL fRepaint);
VOID    VBAPI VBGetControlRect(HCTL hctl, LPRECT lprect);
VOID    VBAPI VBGetRectInContainer(HCTL hctl, LPRECT lprect);
VOID    VBAPI VBGetClientRect(HCTL hctl, LPRECT lprect);
VOID    VBAPI VBClientToScreen(HCTL hctl, LPPOINT lppt);
VOID    VBAPI VBScreenToClient(HCTL hctl, LPPOINT lppt);
BOOL    VBAPI VBIsControlVisible(HCTL hctl);
BOOL    VBAPI VBIsControlEnabled(HCTL hctl);
VOID    VBAPI VBInvalidateRect(HCTL hctl, LPRECT lprect, BOOL fEraseBkGnd);
VOID    VBAPI VBUpdateControl(HCTL hctl);
HCTL    VBAPI VBGetControl(HCTL hctl, WORD gc);
VOID    VBAPI VBZOrder(HCTL hctl, WORD zorder);

/* The gc of VBGetControl. */
#define GC_FIRSTSIBLING  0x0000
#define GC_LASTSIBLING   0x0001
#define GC_NEXTSIBLING   0x0002
#define GC_PREVSIBLING   0x0003
#define GC_CHILD         0x0005
#define GC_CONTAINER     0x1000
#define GC_FORM          0x1001
#define GC_FIRSTCONTROL  0x1002
#define GC_NEXTCONTROL   0x1003
#define GC_FIRSTSELECTED 0x1004
#define GC_NEXTSELECTED  0x1005

/* The zorder of VBZOrder. */
#define ZORDER_FRONT 0
#define ZORDER_BACK  1

/* Strings in a local heap (HSZ). */
HSZ     VBAPI VBCreateHsz(HANDLE seg, LPSTR lpszString);
HSZ     VBAPI VBDestroyHsz(HSZ hsz);
LPSTR   VBAPI VBDerefHsz(HSZ hsz);
LPSTR   VBAPI VBLockHsz(HSZ hsz);
VOID    VBAPI VBUnlockHsz(HSZ hsz);

/* Basic strings (HLSTR). */
HLSTR   VBAPI VBCreateHlstr(LPVOID pb, USHORT cbLen);
HLSTR   VBAPI VBCreateTempHlstr(LPVOID pb, USHORT cbLen);
VOID    VBAPI VBDestroyHlstr(HLSTR hlstr);
LPSTR   VBAPI VBDerefHlstr(HLSTR hlstr);
LPSTR   VBAPI VBDerefHlstrLen(HLSTR hlstr, LPUSHORT lpcbLen);
LPSTR   VBAPI VBDerefZeroTermHlstr(HLSTR hlstr);
USHORT  VBAPI VBGetHlstr(HLSTR hlstr, LPVOID pb, USHORT cbLen);
USHORT  VBAPI VBGetHlstrLen(HLSTR hlstr);
ERR     VBAPI VBSetHlstr(LPHLSTR lphlstr, LPVOID pb, USHORT cbLen);
ERR     VBAPI VBResizeHlstr(HLSTR hlstr, USHORT newCbLen);

/* Variants. */
ERR     VBAPI VBCoerceVariant(LPVAR lpVar, SHORT vtype, LPVOID lpData);
SHORT   VBAPI VBGetVariantType(LPVAR lpVar);
SHORT   VBAPI VBGetVariantValue(LPVAR lpVar, LPVAL lpVal);
ERR     VBAPI VBSetVariantValue(LPVAR lpVar, SHORT vtype, LPVOID lpData);

/* Basic arrays. VBArrayBounds gives the low bound in the low word and the
 * high bound in the high word, or AB_INVALIDINDEX. */
LPVOID  VBAPI VBArrayElement(HAD hAD, SHORT cIndex, LPSHORT lpi);
LONG    VBAPI VBArrayBounds(HAD hAD, SHORT index);
USHORT  VBAPI VBArrayElemSize(HAD hAD);
LPVOID  VBAPI VBArrayFirstElem(HAD hAD);
SHORT   VBAPI VBArrayIndexCount(HAD hAD);
#define AB_INVALIDINDEX 1
#define LOBOUND(x)      ((SHORT)((x) & 0xFFFF))
#define HIBOUND(x)      ((SHORT)((x) >> 16))

/* Errors. */
VOID    VBAPI VBRuntimeError(ERR err);

/* The state of the floating-point unit. */
USHORT  VBAPI VBCbSaveFPState(LPVOID pb, USHORT cb);
VOID    VBAPI VBRestoreFPState(LPVOID pb);

/* Events. VBFireEvent gives EVENTNOTFIRED when Basic has no procedure
 * for the event. */
ERR     VBAPI VBFireEvent(HCTL hctl, USHORT idEvent, LPVOID lpparams);
#define EVENTNOTFIRED (0x7FFF)

/* Properties. */
ERR     VBAPI VBGetControlProperty(HCTL hctl, USHORT idProp, LPVOID pdata);
ERR     VBAPI VBSetControlProperty(HCTL hctl, USHORT idProp, LONG data);

/* Pictures and colors. */
HPIC    VBAPI VBAllocPic(LPPIC lppic);
HPIC    VBAPI VBAllocPicEx(LPPIC lppic, USHORT usVersion);
VOID    VBAPI VBFreePic(HPIC hpic);
HPIC    VBAPI VBGetPic(HPIC hpic, LPPIC lppic);
HPIC    VBAPI VBGetPicEx(HPIC hpic, LPPIC lppic, USHORT usVersion);
ERR     VBAPI VBPicFromCF(LPHPIC lphpic, HANDLE hData, WORD wFormat);
HPIC    VBAPI VBRefPic(HPIC hpic);
VOID    VBAPI VBPaletteChanged(HCTL hctl);
COLOR   VBAPI VBTranslateColor(HCTL hctl, COLOR clr);

/* Form files and Basic files. */
ERR     VBAPI VBReadFormFile(HFORMFILE hformfile, LPVOID pb, WORD cb);
ERR     VBAPI VBWriteFormFile(HFORMFILE hformfile, LPVOID pb, WORD cb);
LONG    VBAPI VBSeekFormFile(HFORMFILE hformfile, LONG offset);
LONG    VBAPI VBRelSeekFormFile(HFORMFILE hformfile, LONG offset);
ERR     VBAPI VBReadBasicFile(USHORT usFileNo, LPVOID pb, WORD cb);
ERR     VBAPI VBWriteBasicFile(USHORT usFileNo, LPVOID pb, WORD cb);
LONG    VBAPI VBSeekBasicFile(USHORT usFileNo, LONG offset);
LONG    VBAPI VBRelSeekBasicFile(USHORT usFileNo, LONG offset);

/* Pixels and twips. */
LONG    VBAPI VBYPixelsToTwips(SHORT Pixels);
LONG    VBAPI VBXPixelsToTwips(SHORT Pixels);
SHORT   VBAPI VBYTwipsToPixels(LONG Twips);
SHORT   VBAPI VBXTwipsToPixels(LONG Twips);

/* Links (DDE). */
ERR     VBAPI VBLinkPostAdvise(HCTL hctl);
BOOL    VBAPI VBPasteLinkOk(HANDLE FAR *phTriplet, HCTL hctl);
VOID    VBAPI VBLinkMakeItemName(HCTL hctl, LPSTR lpszBuf);

/* Bound controls. */
HCTL    VBAPI VBGetDataSourceControl(HCTL hctl, BOOL FAR *pfSet);

/* Formats a value as the Basic Format function does. */
SHORT   VBAPI VBFormat(SHORT vtype, LPVOID lpData, LPSTR lpszFmt, LPVOID pb, USHORT cb);

/* What a VBX exports. Visual Basic finds them by their upper-case names,
 * which are the names that FAR PASCAL gives. */
#ifndef NOCCEXPORTS
BOOL FAR PASCAL __export        VBINITCC(USHORT usVersion, BOOL fRuntime);
VOID FAR PASCAL __export        VBTERMCC(VOID);
LPMODELINFO FAR PASCAL __export VBGetModelInfo(USHORT usVersion);
#endif

/* Visual Basic messages to a control. */
#define VBM__BASE             (WM_USER + 0x0C00)
#define VBM_CREATED           (VBM__BASE + 0x00) /* MODEL_fLoadMsg. */
#define VBM_LOADED            (VBM__BASE + 0x01) /* MODEL_fLoadMsg. */
#define VBM_INITIALIZE        (VBM__BASE + 0x02) /* MODEL_fInitMsg. */
#define VBM_GETPROPERTY       (VBM__BASE + 0x03) /* wp iprop, lp pdata. */
#define VBM_CHECKPROPERTY     (VBM__BASE + 0x04) /* wp iprop, lp data. */
#define VBM_SETPROPERTY       (VBM__BASE + 0x05) /* wp iprop, lp data. */
#define VBM_SAVEPROPERTY      (VBM__BASE + 0x06) /* wp iprop, lp file. */
#define VBM_LOADPROPERTY      (VBM__BASE + 0x07) /* wp iprop, lp file. */
#define VBM_GETPROPERTYHSZ    (VBM__BASE + 0x08) /* wp iprop, lp HSZ FAR *. */
#define VBM_INITPROPPOPUP     (VBM__BASE + 0x09)
#define VBM_QPASTEOK          (VBM__BASE + 0x0A) /* wp PT_. */
#define VBM_PASTE             (VBM__BASE + 0x0B) /* wp PT_. */
#define VBM_COPY              (VBM__BASE + 0x0C)
#define VBM_MNEMONIC          (VBM__BASE + 0x0D)
#define VBM_FIREEVENT         (VBM__BASE + 0x0E) /* wp event. */
#define VBM_DRAGOVER          (VBM__BASE + 0x0F) /* lp LPDRAGINFO. */
#define VBM_DRAGDROP          (VBM__BASE + 0x10) /* lp LPDRAGINFO. */
#define VBM_METHOD            (VBM__BASE + 0x11) /* wp METH_, lp params. */
#define VBM_CANCELMODE        (VBM__BASE + 0x12)
#define VBM_PAINT             (VBM__BASE + 0x14) /* wp hdc, lp LPRECT. */
#define VBM_HITTEST           (VBM__BASE + 0x15) /* lp LPHITTEST. */
#define VBM_PAINTMULTISEL     (VBM__BASE + 0x16)
#define VBM_PAINTOUTLINE      (VBM__BASE + 0x17)
#define VBM_PALETTECHANGED    (VBM__BASE + 0x1B)
#define VBM_GETPALETTE        (VBM__BASE + 0x1C)
#define VBM_LINKENUMFORMATS   (VBM__BASE + 0x1E)
#define VBM_LINKGETITEMNAME   (VBM__BASE + 0x1F)
#define VBM_LINKGETDATA       (VBM__BASE + 0x20)
#define VBM_LINKSETDATA       (VBM__BASE + 0x21)
#define VBM_HELP              (VBM__BASE + 0x22) /* LOBYTE(wp) VBHELP_. */
#define VBM_GETDEFSIZE        (VBM__BASE + 0x23)
#define VBM_SAVETEXTPROPERTY  (VBM__BASE + 0x24)
#define VBM_LOADTEXTPROPERTY  (VBM__BASE + 0x25)
#define VBM_ISMNEMONIC        (VBM__BASE + 0x26)
#define VBM_WANTSPECIALKEY    (VBM__BASE + 0x27)
#define VBM_SELECTED          (VBM__BASE + 0x28)
#define VBM_DATA_INITIATE     (VBM__BASE + 0x29)
#define VBM_DATA_TERMINATE    (VBM__BASE + 0x2A)
#define VBM_DATA_AVAILABLE    (VBM__BASE + 0x2B)
#define VBM_DATA_GET          (VBM__BASE + 0x2C)
#define VBM_DATA_REQUEST      (VBM__BASE + 0x2D)
#define VBM_DATA_SET          (VBM__BASE + 0x2E)
#define VBM_DATA_METHOD       (VBM__BASE + 0x2F)

/* VBM_QPASTEOK and VBM_PASTE. */
#define PT_PASTE     0
#define PT_PASTELINK 1

/* VBM_METHOD. */
#define METH_ADDITEM    0x0001
#define METH_REMOVEITEM 0x0002
#define METH_REFRESH    0x0003
#define METH_MOVE       0x0004
#define METH_DRAG       0x0005
#define METH_LINKSEND   0x0006
#define METH_ZORDER     0x0007
#define METH_CLEAR      0x000A

/* VBM_HITTEST: the result, and the parameter. */
#define HT_ON           7
#define HT_SOLID_NEAR   6
#define HT_PATTERN_NEAR 5
#define HT_HOLLOW_NEAR  4
#define HT_SOLID        3
#define HT_PATTERN      2
#define HT_HOLLOW       1
#define HT_MISS         0
typedef struct tagHITTEST {
  POINT pt;   /* The point, in the coordinates of the parent. */
  RECT  rect; /* The control, in the coordinates of the parent. */
} HITTEST;
typedef HITTEST FAR *LPHITTEST;

/* VBM_HELP. */
#define VBHELP_PROP 0x0001
#define VBHELP_EVT  0x0002
#define VBHELP_CTL  0x0003

/* Notifications that the parent sends back to the control. */
#define VBN__BASE         (VBM__BASE + 0x1000)
#define VBN_COMMAND       (VBN__BASE + WM_COMMAND)
#define VBN_CTLCOLOR      (VBN__BASE + WM_CTLCOLOR)
#define VBN_DRAWITEM      (VBN__BASE + WM_DRAWITEM)
#define VBN_MEASUREITEM   (VBN__BASE + WM_MEASUREITEM)
#define VBN_DELETEITEM    (VBN__BASE + WM_DELETEITEM)
#define VBN_VKEYTOITEM    (VBN__BASE + WM_VKEYTOITEM)
#define VBN_CHARTOITEM    (VBN__BASE + WM_CHARTOITEM)
#define VBN_COMPAREITEM   (VBN__BASE + WM_COMPAREITEM)
#define VBN_HSCROLL       (VBN__BASE + WM_HSCROLL)
#define VBN_VSCROLL       (VBN__BASE + WM_VSCROLL)
#define VBN_PARENTNOTIFY  (VBN__BASE + WM_PARENTNOTIFY)

#endif /* !RC_INVOKED */

#ifndef RC_INVOKED
#pragma pack(pop)
#ifdef __cplusplus
}
#endif
#endif

#endif /* _INC_VBAPI */
