/* compobj.h: the OLE 2 component object model for 16-bit Windows
 * (COMPOBJ.DLL): GUIDs, HRESULT and SCODE, interfaces, IUnknown,
 * IClassFactory and IMalloc, and the Co* functions. Written for clang-dos
 * from the published API; AGPL-3.0-only, see the win16-sdk LICENSE.
 *
 * The functions are FAR PASCAL (STDAPI); the methods of an interface are
 * FAR CDECL (STDMETHODCALLTYPE), with the interface pointer as their first
 * argument. Interface and data pointers are far. An interface is declared
 * as a structure that has a far pointer to its table of methods, in C and
 * in C++ alike, and is called as p->lpVtbl->Method(p, ...): the C++ ABI of
 * the compiler is not the one of Microsoft's 16-bit compiler, so an
 * interface is not a C++ class here. */
#ifndef _COMPOBJ_H_
#define _COMPOBJ_H_
#define _INC_COMPOBJ

#include <windows.h>
#include <scode.h>

#ifndef RC_INVOKED
#pragma pack(push, 1)

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#define STDAPICALLTYPE __far __pascal
#define STDMETHODCALLTYPE __far __cdecl
#define STDAPI EXTERN_C HRESULT STDAPICALLTYPE
#define STDAPI_(type) EXTERN_C type STDAPICALLTYPE
#define STDMETHODIMP HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type) type STDMETHODCALLTYPE
/* A pointer to a buffer that can be larger than 64 KiB. */
#define HUGEP __huge

/* Interfaces: a structure with a far pointer to its table of methods.
 *   #undef INTERFACE
 *   #define INTERFACE IFoo
 *   DECLARE_INTERFACE_(IFoo, IUnknown) {
 *     STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
 *     ...
 *   };
 */
#define interface struct
#define PURE
#define THIS_ INTERFACE FAR *This,
#define THIS INTERFACE FAR *This
#define STDMETHOD(method) HRESULT(STDMETHODCALLTYPE *method)
#define STDMETHOD_(type, method) type(STDMETHODCALLTYPE *method)
#define DECLARE_INTERFACE(iface)                                               \
  typedef interface iface {                                                    \
    const struct iface##Vtbl FAR *lpVtbl;                                      \
  } iface;                                                                     \
  typedef struct iface##Vtbl iface##Vtbl;                                      \
  struct iface##Vtbl
#define DECLARE_INTERFACE_(iface, baseiface) DECLARE_INTERFACE(iface)

#ifdef __cplusplus
extern "C" {
#endif

typedef char OLECHAR;
typedef OLECHAR FAR *LPOLESTR;
typedef const OLECHAR FAR *LPCOLESTR;
#define OLESTR(str) str

#ifndef _ULONG_DEFINED
#define _ULONG_DEFINED
typedef unsigned long ULONG;
#endif
typedef ULONG FAR *LPULONG;
typedef unsigned short USHORT;
typedef short SHORT;
typedef char CHAR;
/* In C++ WCHAR is wchar_t, so that L"" strings are WCHAR strings; in C
 * wchar_t is unsigned short, which is WORD. */
#ifdef __cplusplus
typedef wchar_t WCHAR;
#else
typedef WORD WCHAR;
#endif

/* 64-bit integers in two halves. Where the compiler has anonymous
 * structures (C++ and C11), they are unions with QuadPart too, as in
 * Win32. */
#if defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG HighPart;
  };
  struct {
    DWORD LowPart;
    LONG HighPart;
  } u;
  long long QuadPart;
} LARGE_INTEGER, FAR *PLARGE_INTEGER;
typedef union _ULARGE_INTEGER {
  struct {
    DWORD LowPart;
    DWORD HighPart;
  };
  struct {
    DWORD LowPart;
    DWORD HighPart;
  } u;
  unsigned long long QuadPart;
} ULARGE_INTEGER, FAR *PULARGE_INTEGER;
#else
typedef struct _LARGE_INTEGER {
  DWORD LowPart;
  LONG HighPart;
} LARGE_INTEGER, FAR *PLARGE_INTEGER;
typedef struct _ULARGE_INTEGER {
  DWORD LowPart;
  DWORD HighPart;
} ULARGE_INTEGER, FAR *PULARGE_INTEGER;
#endif

#ifndef _FILETIME_
#define _FILETIME_
typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, FAR *LPFILETIME;
#endif

/* GUIDs. */
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
  DWORD Data1;
  WORD Data2;
  WORD Data3;
  BYTE Data4[8];
} GUID;
#endif
typedef GUID FAR *LPGUID;
typedef GUID IID;
typedef IID FAR *LPIID;
typedef GUID CLSID;
typedef CLSID FAR *LPCLSID;
typedef const GUID FAR *REFGUID;
typedef const IID FAR *REFIID;
typedef const CLSID FAR *REFCLSID;

/* DEFINE_GUID declares a GUID that COMPOBJ.DLL (or another DLL) exports;
 * after #include <initguid.h> it defines it in the module instead. */
#ifdef INITGUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)          \
  EXTERN_C const GUID FAR name = {                                       \
      l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}
#else
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)          \
  EXTERN_C const GUID FAR name
#endif
#define DEFINE_OLEGUID(name, l, w1, w2)                                        \
  DEFINE_GUID(name, l, w1, w2, 0xC0, 0, 0, 0, 0, 0, 0, 0x46)

#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)

DEFINE_GUID(GUID_NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define IID_NULL GUID_NULL
#define CLSID_NULL GUID_NULL
DEFINE_OLEGUID(IID_IUnknown, 0x00000000L, 0, 0);
DEFINE_OLEGUID(IID_IClassFactory, 0x00000001L, 0, 0);
DEFINE_OLEGUID(IID_IMalloc, 0x00000002L, 0, 0);
DEFINE_OLEGUID(IID_IMarshal, 0x00000003L, 0, 0);
DEFINE_OLEGUID(IID_ILockBytes, 0x0000000aL, 0, 0);
DEFINE_OLEGUID(IID_IStorage, 0x0000000bL, 0, 0);
DEFINE_OLEGUID(IID_IStream, 0x0000000cL, 0, 0);
DEFINE_OLEGUID(IID_IEnumSTATSTG, 0x0000000dL, 0, 0);
DEFINE_OLEGUID(IID_IRootStorage, 0x00000012L, 0, 0);
DEFINE_OLEGUID(IID_IMessageFilter, 0x00000016L, 0, 0);
DEFINE_OLEGUID(IID_IExternalConnection, 0x00000019L, 0, 0);
DEFINE_OLEGUID(IID_IEnumUnknown, 0x00000100L, 0, 0);
DEFINE_OLEGUID(IID_IEnumString, 0x00000101L, 0, 0);
DEFINE_OLEGUID(CLSID_StdMarshal, 0x00000017L, 0, 0);

/* Where an object runs (CoGetClassObject, CoCreateInstance). */
typedef enum tagCLSCTX {
  CLSCTX_INPROC_SERVER = 1,
  CLSCTX_INPROC_HANDLER = 2,
  CLSCTX_LOCAL_SERVER = 4
} CLSCTX;
#define CLSCTX_ALL                                                             \
  (CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER)
#define CLSCTX_INPROC (CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER)
#define CLSCTX_SERVER (CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER)

/* CoRegisterClassObject. */
typedef enum tagREGCLS {
  REGCLS_SINGLEUSE = 0,
  REGCLS_MULTIPLEUSE = 1,
  REGCLS_MULTI_SEPARATE = 2
} REGCLS;

/* The memory contexts of CoGetMalloc. */
typedef enum tagMEMCTX {
  MEMCTX_TASK = 1,
  MEMCTX_SHARED = 2,
  MEMCTX_SAME = -2,
  MEMCTX_UNKNOWN = -1
} MEMCTX;

/* IUnknown. */
typedef interface IUnknown FAR *LPUNKNOWN;
#undef INTERFACE
#define INTERFACE IUnknown
DECLARE_INTERFACE(IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
};

/* IClassFactory. */
typedef interface IClassFactory FAR *LPCLASSFACTORY;
#undef INTERFACE
#define INTERFACE IClassFactory
DECLARE_INTERFACE_(IClassFactory, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(CreateInstance)(THIS_ LPUNKNOWN, REFIID, LPVOID FAR *) PURE;
  STDMETHOD(LockServer)(THIS_ BOOL) PURE;
};

/* IMalloc: the task allocator, which callers free OLE's memory with. */
typedef interface IMalloc FAR *LPMALLOC;
#undef INTERFACE
#define INTERFACE IMalloc
DECLARE_INTERFACE_(IMalloc, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD_(void FAR *, Alloc)(THIS_ ULONG) PURE;
  STDMETHOD_(void FAR *, Realloc)(THIS_ void FAR *, ULONG) PURE;
  STDMETHOD_(void, Free)(THIS_ void FAR *) PURE;
  STDMETHOD_(ULONG, GetSize)(THIS_ void FAR *) PURE;
  STDMETHOD_(int, DidAlloc)(THIS_ void FAR *) PURE;
  STDMETHOD_(void, HeapMinimize)(THIS) PURE;
};

/* IEnumUnknown and IEnumString. */
typedef interface IEnumUnknown FAR *LPENUMUNKNOWN;
#undef INTERFACE
#define INTERFACE IEnumUnknown
DECLARE_INTERFACE_(IEnumUnknown, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Next)(THIS_ ULONG, LPUNKNOWN FAR *, ULONG FAR *) PURE;
  STDMETHOD(Skip)(THIS_ ULONG) PURE;
  STDMETHOD(Reset)(THIS) PURE;
  STDMETHOD(Clone)(THIS_ IEnumUnknown FAR *FAR *) PURE;
};

typedef interface IEnumString FAR *LPENUMSTRING;
#undef INTERFACE
#define INTERFACE IEnumString
DECLARE_INTERFACE_(IEnumString, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Next)(THIS_ ULONG, LPSTR FAR *, ULONG FAR *) PURE;
  STDMETHOD(Skip)(THIS_ ULONG) PURE;
  STDMETHOD(Reset)(THIS) PURE;
  STDMETHOD(Clone)(THIS_ IEnumString FAR *FAR *) PURE;
};

/* IMessageFilter: how a program answers calls while it is busy. */
typedef struct tagINTERFACEINFO {
  LPUNKNOWN pUnk;
  IID iid;
  WORD wMethod;
} INTERFACEINFO, FAR *LPINTERFACEINFO;

typedef enum tagCALLTYPE {
  CALLTYPE_TOPLEVEL = 1,
  CALLTYPE_NESTED = 2,
  CALLTYPE_ASYNC = 3,
  CALLTYPE_TOPLEVEL_CALLPENDING = 4,
  CALLTYPE_ASYNC_CALLPENDING = 5
} CALLTYPE;

typedef enum tagSERVERCALL {
  SERVERCALL_ISHANDLED = 0,
  SERVERCALL_REJECTED = 1,
  SERVERCALL_RETRYLATER = 2
} SERVERCALL;

typedef enum tagPENDINGMSG {
  PENDINGMSG_CANCELCALL = 0,
  PENDINGMSG_WAITNOPROCESS = 1,
  PENDINGMSG_WAITDEFPROCESS = 2
} PENDINGMSG;

typedef interface IMessageFilter FAR *LPMESSAGEFILTER;
#undef INTERFACE
#define INTERFACE IMessageFilter
DECLARE_INTERFACE_(IMessageFilter, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD_(DWORD, HandleInComingCall)(THIS_ DWORD, HTASK, DWORD, DWORD)
  PURE;
  STDMETHOD_(DWORD, RetryRejectedCall)(THIS_ HTASK, DWORD, DWORD) PURE;
  STDMETHOD_(DWORD, MessagePending)(THIS_ HTASK, DWORD, DWORD) PURE;
};

/* IExternalConnection. */
typedef interface IExternalConnection FAR *LPEXTERNALCONNECTION;
#undef INTERFACE
#define INTERFACE IExternalConnection
DECLARE_INTERFACE_(IExternalConnection, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD_(DWORD, AddConnection)(THIS_ DWORD, DWORD) PURE;
  STDMETHOD_(DWORD, ReleaseConnection)(THIS_ DWORD, DWORD, BOOL) PURE;
};

#if WINVER >= 0x030A
STDAPI_(DWORD) CoBuildVersion(void);
STDAPI CoInitialize(LPMALLOC);
STDAPI_(void) CoUninitialize(void);
STDAPI CoGetMalloc(DWORD, LPMALLOC FAR *);
STDAPI_(DWORD) CoGetCurrentProcess(void);
STDAPI CoCreateStandardMalloc(DWORD, LPMALLOC FAR *);
STDAPI CoGetClassObject(REFCLSID, DWORD, LPVOID, REFIID, LPVOID FAR *);
STDAPI CoRegisterClassObject(REFCLSID, LPUNKNOWN, DWORD, DWORD, LPDWORD);
STDAPI CoRevokeClassObject(DWORD);
STDAPI CoCreateInstance(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID FAR *);
STDAPI CoDisconnectObject(LPUNKNOWN, DWORD);
STDAPI CoLockObjectExternal(LPUNKNOWN, BOOL, BOOL);
STDAPI_(BOOL) CoIsHandlerConnected(LPUNKNOWN);
STDAPI_(HINSTANCE) CoLoadLibrary(LPSTR, BOOL);
STDAPI_(void) CoFreeLibrary(HINSTANCE);
STDAPI_(void) CoFreeAllLibraries(void);
STDAPI_(void) CoFreeUnusedLibraries(void);
STDAPI CoRegisterMessageFilter(LPMESSAGEFILTER, LPMESSAGEFILTER FAR *);
STDAPI CoGetTreatAsClass(REFCLSID, LPCLSID);
STDAPI CoTreatAsClass(REFCLSID, REFCLSID);
STDAPI_(BOOL) CoIsOle1Class(REFCLSID);
STDAPI CoCreateGuid(GUID FAR *);
STDAPI_(BOOL) CoFileTimeToDosDateTime(FILETIME FAR *, LPWORD, LPWORD);
STDAPI_(BOOL) CoDosDateTimeToFileTime(WORD, WORD, FILETIME FAR *);
STDAPI CoFileTimeNow(FILETIME FAR *);
STDAPI StringFromCLSID(REFCLSID, LPSTR FAR *);
STDAPI CLSIDFromString(LPSTR, LPCLSID);
STDAPI StringFromIID(REFIID, LPSTR FAR *);
STDAPI IIDFromString(LPSTR, LPIID);
STDAPI ProgIDFromCLSID(REFCLSID, LPSTR FAR *);
STDAPI CLSIDFromProgID(LPCSTR, LPCLSID);
STDAPI_(int) StringFromGUID2(REFGUID, LPSTR, int);
STDAPI_(BOOL) IsEqualGUID(REFGUID, REFGUID);
STDAPI_(BOOL) IsValidInterface(LPVOID);
STDAPI_(BOOL) IsValidIid(REFIID);
#endif

/* Defined by an in-process server DLL, which COMPOBJ calls. */
STDAPI DllGetClassObject(REFCLSID, REFIID, LPVOID FAR *);
STDAPI DllCanUnloadNow(void);

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif /* RC_INVOKED */

#endif /* _COMPOBJ_H_ */
