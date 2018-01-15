#ifndef CXDATABASE_ODBC_H
#define CXDATABASE_ODBC_H

#include "cxdatabase.h"

#if defined(GM_BUILD_CCXX_ODBC_LIB)
#define GM_CCXX_ODBC_API  GM_DECL_EXPORT
#elif defined(GM_BUILD_CCXX_ODBC_CODE)
#define GM_CCXX_ODBC_API
#else
#define GM_CCXX_ODBC_API  GM_DECL_IMPORT
// GM_BUILD_CCXX_ODBC_INCLUDE
#endif

class GM_CCXX_ODBC_API CxDatabaseOdbcFactory
{
public:
    static bool registDatabaseConstructor();

    static bool isMyDatabase(const std::string& sConnectSource);

    static CxDatabase* createDatabase(void);

};

GM_CCXX_ODBC_API extern bool g_bHasRegistDatabaseConstructorOdbc;

static bool g_bHasRegistDatabaseConstructorOdbc2 = g_bHasRegistDatabaseConstructorOdbc;

#endif // CXDATABASE_ODBC_H
