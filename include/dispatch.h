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
typedef CY CURRENCY;

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

/* The union of a VARIANT has no name in C++, as in the 16-bit OLE 2 SDK
 * and in Win32, so that v.lVal works. In C, and with NONAMELESSUNION, it
 * is named u (v.u.lVal). The V_ macros work in both forms. */
#if defined(__cplusplus) && !defined(NONAMELESSUNION)
#define __VARIANT_UNION
#define __VARIANT_MEMBER(v, m) ((v).m)
#else
#define __VARIANT_UNION u
#define __VARIANT_MEMBER(v, m) ((v).u.m)
#endif

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
  } __VARIANT_UNION;
};

#ifndef V_VT
#define V_VT(X) ((X)->vt)
#define V_ISBYREF(X) (V_VT(X) & VT_BYREF)
#define V_ISARRAY(X) (V_VT(X) & VT_ARRAY)
#define V_UI1(X) __VARIANT_MEMBER(*(X), bVal)
#define V_I2(X) __VARIANT_MEMBER(*(X), iVal)
#define V_I4(X) __VARIANT_MEMBER(*(X), lVal)
#define V_R4(X) __VARIANT_MEMBER(*(X), fltVal)
#define V_R8(X) __VARIANT_MEMBER(*(X), dblVal)
#define V_BOOL(X) __VARIANT_MEMBER(*(X), boolVal)
#define V_ERROR(X) __VARIANT_MEMBER(*(X), scode)
#define V_CY(X) __VARIANT_MEMBER(*(X), cyVal)
#define V_DATE(X) __VARIANT_MEMBER(*(X), date)
#define V_BSTR(X) __VARIANT_MEMBER(*(X), bstrVal)
#define V_UNKNOWN(X) __VARIANT_MEMBER(*(X), punkVal)
#define V_DISPATCH(X) __VARIANT_MEMBER(*(X), pdispVal)
#define V_ARRAY(X) __VARIANT_MEMBER(*(X), parray)
#define V_BYREF(X) __VARIANT_MEMBER(*(X), byref)
#define V_VARIANTREF(X) __VARIANT_MEMBER(*(X), pvarVal)
#endif

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

/* The conversions of OLE2DISP. A string or an object source, and a string
 * result, take a locale and flags (VAR_ flags) too. */
#define VAR_TIMEVALUEONLY 0x0001L
#define VAR_DATEVALUEONLY 0x0002L
STDAPI VarI2FromI4(long, short FAR *);
STDAPI VarI2FromR4(float, short FAR *);
STDAPI VarI2FromR8(double, short FAR *);
STDAPI VarI2FromCy(CY, short FAR *);
STDAPI VarI2FromDate(DATE, short FAR *);
STDAPI VarI2FromStr(char FAR *, LCID, unsigned long, short FAR *);
STDAPI VarI2FromDisp(IDispatch FAR *, LCID, short FAR *);
STDAPI VarI2FromBool(VARIANT_BOOL, short FAR *);
STDAPI VarI2FromUI1(unsigned char, short FAR *);
STDAPI VarI4FromI2(short, long FAR *);
STDAPI VarI4FromR4(float, long FAR *);
STDAPI VarI4FromR8(double, long FAR *);
STDAPI VarI4FromCy(CY, long FAR *);
STDAPI VarI4FromDate(DATE, long FAR *);
STDAPI VarI4FromStr(char FAR *, LCID, unsigned long, long FAR *);
STDAPI VarI4FromDisp(IDispatch FAR *, LCID, long FAR *);
STDAPI VarI4FromBool(VARIANT_BOOL, long FAR *);
STDAPI VarI4FromUI1(unsigned char, long FAR *);
STDAPI VarR4FromI2(short, float FAR *);
STDAPI VarR4FromI4(long, float FAR *);
STDAPI VarR4FromR8(double, float FAR *);
STDAPI VarR4FromCy(CY, float FAR *);
STDAPI VarR4FromDate(DATE, float FAR *);
STDAPI VarR4FromStr(char FAR *, LCID, unsigned long, float FAR *);
STDAPI VarR4FromDisp(IDispatch FAR *, LCID, float FAR *);
STDAPI VarR4FromBool(VARIANT_BOOL, float FAR *);
STDAPI VarR4FromUI1(unsigned char, float FAR *);
STDAPI VarR8FromI2(short, double FAR *);
STDAPI VarR8FromI4(long, double FAR *);
STDAPI VarR8FromR4(float, double FAR *);
STDAPI VarR8FromCy(CY, double FAR *);
STDAPI VarR8FromDate(DATE, double FAR *);
STDAPI VarR8FromStr(char FAR *, LCID, unsigned long, double FAR *);
STDAPI VarR8FromDisp(IDispatch FAR *, LCID, double FAR *);
STDAPI VarR8FromBool(VARIANT_BOOL, double FAR *);
STDAPI VarR8FromUI1(unsigned char, double FAR *);
STDAPI VarDateFromI2(short, DATE FAR *);
STDAPI VarDateFromI4(long, DATE FAR *);
STDAPI VarDateFromR4(float, DATE FAR *);
STDAPI VarDateFromR8(double, DATE FAR *);
STDAPI VarDateFromCy(CY, DATE FAR *);
STDAPI VarDateFromStr(char FAR *, LCID, unsigned long, DATE FAR *);
STDAPI VarDateFromDisp(IDispatch FAR *, LCID, DATE FAR *);
STDAPI VarDateFromBool(VARIANT_BOOL, DATE FAR *);
STDAPI VarDateFromUI1(unsigned char, DATE FAR *);
STDAPI VarCyFromI2(short, CY FAR *);
STDAPI VarCyFromI4(long, CY FAR *);
STDAPI VarCyFromR4(float, CY FAR *);
STDAPI VarCyFromR8(double, CY FAR *);
STDAPI VarCyFromDate(DATE, CY FAR *);
STDAPI VarCyFromStr(char FAR *, LCID, unsigned long, CY FAR *);
STDAPI VarCyFromDisp(IDispatch FAR *, LCID, CY FAR *);
STDAPI VarCyFromBool(VARIANT_BOOL, CY FAR *);
STDAPI VarCyFromUI1(unsigned char, CY FAR *);
STDAPI VarBstrFromI2(short, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromI4(long, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromR4(float, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromR8(double, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromCy(CY, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromDate(DATE, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromDisp(IDispatch FAR *, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromBool(VARIANT_BOOL, LCID, unsigned long, BSTR FAR *);
STDAPI VarBstrFromUI1(unsigned char, LCID, unsigned long, BSTR FAR *);
STDAPI VarBoolFromI2(short, VARIANT_BOOL FAR *);
STDAPI VarBoolFromI4(long, VARIANT_BOOL FAR *);
STDAPI VarBoolFromR4(float, VARIANT_BOOL FAR *);
STDAPI VarBoolFromR8(double, VARIANT_BOOL FAR *);
STDAPI VarBoolFromDate(DATE, VARIANT_BOOL FAR *);
STDAPI VarBoolFromCy(CY, VARIANT_BOOL FAR *);
STDAPI VarBoolFromStr(char FAR *, LCID, unsigned long, VARIANT_BOOL FAR *);
STDAPI VarBoolFromDisp(IDispatch FAR *, LCID, VARIANT_BOOL FAR *);
STDAPI VarBoolFromUI1(unsigned char, VARIANT_BOOL FAR *);
STDAPI VarUI1FromI2(short, unsigned char FAR *);
STDAPI VarUI1FromI4(long, unsigned char FAR *);
STDAPI VarUI1FromR4(float, unsigned char FAR *);
STDAPI VarUI1FromR8(double, unsigned char FAR *);
STDAPI VarUI1FromCy(CY, unsigned char FAR *);
STDAPI VarUI1FromDate(DATE, unsigned char FAR *);
STDAPI VarUI1FromStr(char FAR *, LCID, unsigned long, unsigned char FAR *);
STDAPI VarUI1FromDisp(IDispatch FAR *, LCID, unsigned char FAR *);
STDAPI VarUI1FromBool(VARIANT_BOOL, unsigned char FAR *);

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif /* RC_INVOKED */

#endif /* _DISPATCH_H_ */
