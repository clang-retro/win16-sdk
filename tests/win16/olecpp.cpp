// OLE 2 from C++: the interfaces are classes with virtual functions, a
// GUID goes by reference (REFIID), the VARIANT union has no name, and the
// V_ macros work. The program prints ALL PASSED when each check holds.
#include <stdio.h>
#include <windows.h>
#include <ole2.h>
#include <dispatch.h>

static int failures;
#define CHECK(c)                                                               \
  do {                                                                         \
    if (!(c)) {                                                                \
      printf("FAIL line %d: %s\n", __LINE__, #c);                              \
      failures++;                                                              \
    }                                                                          \
  } while (0)

// The Win32 members of the union do not change the size of a VARIANT.
static_assert(sizeof(VARIANT) == 16, "VARIANT");
static_assert(sizeof(CY) == 8, "CY");
static_assert(sizeof(TYPEDESC) == 6, "TYPEDESC");
static_assert(sizeof(ELEMDESC) == 12, "ELEMDESC");
static_assert(sizeof(TYPEATTR) == 70, "TYPEATTR");

/* Gives TRUE when the two IIDs are the same, through a REFIID. */
static BOOL SameIID(REFIID a, REFIID b) { return IsEqualIID(a, b); }

/* WinMain calls the task allocator through C++ and checks the GUIDs and
 * a VARIANT. */
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  CHECK(SUCCEEDED(CoInitialize(NULL)));
  LPMALLOC pMalloc = NULL;
  CHECK(CoGetMalloc(MEMCTX_TASK, &pMalloc) == S_OK);
  if (pMalloc != NULL) {
    void FAR *p = pMalloc->Alloc(64);
    CHECK(p != NULL);
    pMalloc->Free(p);
    // An IMalloc is an IUnknown. (Wine's 16-bit IMalloc gives a 32-bit
    // pointer from QueryInterface, thus the test does not call it.)
    LPUNKNOWN pUnk = pMalloc;
    CHECK(pUnk->AddRef() >= 2);
    pUnk->Release();
    pMalloc->Release();
  }
  CHECK(SameIID(IID_IUnknown, IID_IUnknown));
  CHECK(!SameIID(IID_IUnknown, IID_IClassFactory));
  CHECK(IID_IUnknown == IID_IUnknown);
  CHECK(IID_IUnknown != IID_IClassFactory);
  /* TYPELIB and OLE2DISP export these IIDs. */
  CHECK(IID_ITypeInfo.Data1 == 0x00020401L);
  CHECK(IID_IErrorInfo.Data1 == 0x1CF2B120L);

  VARIANT v;
  VariantInit(&v);
  v.vt = VT_I4;
  v.lVal = 42;
  CHECK(V_VT(&v) == VT_I4 && V_I4(&v) == 42);
  BSTR b = SysAllocString("text");
  v.vt = VT_BSTR;
  v.bstrVal = b;
  CHECK(SysStringLen(V_BSTR(&v)) == 4);
  SysFreeString(b);

  CoUninitialize();
  if (!failures) printf("ALL PASSED\n");
  return failures;
}
