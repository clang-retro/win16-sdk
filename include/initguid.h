/* initguid.h: include it before the OLE headers in one source file of a
 * program, so that DEFINE_GUID defines the GUIDs that the headers declare
 * there instead of importing them. Written for clang-dos; AGPL-3.0-only,
 * see the win16-sdk LICENSE. */
#ifndef INITGUID
#define INITGUID
#endif
#ifdef DEFINE_GUID
#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)          \
  EXTERN_C const GUID __far name = {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}
#endif
