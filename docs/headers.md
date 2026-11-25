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
| `ole2.h`, `moniker.h`, `dvobj.h` | OLE2 | OLE 2 data transfer, drag and drop, persistence, compound documents, monikers, in-place activation, `Ole*` |
| `dispatch.h`, `olenls.h` | OLE2DISP, TYPELIB, OLE2NLS | OLE Automation: `IDispatch`, `VARIANT`, `BSTR`, `SAFEARRAY`, `ITypeInfo`, `ITypeLib`, `IErrorInfo`; locales |
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
- In C, an interface is a structure with a `lpVtbl` pointer. Call a
  method as `p->lpVtbl->Release(p)`.
- In C++ in the large and huge models, an interface is a class with pure
  virtual methods. Call a method as `p->Release()`. The C++ object layout
  in these models is the same as the layout of an OLE object. In the
  other models, and when you define `CINTERFACE`, C++ uses the C form.
- In C++, `REFIID`, `REFCLSID` and `REFGUID` are references, and GUIDs
  have `==` and `!=`. In C, they are pointers.
- The GUIDs (`IID_IUnknown` and the rest) are far data that COMPOBJ, OLE2,
  OLE2DISP and AVIFILE export. To define a GUID of your own, include
  `initguid.h` in one source file before `DEFINE_GUID`.
- In C, `VARIANT` has its union in `u`. In C++, the union has no name
  (`v.lVal`), but `NONAMELESSUNION` gives the name `u`. The `V_` macros
  work in the two forms. The member `bool` is `boolVal` in C++ and C23,
  where `bool` is a keyword.
- `dispatch.h` declares the Win32 `VT_` types (`VT_I1`, `VT_UI4` and
  others) for code that is shared with Win32. 16-bit OLE does not make
  these types.
- OLE 1 (`ole.h`) and OLE 2 (`ole2.h`) have types and functions with the
  same names, such as `LPOLEOBJECT` and `OleCreate`. Do not include the
  two headers in one source file. The import names of the OLE 2
  functions `OleCreate`, `OleCreateFromFile`, `OleDraw`,
  `OleLoadFromStream` and `OleSaveToStream` are `OLE2_OLECREATE` and the
  others, so that a program does not import the OLE 1 function.

The ODBC and MAPI imports are by name, because Wine has no `.spec` files
for those DLLs to give the ordinals. The other imports are by ordinal, from
Wine's `.spec` files.

## Checking the headers
