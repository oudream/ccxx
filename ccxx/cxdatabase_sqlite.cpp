#include "cxdatabase.h"

#include "cxstring.h"
#include "cxcontainer.h"

#include "cxapplication.h"
#include "cxfile.h"


using namespace std;

//********************* ********************* ********************* *********************
//********************* sqlite begin **********************************************************
//********************* ********************* ********************* *********************

#include "sqlite3.h"

//const std::map<int, string>::value_type f_sqlite_int_column_type_names[] =
//{
//    std::map<int, string>::value_type( CxDatabase::column_type_boolean, "BOOLEAN" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_int, "INTEGER " ),
//    std::map<int, string>::value_type( CxDatabase::column_type_double, "DOUBLE" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_longint, "BIGINT" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_blob, "BLOB" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_text, "TEXT" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_varchar64, "VARCHAR(64)" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_varchar128, "VARCHAR(128)" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_varchar255, "VARCHAR(255)" ),
//    std::map<int, string>::value_type( CxDatabase::column_type_varchar_n, "VARCHAR(1024)" ),
//};
//const static std::map<int, string> f_sqlite_column_type_names(f_sqlite_int_column_type_names, f_sqlite_int_column_type_names+sizeof(f_sqlite_int_column_type_names)/sizeof(f_sqlite_int_column_type_names[0]));

//string fn_sqlite_column_type_name(int iColumnType)
//{
//    std::map<int,string>::const_iterator it = f_sqlite_column_type_names.find(iColumnType);
//    if (it != f_sqlite_column_type_names.end())
//        return it->second;
//    else
//        return std::string();
//}

class CxDatabaseSqlite : public CxDatabase
{
public:
    static bool isMyDatabase(const std::string & sConnectSource)
    {
        return CxString::equalCase(CxFileSystem::extractFileSuffixName(sConnectSource), ".db");
    }

    static CxDatabase * createDatabase(void)
    {
        return new CxDatabaseSqlite();
    }

public:
    CxDatabaseSqlite()
    {
        db = NULL;
    }

    ~CxDatabaseSqlite()
    {
        if (db)
        {
            closeDatabaseImpl();
        }
    }

protected:
    bool openDatabaseImpl(const std::string & sDatabase,  bool bCreate, const std::map<std::string, std::string> * oParams)
    {
        if (!db)
        {
            // 打开数据
            int flags = bCreate ? SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE : SQLITE_OPEN_READWRITE;
            int rc=sqlite3_open_v2(sDatabase.c_str(),&db, flags, NULL);
            if(rc){
                fprintf(stderr,"can not open database :%s",sqlite3_errmsg(db));
                sqlite3_close(db);
                db = NULL;
                return false;
            }
        }
        return true;
    }

    void closeDatabaseImpl()
    {
        if (db)
        {
            sqlite3_close(db);
            db = NULL;
        }
    }

    bool isOpenImpl() const
    {
        return db;
    }

    bool createTableImpl(const std::string & sTableName, const std::map<std::string, std::string> & columns)
    {
        string sSql = CxString::format("CREATE TABLE [%s] (", sTableName.c_str());
        int iCount = 0;
        for(std::map<string,string>::const_iterator it = columns.begin(); it != columns.end(); ++it)
        {
            string sColumn = "[" + it->first + "] " + it->second + " ,";
            sSql += sColumn;
            ++ iCount;
        }
        if (iCount > 0)
        {
            char * pch = (char *)sSql.data();
            pch[sSql.size()-1] = ')';
            return execSql(sSql);
        }
        return true;
    }

    bool alterPrimaryKeyAddImpl(const std::string & sTableName, const std::vector<std::string> & columns)
    {
        string sSql = CxString::format("ALTER TABLE ADD CONSTRAINT %s PRIMARY KEY (" , sTableName.c_str());
        int iCount = 0;
        for (size_t i = 0; i < columns.size(); ++i)
        {
            sSql += columns.at(i) + " ,";
            ++ iCount;
        }
        if (iCount > 0)
        {
            char * pch = (char *)sSql.data();
            pch[sSql.size()-1] = ')';
            return execSql(sSql);
        }
        return false;
    }

    int loadTableImpl(const std::string & sTableName, std::vector<std::vector<std::string> > & rows, std::vector<std::string> * oColumnNames = NULL, int iMaxRowCount = -1)
    {
        string sSql = "SELECT ";
        if (oColumnNames && oColumnNames->size() > 0)
        {
            int iCount = 0;
            for (size_t i = 0; i < oColumnNames->size(); ++i)
            {
                sSql += oColumnNames->at(i) + " ,";
                ++ iCount;
            }
            sSql.resize(sSql.size()-1);
        }
        else
        {
            sSql += "* ";
        }
        sSql += "FROM " + sTableName;

        return loadSqlImpl(sSql, rows, oColumnNames);
    }

    int saveTableImpl(const std::string & sTableName, const std::vector<std::string> & columnNames, const std::vector<std::vector<std::string> > & rows, const std::vector<int> & columnTypes = std::vector<int>(), bool bTransaction = true)
    {
        if (! db)
        {
            string sErrorString = "error : db is not open sql!";
            setLastError(-1, sErrorString);
            return -1;
        }
        string sSql = CxString::format("INSERT INTO [%s] ", sTableName.c_str());
        string sColumnes = "(";
        string sValues = "(";
        int iCount = 0;
        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            sColumnes += columnNames.at(i) + " ,";
            sValues += "? ,";
            ++ iCount;
        }
        if (iCount > 0)
        {
            char * pch = (char *)sColumnes.data();
            pch[sColumnes.size()-1] = ')';
            pch = (char *)sValues.data();
            pch[sValues.size()-1] = ')';
        }
        sSql = sSql + sColumnes + " VALUES " + sValues;

        //启动事务
        if (bTransaction)
            if (! beginTransaction())
                return -1;

        sqlite3_stmt *stmt;
        const char*tail;
        int rc = sqlite3_prepare(db, sSql.c_str(), sSql.size(), &stmt, &tail);
        if(rc != SQLITE_OK)
        {
            //设置LastError
            string sErrorString = CxString::format("sql exec error: %s" , sqlite3_errmsg(db));
            setLastError(rc, sErrorString);
            //回滚
            if (bTransaction) rollbackTransaction();
            return -1;
        }

        for (size_t i = 0; i < rows.size(); ++i)
        {
            rc = sqlite3_reset(stmt);
            if(rc != SQLITE_OK)
            {
                continue;
            }
            const vector<string> & row = rows.at(i);
            for (size_t j = 0; j < row.size(); ++j)
            {
                const string & sValue = row.at(j);
                sqlite3_bind_text(stmt, j + 1, sValue.c_str(), sValue.size(), 0);
            }
            rc = sqlite3_step(stmt);
            if(rc != SQLITE_OK && rc != SQLITE_DONE)
            {
                //设置LastError
                string sErrorString = CxString::format("sql exec error: %s" , sqlite3_errmsg(db));
                setLastError(rc, sErrorString);
                //回滚
                if (bTransaction) rollbackTransaction();
                return i;
            }
        }
        sqlite3_finalize(stmt);
        //提交
        if (bTransaction) commitTransaction();
        return rows.size();
    }

    int saveTableImpl(const std::string & sSql, const std::vector<std::vector<std::string> > & rows, const std::vector<int> * oColumnTypes, bool bTransaction = false)
    {
        if (! db)
        {
            string sErrorString = "error : db is not open sql!";
            setLastError(-1, sErrorString);
            return -1;
        }
//        string sSql = CxString::format("INSERT INTO [%s] ", sTableName.c_str());
//        string sColumnes = "(";
//        string sValues = "(";
//        int iCount = 0;
//        for (size_t i = 0; i < columnNames.size(); ++i)
//        {
//            sColumnes += columnNames.at(i) + " ,";
//            sValues += "? ,";
//            ++ iCount;
//        }
//        if (iCount > 0)
//        {
//            char * pch = (char *)sColumnes.data();
//            pch[sColumnes.size()-1] = ')';
//            pch = (char *)sValues.data();
//            pch[sValues.size()-1] = ')';
//        }
//        sSql = sSql + sColumnes + " VALUES " + sValues;

        //启动事务
        if (bTransaction)
            if (! beginTransaction())
                return -1;

        sqlite3_stmt *stmt;
        const char*tail;
        int rc = sqlite3_prepare(db, sSql.c_str(), sSql.size(), &stmt, &tail);
        if(rc != SQLITE_OK)
        {
            string sErrorString = CxString::format("sql exec error: %s" , sqlite3_errmsg(db));
            setLastError(rc, sErrorString);
            //回滚
            if (bTransaction) rollbackTransaction();
            return -1;
        }
        for (size_t i = 0; i < rows.size(); ++i)
        {
            rc = sqlite3_reset(stmt);
            if(rc != SQLITE_OK)
            {
                continue;
            }
            const vector<string> & row = rows.at(i);
            for (size_t j = 0; j < row.size(); ++j)
            {
                const string & sValue = row.at(j);
                sqlite3_bind_text(stmt, j + 1, sValue.c_str(), sValue.size(), 0);
            }
            rc = sqlite3_step(stmt);
            if(rc != SQLITE_OK && rc != SQLITE_DONE)
            {
                string sErrorString = CxString::format("sql exec error: %s" , sqlite3_errmsg(db));
                setLastError(rc, sErrorString);
                //回滚
                if (bTransaction) rollbackTransaction();
                return i;
            }
        }
        sqlite3_finalize(stmt);
        //提交
        if (bTransaction) commitTransaction();
        return rows.size();
    }

    int execSqlImpl(const string &sSql)
    {
//        if (! db)
//        {
//            string sErrorString = "error : db is not open sql!";
//            setLastError(-1, sErrorString);
//            return -1;
//        }
//        int rc, ncols;
//        sqlite3_stmt * stmt;
//        const char * tail;
//        rc = sqlite3_prepare(db, sSql.c_str(), sSql.size(), &stmt, &tail);
//        if(rc!=SQLITE_OK)
//        {
//            string sErrorString = CxString::format("sql exec error: %s" , sqlite3_errmsg(db));
//            setLastError(rc, sErrorString);
//            return -1;
//        }
//        rc=sqlite3_step(stmt);
//        sqlite3_finalize(stmt);
//        return true;

        if (! db)
        {
            string sErrorString = "error : db is not open sql!";
            setLastError(-1, sErrorString);
            return FALSE;
        }
        char *zErr;
        int rc = sqlite3_exec(db, sSql.c_str(), NULL, NULL, &zErr);
        if(rc != SQLITE_OK) {
            string sErrorString;
            if (zErr != NULL) {
                sErrorString = CxString::format("sql exec error: %s" , zErr);
                sqlite3_free(zErr);
            }
            setLastError(rc, sErrorString);
            return FALSE;
        }
        return TRUE;
    }

    int execSqlListImpl(const std::vector<std::string> & sqlList)
    {
        return FALSE;
    }

    int loadSqlImpl(const std::string & sSql, std::vector<std::vector<std::string> > & rows, std::vector<std::string> * oColumnNames = NULL, int iMaxRowCount = -1)
    {
        if (! db)
        {
            string sErrorString = "error : db is not open sql!";
            setLastError(-1, sErrorString);
            return -1;
        }
        int rc, ncols;
        sqlite3_stmt * stmt;
        const char * tail;
        rc = sqlite3_prepare(db, sSql.c_str(), sSql.size(), &stmt, &tail);
        if(rc!=SQLITE_OK)
        {
            string sErrorString = CxString::format("sql exec error: %s" , sqlite3_errmsg(db));
            setLastError(rc, sErrorString);
            return -1;
        }
        rc=sqlite3_step(stmt);
        ncols=sqlite3_column_count(stmt);
        if (oColumnNames && oColumnNames->empty())
        {
            for (int i = 0; i < ncols; ++i)
            {
                string sColumnName = sqlite3_column_name(stmt, i);
                oColumnNames->push_back(sColumnName);
            }
        }
        while(rc==SQLITE_ROW)
        {
            vector<string> row;
            for (int i = 0; i < ncols; ++i)
            {
                string sValue;
                const unsigned char * pch = sqlite3_column_text(stmt,i);
                if (pch)
                    sValue = string((const char *)pch);
                row.push_back(sValue);
            }
            rows.push_back(row);
            rc=sqlite3_step(stmt);
            if (iMaxRowCount>0 && rows.size()>=iMaxRowCount)
            {
                break;
            }
        }
        sqlite3_finalize(stmt);
        return rows.size( );
    }

private:
    bool beginTransaction() {
        return execSqlImpl("BEGIN");
    }

    bool commitTransaction() {
        return execSqlImpl("COMMIT");
    }

    bool rollbackTransaction() {
        return execSqlImpl("ROLLBACK");
    }

private:
    sqlite3 * db;

};



CxDatabaseSqlite * fn_oDatabaseSqlite()
{
    CxDatabaseManager::registDatabaseConstructor(CxDatabaseSqlite::isMyDatabase, CxDatabaseSqlite::createDatabase);

    static CxDatabaseSqlite m;
    return & m;
}

static CxDatabaseSqlite * f_oDatabaseSqlite = fn_oDatabaseSqlite();
