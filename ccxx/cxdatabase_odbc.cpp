#include "cxdatabase_odbc.h"


//for Ubuntu: -D"OTL_ODBC_UNIX" -lodbc
//sudo apt-get install unixodbc unixodbc-dev
//
//for CentOS: -D"OTL_ODBC_UNIX" -lodbc
//sudo yum install unixODBC-devel

#include "cxstring.h"
#include "cxcontainer.h"

#include "cxapplication.h"
#include "cxfile.h"
#include "cxinterinfo.h"
#include "cxlog.h"

#undef OTL_ORA8I
#undef UNICODE
#undef _UNICODE
#define OTL_ODBC // Compile OTL 4/ODBC
#define OTL_STL // Compile OTL 4/ODBC
#define OTL_BIGINT long long // SQL_C_SBIGINT

// otl的函数异常已关闭，用以下方法：
//   修改 otlv4.h 中代码#define OTL_ANSI_CPP_11_NOEXCEPT_FALSE noexcept(false) 改成 #define OTL_ANSI_CPP_11_NOEXCEPT_FALSE noexcept(true)
#define OTL_DESTRUCTORS_DO_NOT_THROW
//#define OTL_ANSI_CPP_11_NOEXCEPT_FALSE noexcept(true)

// The following #define is required with MyODBC 3.51.11 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
//#define OTL_UNCAUGHT_EXCEPTION_OFF 关异常

#include "otl/otlv4.h" // include the OTL 4 header file

using namespace std;

vector<string> fn_getDdlStrings()
{
    vector<string> r;
    r.push_back("create");
    r.push_back("drop");
    r.push_back("insert");
    r.push_back("delete");
    return r;
}

static vector<string> f_sDdlStrings = fn_getDdlStrings();

bool g_bHasRegistDatabaseConstructorOdbc = CxDatabaseOdbcFactory::registDatabaseConstructor();

struct OdbcColumnDesc {
    std::string columnName;
    int ftype;
    int fsize;
};

static OdbcColumnDesc fn_getEmptyOdbcColumnDesc()
{
    OdbcColumnDesc m;
    m.columnName.clear();
    m.fsize = 0;
    m.ftype = 0;
    return m;
}

static OdbcColumnDesc c_mEmptyOdbcColumnDesc = fn_getEmptyOdbcColumnDesc();

class CxDatabaseOdbc : public CxDatabase {
private:
    otl_connect _db;
    string _sqlTableInfo;

public:
    CxDatabaseOdbc()
    {

    }

    ~CxDatabaseOdbc()
    {
        if (_db.connected)
        {
            closeDatabaseImpl();
        }
    }

protected:
    bool
    openDatabaseImpl(const std::string &sConnectSource, const std::string &sDatabaseType, bool bCreate, const std::map<std::string, std::string> *oParams)
    {
        otl_connect::otl_initialize();                  // initialize ODBC environment
        //    _db.logoff();
        if (!_db.connected)
        {
            try
            {
                cxDebug() << "OpenDatabase.odbc: rlogon-begin: " << sConnectSource;
                _db.rlogon(sConnectSource.c_str(), 1);                      // connect to ODBC
            }
            catch (otl_exception &p)
            {                        // intercept OTL exceptions
                cxDebug() << p.msg << p.stm_text << p.sqlstate << p.var_info;
                return false;
            }

            string sql_user = "SELECT user() LIMIT 1;";
            string sql_select = "SELECT * FROM %s LIMIT 1;";
            if (CxString::equalIgnoreAll(sDatabaseType, "Oracle"))
            {
                sql_user = "select user from dual;";
                sql_select = "select * from ICS.T_ICS_CFG_PROP where rownum=1;";
            }
            if (CxString::equalIgnoreAll(sDatabaseType, "SQLServer"))
            {
                sql_user = "select * from sysusers where uid between 0 and 4;";
                sql_select = "select top 1 * from %s;";
            }
            string sHeartJumpSql = CxString::trim(CxContainer::valueTo(*oParams, string("HeartJumpSql"), string()));
            if (CxString::equalIgnoreAll(sDatabaseType, "access") || CxString::equalIgnoreAll(sDatabaseType, "sqlite") ||
                (sHeartJumpSql.empty() && CxContainer::contain(*oParams, string("HeartJumpSql"))))
            {
                cxDebug() << "OpenDatabase HeartJumpSql Disabled. ";
            }
            else
            {
                if (sHeartJumpSql.empty()) sHeartJumpSql = sql_user;
                bool bOk = execSqlImpl(sql_user);
                _sqlTableInfo = bOk ? sql_select : "SELECT TOP 1 * FROM %s;";
                if (bOk)
                {
                    _databaseAttrs[string("HeartJumpSql")] = sHeartJumpSql;
                    _databaseAttrs[string("TableInfoSql")] = sql_select;
                    cxDebug() << "OpenDatabase HeartJumpSql : " << sHeartJumpSql;
                    cxDebug() << "OpenDatabase HeartJumpSql Enabled. ";
                }
                else
                {
                    cxDebug() << "OpenDatabase HeartJumpSql ExecSql Fail. HeartJumpSql Disabled.";
                }
            }
            cxDebug() << "OpenDatabase.odbc: rlogon-end: " << _db.connected;
        }

        return _db.connected;
    }

    void closeDatabaseImpl()
    {
        if (_db.connected)
        {
            _db.logoff(); // disconnect from Oracle
        }
    }

    bool isOpenImpl() const
    {
        return _db.connected;
    }

    bool createTableImpl(const std::string &sTableName, const std::map<std::string, std::string> &columns)
    {
        string sSql = CxString::format("CREATE TABLE [%s] (", sTableName.c_str());
        int iCount = 0;
        for (std::map<string, string>::const_iterator it = columns.begin(); it != columns.end(); ++it)
        {
            string sColumn = "[" + it->first + "] " + it->second + ",";
            sSql += sColumn;
            ++iCount;
        }
        if (iCount > 0)
        {
            char *pch = (char *) sSql.data();
            pch[sSql.size() - 1] = ')';
            return execSql(sSql);
        }
        return true;
    }

    bool alterPrimaryKeyAddImpl(const std::string &sTableName, const std::vector<std::string> &columns)
    {
        string sSql = CxString::format("ALTER TABLE ADD CONSTRAINT %s PRIMARY KEY (", sTableName.c_str());
        int iCount = 0;
        for (size_t i = 0; i < columns.size(); ++i)
        {
            sSql += columns.at(i) + " ,";
            ++iCount;
        }
        if (iCount > 0)
        {
            char *pch = (char *) sSql.data();
            pch[sSql.size() - 1] = ')';
            return execSql(sSql);
        }
        return false;
    }

    int
    loadTableImpl(const std::string &sTableName, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1)
    {
        string sSql = "SELECT ";
        if (oColumnNames && oColumnNames->size() > 0)
        {
            int iCount = 0;
            for (size_t i = 0; i < oColumnNames->size(); ++i)
            {
                sSql += oColumnNames->at(i) + " ,";
                ++iCount;
            }
            sSql.resize(sSql.size() - 1);
        }
        else
        {
            sSql += "* ";
        }
        sSql += "FROM " + sTableName;

        return loadSqlImpl(sSql, rows, oColumnNames);
    }

    int
    saveTableImpl(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool bTransaction = true)
    {
        if (!_db.connected)
        {
            string sErrorString = ", error : db is not open sql!";
            setLastError(-1, sErrorString);
            cxDebug() << "CxDatabaseOdbc->saveTableImpl connectSource:" << connectSource() << sErrorString;
            return -1;
        }

        std::vector<OdbcColumnDesc> columnDescs;
        getColumnDescs(sTableName, columnDescs);

//        bool bBlod = false;
//        map<string, string> columnTypeNames;
//        for (size_t i = 0; i < columnNames.size(); ++i)
//        {
//            const string & sColumnName = columnNames.at(i);
//            int ftype = CxString::findValueCase(ftypes, sColumnName, ci_int_zero);
//            if (ftype == 0 && columnTypes.size()>=i)
//                ftype = columnTypes.at(i);
//            if (ftype == 0)
//                return -2;
//            string sTypeName = otl_var_type_name(ftype);
//            if (sTypeName.empty())
//                return -3;
//            columnTypeNames[sColumnName] = sTypeName;
////            if (ftype == otl_var_blob || ftype == otl_var_clob)
//        }

//        msepoch_t dt0 = 0, dt1 = 0, dt2 = 0, dt3 = 0, dt4 = 0, dt5 = 0, dt6 = 0, dt7 = 0;

        try
        {
            cxLog() << "CxDatabaseOdbc->saveTableImpl begin : " << connectSource();

            string sSql = CxString::format("insert into %s ", sTableName.c_str());
            string sColumnes = "(";
            string sValues = "(";
            int iCount = 0;
            vector<int> columnTypes2;
            for (size_t i = 0; i < columnNames.size(); ++i)
            {
                const string &sColumnName = columnNames.at(i);
                OdbcColumnDesc columnDesc = findColumnDesc(columnDescs, sColumnName);
                if (columnDesc.ftype == 0 && columnTypes.size() >= i)
                    columnDesc.ftype = columnTypes.at(i);
                if (columnDesc.ftype < 1 || columnDesc.ftype > 29) // otl_var_char otl_var_bdouble
                    return -2;
                columnTypes2.push_back(columnDesc.ftype);

                sColumnes += sColumnName + " ,";
                if (columnDesc.ftype == otl_var_char)
                    sValues += CxString::format(":%s<%s[%d]> ,", sColumnName.c_str(), otl_var_type_name(columnDesc.ftype), columnDesc.fsize);
                else
                    sValues += CxString::format(":%s<%s> ,", sColumnName.c_str(), otl_var_type_name(columnDesc.ftype));
                ++iCount;
            }
            if (iCount > 0)
            {
                char *pch = (char *) sColumnes.data();
                pch[sColumnes.size() - 1] = ')';
                pch = (char *) sValues.data();
                pch[sValues.size() - 1] = ')';
            }
            sSql = sSql + sColumnes + " VALUES " + sValues + " ;";

//            sSql = "insert into t1 (f1 ,f2 ,f3 ,f4 ,f5 ,f6 ,f7 ) VALUES (:f1<SHORT INT> ,:f2<INT> ,:f3<DOUBLE> ,:f4<DOUBLE> ,:f5<CHAR[31]> ,:f6<VARCHAR LONG> ,:f7<RAW LONG> )";
//            cout<<"CxDatabaseOdbc->saveTableImpl sql : "<<sSql;

            //以下数据插入参考 ：ex702_odbc.cpp
            otl_stream o(1, // buffer size has to be set to 1 for operations with LONGTEXTs
//                   "insert into test_tab values(:f1<int>,:f2<varchar_long>)",
                         sSql.c_str(),
                // SQL statement
                         _db // connect object
            );
//            o.set_flush(false);
//            o.get_shell()->io->set_flush_flag(false);
//            o.get_shell()->io->set_flush_flag2(false);
//            (* o.io)->set_flush_flag2(false);
//            o.setBufSize(1024 * 1024 * 2);
//            o.get_shell()

            //启动事务
            if (bTransaction)
            {
                _db.auto_commit_off();
                o.set_commit(0); // setting stream "auto-commit" to "off". It is required
//                o.get_shell()->adb->auto_commit_off();
            }

            for (size_t i = 0; i < rows.size(); ++i)
            {
                const vector<string> &row = rows.at(i);
                if (row.size() < columnTypes2.size())
                    continue;

                for (size_t j = 0; j < row.size(); ++j)
                {
                    const string &sValue = row.at(j);
                    int iFtype = columnTypes2.at(j);
                    switch (iFtype)
                    {
                        case otl_var_short:
                            o << (short) CxString::toInt32(sValue);
                            break;
                        case otl_var_int:
                        case otl_var_unsigned_int:
                            o << CxString::toInt32(sValue);
                            break;
                        case otl_var_long_int:
                        case otl_var_bigint:
                        case otl_var_ubigint:
                            o << (long int) CxString::toInt64(sValue);
                            break;
                        case otl_var_float:
                        case otl_var_bfloat:
                            o << CxString::toFloat(sValue);
                            break;
                        case otl_var_double:
                        case otl_var_bdouble:
                            o << CxString::toDouble(sValue);
                            break;
                        case otl_var_raw:
                        case otl_var_char:
                        case otl_var_varchar_long:
                        case otl_var_raw_long:
                        case otl_var_blob:
                        case otl_var_clob:
                            o << sValue;
                            break;
                        default:
                            o << sValue;
                    }
                }
            }
            if (bTransaction)
            {
                _db.commit(); // committing transaction.
                _db.auto_commit_on();
            }
        }

        catch (otl_exception &p)
        {                        // intercept OTL exceptions
            if (bTransaction)
            {
                _db.rollback(); // committing transaction.
                _db.auto_commit_on();
            }
            cxDebug() << p.msg << p.stm_text << p.sqlstate << p.var_info;
            return -9;
        }

//        cxDebug()<<"CxDatabaseOdbc->saveTableImpl end "<<_db.connected << ";dt1-dt0=" << dt1-dt0 << ";dt2-dt1=" << dt2-dt1 << ";dt3-dt2=" << dt3-dt2 << ";dt4-dt3=" << dt4-dt3 << ";dt5-dt4=" << dt5-dt4 << ";dt6-dt5=" << dt6-dt5 << ";dt7-dt6=" << dt7-dt6;
        cxLog() << "CxDatabaseOdbc->saveTableImpl end " << _db.connected;

        return rows.size();
    }

    int
    saveTableImpl(const std::string &sSql, const std::vector<std::vector<std::string> > &rows, const std::vector<int> *oColumnTypes, bool bTransaction = false)
    {
        cxWarning() << "CxDatabaseOdbc->saveTableImpl by sql is not impl";
        return -1;
    }

    int
    updateTableImpl(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool bTransaction = true)
    {
        if (!_db.connected)
        {
            string sErrorString = ", error : db is not open sql!";
            setLastError(-1, sErrorString);
            cxDebug() << "CxDatabaseOdbc->saveTableImpl connectSource:" << connectSource() << sErrorString;
            return -1;
        }
        if (columnNames.size() < 3)
        {
            string sErrorString = ", error : columnNames size less 3!";
            setLastError(-2, sErrorString);
            cxDebug() << "CxDatabaseOdbc->saveTableImpl connectSource:" << connectSource() << sErrorString;
            return -2;
        }

        std::vector<OdbcColumnDesc> columnDescs;
        getColumnDescs(sTableName, columnDescs);

//        bool bBlod = false;
//        map<string, string> columnTypeNames;
//        for (size_t i = 0; i < columnNames.size(); ++i)
//        {
//            const string & sColumnName = columnNames.at(i);
//            int ftype = CxString::findValueCase(ftypes, sColumnName, ci_int_zero);
//            if (ftype == 0 && columnTypes.size()>=i)
//                ftype = columnTypes.at(i);
//            if (ftype == 0)
//                return -2;
//            string sTypeName = otl_var_type_name(ftype);
//            if (sTypeName.empty())
//                return -3;
//            columnTypeNames[sColumnName] = sTypeName;
////            if (ftype == otl_var_blob || ftype == otl_var_clob)
//        }

//        msepoch_t dt0 = 0, dt1 = 0, dt2 = 0, dt3 = 0, dt4 = 0, dt5 = 0, dt6 = 0, dt7 = 0;

        try
        {
            cxLog() << "CxDatabaseOdbc->saveTableImpl begin : " << connectSource();

            string sSql = CxString::format("UPDATE %s ", sTableName.c_str());
            string sColumnes = "(";
            vector<int> columnTypes2;
            for (size_t i = 0; i < columnNames.size(); ++i)
            {
                const string &sColumnName = columnNames.at(i);
                OdbcColumnDesc columnDesc = findColumnDesc(columnDescs, sColumnName);
                if (columnDesc.ftype == 0 && columnTypes.size() >= i)
                    columnDesc.ftype = columnTypes.at(i);
                if (columnDesc.ftype < 1 || columnDesc.ftype > 29) // otl_var_char otl_var_bdouble
                    return -2;
                columnTypes2.push_back(columnDesc.ftype);

                if (i == columnNames.size() - 1)
                {
                    char *pch = (char *) sColumnes.data();
                    pch[sColumnes.size() - 1] = ' ';
                    sColumnes += CxString::format(" WHERE %s = ", sColumnName.c_str());
                }
                else
                {
                    sColumnes += sColumnName + " = ";
                }
                if (columnDesc.ftype == otl_var_char)
                    sColumnes += CxString::format(":%s<%s[%d]> ,", sColumnName.c_str(), otl_var_type_name(columnDesc.ftype), columnDesc.fsize);
                else
                    sColumnes += CxString::format(":%s<%s> ,", sColumnName.c_str(), otl_var_type_name(columnDesc.ftype));
            }
            sSql = sSql + sColumnes;

//            sSql = "insert into t1 (f1 ,f2 ,f3 ,f4 ,f5 ,f6 ,f7 ) VALUES (:f1<SHORT INT> ,:f2<INT> ,:f3<DOUBLE> ,:f4<DOUBLE> ,:f5<CHAR[31]> ,:f6<VARCHAR LONG> ,:f7<RAW LONG> )";
//            cout<<"CxDatabaseOdbc->saveTableImpl sql : "<<sSql;

            //以下数据插入参考 ：ex702_odbc.cpp
            otl_stream o(1, // buffer size has to be set to 1 for operations with LONGTEXTs
//                   "insert into test_tab values(:f1<int>,:f2<varchar_long>)",
                         sSql.c_str(),
                // SQL statement
                         _db // connect object
            );
//            o.set_flush(false);
//            o.get_shell()->io->set_flush_flag(false);
//            o.get_shell()->io->set_flush_flag2(false);
//            (* o.io)->set_flush_flag2(false);
//            o.setBufSize(1024 * 1024 * 2);
//            o.get_shell()

            //启动事务
            if (bTransaction)
            {
                _db.auto_commit_off();
                o.set_commit(0); // setting stream "auto-commit" to "off". It is required
//                o.get_shell()->adb->auto_commit_off();
            }

            for (size_t i = 0; i < rows.size(); ++i)
            {
                const vector<string> &row = rows.at(i);
                if (row.size() < columnTypes2.size())
                    continue;

                for (size_t j = 0; j < row.size(); ++j)
                {
                    const string &sValue = row.at(j);
                    int iFtype = columnTypes2.at(j);
                    switch (iFtype)
                    {
                        case otl_var_short:
                            o << (short) CxString::toInt32(sValue);
                            break;
                        case otl_var_int:
                        case otl_var_unsigned_int:
                            o << CxString::toInt32(sValue);
                            break;
                        case otl_var_long_int:
                        case otl_var_bigint:
                        case otl_var_ubigint:
                            o << (long int) CxString::toInt64(sValue);
                            break;
                        case otl_var_float:
                        case otl_var_bfloat:
                            o << CxString::toFloat(sValue);
                            break;
                        case otl_var_double:
                        case otl_var_bdouble:
                            o << CxString::toDouble(sValue);
                            break;
                        case otl_var_raw:
                        case otl_var_char:
                        case otl_var_varchar_long:
                        case otl_var_raw_long:
                        case otl_var_blob:
                        case otl_var_clob:
                            o << sValue;
                            break;
                        default:
                            o << sValue;
                    }
                }
            }
            if (bTransaction)
            {
                _db.commit(); // committing transaction.
                _db.auto_commit_on();
            }
        }

        catch (otl_exception &p)
        {                        // intercept OTL exceptions
            if (bTransaction)
            {
                _db.rollback(); // committing transaction.
                _db.auto_commit_on();
            }
            cxDebug() << p.msg << p.stm_text << p.sqlstate << p.var_info;
            return -9;
        }

//        cxDebug()<<"CxDatabaseOdbc->saveTableImpl end "<<_db.connected << ";dt1-dt0=" << dt1-dt0 << ";dt2-dt1=" << dt2-dt1 << ";dt3-dt2=" << dt3-dt2 << ";dt4-dt3=" << dt4-dt3 << ";dt5-dt4=" << dt5-dt4 << ";dt6-dt5=" << dt6-dt5 << ";dt7-dt6=" << dt7-dt6;
        cxLog() << "CxDatabaseOdbc->saveTableImpl end " << _db.connected;

        return rows.size();
    }

    int execSqlImpl(const string &sSql)
    {
        int iResult = FALSE;

        if (_db.connected && sSql.size() > 0)
        {
            try
            {
                _db.auto_commit_on();

                if (CxString::existCase(sSql, f_sDdlStrings))
                {
                    iResult = otl_cursor::direct_exec
                        (
                            _db,
                            sSql.c_str(),
                            otl_exception::disabled
                        );
//                    _db.commit();
                }
                else
                {
                    otl_stream o(1, sSql.c_str(), _db); // buffer size （SQL执行后数据往返次数）, SQL statement,connect object
                    iResult = TRUE;
                }
            }
            catch (otl_exception &p)
            {                        // intercept OTL exceptions
                cxDebug() << "odbc execSqlImpl exception: " << p.msg << p.stm_text << p.sqlstate << p.var_info
                          << cxEndLine;
                iResult = FALSE;
            }
            catch (...)
            {
                cxDebug() << "odbc execSqlImpl exception : unknow";
                iResult = FALSE;
            }
        }
        return iResult;
    }

    int execSqlListImpl(const std::vector<std::string> &sqlList)
    {
        int iResult = FALSE;

        if (_db.connected && sqlList.size() > 0)
        {
            try
            {
                _db.auto_commit_off();

                for (size_t i = 0; i < sqlList.size(); ++i)
                {
                    const std::string &sSql = sqlList.at(i);

                    otl_stream o;
                    o.set_commit(0);
                    o.open(1, sSql.c_str(), _db);
                }

                _db.commit();

                iResult = TRUE;

//                if (CxString::existCase(sSql, "delete"))
//                {
//                    iResult = otl_cursor::direct_exec(_db,sSql.c_str());
//                }
//                else
//                {
//                    otl_stream o(1,sSql.c_str(),_db); // buffer size （SQL执行后数据往返次数）, SQL statement,connect object
//                    iResult = TRUE;
//                }
            }
            catch (otl_exception &p)
            {
                _db.rollback();
                // intercept OTL exceptions
                cxDebug() << "odbc execSqlImpl : " << p.msg << p.stm_text << p.sqlstate << p.var_info << cxEndLine;
            }

        }

        return iResult;
    }

    int
    loadSqlImpl(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1)
    {
        if (!_db.connected)
        {
            string sErrorString = ", error : db is not open sql!";
            setLastError(-1, sErrorString);
            cxDebug() << "CxDatabaseOdbc->saveTableImpl connectSource:" << connectSource() << sErrorString;
            return -1;
        }

        vector<OdbcColumnDesc> columnDescs;

        try
        {
            cxLog() << "CxDatabaseOdbc->saveTableImpl begin : " << connectSource();

            otl_stream o(1, // buffer size has to be set to 1 for operations with LONGTEXTs
//                   "insert into test_tab values(:f1<int>,:f2<varchar_long>)",
                         sSql.c_str(),
                // SQL statement
                         _db // connect object
            );

            otl_column_desc *desc;
            int desc_len;

            desc = o.describe_select(desc_len);

            OdbcColumnDesc columnDesc;
            if (oColumnNames)
                oColumnNames->clear();
            for (int i = 0; i < desc_len; ++i)
            {
                if (oColumnNames)
                    oColumnNames->push_back(string(desc[i].name));
                columnDesc.columnName = desc[i].name;
                columnDesc.ftype = desc[i].otl_var_dbtype;
                columnDesc.fsize = desc[i].dbsize;
                columnDescs.push_back(columnDesc);
            }
            if (columnDescs.empty())
                return -2;

            while (!o.eof())
            {
                std::vector<std::string> row;
                for (size_t i = 0; i < columnDescs.size(); i++)
                {
                    OdbcColumnDesc &columnDesc = columnDescs.at(i);

                    switch (columnDesc.ftype)
                    {
                        case otl_var_short:
                        {
                            short value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string(""));
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_int:
                        case otl_var_unsigned_int:
                        {
                            int value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_long_int:
                        case otl_var_bigint:
                        case otl_var_ubigint:
                        {
                            OTL_BIGINT value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_float:
                        case otl_var_bfloat:
                        {
                            float value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_double:
                        case otl_var_bdouble:
                        {
                            double value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_timestamp:
                        case otl_var_tz_timestamp:
                        case otl_var_ltz_timestamp:
                        case otl_var_db2time:
                        case otl_var_db2date:
                        {
                            otl_datetime value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxTime::toString(value.year, value.month, value.day, value.hour, value.minute, value.second, '-', ' ', ':'));
                        }
                            break;
                        case otl_var_char:
                        case otl_var_varchar_long:
                        {
                            string value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(value);
                        }
                            break;
                        case otl_var_raw:
                        case otl_var_raw_long:
                        case otl_var_blob:
                        case otl_var_clob:
                        {
                            string value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        default:
                        {
                            string value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                    }
                }
                rows.push_back(row);
                if (rows.size() > iMaxRowCount)
                    break;
            }
        }
        catch (otl_exception &p)
        {                        // intercept OTL exceptions
            cxDebug() << p.msg << p.stm_text << p.sqlstate << p.var_info;
            return -9;
        }

        cxLog() << "CxDatabaseOdbc->loadSqlImpl end " << rows.size();

        return rows.size();
    }

    int
    loadSql2Impl(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, std::vector<int> *oColumnTypes = NULL, int iMaxRowCount = -1)
    {
        if (!_db.connected)
        {
            string sErrorString = ", error : db is not open sql!";
            setLastError(-1, sErrorString);
            cxDebug() << "CxDatabaseOdbc->saveTableImpl connectSource:" << connectSource() << sErrorString;
            return -1;
        }

        vector<OdbcColumnDesc> columnDescs;

        try
        {
            cxLog() << "CxDatabaseOdbc->saveTableImpl begin : " << connectSource();

            otl_stream o(1, // buffer size has to be set to 1 for operations with LONGTEXTs
                //                   "insert into test_tab values(:f1<int>,:f2<varchar_long>)",
                         sSql.c_str(),
                // SQL statement
                         _db // connect object
            );

            otl_column_desc *desc;
            int desc_len;

            desc = o.describe_select(desc_len);

            OdbcColumnDesc columnDesc;
            if (oColumnNames)
                oColumnNames->clear();
            if (oColumnTypes)
                oColumnTypes->clear();
            for (int i = 0; i < desc_len; ++i)
            {
                if (oColumnNames)
                    oColumnNames->push_back(string(desc[i].name));
                if (oColumnTypes)
                {
                    switch (desc[i].otl_var_dbtype)
                    {
                        case otl_var_short:
                        case otl_var_int:
                        case otl_var_unsigned_int:
                        {
                            oColumnTypes->push_back(column_type_int);
                        }
                            break;
                        case otl_var_long_int:
                        case otl_var_bigint:
                        case otl_var_ubigint:
                        {
                            oColumnTypes->push_back(column_type_longint);
                        }
                            break;
                        case otl_var_float:
                        case otl_var_bfloat:
                        case otl_var_double:
                        case otl_var_bdouble:
                        {
                            oColumnTypes->push_back(column_type_double);
                        }
                            break;
                        case otl_var_timestamp:
                        case otl_var_tz_timestamp:
                        case otl_var_ltz_timestamp:
                        case otl_var_db2time:
                        case otl_var_db2date:
                        {
                            oColumnTypes->push_back(column_type_datetime);
                        }
                            break;
                        case otl_var_char:
                        case otl_var_varchar_long:
                        {
                            oColumnTypes->push_back(column_type_string);
                        }
                            break;
                        case otl_var_raw:
                        case otl_var_raw_long:
                        case otl_var_blob:
                        case otl_var_clob:
                        {
                            oColumnTypes->push_back(column_type_blob);
                        }
                            break;
                        default:
                        {
                            oColumnTypes->push_back(column_type_string);
                        }
                    }
                }
                columnDesc.columnName = desc[i].name;
                columnDesc.ftype = desc[i].otl_var_dbtype;
                columnDesc.fsize = desc[i].dbsize;
                columnDescs.push_back(columnDesc);
            }
            if (columnDescs.empty())
                return -2;

            while (!o.eof())
            {
                std::vector<std::string> row;
                for (size_t i = 0; i < columnDescs.size(); i++)
                {
                    OdbcColumnDesc &columnDesc = columnDescs.at(i);

                    switch (columnDesc.ftype)
                    {
                        case otl_var_short:
                        {
                            short value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string(""));
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_int:
                        case otl_var_unsigned_int:
                        {
                            int value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_long_int:
                        case otl_var_bigint:
                        case otl_var_ubigint:
                        {
                            OTL_BIGINT value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_float:
                        case otl_var_bfloat:
                        {
                            float value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_double:
                        case otl_var_bdouble:
                        {
                            double value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        case otl_var_timestamp:
                        case otl_var_tz_timestamp:
                        case otl_var_ltz_timestamp:
                        case otl_var_db2time:
                        case otl_var_db2date:
                        {
                            otl_datetime value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxTime::toString(value.year, value.month, value.day, value.hour, value.minute, value.second, '-', ' ', ':'));
                        }
                            break;
                        case otl_var_char:
                        case otl_var_varchar_long:
                        {
                            string value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(value);
                        }
                            break;
                        case otl_var_raw:
                        case otl_var_raw_long:
                        case otl_var_blob:
                        case otl_var_clob:
                        {
                            string value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                            break;
                        default:
                        {
                            string value;
                            o >> value;
                            if (o.is_null())
                                row.push_back(string());
                            else
                                row.push_back(CxString::toString(value));
                        }
                    }
                }
                rows.push_back(row);
                if (rows.size() > iMaxRowCount)
                    break;
            }
        }
        catch (otl_exception &p)
        {                        // intercept OTL exceptions
            cxDebug() << p.msg << p.stm_text << p.sqlstate << p.var_info;
            return -9;
        }

        cxLog() << "CxDatabaseOdbc->loadSqlImpl end " << rows.size();

        return rows.size();
    }

private:
    void getColumnDescs(const std::string &sTableName, std::vector<OdbcColumnDesc> &fieldTypes)
    {
        if (_db.connected)
        {
            string sSql = CxString::format(_sqlTableInfo.c_str(), sTableName.c_str());

            try
            {

                otl_stream o(1, sSql.c_str(), _db); // buffer size （SQL执行后数据往返次数）, SQL statement,connect object

                otl_column_desc *desc;
                int desc_len;

                desc = o.describe_select(desc_len);

                OdbcColumnDesc columnDesc;
                for (int i = 0; i < desc_len; ++i)
                {
                    columnDesc.columnName = string(desc[i].name);
                    columnDesc.ftype = desc[i].otl_var_dbtype;
                    columnDesc.fsize = desc[i].dbsize;
//                    cout<<"========== COLUMN #"<<i+1<<" ==========="<<endl;
//                    cout<<"name="<<desc[i].name<<endl;
//                    cout<<"dbtype="<<desc[i].dbtype<<endl;
//                    cout<<"otl_var_dbtype="<<desc[i].otl_var_dbtype<<endl;
//                    cout<<"dbsize="<<desc[i].dbsize<<endl;
//                    cout<<"scale="<<desc[i].scale<<endl;
//                    cout<<"prec="<<desc[i].prec<<endl;
//                    cout<<"nullok="<<desc[i].nullok<<endl;
                    fieldTypes.push_back(columnDesc);
//                    otl_var_type_name(columnDesc.ftype);
                }
            }

            catch (otl_exception &p)
            {                        // intercept OTL exceptions
                cxDebug() << "odbc getFieldTypes : " << p.msg << p.stm_text << p.sqlstate << p.var_info << cxEndLine;
            }
        }
    }

    OdbcColumnDesc findColumnDesc(const vector<OdbcColumnDesc> &columnDescs, const std::string &sColumnName)
    {
        for (size_t i = 0; i < columnDescs.size(); ++i)
        {
            const OdbcColumnDesc &columnDesc = columnDescs.at(i);
            if (CxString::equalCase(columnDesc.columnName, sColumnName))
                return columnDesc;
        }
        return c_mEmptyOdbcColumnDesc;
    }

};




//insert into T_ATM330_MCXX (F_DXID,F_DXMC,F_MCLX,F_CZBZW) values ('aa', 'bb', 'cc', 9966);

bool CxDatabaseOdbcFactory::registDatabaseConstructor()
{
    CxDatabaseManager::registDatabaseConstructor(CxDatabaseOdbcFactory::isMyDatabase, CxDatabaseOdbcFactory::createDatabase);
    return true;
}

bool CxDatabaseOdbcFactory::isMyDatabase(const std::string &sConnectSource)
{
    return CxString::containCase(sConnectSource, "DSN");
}

CxDatabase *CxDatabaseOdbcFactory::createDatabase(void)
{
    return new CxDatabaseOdbc();
}
