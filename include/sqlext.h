/* sqlext.h: the ODBC 2.0 extension API for 16-bit Windows (ODBC.DLL): the
 * level 1 and level 2 functions, the extended data types, and the options
 * of SQLGetInfo, SQLSetConnectOption and SQLSetStmtOption. Written for
 * clang-dos from the published API; AGPL-3.0-only, see the win16-sdk
 * LICENSE. */
#ifndef __SQLEXT
#define __SQLEXT

#include <sql.h>

#ifndef RC_INVOKED
#if WINVER >= 0x030A /* Windows 3.1 and later */
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

/* The extended SQL data types. */
#define SQL_DATE 9
#define SQL_TIME 10
#define SQL_TIMESTAMP 11
#define SQL_LONGVARCHAR (-1)
#define SQL_BINARY (-2)
#define SQL_VARBINARY (-3)
#define SQL_LONGVARBINARY (-4)
#define SQL_BIGINT (-5)
#define SQL_TINYINT (-6)
#define SQL_BIT (-7)

/* The extended C data types. */
#define SQL_C_DATE SQL_DATE
#define SQL_C_TIME SQL_TIME
#define SQL_C_TIMESTAMP SQL_TIMESTAMP
#define SQL_C_BINARY SQL_BINARY
#define SQL_C_BIT SQL_BIT
#define SQL_C_TINYINT SQL_TINYINT
#define SQL_SIGNED_OFFSET (-20)
#define SQL_UNSIGNED_OFFSET (-22)
#define SQL_C_SLONG (SQL_C_LONG + SQL_SIGNED_OFFSET)
#define SQL_C_SSHORT (SQL_C_SHORT + SQL_SIGNED_OFFSET)
#define SQL_C_STINYINT (SQL_TINYINT + SQL_SIGNED_OFFSET)
#define SQL_C_ULONG (SQL_C_LONG + SQL_UNSIGNED_OFFSET)
#define SQL_C_USHORT (SQL_C_SHORT + SQL_UNSIGNED_OFFSET)
#define SQL_C_UTINYINT (SQL_TINYINT + SQL_UNSIGNED_OFFSET)
#define SQL_C_BOOKMARK SQL_C_ULONG

typedef unsigned long int BOOKMARK;

/* The C structures of the date and time types. */
typedef struct tagDATE_STRUCT {
  SWORD year;
  UWORD month;
  UWORD day;
} DATE_STRUCT;

typedef struct tagTIME_STRUCT {
  UWORD hour;
  UWORD minute;
  UWORD second;
} TIME_STRUCT;

typedef struct tagTIMESTAMP_STRUCT {
  SWORD year;
  UWORD month;
  UWORD day;
  UWORD hour;
  UWORD minute;
  UWORD second;
  UDWORD fraction;
} TIMESTAMP_STRUCT;

/* The options of SQLDriverConnect. */
#define SQL_DRIVER_NOPROMPT 0
#define SQL_DRIVER_COMPLETE 1
#define SQL_DRIVER_PROMPT 2
#define SQL_DRIVER_COMPLETE_REQUIRED 3

/* The directions of SQLDataSources and SQLDrivers. */
#define SQL_FETCH_NEXT 1
#define SQL_FETCH_FIRST 2
#define SQL_FETCH_LAST 3
#define SQL_FETCH_PRIOR 4
#define SQL_FETCH_ABSOLUTE 5
#define SQL_FETCH_RELATIVE 6
#define SQL_FETCH_BOOKMARK 8

/* The row status of SQLExtendedFetch. */
#define SQL_ROW_SUCCESS 0
#define SQL_ROW_DELETED 1
#define SQL_ROW_UPDATED 2
#define SQL_ROW_NOROW 3
#define SQL_ROW_ADDED 4
#define SQL_ROW_ERROR 5

/* The options of SQLSetPos. */
#define SQL_POSITION 0
#define SQL_REFRESH 1
#define SQL_UPDATE 2
#define SQL_DELETE 3
#define SQL_ADD 4

#define SQL_LOCK_NO_CHANGE 0
#define SQL_LOCK_EXCLUSIVE 1
#define SQL_LOCK_UNLOCK 2

/* The parameter types of SQLBindParameter. */
#define SQL_PARAM_TYPE_UNKNOWN 0
#define SQL_PARAM_INPUT 1
#define SQL_PARAM_INPUT_OUTPUT 2
#define SQL_RESULT_COL 3
#define SQL_PARAM_OUTPUT 4
#define SQL_RETURN_VALUE 5

#define SQL_LEN_DATA_AT_EXEC_OFFSET (-100)
#define SQL_LEN_DATA_AT_EXEC(length) (-(length) + SQL_LEN_DATA_AT_EXEC_OFFSET)

/* The options of SQLSpecialColumns and SQLStatistics. */
#define SQL_BEST_ROWID 1
#define SQL_ROWVER 2
#define SQL_SCOPE_CURROW 0
#define SQL_SCOPE_TRANSACTION 1
#define SQL_SCOPE_SESSION 2
#define SQL_INDEX_UNIQUE 0
#define SQL_INDEX_ALL 1
#define SQL_ENSURE 1
#define SQL_QUICK 0

/* The options of SQLSetConnectOption. */
#define SQL_ACCESS_MODE 101
#define SQL_AUTOCOMMIT 102
#define SQL_LOGIN_TIMEOUT 103
#define SQL_OPT_TRACE 104
#define SQL_OPT_TRACEFILE 105
#define SQL_TRANSLATE_DLL 106
#define SQL_TRANSLATE_OPTION 107
#define SQL_TXN_ISOLATION 108
#define SQL_CURRENT_QUALIFIER 109
#define SQL_ODBC_CURSORS 110
#define SQL_QUIET_MODE 111
#define SQL_PACKET_SIZE 112

#define SQL_MODE_READ_WRITE 0UL
#define SQL_MODE_READ_ONLY 1UL
#define SQL_AUTOCOMMIT_OFF 0UL
#define SQL_AUTOCOMMIT_ON 1UL
#define SQL_CUR_USE_IF_NEEDED 0UL
#define SQL_CUR_USE_ODBC 1UL
#define SQL_CUR_USE_DRIVER 2UL

/* The options of SQLSetStmtOption. */
#define SQL_QUERY_TIMEOUT 0
#define SQL_MAX_ROWS 1
#define SQL_NOSCAN 2
#define SQL_MAX_LENGTH 3
#define SQL_ASYNC_ENABLE 4
#define SQL_BIND_TYPE 5
#define SQL_CURSOR_TYPE 6
#define SQL_CONCURRENCY 7
#define SQL_KEYSET_SIZE 8
#define SQL_ROWSET_SIZE 9
#define SQL_SIMULATE_CURSOR 10
#define SQL_RETRIEVE_DATA 11
#define SQL_USE_BOOKMARKS 12
#define SQL_GET_BOOKMARK 13
#define SQL_ROW_NUMBER 14

#define SQL_CURSOR_FORWARD_ONLY 0UL
#define SQL_CURSOR_KEYSET_DRIVEN 1UL
#define SQL_CURSOR_DYNAMIC 2UL
#define SQL_CURSOR_STATIC 3UL
#define SQL_CONCUR_READ_ONLY 1
#define SQL_CONCUR_LOCK 2
#define SQL_CONCUR_ROWVER 3
#define SQL_CONCUR_VALUES 4

/* Some of the information types of SQLGetInfo. */
#define SQL_ACTIVE_CONNECTIONS 0
#define SQL_ACTIVE_STATEMENTS 1
#define SQL_DATA_SOURCE_NAME 2
#define SQL_DRIVER_HDBC 3
#define SQL_DRIVER_HENV 4
#define SQL_DRIVER_HSTMT 5
#define SQL_DRIVER_NAME 6
#define SQL_DRIVER_VER 7
#define SQL_FETCH_DIRECTION 8
#define SQL_ODBC_API_CONFORMANCE 9
#define SQL_ODBC_VER 10
#define SQL_ROW_UPDATES 11
#define SQL_ODBC_SAG_CLI_CONFORMANCE 12
#define SQL_SERVER_NAME 13
#define SQL_SEARCH_PATTERN_ESCAPE 14
#define SQL_ODBC_SQL_CONFORMANCE 15
#define SQL_DBMS_NAME 17
#define SQL_DBMS_VER 18
#define SQL_ACCESSIBLE_TABLES 19
#define SQL_ACCESSIBLE_PROCEDURES 20
#define SQL_PROCEDURES 21
#define SQL_CONCAT_NULL_BEHAVIOR 22
#define SQL_CURSOR_COMMIT_BEHAVIOR 23
#define SQL_CURSOR_ROLLBACK_BEHAVIOR 24
#define SQL_DATA_SOURCE_READ_ONLY 25
#define SQL_DEFAULT_TXN_ISOLATION 26
#define SQL_EXPRESSIONS_IN_ORDERBY 27
#define SQL_IDENTIFIER_CASE 28
#define SQL_IDENTIFIER_QUOTE_CHAR 29
#define SQL_MAX_COLUMN_NAME_LEN 30
#define SQL_MAX_CURSOR_NAME_LEN 31
#define SQL_MAX_OWNER_NAME_LEN 32
#define SQL_MAX_PROCEDURE_NAME_LEN 33
#define SQL_MAX_QUALIFIER_NAME_LEN 34
#define SQL_MAX_TABLE_NAME_LEN 35
#define SQL_MULT_RESULT_SETS 36
#define SQL_MULTIPLE_ACTIVE_TXN 37
#define SQL_OUTER_JOINS 38
#define SQL_OWNER_TERM 39
#define SQL_PROCEDURE_TERM 40
#define SQL_QUALIFIER_NAME_SEPARATOR 41
#define SQL_QUALIFIER_TERM 42
#define SQL_SCROLL_CONCURRENCY 43
#define SQL_SCROLL_OPTIONS 44
#define SQL_TABLE_TERM 45
#define SQL_TXN_CAPABLE 46
#define SQL_USER_NAME 47
#define SQL_CONVERT_FUNCTIONS 48
#define SQL_NUMERIC_FUNCTIONS 49
#define SQL_STRING_FUNCTIONS 50
#define SQL_SYSTEM_FUNCTIONS 51
#define SQL_TIMEDATE_FUNCTIONS 52
#define SQL_DATABASE_NAME 16
#define SQL_GETDATA_EXTENSIONS 81
#define SQL_KEYWORDS 89

/* The function identifiers of SQLGetFunctions. */
#define SQL_API_ALL_FUNCTIONS 0
#define SQL_API_SQLALLOCCONNECT 1
#define SQL_API_SQLALLOCENV 2
#define SQL_API_SQLALLOCSTMT 3
#define SQL_API_SQLBINDCOL 4
#define SQL_API_SQLCANCEL 5
#define SQL_API_SQLCOLATTRIBUTES 6
#define SQL_API_SQLCONNECT 7
#define SQL_API_SQLDESCRIBECOL 8
#define SQL_API_SQLDISCONNECT 9
#define SQL_API_SQLERROR 10
#define SQL_API_SQLEXECDIRECT 11
#define SQL_API_SQLEXECUTE 12
#define SQL_API_SQLFETCH 13
#define SQL_API_SQLFREECONNECT 14
#define SQL_API_SQLFREEENV 15
#define SQL_API_SQLFREESTMT 16
#define SQL_API_SQLGETCURSORNAME 17
#define SQL_API_SQLNUMRESULTCOLS 18
#define SQL_API_SQLPREPARE 19
#define SQL_API_SQLROWCOUNT 20
#define SQL_API_SQLSETCURSORNAME 21
#define SQL_API_SQLSETPARAM 22
#define SQL_API_SQLTRANSACT 23
#define SQL_API_SQLCOLUMNS 40
#define SQL_API_SQLDRIVERCONNECT 41
#define SQL_API_SQLGETCONNECTOPTION 42
#define SQL_API_SQLGETDATA 43
#define SQL_API_SQLGETFUNCTIONS 44
#define SQL_API_SQLGETINFO 45
#define SQL_API_SQLGETSTMTOPTION 46
#define SQL_API_SQLGETTYPEINFO 47
#define SQL_API_SQLPARAMDATA 48
#define SQL_API_SQLPUTDATA 49
#define SQL_API_SQLSETCONNECTOPTION 50
#define SQL_API_SQLSETSTMTOPTION 51
#define SQL_API_SQLSPECIALCOLUMNS 52
#define SQL_API_SQLSTATISTICS 53
#define SQL_API_SQLTABLES 54
#define SQL_API_SQLBROWSECONNECT 55
#define SQL_API_SQLCOLUMNPRIVILEGES 56
#define SQL_API_SQLDATASOURCES 57
#define SQL_API_SQLDESCRIBEPARAM 58
#define SQL_API_SQLEXTENDEDFETCH 59
#define SQL_API_SQLFOREIGNKEYS 60
#define SQL_API_SQLMORERESULTS 61
#define SQL_API_SQLNATIVESQL 62
#define SQL_API_SQLNUMPARAMS 63
#define SQL_API_SQLPARAMOPTIONS 64
#define SQL_API_SQLPRIMARYKEYS 65
#define SQL_API_SQLPROCEDURECOLUMNS 66
#define SQL_API_SQLPROCEDURES 67
#define SQL_API_SQLSETPOS 68
#define SQL_API_SQLSETSCROLLOPTIONS 69
#define SQL_API_SQLTABLEPRIVILEGES 70
#define SQL_API_SQLDRIVERS 71
#define SQL_API_SQLBINDPARAMETER 72

/* The level 1 functions. */
RETCODE SQL_API SQLColumns(HSTMT hstmt, UCHAR FAR *szTableQualifier,
                           SWORD cbTableQualifier, UCHAR FAR *szTableOwner,
                           SWORD cbTableOwner, UCHAR FAR *szTableName,
                           SWORD cbTableName, UCHAR FAR *szColumnName,
                           SWORD cbColumnName);
RETCODE SQL_API SQLDriverConnect(HDBC hdbc, HWND hwnd,
                                 UCHAR FAR *szConnStrIn, SWORD cbConnStrIn,
                                 UCHAR FAR *szConnStrOut,
                                 SWORD cbConnStrOutMax,
                                 SWORD FAR *pcbConnStrOut,
                                 UWORD fDriverCompletion);
RETCODE SQL_API SQLGetConnectOption(HDBC hdbc, UWORD fOption, PTR pvParam);
RETCODE SQL_API SQLGetData(HSTMT hstmt, UWORD icol, SWORD fCType,
                           PTR rgbValue, SDWORD cbValueMax,
                           SDWORD FAR *pcbValue);
RETCODE SQL_API SQLGetFunctions(HDBC hdbc, UWORD fFunction,
                                UWORD FAR *pfExists);
RETCODE SQL_API SQLGetInfo(HDBC hdbc, UWORD fInfoType, PTR rgbInfoValue,
                           SWORD cbInfoValueMax, SWORD FAR *pcbInfoValue);
RETCODE SQL_API SQLGetStmtOption(HSTMT hstmt, UWORD fOption, PTR pvParam);
RETCODE SQL_API SQLGetTypeInfo(HSTMT hstmt, SWORD fSqlType);
RETCODE SQL_API SQLParamData(HSTMT hstmt, PTR FAR *prgbValue);
RETCODE SQL_API SQLPutData(HSTMT hstmt, PTR rgbValue, SDWORD cbValue);
RETCODE SQL_API SQLSetConnectOption(HDBC hdbc, UWORD fOption,
                                    UDWORD vParam);
RETCODE SQL_API SQLSetStmtOption(HSTMT hstmt, UWORD fOption, UDWORD vParam);
RETCODE SQL_API SQLSpecialColumns(HSTMT hstmt, UWORD fColType,
                                  UCHAR FAR *szTableQualifier,
                                  SWORD cbTableQualifier,
                                  UCHAR FAR *szTableOwner, SWORD cbTableOwner,
                                  UCHAR FAR *szTableName, SWORD cbTableName,
                                  UWORD fScope, UWORD fNullable);
RETCODE SQL_API SQLStatistics(HSTMT hstmt, UCHAR FAR *szTableQualifier,
                              SWORD cbTableQualifier,
                              UCHAR FAR *szTableOwner, SWORD cbTableOwner,
                              UCHAR FAR *szTableName, SWORD cbTableName,
                              UWORD fUnique, UWORD fAccuracy);
RETCODE SQL_API SQLTables(HSTMT hstmt, UCHAR FAR *szTableQualifier,
                          SWORD cbTableQualifier, UCHAR FAR *szTableOwner,
                          SWORD cbTableOwner, UCHAR FAR *szTableName,
                          SWORD cbTableName, UCHAR FAR *szTableType,
                          SWORD cbTableType);

/* The level 2 functions. */
RETCODE SQL_API SQLBrowseConnect(HDBC hdbc, UCHAR FAR *szConnStrIn,
                                 SWORD cbConnStrIn, UCHAR FAR *szConnStrOut,
                                 SWORD cbConnStrOutMax,
                                 SWORD FAR *pcbConnStrOut);
RETCODE SQL_API SQLColumnPrivileges(HSTMT hstmt, UCHAR FAR *szTableQualifier,
                                    SWORD cbTableQualifier,
                                    UCHAR FAR *szTableOwner,
                                    SWORD cbTableOwner,
                                    UCHAR FAR *szTableName, SWORD cbTableName,
                                    UCHAR FAR *szColumnName,
                                    SWORD cbColumnName);
RETCODE SQL_API SQLDataSources(HENV henv, UWORD fDirection,
                               UCHAR FAR *szDSN, SWORD cbDSNMax,
                               SWORD FAR *pcbDSN, UCHAR FAR *szDescription,
                               SWORD cbDescriptionMax,
                               SWORD FAR *pcbDescription);
RETCODE SQL_API SQLDescribeParam(HSTMT hstmt, UWORD ipar,
                                 SWORD FAR *pfSqlType, UDWORD FAR *pcbColDef,
                                 SWORD FAR *pibScale, SWORD FAR *pfNullable);
RETCODE SQL_API SQLExtendedFetch(HSTMT hstmt, UWORD fFetchType, SDWORD irow,
                                 UDWORD FAR *pcrow,
                                 UWORD FAR *rgfRowStatus);
RETCODE SQL_API SQLForeignKeys(HSTMT hstmt, UCHAR FAR *szPkTableQualifier,
                               SWORD cbPkTableQualifier,
                               UCHAR FAR *szPkTableOwner,
                               SWORD cbPkTableOwner,
                               UCHAR FAR *szPkTableName, SWORD cbPkTableName,
                               UCHAR FAR *szFkTableQualifier,
                               SWORD cbFkTableQualifier,
                               UCHAR FAR *szFkTableOwner,
                               SWORD cbFkTableOwner,
                               UCHAR FAR *szFkTableName,
                               SWORD cbFkTableName);
RETCODE SQL_API SQLMoreResults(HSTMT hstmt);
RETCODE SQL_API SQLNativeSql(HDBC hdbc, UCHAR FAR *szSqlStrIn,
                             SDWORD cbSqlStrIn, UCHAR FAR *szSqlStr,
                             SDWORD cbSqlStrMax, SDWORD FAR *pcbSqlStr);
RETCODE SQL_API SQLNumParams(HSTMT hstmt, SWORD FAR *pcpar);
RETCODE SQL_API SQLParamOptions(HSTMT hstmt, UDWORD crow, UDWORD FAR *pirow);
RETCODE SQL_API SQLPrimaryKeys(HSTMT hstmt, UCHAR FAR *szTableQualifier,
                               SWORD cbTableQualifier,
                               UCHAR FAR *szTableOwner, SWORD cbTableOwner,
                               UCHAR FAR *szTableName, SWORD cbTableName);
RETCODE SQL_API SQLProcedureColumns(HSTMT hstmt, UCHAR FAR *szProcQualifier,
                                    SWORD cbProcQualifier,
                                    UCHAR FAR *szProcOwner,
                                    SWORD cbProcOwner, UCHAR FAR *szProcName,
                                    SWORD cbProcName,
                                    UCHAR FAR *szColumnName,
                                    SWORD cbColumnName);
RETCODE SQL_API SQLProcedures(HSTMT hstmt, UCHAR FAR *szProcQualifier,
                              SWORD cbProcQualifier, UCHAR FAR *szProcOwner,
                              SWORD cbProcOwner, UCHAR FAR *szProcName,
                              SWORD cbProcName);
RETCODE SQL_API SQLSetPos(HSTMT hstmt, UWORD irow, UWORD fOption,
                          UWORD fLock);
RETCODE SQL_API SQLSetScrollOptions(HSTMT hstmt, UWORD fConcurrency,
                                    SDWORD crowKeyset, UWORD crowRowset);
RETCODE SQL_API SQLTablePrivileges(HSTMT hstmt, UCHAR FAR *szTableQualifier,
                                   SWORD cbTableQualifier,
                                   UCHAR FAR *szTableOwner,
                                   SWORD cbTableOwner,
                                   UCHAR FAR *szTableName,
                                   SWORD cbTableName);
RETCODE SQL_API SQLDrivers(HENV henv, UWORD fDirection,
                           UCHAR FAR *szDriverDesc, SWORD cbDriverDescMax,
                           SWORD FAR *pcbDriverDesc,
                           UCHAR FAR *szDriverAttributes,
                           SWORD cbDrvrAttrMax, SWORD FAR *pcbDrvrAttr);
RETCODE SQL_API SQLBindParameter(HSTMT hstmt, UWORD ipar, SWORD fParamType,
                                 SWORD fCType, SWORD fSqlType,
                                 UDWORD cbColDef, SWORD ibScale,
                                 PTR rgbValue, SDWORD cbValueMax,
                                 SDWORD FAR *pcbValue);

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
#endif /* WINVER >= 0x030A */
#endif /* RC_INVOKED */

#endif /* __SQLEXT */
