# Headers

<!-- include README.md "Headers" -->

## The redistributable DLLs

These headers are for DLLs that a program installs with it on Windows 3.1
(or that Windows 3.1 has). Their functions are declared for 3.10 only.

| Header | DLL | API |
| --- | --- | --- |
| `ole.h` | OLECLI, OLESVR | OLE 1.0 clients and servers |
| `compobj.h`, `scode.h`, `initguid.h` | COMPOBJ | OLE 2: GUIDs, `HRESULT`, `IUnknown`, `IClassFactory`, `IMalloc`, `Co*` |
| `storage.h` | STORAGE | OLE 2 structured storage: `IStorage`, `IStream`, `ILockBytes`, `Stg*` |
| `ole2.h` | OLE2 | OLE 2 data transfer, drag and drop, persistence, `Ole*` |
| `dispatch.h`, `olenls.h` | OLE2DISP, TYPELIB, OLE2NLS | OLE Automation: `IDispatch`, `VARIANT`, `BSTR`, `SAFEARRAY`; locales |
| `vfw.h` | MSVIDEO, AVIFILE | Video for Windows 1.1: DrawDib, the compression manager, MCIWnd, AVI files |
| `wing.h` | WING | WinG 1.0 |
| `sql.h`, `sqlext.h` | ODBC | ODBC 2.0, core, level 1 and level 2 |
| `mapi.h` | MAPI | Simple MAPI 1.0 |

The 16-bit OLE 2 rules are not the 32-bit ones:

- The API functions are `FAR PASCAL` (`STDAPI`). The interface methods are
  `FAR CDECL`, with the object pointer as the first argument.
- Strings are `char` strings. `OLECHAR` is `char`, and `BSTR` is
  `char FAR *`.
- The `E_*` codes are `0x80000001` to `0x80000009`. Win32 changed them to
  `0x8000400x`.
- `SAFEARRAY` has 16-bit `cbElements` and `cLocks`, and a `handle`.
- The interfaces are C structures with a `lpVtbl` pointer, in C and in
  C++. Call a method as `p->lpVtbl->Release(p)`.
- The GUIDs (`IID_IUnknown` and the rest) are far data that COMPOBJ, OLE2,
  OLE2DISP and AVIFILE export. To define a GUID of your own, include
  `initguid.h` in one source file before `DEFINE_GUID`.
- `VARIANT` has its union in `u`. The member `bool` is `boolVal` in C++
  and C23, where `bool` is a keyword.

The ODBC and MAPI imports are by name, because Wine has no `.spec` files
for those DLLs to give the ordinals. The other imports are by ordinal, from
Wine's `.spec` files.

## Checking the headers
