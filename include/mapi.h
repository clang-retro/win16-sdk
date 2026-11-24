/* mapi.h: Simple MAPI 1.0 for 16-bit Windows (MAPI.DLL, which Microsoft
 * Mail and other mail programs install): log on, send, read, find, save
 * and delete messages, and resolve and show recipients. Written for
 * clang-dos from the published API; AGPL-3.0-only, see the win16-sdk
 * LICENSE.
 *
 * The programs import the functions by name. */
#ifndef MAPI_H
#define MAPI_H

#include <windows.h>

#ifndef RC_INVOKED
#if WINVER >= 0x030A /* Windows 3.1 and later */
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

#ifndef _ULONG_DEFINED
#define _ULONG_DEFINED
typedef unsigned long ULONG;
#endif
typedef unsigned long LHANDLE, FAR *LPLHANDLE;
typedef unsigned long FLAGS;

#define lhSessionNull ((LHANDLE)0)

/* A file attachment of a message. */
typedef struct {
  ULONG ulReserved;
  ULONG flFlags;
  ULONG nPosition;
  LPSTR lpszPathName;
  LPSTR lpszFileName;
  LPVOID lpFileType;
} MapiFileDesc, FAR *lpMapiFileDesc;

#define MAPI_OLE 0x00000001
#define MAPI_OLE_STATIC 0x00000002

/* A sender or recipient of a message. */
typedef struct {
  ULONG ulReserved;
  ULONG ulRecipClass;
  LPSTR lpszName;
  LPSTR lpszAddress;
  ULONG ulEIDSize;
  LPVOID lpEntryID;
} MapiRecipDesc, FAR *lpMapiRecipDesc;

#define MAPI_ORIG 0
#define MAPI_TO 1
#define MAPI_CC 2
#define MAPI_BCC 3

/* A message. */
typedef struct {
  ULONG ulReserved;
  LPSTR lpszSubject;
  LPSTR lpszNoteText;
  LPSTR lpszMessageType;
  LPSTR lpszDateReceived;
  LPSTR lpszConversationID;
  FLAGS flFlags;
  lpMapiRecipDesc lpOriginator;
  ULONG nRecipCount;
  lpMapiRecipDesc lpRecips;
  ULONG nFileCount;
  lpMapiFileDesc lpFiles;
} MapiMessage, FAR *lpMapiMessage;

#define MAPI_UNREAD 0x00000001
#define MAPI_RECEIPT_REQUESTED 0x00000002
#define MAPI_SENT 0x00000004

/* The flags of the functions. */
#define MAPI_LOGON_UI 0x00000001
#define MAPI_NEW_SESSION 0x00000002
#define MAPI_DIALOG 0x00000008
#define MAPI_UNREAD_ONLY 0x00000020
#define MAPI_ENVELOPE_ONLY 0x00000040
#define MAPI_PEEK 0x00000080
#define MAPI_GUARANTEE_FIFO 0x00000100
#define MAPI_BODY_AS_FILE 0x00000200
#define MAPI_AB_NOMODIFY 0x00000400
#define MAPI_SUPPRESS_ATTACH 0x00000800
#define MAPI_FORCE_DOWNLOAD 0x00001000
#define MAPI_LONG_MSGID 0x00004000

/* The return codes. */
#define SUCCESS_SUCCESS 0
#define MAPI_USER_ABORT 1
#define MAPI_E_FAILURE 2
#define MAPI_E_LOGIN_FAILURE 3
#define MAPI_E_DISK_FULL 4
#define MAPI_E_INSUFFICIENT_MEMORY 5
#define MAPI_E_ACCESS_DENIED 6
#define MAPI_E_TOO_MANY_SESSIONS 8
#define MAPI_E_TOO_MANY_FILES 9
#define MAPI_E_TOO_MANY_RECIPIENTS 10
#define MAPI_E_ATTACHMENT_NOT_FOUND 11
#define MAPI_E_ATTACHMENT_OPEN_FAILURE 12
#define MAPI_E_ATTACHMENT_WRITE_FAILURE 13
#define MAPI_E_UNKNOWN_RECIPIENT 14
#define MAPI_E_BAD_RECIPTYPE 15
#define MAPI_E_NO_MESSAGES 16
#define MAPI_E_INVALID_MESSAGE 17
#define MAPI_E_TEXT_TOO_LARGE 18
#define MAPI_E_INVALID_SESSION 19
#define MAPI_E_TYPE_NOT_SUPPORTED 20
#define MAPI_E_AMBIGUOUS_RECIPIENT 21
#define MAPI_E_MESSAGE_IN_USE 22
#define MAPI_E_NETWORK_FAILURE 23
#define MAPI_E_INVALID_EDITFIELDS 24
#define MAPI_E_INVALID_RECIPS 25
#define MAPI_E_NOT_SUPPORTED 26

ULONG FAR PASCAL MAPILogon(ULONG ulUIParam, LPSTR lpszName,
                           LPSTR lpszPassword, FLAGS flFlags,
                           ULONG ulReserved, LPLHANDLE lplhSession);
ULONG FAR PASCAL MAPILogoff(LHANDLE lhSession, ULONG ulUIParam,
                            FLAGS flFlags, ULONG ulReserved);
ULONG FAR PASCAL MAPISendMail(LHANDLE lhSession, ULONG ulUIParam,
                              lpMapiMessage lpMessage, FLAGS flFlags,
                              ULONG ulReserved);
ULONG FAR PASCAL MAPISendDocuments(ULONG ulUIParam, LPSTR lpszDelimChar,
                                   LPSTR lpszFullPaths, LPSTR lpszFileNames,
                                   ULONG ulReserved);
ULONG FAR PASCAL MAPIFindNext(LHANDLE lhSession, ULONG ulUIParam,
                              LPSTR lpszMessageType, LPSTR lpszSeedMessageID,
                              FLAGS flFlags, ULONG ulReserved,
                              LPSTR lpszMessageID);
ULONG FAR PASCAL MAPIReadMail(LHANDLE lhSession, ULONG ulUIParam,
                              LPSTR lpszMessageID, FLAGS flFlags,
                              ULONG ulReserved,
                              lpMapiMessage FAR *lppMessageOut);
ULONG FAR PASCAL MAPISaveMail(LHANDLE lhSession, ULONG ulUIParam,
                              lpMapiMessage lpMessage, FLAGS flFlags,
                              ULONG ulReserved, LPSTR lpszMessageID);
ULONG FAR PASCAL MAPIDeleteMail(LHANDLE lhSession, ULONG ulUIParam,
                                LPSTR lpszMessageID, FLAGS flFlags,
                                ULONG ulReserved);
ULONG FAR PASCAL MAPIFreeBuffer(LPVOID pv);
ULONG FAR PASCAL MAPIAddress(LHANDLE lhSession, ULONG ulUIParam,
                             LPSTR lpszCaption, ULONG nEditFields,
                             LPSTR lpszLabels, ULONG nRecips,
                             lpMapiRecipDesc lpRecips, FLAGS flFlags,
                             ULONG ulReserved, ULONG FAR *lpnNewRecips,
                             lpMapiRecipDesc FAR *lppNewRecips);
ULONG FAR PASCAL MAPIDetails(LHANDLE lhSession, ULONG ulUIParam,
                             lpMapiRecipDesc lpRecip, FLAGS flFlags,
                             ULONG ulReserved);
ULONG FAR PASCAL MAPIResolveName(LHANDLE lhSession, ULONG ulUIParam,
                                 LPSTR lpszName, FLAGS flFlags,
                                 ULONG ulReserved,
                                 lpMapiRecipDesc FAR *lppRecip);

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
#endif /* WINVER >= 0x030A */
#endif /* RC_INVOKED */

#endif /* MAPI_H */
