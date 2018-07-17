#ifndef CXDATABASE_SQLITE_H
#define CXDATABASE_SQLITE_H

#include "cxdatabase.h"

#if defined(GM_BUILD_CCXX_SQLITE_LIB)
#define GM_CCXX_SQLITE_API  GM_DECL_EXPORT
#elif defined(GM_BUILD_CCXX_SQLITE_CODE)
#define GM_CCXX_SQLITE_API
#else
#define GM_CCXX_SQLITE_API  GM_DECL_IMPORT
// GM_BUILD_CCXX_SQLITE_INCLUDE
#endif

class GM_CCXX_SQLITE_API CxDatabaseSqliteFactory
{
public:
    static bool registDatabaseConstructor();

    static bool isMyDatabase(const std::string& sConnectSource);

    static CxDatabase* createDatabase(void);

};

GM_CCXX_SQLITE_API extern bool g_bHasRegistDatabaseConstructorSqlite;

static bool g_bHasRegistDatabaseConstructorSqlite2 = g_bHasRegistDatabaseConstructorSqlite;

#endif // CXDATABASE_SQLITE_H
