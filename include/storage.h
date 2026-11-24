/* storage.h: OLE 2 structured storage for 16-bit Windows (STORAGE.DLL):
 * compound files, IStorage, IStream, ILockBytes and IEnumSTATSTG. Written
 * for clang-dos from the published API; AGPL-3.0-only, see the win16-sdk
 * LICENSE. See compobj.h for how interfaces are called. */
#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <compobj.h>

/* The access modes of storages and streams. */
#define STGM_DIRECT 0x00000000L
#define STGM_TRANSACTED 0x00010000L
#define STGM_READ 0x00000000L
#define STGM_WRITE 0x00000001L
#define STGM_READWRITE 0x00000002L
#define STGM_SHARE_DENY_NONE 0x00000040L
#define STGM_SHARE_DENY_READ 0x00000030L
#define STGM_SHARE_DENY_WRITE 0x00000020L
#define STGM_SHARE_EXCLUSIVE 0x00000010L
#define STGM_PRIORITY 0x00040000L
#define STGM_DELETEONRELEASE 0x04000000L
#define STGM_CREATE 0x00001000L
#define STGM_CONVERT 0x00020000L
#define STGM_FAILIFTHERE 0x00000000L

#ifndef RC_INVOKED
#pragma pack(push, 1)
#ifdef __cplusplus
extern "C" {
#endif

typedef enum tagSTGTY {
  STGTY_STORAGE = 1,
  STGTY_STREAM = 2,
  STGTY_LOCKBYTES = 3,
  STGTY_PROPERTY = 4
} STGTY;

typedef enum tagSTREAM_SEEK {
  STREAM_SEEK_SET = 0,
  STREAM_SEEK_CUR = 1,
  STREAM_SEEK_END = 2
} STREAM_SEEK;

typedef enum tagLOCKTYPE {
  LOCK_WRITE = 1,
  LOCK_EXCLUSIVE = 2,
  LOCK_ONLYONCE = 4
} LOCKTYPE;

typedef enum tagSTGC {
  STGC_DEFAULT = 0,
  STGC_OVERWRITE = 1,
  STGC_ONLYIFCURRENT = 2,
  STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE = 4
} STGC;

typedef enum tagSTGMOVE { STGMOVE_MOVE = 0, STGMOVE_COPY = 1 } STGMOVE;

typedef enum tagSTATFLAG { STATFLAG_DEFAULT = 0, STATFLAG_NONAME = 1 } STATFLAG;

typedef LPSTR FAR *SNB;

/* What Stat returns about a storage, a stream or a byte array. */
typedef struct tagSTATSTG {
  char FAR *pwcsName;
  DWORD type;
  ULARGE_INTEGER cbSize;
  FILETIME mtime;
  FILETIME ctime;
  FILETIME atime;
  DWORD grfMode;
  DWORD grfLocksSupported;
  CLSID clsid;
  DWORD grfStateBits;
  DWORD reserved;
} STATSTG;

typedef interface IStream FAR *LPSTREAM;
typedef interface IStorage FAR *LPSTORAGE;
typedef interface ILockBytes FAR *LPLOCKBYTES;
typedef interface IEnumSTATSTG FAR *LPENUMSTATSTG;
typedef interface IRootStorage FAR *LPROOTSTORAGE;

#undef INTERFACE
#define INTERFACE ILockBytes
DECLARE_INTERFACE_(ILockBytes, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(ReadAt)(THIS_ ULARGE_INTEGER, VOID HUGEP *, ULONG, ULONG FAR *)
  PURE;
  STDMETHOD(WriteAt)(THIS_ ULARGE_INTEGER, VOID const HUGEP *, ULONG,
                     ULONG FAR *) PURE;
  STDMETHOD(Flush)(THIS) PURE;
  STDMETHOD(SetSize)(THIS_ ULARGE_INTEGER) PURE;
  STDMETHOD(LockRegion)(THIS_ ULARGE_INTEGER, ULARGE_INTEGER, DWORD) PURE;
  STDMETHOD(UnlockRegion)(THIS_ ULARGE_INTEGER, ULARGE_INTEGER, DWORD) PURE;
  STDMETHOD(Stat)(THIS_ STATSTG FAR *, DWORD) PURE;
};

#undef INTERFACE
#define INTERFACE IEnumSTATSTG
DECLARE_INTERFACE_(IEnumSTATSTG, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Next)(THIS_ ULONG, STATSTG FAR *, ULONG FAR *) PURE;
  STDMETHOD(Skip)(THIS_ ULONG) PURE;
  STDMETHOD(Reset)(THIS) PURE;
  STDMETHOD(Clone)(THIS_ IEnumSTATSTG FAR *FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IStream
DECLARE_INTERFACE_(IStream, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Read)(THIS_ VOID HUGEP *, ULONG, ULONG FAR *) PURE;
  STDMETHOD(Write)(THIS_ VOID const HUGEP *, ULONG, ULONG FAR *) PURE;
  STDMETHOD(Seek)(THIS_ LARGE_INTEGER, DWORD, ULARGE_INTEGER FAR *) PURE;
  STDMETHOD(SetSize)(THIS_ ULARGE_INTEGER) PURE;
  STDMETHOD(CopyTo)(THIS_ IStream FAR *, ULARGE_INTEGER, ULARGE_INTEGER FAR *,
                    ULARGE_INTEGER FAR *) PURE;
  STDMETHOD(Commit)(THIS_ DWORD) PURE;
  STDMETHOD(Revert)(THIS) PURE;
  STDMETHOD(LockRegion)(THIS_ ULARGE_INTEGER, ULARGE_INTEGER, DWORD) PURE;
  STDMETHOD(UnlockRegion)(THIS_ ULARGE_INTEGER, ULARGE_INTEGER, DWORD) PURE;
  STDMETHOD(Stat)(THIS_ STATSTG FAR *, DWORD) PURE;
  STDMETHOD(Clone)(THIS_ IStream FAR *FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IStorage
DECLARE_INTERFACE_(IStorage, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(CreateStream)(THIS_ const char FAR *, DWORD, DWORD, DWORD,
                          IStream FAR *FAR *) PURE;
  STDMETHOD(OpenStream)(THIS_ const char FAR *, void FAR *, DWORD, DWORD,
                        IStream FAR *FAR *) PURE;
  STDMETHOD(CreateStorage)(THIS_ const char FAR *, DWORD, DWORD, DWORD,
                           IStorage FAR *FAR *) PURE;
  STDMETHOD(OpenStorage)(THIS_ const char FAR *, IStorage FAR *, DWORD, SNB,
                         DWORD, IStorage FAR *FAR *) PURE;
  STDMETHOD(CopyTo)(THIS_ DWORD, IID const FAR *, SNB, IStorage FAR *) PURE;
  STDMETHOD(MoveElementTo)(THIS_ const char FAR *, IStorage FAR *,
                           const char FAR *, DWORD) PURE;
  STDMETHOD(Commit)(THIS_ DWORD) PURE;
  STDMETHOD(Revert)(THIS) PURE;
  STDMETHOD(EnumElements)(THIS_ DWORD, void FAR *, DWORD,
                          IEnumSTATSTG FAR *FAR *) PURE;
  STDMETHOD(DestroyElement)(THIS_ const char FAR *) PURE;
  STDMETHOD(RenameElement)(THIS_ const char FAR *, const char FAR *) PURE;
  STDMETHOD(SetElementTimes)(THIS_ const char FAR *, FILETIME const FAR *,
                             FILETIME const FAR *, FILETIME const FAR *)
  PURE;
  STDMETHOD(SetClass)(THIS_ REFCLSID) PURE;
  STDMETHOD(SetStateBits)(THIS_ DWORD, DWORD) PURE;
  STDMETHOD(Stat)(THIS_ STATSTG FAR *, DWORD) PURE;
};

#undef INTERFACE
#define INTERFACE IRootStorage
DECLARE_INTERFACE_(IRootStorage, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(SwitchToFile)(THIS_ LPSTR) PURE;
};

#if WINVER >= 0x030A
STDAPI StgCreateDocfile(const char FAR *, DWORD, DWORD, IStorage FAR *FAR *);
STDAPI StgCreateDocfileOnILockBytes(ILockBytes FAR *, DWORD, DWORD,
                                    IStorage FAR *FAR *);
STDAPI StgOpenStorage(const char FAR *, IStorage FAR *, DWORD, SNB, DWORD,
                      IStorage FAR *FAR *);
STDAPI StgOpenStorageOnILockBytes(ILockBytes FAR *, IStorage FAR *, DWORD,
                                  SNB, DWORD, IStorage FAR *FAR *);
STDAPI StgIsStorageFile(const char FAR *);
STDAPI StgIsStorageILockBytes(ILockBytes FAR *);
STDAPI StgSetTimes(char const FAR *, FILETIME const FAR *,
                   FILETIME const FAR *, FILETIME const FAR *);
#endif

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif /* RC_INVOKED */

#endif /* _STORAGE_H_ */
