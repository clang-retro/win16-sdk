/* dispatch.h: OLE Automation for 16-bit Windows (OLE2DISP.DLL, with
 * TYPELIB.DLL): BSTR strings, VARIANTs, safe arrays, IDispatch and the
 * functions that convert and dispatch them. Written for clang-dos from the
 * published API; AGPL-3.0-only, see the win16-sdk LICENSE. See compobj.h
 * for how interfaces are called.
 *
 * Strings are 8-bit (a BSTR is a far char pointer with the length before
 * it), as in all of 16-bit OLE. */
#ifndef _DISPATCH_H_
#define _DISPATCH_H_

#include <ole2.h>
#include <olenls.h>

/* The types of a VARIANT (vt), and the flags added to them. */
#define VT_EMPTY 0
#define VT_NULL 1
#define VT_I2 2
#define VT_I4 3
#define VT_R4 4
#define VT_R8 5
#define VT_CY 6
#define VT_DATE 7
#define VT_BSTR 8
#define VT_DISPATCH 9
#define VT_ERROR 10
#define VT_BOOL 11
#define VT_VARIANT 12
#define VT_UNKNOWN 13
#define VT_UI1 17
#define VT_VECTOR 0x1000
#define VT_ARRAY 0x2000
#define VT_BYREF 0x4000
#define VT_RESERVED 0x8000

/* VariantChangeType. */
#define VARIANT_NOVALUEPROP 1

/* IDispatch::Invoke. */
#define DISPATCH_METHOD 0x1
#define DISPATCH_PROPERTYGET 0x2
#define DISPATCH_PROPERTYPUT 0x4
#define DISPATCH_PROPERTYPUTREF 0x8

#define DISPID_UNKNOWN (-1)
#define DISPID_VALUE 0
#define DISPID_PROPERTYPUT (-3)
#define DISPID_NEWENUM (-4)
#define DISPID_EVALUATE (-5)
#define DISPID_CONSTRUCTOR (-6)
#define DISPID_DESTRUCTOR (-7)

/* Safe array features. */
#define FADF_AUTO 0x0001
#define FADF_STATIC 0x0002
#define FADF_EMBEDDED 0x0004
#define FADF_FIXEDSIZE 0x0010
#define FADF_BSTR 0x0100
#define FADF_UNKNOWN 0x0200
#define FADF_DISPATCH 0x0400
#define FADF_VARIANT 0x0800
#define FADF_RESERVED 0xF0E8

/* Errors of FACILITY_DISPATCH. */
#define DISP_E_UNKNOWNINTERFACE 0x80020001L
#define DISP_E_MEMBERNOTFOUND 0x80020003L
#define DISP_E_PARAMNOTFOUND 0x80020004L
#define DISP_E_TYPEMISMATCH 0x80020005L
#define DISP_E_UNKNOWNNAME 0x80020006L
#define DISP_E_NONAMEDARGS 0x80020007L
#define DISP_E_BADVARTYPE 0x80020008L
#define DISP_E_EXCEPTION 0x80020009L
#define DISP_E_OVERFLOW 0x8002000AL
#define DISP_E_BADINDEX 0x8002000BL
#define DISP_E_UNKNOWNLCID 0x8002000CL
#define DISP_E_ARRAYISLOCKED 0x8002000DL
#define DISP_E_BADPARAMCOUNT 0x8002000EL
#define DISP_E_PARAMNOTOPTIONAL 0x8002000FL
#define DISP_E_BADCALLEE 0x80020010L
#define DISP_E_NOTACOLLECTION 0x80020011L

#ifndef RC_INVOKED
#pragma pack(push, 1)
#ifdef __cplusplus
extern "C" {
#endif

DEFINE_GUID(IID_IDispatch, 0x00020400L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(IID_IEnumVARIANT, 0x00020404L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);

typedef char FAR *BSTR;
typedef BSTR FAR *LPBSTR;
typedef unsigned short VARTYPE;
typedef short VARIANT_BOOL;
#define VARIANT_TRUE ((VARIANT_BOOL)0xffff)
#define VARIANT_FALSE ((VARIANT_BOOL)0)
typedef double DATE;
typedef long DISPID;
typedef DISPID MEMBERID;

/* Currency: a 64-bit integer, scaled by 10000. */
typedef struct tagCY {
  unsigned long Lo;
  long Hi;
} CY;

typedef struct tagSAFEARRAYBOUND {
  unsigned long cElements;
  long lLbound;
} SAFEARRAYBOUND, FAR *LPSAFEARRAYBOUND;

typedef struct tagSAFEARRAY {
  unsigned short cDims;
  unsigned short fFeatures;
  unsigned short cbElements;
  unsigned short cLocks;
  unsigned long handle;
  void HUGEP *pvData;
  SAFEARRAYBOUND rgsabound[1];
} SAFEARRAY, FAR *LPSAFEARRAY;

typedef interface IDispatch FAR *LPDISPATCH;
typedef interface IEnumVARIANT FAR *LPENUMVARIANT;
typedef interface ITypeInfo FAR *LPTYPEINFO;
typedef interface ITypeLib FAR *LPTYPELIB;

typedef struct tagVARIANT VARIANT, VARIANTARG;
typedef VARIANT FAR *LPVARIANT;
typedef VARIANT FAR *LPVARIANTARG;

struct tagVARIANT {
  VARTYPE vt;
  unsigned short wReserved1;
  unsigned short wReserved2;
  unsigned short wReserved3;
  union {
    unsigned char bVal;
    short iVal;
    long lVal;
    float fltVal;
    double dblVal;
    VARIANT_BOOL boolVal;
    /* The 16-bit SDK name. "bool" is a keyword in C++ and C23. */
#if !defined(__cplusplus) && !defined(bool) && \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L)
    VARIANT_BOOL bool;
#endif
    SCODE scode;
    CY cyVal;
    DATE date;
    BSTR bstrVal;
    IUnknown FAR *punkVal;
    LPDISPATCH pdispVal;
    SAFEARRAY FAR *parray;
    unsigned char FAR *pbVal;
    short FAR *piVal;
    long FAR *plVal;
    float FAR *pfltVal;
    double FAR *pdblVal;
    VARIANT_BOOL FAR *pbool;
    SCODE FAR *pscode;
    CY FAR *pcyVal;
    DATE FAR *pdate;
    BSTR FAR *pbstrVal;
    IUnknown FAR *FAR *ppunkVal;
    LPDISPATCH FAR *ppdispVal;
    SAFEARRAY FAR *FAR *pparray;
    VARIANT FAR *pvarVal;
    void FAR *byref;
  } u;
};

/* The arguments of IDispatch::Invoke, last argument first. */
typedef struct tagDISPPARAMS {
  VARIANTARG FAR *rgvarg;
  DISPID FAR *rgdispidNamedArgs;
  unsigned int cArgs;
  unsigned int cNamedArgs;
} DISPPARAMS;

/* An exception that IDispatch::Invoke reports (DISP_E_EXCEPTION). */
typedef struct tagEXCEPINFO {
  unsigned short wCode;
  unsigned short wReserved;
  BSTR bstrSource;
  BSTR bstrDescription;
  BSTR bstrHelpFile;
  unsigned long dwHelpContext;
  void FAR *pvReserved;
  HRESULT(STDAPICALLTYPE *pfnDeferredFillIn)(struct tagEXCEPINFO FAR *);
  SCODE scode;
} EXCEPINFO, FAR *LPEXCEPINFO;

#undef INTERFACE
#define INTERFACE IDispatch
DECLARE_INTERFACE_(IDispatch, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(GetTypeInfoCount)(THIS_ unsigned int FAR *) PURE;
  STDMETHOD(GetTypeInfo)(THIS_ unsigned int, LCID, LPTYPEINFO FAR *) PURE;
  STDMETHOD(GetIDsOfNames)(THIS_ REFIID, char FAR *FAR *, unsigned int, LCID,
                           DISPID FAR *) PURE;
  STDMETHOD(Invoke)(THIS_ DISPID, REFIID, LCID, unsigned short,
                    DISPPARAMS FAR *, VARIANT FAR *, EXCEPINFO FAR *,
                    unsigned int FAR *) PURE;
};

#undef INTERFACE
#define INTERFACE IEnumVARIANT
DECLARE_INTERFACE_(IEnumVARIANT, IUnknown) {
  STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID FAR *) PURE;
  STDMETHOD_(ULONG, AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Release)(THIS) PURE;
  STDMETHOD(Next)(THIS_ unsigned long, VARIANT FAR *, unsigned long FAR *)
  PURE;
  STDMETHOD(Skip)(THIS_ unsigned long) PURE;
  STDMETHOD(Reset)(THIS) PURE;
  STDMETHOD(Clone)(THIS_ IEnumVARIANT FAR *FAR *) PURE;
};

#if WINVER >= 0x030A
/* Strings. */
STDAPI_(BSTR) SysAllocString(const char FAR *);
STDAPI_(int) SysReAllocString(BSTR FAR *, const char FAR *);
STDAPI_(BSTR) SysAllocStringLen(const char FAR *, unsigned int);
STDAPI_(int) SysReAllocStringLen(BSTR FAR *, const char FAR *, unsigned int);
STDAPI_(void) SysFreeString(BSTR);
STDAPI_(unsigned int) SysStringLen(BSTR);

/* VARIANTs. */
STDAPI_(void) VariantInit(VARIANTARG FAR *);
STDAPI VariantClear(VARIANTARG FAR *);
STDAPI VariantCopy(VARIANTARG FAR *, VARIANTARG FAR *);
STDAPI VariantCopyInd(VARIANT FAR *, VARIANTARG FAR *);
STDAPI VariantChangeType(VARIANTARG FAR *, VARIANTARG FAR *, unsigned short,
                         VARTYPE);
STDAPI VariantChangeTypeEx(VARIANTARG FAR *, VARIANTARG FAR *, LCID,
                           unsigned short, VARTYPE);
STDAPI_(int) DosDateTimeToVariantTime(unsigned short, unsigned short,
                                      double FAR *);
STDAPI_(int) VariantTimeToDosDateTime(double, unsigned short FAR *,
                                      unsigned short FAR *);

/* Safe arrays. */
STDAPI_(SAFEARRAY FAR *) SafeArrayCreate(VARTYPE, unsigned int,
                                         SAFEARRAYBOUND FAR *);
STDAPI SafeArrayDestroy(SAFEARRAY FAR *);
STDAPI SafeArrayAllocDescriptor(unsigned int, SAFEARRAY FAR *FAR *);
STDAPI SafeArrayAllocData(SAFEARRAY FAR *);
STDAPI SafeArrayDestroyDescriptor(SAFEARRAY FAR *);
STDAPI SafeArrayDestroyData(SAFEARRAY FAR *);
STDAPI SafeArrayRedim(SAFEARRAY FAR *, SAFEARRAYBOUND FAR *);
STDAPI_(unsigned int) SafeArrayGetDim(SAFEARRAY FAR *);
STDAPI_(unsigned int) SafeArrayGetElemsize(SAFEARRAY FAR *);
STDAPI SafeArrayGetUBound(SAFEARRAY FAR *, unsigned int, long FAR *);
STDAPI SafeArrayGetLBound(SAFEARRAY FAR *, unsigned int, long FAR *);
STDAPI SafeArrayLock(SAFEARRAY FAR *);
STDAPI SafeArrayUnlock(SAFEARRAY FAR *);
STDAPI SafeArrayAccessData(SAFEARRAY FAR *, void HUGEP *FAR *);
STDAPI SafeArrayUnaccessData(SAFEARRAY FAR *);
STDAPI SafeArrayGetElement(SAFEARRAY FAR *, long FAR *, void FAR *);
STDAPI SafeArrayPutElement(SAFEARRAY FAR *, long FAR *, void FAR *);
STDAPI SafeArrayCopy(SAFEARRAY FAR *, SAFEARRAY FAR *FAR *);
STDAPI SafeArrayPtrOfIndex(SAFEARRAY FAR *, long FAR *, void HUGEP *FAR *);

/* Dispatching. */
STDAPI DispGetParam(DISPPARAMS FAR *, unsigned int, VARTYPE, VARIANT FAR *,
                    unsigned int FAR *);
STDAPI DispGetIDsOfNames(LPTYPEINFO, char FAR *FAR *, unsigned int,
                         DISPID FAR *);
STDAPI DispInvoke(void FAR *, LPTYPEINFO, DISPID, unsigned short,
                  DISPPARAMS FAR *, VARIANT FAR *, EXCEPINFO FAR *,
                  unsigned int FAR *);
STDAPI CreateStdDispatch(IUnknown FAR *, void FAR *, LPTYPEINFO,
                         IUnknown FAR *FAR *);
STDAPI RegisterActiveObject(IUnknown FAR *, REFCLSID, void FAR *,
                            unsigned long FAR *);
STDAPI RevokeActiveObject(unsigned long, void FAR *);
STDAPI GetActiveObject(REFCLSID, void FAR *, IUnknown FAR *FAR *);

/* TYPELIB. */
STDAPI LoadTypeLib(const char FAR *, LPTYPELIB FAR *);
STDAPI LoadRegTypeLib(REFGUID, unsigned short, unsigned short, LCID,
                      LPTYPELIB FAR *);
STDAPI RegisterTypeLib(LPTYPELIB, char FAR *, char FAR *);
STDAPI QueryPathOfRegTypeLib(REFGUID, unsigned short, unsigned short, LCID,
                             LPBSTR);
STDAPI_(unsigned long) OaBuildVersion(void);
#endif

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif /* RC_INVOKED */

#endif /* _DISPATCH_H_ */
