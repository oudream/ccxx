#include "cxdatabase.h"

#include "cxstring.h"
#include "cxcontainer.h"

#include "cxapplication.h"
#include "cxfile.h"
#include "cxinterinfo.h"


//#define OTL_ORA8I
#define OTL_ORA11G
#define OTL_STL // Compile OTL 4/ODBC
//#define OTL_ODBC // Compile OTL 4/ODBC
//#undef UNICODE
//#undef _UNICODE
#include "otl/otlv4.h" // include the OTL 4 header file


using namespace std;



struct OciColumnDesc
{
    std::string columnName;
    int ftype;
    int fsize;
};

OciColumnDesc emptyOciColumnDesc;


class CxDatabaseOci : public CxDatabase
{
public:
    static bool isMyDatabase(const std::string & sConnectSource)
    {
        return CxString::containCase(sConnectSource, "@");
    }

    static CxDatabase * createDatabase(void)
    {
        return new CxDatabaseOci();
    }

private:
    otl_connect _db;
    string _sqlTableInfo;

public:
    CxDatabaseOci()
    {

    }

    ~CxDatabaseOci()
    {
        if (_db.connected)
        {
            closeDatabaseImpl();
        }
    }

protected:
    bool openDatabaseImpl(const std::string & sConnectSource,  bool bCreate, const std::map<std::string, std::string> * oParams)
    {
        otl_connect::otl_initialize();                  // initialize ODBC environment

        cxDebug()<<"CxDatabaseOci->rlogon begin"<<_db.connected;

        //    _db.logoff();
        if(!_db.connected)
        {
            try
            {
                cxDebug()<<"CxDatabaseOci->rlogon begin : "<<sConnectSource;

                _db.rlogon(sConnectSource.c_str(),1);                      // connect to ODBC

                bool bOk = execSqlImpl("SELECT user() LIMIT 1");
                _sqlTableInfo = bOk ? "SELECT * FROM %s LIMIT 1;" : "SELECT TOP 1 * FROM %s;";
            }
            catch(otl_exception& p)
            {                        // intercept OTL exceptions
                cxDebug()<<p.msg<<p.stm_text<<p.sqlstate<<p.var_info;
                return -1;
            }

        }
         cxDebug()<<"CxDatabaseOci->rlogon end"<<_db.connected;

        return _db.connected;
    }

    void closeDatabaseImpl()
    {
        if(_db.connected)
        {
            _db.logoff(); // disconnect from Oracle
        }
    }

    bool isOpenImpl() const
    {
        return _db.connected;
    }

    bool createTableImpl(const std::string & sTableName, const std::map<std::string, std::string> & columns)
    {
        string sSql = CxString::format("CREATE TABLE [%s] (", sTableName.c_str());
        int iCount = 0;
        for(std::map<string,string>::const_iterator it = columns.begin(); it != columns.end(); ++it)
        {
            string sColumn = "[" + it->first + "] " + it->second + ",";
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

    int saveTableImpl(const std::string & sTableName, const std::vector<std::string> & columnNames, const std::vector<std::vector<std::string> > & rows, const std::vector<int> & columnTypes = std::vector<int>(), bool bTransaction = false)
    {
        if (! _db.connected)
        {
            string sErrorString = "error : db is not open sql!";
            setLastError(-1, sErrorString);
            return -1;
        }

        std::vector<OciColumnDesc> columnDescs;
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

        try{
            cxDebug()<<"CxDatabaseOci->saveTableImpl begin : "<<connectSource();

            string sSql = CxString::format("insert into %s ", sTableName.c_str());
            string sColumnes = "(";
            string sValues = "(";
            int iCount = 0;
            vector<int> columnTypes2;
            for (size_t i = 0; i < columnNames.size(); ++i)
            {
                const string & sColumnName = columnNames.at(i);
                OciColumnDesc columnDesc = findColumnDesc(columnDescs, sColumnName);
                if (columnDesc.ftype == 0 && columnTypes.size()>=i)
                    columnDesc.ftype = columnTypes.at(i);
                if (columnDesc.ftype < 1 || columnDesc.ftype > 29) // otl_var_char otl_var_bdouble
                    return -2;
                columnTypes2.push_back(columnDesc.ftype);

                sColumnes += sColumnName + " ,";
                if (columnDesc.ftype == otl_var_char)
                    sValues += CxString::format(":%s<%s[%d]> ,", sColumnName.c_str(), otl_var_type_name(columnDesc.ftype), columnDesc.fsize);
                else
                    sValues += CxString::format(":%s<%s> ,", sColumnName.c_str(), otl_var_type_name(columnDesc.ftype));
                ++ iCount;
            }
            if (iCount > 0)
            {
                char * pch = (char *)sColumnes.data();
                pch[sColumnes.size()-1] = ')';
                pch = (char *)sValues.data();
                pch[sValues.size()-1] = ')';
            }
            sSql = sSql + sColumnes + " VALUES " + sValues + " ;";

//            sSql = "insert into t1 (f1 ,f2 ,f3 ,f4 ,f5 ,f6 ,f7 ) VALUES (:f1<SHORT INT> ,:f2<INT> ,:f3<DOUBLE> ,:f4<DOUBLE> ,:f5<CHAR[31]> ,:f6<VARCHAR LONG> ,:f7<RAW LONG> )";
//            cout<<"CxDatabaseOci->saveTableImpl sql : "<<sSql;

            //以下数据插入参考 ：ex702_odbc.cpp
            otl_stream o;

            o.open(1, // buffer size has to be set to 1 for operations with LONGTEXTs
//                   "insert into test_tab values(:f1<int>,:f2<varchar_long>)",
                   sSql.c_str(),
                   // SQL statement
                   _db // connect object
                   );

            //启动事务
            if (bTransaction)
                o.set_commit(0); // setting stream "auto-commit" to "off". It is required

            for (size_t i = 0; i < rows.size(); ++i)
            {
                const vector<string> & row = rows.at(i);
                if (row.size()<columnTypes2.size())
                    continue;
                for (size_t j = 0; j < row.size(); ++j)
                {
                    const string & sValue = row.at(j);
                    int iFtype = columnTypes2.at(j);
                    switch (iFtype)
                    {
                    case otl_var_short:
                        o << (short)CxString::toInt32(sValue);
                        break;
                    case otl_var_int:
                    case otl_var_unsigned_int:
                        o << CxString::toInt32(sValue);
                        break;
                    case otl_var_long_int:
                    case otl_var_bigint:
                    case otl_var_ubigint:
                        o << (long int)CxString::toInt64(sValue);
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
                _db.commit(); // committing transaction.
        }

        catch(otl_exception& p){                        // intercept OTL exceptions
            cxDebug()<<p.msg<<p.stm_text<<p.sqlstate<<p.var_info;
            return -9;
        }

        cxDebug()<<"CxDatabaseOci->saveTableImpl end"<<_db.connected;

        return rows.size();
    }

    int execSqlImpl(const string &sSql)
    {
        int iResult = FALSE;

        if(_db.connected)
        {
            try{

                otl_stream o(1,sSql.c_str(),_db); // buffer size （SQL执行后数据往返次数）, SQL statement,connect object

                iResult = TRUE;
            }

            catch(otl_exception& p)
            {                        // intercept OTL exceptions
                cxDebug()<<"oci execSqlImpl : "<<p.msg<<p.stm_text<<p.sqlstate<<p.var_info<<cxEndLine;
            }
        }

        return iResult;
    }

    int loadSqlImpl(const std::string & sSql, std::vector<std::vector<std::string> > & rows, std::vector<std::string> * oColumnNames = NULL, int iMaxRowCount = -1)
    {
        if (! _db.connected)
        {
            string sErrorString = "error : db is not open sql!";
            setLastError(-1, sErrorString);
            return -1;
        }

        vector<OciColumnDesc> columnDescs;

        try
        {
            cxDebug()<<"CxDatabaseOci->saveTableImpl begin : "<<connectSource();

            otl_stream o(1, // buffer size has to be set to 1 for operations with LONGTEXTs
//                   "insert into test_tab values(:f1<int>,:f2<varchar_long>)",
                   sSql.c_str(),
                   // SQL statement
                   _db // connect object
                   );

            otl_column_desc* desc;
            int desc_len;

            desc=o.describe_select(desc_len);

            OciColumnDesc columnDesc;
            if (oColumnNames)
                oColumnNames->clear();
            for(int i=0;i<desc_len;++i)
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

            while(!o.eof())
            {
                std::vector<std::string> row;
                for(size_t i=0;i<columnDescs.size();i++)
                {
                    OciColumnDesc & columnDesc = columnDescs.at(i);

                    switch (columnDesc.ftype)
                    {
                    case otl_var_short:
                    {
                        short value;
                        o >> value;
                        row.push_back(CxString::toString(value));
                    }
                        break;
                    case otl_var_int:
                    case otl_var_unsigned_int:
                    {
                        int value;
                        o >> value;
                        row.push_back(CxString::toString(value));
                    }
                        break;
                    case otl_var_long_int:
                    case otl_var_bigint:
                    case otl_var_ubigint:
                    {
                        long int value;
                        o >> value;
                        row.push_back(CxString::toString(value));
                    }
                        break;
                    case otl_var_float:
                    case otl_var_bfloat:
                    {
                        float value;
                        o >> value;
                        row.push_back(CxString::toString(value));
                    }
                        break;
                    case otl_var_double:
                    case otl_var_bdouble:
                    {
                        double value;
                        o >> value;
                        row.push_back(CxString::toString(value));
                    }
                        break;
                    case otl_var_raw:
                    case otl_var_char:
                    case otl_var_varchar_long:
                    case otl_var_raw_long:
                    case otl_var_blob:
                    case otl_var_clob:
                    {
                        string value;
                        o >> value;
                        row.push_back(CxString::toString(value));
                    }
                        break;
                    default:
                    {
                        string value;
                        o >> value;
                        row.push_back(CxString::toString(value));
                    }
                    }
                }
                rows.push_back(row);
                if (rows.size() > iMaxRowCount)
                    break;
            }
        }
        catch(otl_exception& p)
        {                        // intercept OTL exceptions
            cxDebug()<<p.msg<<p.stm_text<<p.sqlstate<<p.var_info;
            return -9;
        }

        cxDebug()<<"CxDatabaseOci->loadSqlImpl end "<<rows.size();

        return rows.size();
    }

private:
    void getColumnDescs(const std::string &sTableName, std::vector<OciColumnDesc> & fieldTypes)
    {
        if(_db.connected)
        {
            string sSql = CxString::format(_sqlTableInfo.c_str(), sTableName.c_str());

            try{

                otl_stream o(1,sSql.c_str(),_db); // buffer size （SQL执行后数据往返次数）, SQL statement,connect object

                otl_column_desc* desc;
                int desc_len;

                desc=o.describe_select(desc_len);

                OciColumnDesc columnDesc;
                for(int i=0;i<desc_len;++i)
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

            catch(otl_exception& p)
            {                        // intercept OTL exceptions
                cxDebug()<<"oci getFieldTypes : "<<p.msg<<p.stm_text<<p.sqlstate<<p.var_info<<cxEndLine;
            }
        }
    }

    OciColumnDesc findColumnDesc(const vector<OciColumnDesc> & columnDescs, const std::string & sColumnName)
    {
        for (size_t i = 0; i < columnDescs.size(); ++i)
        {
            const OciColumnDesc & columnDesc = columnDescs.at(i);
            if (CxString::equalCase(columnDesc.columnName, sColumnName))
                return columnDesc;
        }
        return emptyOciColumnDesc;
    }

};



CxDatabaseOci * fn_oDatabaseOci()
{
    emptyOciColumnDesc.columnName.clear();
    emptyOciColumnDesc.fsize = 0;
    emptyOciColumnDesc.ftype = 0;

    CxDatabaseManager::registDatabaseConstructor(CxDatabaseOci::isMyDatabase, CxDatabaseOci::createDatabase);

    static CxDatabaseOci m;
    return & m;
}

static CxDatabaseOci * f_oDatabaseOci = fn_oDatabaseOci();

