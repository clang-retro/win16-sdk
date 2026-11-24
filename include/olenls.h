/* olenls.h: the OLE 2 national language support for 16-bit Windows
 * (OLE2NLS.DLL): locale identifiers, locale information, string mapping,
 * character types and locale-aware string comparison. Written for
 * clang-dos from the published API; AGPL-3.0-only, see the win16-sdk
 * LICENSE. */
#ifndef _INC_OLENLS
#define _INC_OLENLS

#include <windows.h>

#ifndef RC_INVOKED
#ifdef __cplusplus
extern "C" {
#endif

typedef DWORD LCID;
typedef WORD LANGID;
typedef DWORD LCTYPE;

/* Language and locale identifiers. */
#define MAKELANGID(p, s) ((((WORD)(s)) << 10) | (WORD)(p))
#define PRIMARYLANGID(lgid) ((WORD)(lgid) & 0x3ff)
#define SUBLANGID(lgid) ((WORD)(lgid) >> 10)
#define MAKELCID(lgid) ((DWORD)(((WORD)(lgid)) | (((DWORD)((WORD)(0))) << 16)))
#define LANGIDFROMLCID(lcid) ((WORD)(lcid))

#define LANG_NEUTRAL 0x00
#define LANG_CHINESE 0x04
#define LANG_CZECH 0x05
#define LANG_DANISH 0x06
#define LANG_GERMAN 0x07
#define LANG_GREEK 0x08
#define LANG_ENGLISH 0x09
#define LANG_SPANISH 0x0a
#define LANG_FINNISH 0x0b
#define LANG_FRENCH 0x0c
#define LANG_HUNGARIAN 0x0e
#define LANG_ICELANDIC 0x0f
#define LANG_ITALIAN 0x10
#define LANG_JAPANESE 0x11
#define LANG_KOREAN 0x12
#define LANG_DUTCH 0x13
#define LANG_NORWEGIAN 0x14
#define LANG_POLISH 0x15
#define LANG_PORTUGUESE 0x16
#define LANG_RUSSIAN 0x19
#define LANG_SLOVAK 0x1b
#define LANG_SWEDISH 0x1d
#define LANG_TURKISH 0x1f

#define SUBLANG_NEUTRAL 0x00
#define SUBLANG_DEFAULT 0x01
#define SUBLANG_SYS_DEFAULT 0x02
#define SUBLANG_ENGLISH_US 0x01
#define SUBLANG_ENGLISH_UK 0x02
#define SUBLANG_GERMAN 0x01
#define SUBLANG_FRENCH 0x01

#define LANG_SYSTEM_DEFAULT (MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT))
#define LANG_USER_DEFAULT (MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
#define LOCALE_SYSTEM_DEFAULT (MAKELCID(LANG_SYSTEM_DEFAULT))
#define LOCALE_USER_DEFAULT (MAKELCID(LANG_USER_DEFAULT))

/* The flags of CompareString and LCMapString. */
#define NORM_IGNORECASE 0x00000001
#define NORM_IGNORENONSPACE 0x00000002
#define NORM_IGNORESYMBOLS 0x00000004
#define NORM_IGNOREWIDTH 0x00000008
#define NORM_IGNOREKANATYPE 0x00000040

#define LCMAP_LOWERCASE 0x00000100
#define LCMAP_UPPERCASE 0x00000200
#define LCMAP_SORTKEY 0x00000400

/* The types of GetStringType. */
#define CT_CTYPE1 0x00000001
#define CT_CTYPE2 0x00000002
#define CT_CTYPE3 0x00000004

#define C1_UPPER 0x0001
#define C1_LOWER 0x0002
#define C1_DIGIT 0x0004
#define C1_SPACE 0x0008
#define C1_PUNCT 0x0010
#define C1_CNTRL 0x0020
#define C1_BLANK 0x0040
#define C1_XDIGIT 0x0080
#define C1_ALPHA 0x0100

/* The values of the locale information of GetLocaleInfoA. */
#define LOCALE_NOUSEROVERRIDE 0x80000000
#define LOCALE_ILANGUAGE 0x0001
#define LOCALE_SLANGUAGE 0x0002
#define LOCALE_SENGLANGUAGE 0x1001
#define LOCALE_SABBREVLANGNAME 0x0003
#define LOCALE_SNATIVELANGNAME 0x0004
#define LOCALE_ICOUNTRY 0x0005
#define LOCALE_SCOUNTRY 0x0006
#define LOCALE_SENGCOUNTRY 0x1002
#define LOCALE_SABBREVCTRYNAME 0x0007
#define LOCALE_SNATIVECTRYNAME 0x0008
#define LOCALE_IDEFAULTLANGUAGE 0x0009
#define LOCALE_IDEFAULTCOUNTRY 0x000A
#define LOCALE_IDEFAULTCODEPAGE 0x000B
#define LOCALE_SLIST 0x000C
#define LOCALE_IMEASURE 0x000D
#define LOCALE_SDECIMAL 0x000E
#define LOCALE_STHOUSAND 0x000F
#define LOCALE_SGROUPING 0x0010
#define LOCALE_IDIGITS 0x0011
#define LOCALE_ILZERO 0x0012
#define LOCALE_SNATIVEDIGITS 0x0013
#define LOCALE_SCURRENCY 0x0014
#define LOCALE_SINTLSYMBOL 0x0015
#define LOCALE_SMONDECIMALSEP 0x0016
#define LOCALE_SMONTHOUSANDSEP 0x0017
#define LOCALE_SMONGROUPING 0x0018
#define LOCALE_ICURRDIGITS 0x0019
#define LOCALE_IINTLCURRDIGITS 0x001A
#define LOCALE_ICURRENCY 0x001B
#define LOCALE_INEGCURR 0x001C
#define LOCALE_SDATE 0x001D
#define LOCALE_STIME 0x001E
#define LOCALE_SSHORTDATE 0x001F
#define LOCALE_SLONGDATE 0x0020
#define LOCALE_IDATE 0x0021
#define LOCALE_ILDATE 0x0022
#define LOCALE_ITIME 0x0023
#define LOCALE_ICENTURY 0x0024
#define LOCALE_ITLZERO 0x0025
#define LOCALE_IDAYLZERO 0x0026
#define LOCALE_IMONLZERO 0x0027
#define LOCALE_S1159 0x0028
#define LOCALE_S2359 0x0029
#define LOCALE_SDAYNAME1 0x002A
#define LOCALE_SABBREVDAYNAME1 0x0031
#define LOCALE_SMONTHNAME1 0x0038
#define LOCALE_SABBREVMONTHNAME1 0x0044
#define LOCALE_SPOSITIVESIGN 0x0050
#define LOCALE_SNEGATIVESIGN 0x0051

#if WINVER >= 0x030A /* Windows 3.1 and later */
LCID FAR PASCAL GetUserDefaultLCID(void);
LCID FAR PASCAL GetSystemDefaultLCID(void);
LANGID FAR PASCAL GetUserDefaultLangID(void);
LANGID FAR PASCAL GetSystemDefaultLangID(void);
int FAR PASCAL GetLocaleInfoA(LCID lcid, LCTYPE LCType, LPSTR lpLCData,
                              int cchData);
int FAR PASCAL LCMapStringA(LCID lcid, DWORD dwMapFlags, LPCSTR lpSrcStr,
                            int cchSrc, LPSTR lpDestStr, int cchDest);
BOOL FAR PASCAL GetStringTypeA(LCID lcid, DWORD dwInfoType, LPCSTR lpSrcStr,
                               int cchSrc, LPWORD lpCharType);
int FAR PASCAL CompareStringA(LCID lcid, DWORD dwCmpFlags, LPCSTR lpString1,
                              int cchCount1, LPCSTR lpString2, int cchCount2);
BOOL FAR PASCAL RegisterNLSInfoChanged(FARPROC lpfnNotifyProc);

#ifdef __cplusplus
}
#endif
#endif /* WINVER >= 0x030A */
#endif /* RC_INVOKED */

#endif /* _INC_OLENLS */
