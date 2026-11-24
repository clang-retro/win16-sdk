/* OLE 2 through compobj.h, ole2.h and dispatch.h: the task allocator
 * (a COMPOBJ interface called through its vtable), GUIDs that COMPOBJ
 * exports, and BSTRs and VARIANTs from OLE2DISP. */
#include <stdio.h>
#include <string.h>
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

/* WinMain calls each part of the OLE 2 API once and prints ALL PASSED when
 * every check holds. */
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {
  LPMALLOC malloc_;
  void FAR *p;
  BSTR b;
  VARIANT v;

  CHECK(SUCCEEDED(CoInitialize(NULL)));
  CHECK(CoGetMalloc(MEMCTX_TASK, &malloc_) == S_OK);
  p = malloc_->lpVtbl->Alloc(malloc_, 100);
  CHECK(p != NULL);
  CHECK(malloc_->lpVtbl->GetSize(malloc_, p) >= 100);
  malloc_->lpVtbl->Free(malloc_, p);
  malloc_->lpVtbl->Release(malloc_);

  /* {00000000-0000-0000-C000-000000000046} */
  CHECK(IID_IUnknown.Data1 == 0 && IID_IUnknown.Data4[0] == 0xC0 &&
        IID_IUnknown.Data4[7] == 0x46);
  CHECK(!IsEqualGUID(&IID_IUnknown, &IID_IClassFactory));

  b = SysAllocString("hello");
  CHECK(b != NULL && SysStringLen(b) == 5 && _fstrcmp(b, "hello") == 0);
  VariantInit(&v);
  CHECK(v.vt == VT_EMPTY);
  v.vt = VT_BSTR;
  v.u.bstrVal = b;
  /* Wine has no VariantClear, so the string is freed here. */
  SysFreeString(v.u.bstrVal);

  CoUninitialize();
  if (!failures)
    printf("ALL PASSED\n");
  return failures;
}
