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
| `penwin.h`, `skbapi.h` | PENWIN | Pen Windows 1.0 (Windows for Pen Computing) and, with `PENVER` 0x0200, the 16-bit API of Pen Services 2.0 |
| `w32sut.h` | WIN32S16 | The Universal Thunk of Win32s: a Win32 program on Windows 3.1 calls a 16-bit DLL |
| `wownt16.h` | KERNEL | The generic thunks: a 16-bit program calls a 32-bit DLL on Windows NT 3.5 and later and on Windows 95 and later |
| `vbapi.h` | (Visual Basic) | The Visual Basic 1.0, 2.0 and 3.0 custom control interface (VBX); the functions are in `vbapi.lib` |

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
for those DLLs to give the ordinals. The PENWIN imports are by ordinal,
from the names and ordinals of `PENWIN.LIB` in the Windows 3.1 SDK and in
the 16-bit libraries of the Win32 SDK. `RemovePenDataStrokes` is imported
by name, because Pen Windows 1.0 and Pen Services 2.0 give it different
ordinals. The WIN32S16 imports are by name, with the names spelled as
WIN32S16.DLL exports them (`UTLinearToSelectorOffset`). The other imports
are by ordinal, from Wine's `.spec` files.

### Pen Windows

- `PENVER` is the Pen Windows version that the program needs. The default
  is 0x0100, Pen Windows 1.0 on Windows 3.1. With 0x0100, the header
  declares only the functions of Pen Windows 1.0, and `DestroyPenData`
  and `EndEnumStrokes` are macros, as in Pen Windows 1.0. Define `PENVER`
  as 0x0200 for the 16-bit API of Pen Services 2.0.
- `ALC_ALL`, `ALC_RESERVED` and `BXS_MASK` have the values of the
  version that `PENVER` selects.
- Each pointer in a structure or a parameter is a far pointer.
- The types and the constants come from mingw-w64's `penwin.h` (public
  domain). The structures have the 16-bit sizes, because `int` and `UINT`
  are 16 bits. The functions are the functions of each version's
  `PENWIN.LIB`.
- Wine has no PENWIN.DLL. `tests/win16/penut.c` makes a DLL that calls
  Pen Windows, and the test checks the imports of the DLL.

### The Universal Thunk and the generic thunks

- `w32sut.h` serves the two halves of a Universal Thunk. A 16-bit target
  gets the 16-bit half (`W32SUT_16`): `UT16CBPROC`,
  `UTLinearToSelectorOffset` and `UTSelectorOffsetToLinear`. A 32-bit
  target gets the Win32 half (`W32SUT_32`): `UT32PROC`, `UTRegister` and
  `UTUnRegister`, which are in the KERNEL32 import library of win32-sdk's
  sysroots. To use the header in a Win32 program, add the win16-sdk
  `include` directory after the sysroot's headers
  (`-idirafter <win16-sdk>/include`).
- The 16-bit DLL exports the two functions whose names the Win32 program
  gives to `UTRegister`: an init function,
  `DWORD FAR PASCAL UTInit(UT16CBPROC, LPVOID)`, and a proc function,
  `DWORD FAR PASCAL UTProc(LPVOID, DWORD)`.
- `wownt16.h` declares `LoadLibraryEx32W`, `GetProcAddress32W`,
  `FreeLibrary32W`, `GetVDMPointer32W` and `CallProcEx32W` (a `CDECL`
  function, `_CallProcEx32W` in the import list). KERNEL has them from
  Windows NT 3.5 and Windows 95 on. Windows 3.1 does not have them.

### Visual Basic custom controls (VBX)

- A custom control is a DLL with the extension `.VBX`. It exports
  `VBINITCC`, `VBTERMCC` and `VBGetModelInfo`, which `vbapi.h` declares.
  `FAR PASCAL` gives them the upper-case names that Visual Basic looks
  for. `VBINITCC` registers each `MODEL` with `VBRegisterModel`.
- The `VB` functions are not in an import list. Visual Basic puts the
  address of its dispatcher at offset 0x20 of its stack segment, and each
  function is a stub that jumps there with the number of the function in
  `BX`. The stubs are in `lib/win16/vbapi.lib` of the sysroot, which has
  one library for all the memory models. Give its path to the link:
  `clang --config=win31.cfg -shared ctl.c $SYSROOT/lib/win16/vbapi.lib -o CTL.VBX`.
- The structures are packed to 1 byte, and their pointers to names and
  lists (`PSTR`, `NPPROPLIST`) are near pointers to the data segment of
  the VBX. Use the small or the medium memory model.
- The function numbers come from the `VBAPI.LIB` of the Visual Basic 3.0
  Professional Edition Control Development Kit. `VBGetVersion` gives
  `VB100_VERSION` when the dispatcher is the one of Visual Basic 1.0, as
  that library does.
- Wine has no Visual Basic. `tests/win16/vbxctl.c` makes a VBX, and the
  test checks its imports and exports and the sizes of the structures.

## Checking the headers
