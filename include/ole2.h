/* ole2.h: OLE 2 for 16-bit Windows (OLE2.DLL): data objects and formats,
 * the clipboard, drag and drop, advise sinks and persistent objects, with
 * the component object model (compobj.h) and structured storage
 * (storage.h). Written for clang-dos from the published API;
 * AGPL-3.0-only, see the win16-sdk LICENSE. See compobj.h for how
 * interfaces are called.
 *
 * The compound document interfaces (IOleObject, IOleClientSite, in-place
 * activation and monikers) are not declared here yet. */
#ifndef _OLE2_H_
#define _OLE2_H_
#define _INC_OLE2

#include <compobj.h>
#include <storage.h>

/* The aspects of a view of an object. */
#define DVASPECT_CONTENT 1
#define DVASPECT_THUMBNAIL 2
#define DVASPECT_ICON 4
#define DVASPECT_DOCPRINT 8

/* The kinds of storage medium. */
#define TYMED_HGLOBAL 1
#define TYMED_FILE 2
#define TYMED_ISTREAM 4
#define TYMED_ISTORAGE 8
#define TYMED_GDI 16
#define TYMED_MFPICT 32
#define TYMED_NULL 0

/* Advise flags. */
#define ADVF_NODATA 1
#define ADVF_PRIMEFIRST 2
#define ADVF_ONLYONCE 4
#define ADVF_DATAONSTOP 64
#define ADVFCACHE_NOHANDLER 8
#define ADVFCACHE_FORCEBUILTIN 16
#define ADVFCACHE_ONSAVE 32

/* IDataObject::EnumFormatEtc. */
#define DATADIR_GET 1
#define DATADIR_SET 2

/* The key state and effects of drag and drop. */
#define MK_ALT 0x0020
#define DROPEFFECT_NONE 0
#define DROPEFFECT_COPY 1
#define DROPEFFECT_MOVE 2
#define DROPEFFECT_LINK 4
#define DROPEFFECT_SCROLL 0x80000000L
#define DD_DEFSCROLLINSET 11
#define DD_DEFSCROLLDELAY 50
#define DD_DEFSCROLLINTERVAL 50
#define DD_DEFDRAGDELAY 200
#define DD_DEFDRAGMINDIST 2

/* The OLE 2 version (OleBuildVersion). */
#define rmm 23
#define rup 639

#ifndef RC_INVOKED
#pragma pack(push, 1)
#ifdef __cplusplus
extern "C" {
#endif

DEFINE_OLEGUID(IID_IEnumFORMATETC, 0x00000103L, 0, 0);
DEFINE_OLEGUID(IID_IEnumSTATDATA, 0x00000105L, 0, 0);
DEFINE_OLEGUID(IID_IPersistStream, 0x00000109L, 0, 0);
DEFINE_OLEGUID(IID_IPersistStorage, 0x0000010aL, 0, 0);
DEFINE_OLEGUID(IID_IPersistFile, 0x0000010bL, 0, 0);
DEFINE_OLEGUID(IID_IPersist, 0x0000010cL, 0, 0);
DEFINE_OLEGUID(IID_IDataObject, 0x0000010eL, 0, 0);
DEFINE_OLEGUID(IID_IAdviseSink, 0x0000010fL, 0, 0);
DEFINE_OLEGUID(IID_IDataAdviseHolder, 0x00000110L, 0, 0);
DEFINE_OLEGUID(IID_IOleWindow, 0x00000114L, 0, 0);
DEFINE_OLEGUID(IID_IDropSource, 0x00000121L, 0, 0);
DEFINE_OLEGUID(IID_IDropTarget, 0x00000122L, 0, 0);

#ifndef _POINTL_DEFINED
#define _POINTL_DEFINED
typedef struct tagPOINTL {
  LONG x;
  LONG y;
} POINTL, FAR *LPPOINTL;
#endif

typedef WORD CLIPFORMAT;
typedef CLIPFORMAT FAR *LPCLIPFORMAT;
typedef HGLOBAL HMETAFILEPICT;

/* The device that data is formatted for. */
typedef struct tagDVTARGETDEVICE {
  DWORD tdSize;
  WORD tdDriverNameOffset;
  WORD tdDeviceNameOffset;
  WORD tdPortNameOffset;
  WORD tdExtDevmodeOffset;
  BYTE tdData[1];
} DVTARGETDEVICE;

/* A format of data: clipboard format, device, aspect and medium. */
typedef struct tagFORMATETC {
  CLIPFORMAT cfFormat;
  DVTARGETDEVICE FAR *ptd;
  DWORD dwAspect;
  LONG lindex;
  DWORD tymed;
} FORMATETC, FAR *LPFORMATETC;

/* Data in a storage medium. pUnkForRelease frees it, or ReleaseStgMedium
 * does when it is NULL. */
typedef struct tagSTGMEDIUM {
  DWORD tymed;
  union {
    HANDLE hGlobal;
    LPSTR lpszFileName;
    IStream FAR *pstm;
    IStorage FAR *pstg;
  } u;
  IUnknown FAR *pUnkForRelease;
} STGMEDIUM, FAR *LPSTGMEDIUM;

typedef interface IAdviseSink FAR *LPADVISESINK;
typedef interface IDataObject FAR *LPDATAOBJECT;
typedef interface IEnumFORMATETC FAR *LPENUMFORMATETC;
typedef interface IEnumSTATDATA FAR *LPENUMSTATDATA;
typedef interface IDataAdviseHolder FAR *LPDATAADVISEHOLDER;
typedef interface IDropSource FAR *LPDROPSOURCE;
typedef interface IDropTarget FAR *LPDROPTARGET;
typedef interface IPersist FAR *LPPERSIST;
typedef interface IPersistStream FAR *LPPERSISTSTREAM;
typedef interface IPersistStorage FAR *LPPERSISTSTORAGE;
typedef interface IPersistFile FAR *LPPERSISTFILE;
typedef interface IOleWindow FAR *LPOLEWINDOW;

/* An advise connection of a data object. */
typedef struct tagSTATDATA {
  FORMATETC formatetc;
  DWORD advf;
  LPADVISESINK pAdvSink;
  DWORD dwConnection;
} STATDATA, FAR *LPSTATDATA;

#undef INTERFACE
#define INTERFACE IEnumFORMATETC
DECLARE_INTERFACE_(IEnumFORMATETC, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Next)(THIS_ ULONG, FORMATETC FAR *, ULONG FAR *) PURE;
  STDMETHOD(Skip)(THIS_ ULONG) PURE;
  STDMETHOD(Reset)(THIS) PURE;
  STDMETHOD(Clone)(THIS_ IEnumFORMATETC FAR *FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IEnumSTATDATA
DECLARE_INTERFACE_(IEnumSTATDATA, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Next)(THIS_ ULONG, STATDATA FAR *, ULONG FAR *) PURE;
  STDMETHOD(Skip)(THIS_ ULONG) PURE;
  STDMETHOD(Reset)(THIS) PURE;
  STDMETHOD(Clone)(THIS_ IEnumSTATDATA FAR *FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IAdviseSink
DECLARE_INTERFACE_(IAdviseSink, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD_(void, OnDataChange)(THIS_ FORMATETC FAR *, STGMEDIUM FAR *)
  PURE;
  STDMETHOD_(void, OnViewChange)(THIS_ DWORD, LONG) PURE;
  STDMETHOD_(void, OnRename)(THIS_ IUnknown FAR *) PURE;
  STDMETHOD_(void, OnSave)(THIS) PURE;
  STDMETHOD_(void, OnClose)(THIS) PURE;
};

#undef INTERFACE
#define INTERFACE IDataObject
DECLARE_INTERFACE_(IDataObject, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(GetData)(THIS_ LPFORMATETC, LPSTGMEDIUM) PURE;
  STDMETHOD(GetDataHere)(THIS_ LPFORMATETC, LPSTGMEDIUM) PURE;
  STDMETHOD(QueryGetData)(THIS_ LPFORMATETC) PURE;
  STDMETHOD(GetCanonicalFormatEtc)(THIS_ LPFORMATETC, LPFORMATETC) PURE;
  STDMETHOD(SetData)(THIS_ LPFORMATETC, STGMEDIUM FAR *, BOOL) PURE;
  STDMETHOD(EnumFormatEtc)(THIS_ DWORD, LPENUMFORMATETC FAR *) PURE;
  STDMETHOD(DAdvise)(THIS_ FORMATETC FAR *, DWORD, LPADVISESINK, DWORD FAR *)
  PURE;
  STDMETHOD(DUnadvise)(THIS_ DWORD) PURE;
  STDMETHOD(EnumDAdvise)(THIS_ LPENUMSTATDATA FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IDataAdviseHolder
DECLARE_INTERFACE_(IDataAdviseHolder, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Advise)(THIS_ LPDATAOBJECT, FORMATETC FAR *, DWORD, LPADVISESINK,
                    DWORD FAR *) PURE;
  STDMETHOD(Unadvise)(THIS_ DWORD) PURE;
  STDMETHOD(EnumAdvise)(THIS_ LPENUMSTATDATA FAR *) PURE;
  STDMETHOD(SendOnDataChange)(THIS_ LPDATAOBJECT, DWORD, DWORD) PURE;
};

#undef INTERFACE
#define INTERFACE IDropSource
DECLARE_INTERFACE_(IDropSource, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(QueryContinueDrag)(THIS_ BOOL, DWORD) PURE;
  STDMETHOD(GiveFeedback)(THIS_ DWORD) PURE;
};

#undef INTERFACE
#define INTERFACE IDropTarget
DECLARE_INTERFACE_(IDropTarget, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(DragEnter)(THIS_ LPDATAOBJECT, DWORD, POINTL, LPDWORD) PURE;
  STDMETHOD(DragOver)(THIS_ DWORD, POINTL, LPDWORD) PURE;
  STDMETHOD(DragLeave)(THIS) PURE;
  STDMETHOD(Drop)(THIS_ LPDATAOBJECT, DWORD, POINTL, LPDWORD) PURE;
};

#undef INTERFACE
#define INTERFACE IPersist
DECLARE_INTERFACE_(IPersist, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(GetClassID)(THIS_ LPCLSID) PURE;
};

#undef INTERFACE
#define INTERFACE IPersistStream
DECLARE_INTERFACE_(IPersistStream, IPersist) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(GetClassID)(THIS_ LPCLSID) PURE;
  STDMETHOD(IsDirty)(THIS) PURE;
  STDMETHOD(Load)(THIS_ LPSTREAM) PURE;
  STDMETHOD(Save)(THIS_ LPSTREAM, BOOL) PURE;
  STDMETHOD(GetSizeMax)(THIS_ ULARGE_INTEGER FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IPersistStorage
DECLARE_INTERFACE_(IPersistStorage, IPersist) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(GetClassID)(THIS_ LPCLSID) PURE;
  STDMETHOD(IsDirty)(THIS) PURE;
  STDMETHOD(InitNew)(THIS_ LPSTORAGE) PURE;
  STDMETHOD(Load)(THIS_ LPSTORAGE) PURE;
  STDMETHOD(Save)(THIS_ LPSTORAGE, BOOL) PURE;
  STDMETHOD(SaveCompleted)(THIS_ LPSTORAGE) PURE;
  STDMETHOD(HandsOffStorage)(THIS) PURE;
};

#undef INTERFACE
#define INTERFACE IPersistFile
DECLARE_INTERFACE_(IPersistFile, IPersist) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(GetClassID)(THIS_ LPCLSID) PURE;
  STDMETHOD(IsDirty)(THIS) PURE;
  STDMETHOD(Load)(THIS_ LPCSTR, DWORD) PURE;
  STDMETHOD(Save)(THIS_ LPCSTR, BOOL) PURE;
  STDMETHOD(SaveCompleted)(THIS_ LPCSTR) PURE;
  STDMETHOD(GetCurFile)(THIS_ LPSTR FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IOleWindow
DECLARE_INTERFACE_(IOleWindow, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(GetWindow)(THIS_ HWND FAR *) PURE;
  STDMETHOD(ContextSensitiveHelp)(THIS_ BOOL) PURE;
};

#if WINVER >= 0x030A
STDAPI_(DWORD) OleBuildVersion(void);
STDAPI OleInitialize(LPMALLOC);
STDAPI_(void) OleUninitialize(void);
STDAPI OleSetClipboard(LPDATAOBJECT);
STDAPI OleGetClipboard(LPDATAOBJECT FAR *);
STDAPI OleFlushClipboard(void);
STDAPI OleIsCurrentClipboard(LPDATAOBJECT);
STDAPI RegisterDragDrop(HWND, LPDROPTARGET);
STDAPI RevokeDragDrop(HWND);
STDAPI DoDragDrop(LPDATAOBJECT, LPDROPSOURCE, DWORD, LPDWORD);
STDAPI_(void) ReleaseStgMedium(LPSTGMEDIUM);
STDAPI_(HANDLE) OleDuplicateData(HANDLE, CLIPFORMAT, UINT);
STDAPI CreateDataAdviseHolder(LPDATAADVISEHOLDER FAR *);
STDAPI ReadClassStg(LPSTORAGE, CLSID FAR *);
STDAPI WriteClassStg(LPSTORAGE, REFCLSID);
STDAPI ReadClassStm(LPSTREAM, CLSID FAR *);
STDAPI WriteClassStm(LPSTREAM, REFCLSID);
STDAPI WriteFmtUserTypeStg(LPSTORAGE, CLIPFORMAT, LPSTR);
STDAPI ReadFmtUserTypeStg(LPSTORAGE, CLIPFORMAT FAR *, LPSTR FAR *);
STDAPI CreateStreamOnHGlobal(HGLOBAL, BOOL, LPSTREAM FAR *);
STDAPI GetHGlobalFromStream(LPSTREAM, HGLOBAL FAR *);
STDAPI CreateILockBytesOnHGlobal(HGLOBAL, BOOL, LPLOCKBYTES FAR *);
STDAPI GetHGlobalFromILockBytes(LPLOCKBYTES, HGLOBAL FAR *);
STDAPI GetClassFile(LPCSTR, CLSID FAR *);
STDAPI_(HGLOBAL) OleGetIconOfFile(LPSTR, BOOL);
STDAPI_(HGLOBAL) OleGetIconOfClass(REFCLSID, LPSTR, BOOL);
STDAPI_(HGLOBAL) OleMetafilePictFromIconAndLabel(HICON, LPSTR, LPSTR, UINT);
STDAPI OleRegGetUserType(REFCLSID, DWORD, LPSTR FAR *);
STDAPI OleRegGetMiscStatus(REFCLSID, DWORD, DWORD FAR *);
STDAPI OleRegEnumFormatEtc(REFCLSID, DWORD, LPENUMFORMATETC FAR *);
#endif

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif /* RC_INVOKED */

#endif /* _OLE2_H_ */
