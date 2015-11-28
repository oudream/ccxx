#ifndef CXDATABASE_H
#define CXDATABASE_H

#include "cxglobal.h"

static const std::string CS_SectionDataBase = "DataBase";

class CxDatabase
{
public:
    enum column_type_enum {column_type_none=0, column_type_boolean, column_type_int, column_type_longint, column_type_double, column_type_string, column_type_text, column_type_blob, column_type_res1, column_type_res2, column_type_res3, column_type_res4, column_type_res5 };

public:
    static CxDatabase * getDefaultDb();

    CxDatabase();

    virtual ~CxDatabase();

    inline const std::string & lastErrorString() const {
        return _lastErrorString;
    }

    inline int lastErrorId() const {
        return _lastErrorId;
    }

    inline const std::string & connectSource() const {
        return _connectSource;
    }

    inline const std::map<std::string, std::string> & connectParams() const {
        return _connectParams;
    }

    inline bool isOpen() const {
        return isOpenImpl();
    }

    bool openDatabase(const std::string & sConnectSource, bool bCreate = true, const std::map<std::string, std::string> * oParams = NULL);

    bool openDatabase();

    void closeDatabase();

    inline bool createTable(const std::string & sTableName, const std::map<std::string, std::string> & columns) {
        return createTableImpl(sTableName, columns);
    }

    inline bool alterPrimaryKeyAdd(const std::string & sTableName, const std::vector<std::string> & columns) {
        return alterPrimaryKeyAddImpl(sTableName, columns);
    }

    inline int loadTable(const std::string & sTableName, std::vector<std::vector<std::string> > & rows, std::vector<std::string> * oColumnNames = NULL, int iMaxRowCount = -1) {
        return loadTableImpl(sTableName, rows, oColumnNames);
    }

    inline int saveTable(const std::string & sTableName, const std::vector<std::string> & columnNames, const std::vector<std::vector<std::string> > & rows, const std::vector<int> & columnTypes = std::vector<int>(), bool bTransaction = true) {
        return saveTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    }

    inline int saveTable(const std::string & sSql, const std::vector<std::vector<std::string> > & rows, const std::vector<int> * oColumnTypes = NULL, bool bTransaction = false) {
        return saveTableImpl(sSql, rows, oColumnTypes, bTransaction);
    }

    inline int execSql(const std::string & sSql) {
        return execSqlImpl(sSql);
    }

    inline int execSqlList(const std::vector<std::string> & sqlList) {
        return execSqlListImpl(sqlList);
    }

    int loadSql(const std::string & sSql, std::vector<std::vector<std::string> > & rows, std::vector<std::string> * oColumnNames = NULL) {
        return loadSqlImpl(sSql, rows, oColumnNames);
    }

    inline std::string queryValue(const std::string & sSql) {
        std::vector<std::vector<std::string> > rows;
        loadSqlImpl(sSql, rows, NULL, 1);
        if (rows.size())
        {
            const std::vector<std::string> & row = rows.at(0);
            if (row.size() > 0)
            {
                return row.at(0);
            }
        }
        return std::string();
    }

    inline bool queryValue(const std::string & sSql, std::string & sValue) {
        std::vector<std::vector<std::string> > rows;
        loadSqlImpl(sSql, rows, NULL, 1);
        if (rows.size())
        {
            const std::vector<std::string> & row = rows.at(0);
            if (row.size() > 0)
            {
                sValue = row.at(0);
                return true;
            }
        }
        return false;
    }

    inline std::map<std::string, std::string> queryValueToMap(const std::string & sSql) {
        std::map<std::string, std::string> r;
        std::vector<std::vector<std::string> > rows;
        std::vector<std::string> columns;
        loadSqlImpl(sSql, rows, &columns, 1);
        if (rows.size() > 0)
        {
            const std::vector<std::string> & row = rows.at(0);
            if (row.size() > 0 && row.size() == columns.size())
            {
                for (size_t i = 0; i < columns.size(); ++i)
                {
                    r[columns.at(i)] = row.at(i);
                }
                return r;
            }
        }
        return r;
    }

    inline std::vector<std::map<std::string, std::string> > queryValueToMapVector(const std::string & sSql)
    {
        std::vector<std::map<std::string, std::string> > v;
        std::map<std::string, std::string> r;
        std::vector<std::vector<std::string> > rows;
        std::vector<std::string> columns;

        loadSqlImpl(sSql, rows, &columns);
        if (rows.size() > 0)
        {
            for(size_t j = 0;j<rows.size();++j)
            {
                const std::vector<std::string> & row = rows.at(j);
                if (row.size() > 0 && row.size() == columns.size())
                {
                    r.clear();
                    for (size_t i = 0; i < columns.size(); ++i)
                    {
                        r[columns.at(i)] = row.at(i);
                    }
                    v.push_back(r);
                }
            }
        }
        return v;
    }

    inline std::vector<std::string> queryValueToList(const std::string & sSql) {
        std::vector<std::vector<std::string> > rows;
        loadSqlImpl(sSql, rows, NULL, 1);
        if (rows.size() > 0)
        {
            return rows.at(0);
        }
        return std::vector<std::string>();
    }

protected:
    inline void setLastError(int iErrorId, const std::string & sErrorString) {
        _lastErrorId = iErrorId;
        _lastErrorString = sErrorString;
    }

protected:
    virtual bool openDatabaseImpl(const std::string & sDatabase,  bool bCreate, const std::map<std::string, std::string> * oParams) = 0;

    virtual void closeDatabaseImpl() = 0;

    virtual bool isOpenImpl() const = 0;

    virtual bool createTableImpl(const std::string & sTableName, const std::map<std::string, std::string> & columns) = 0;

    virtual bool alterPrimaryKeyAddImpl(const std::string & sTableName, const std::vector<std::string> & columns) = 0;

    virtual int loadTableImpl(const std::string & sTableName, std::vector<std::vector<std::string> > & rows, std::vector<std::string> * oColumnNames = NULL, int iMaxRowCount = -1) = 0;

    virtual int saveTableImpl(const std::string & sTableName, const std::vector<std::string> & columnNames, const std::vector<std::vector<std::string> > & rows, const std::vector<int> & columnTypes = std::vector<int>(), bool beginTransaction = false) = 0;

    virtual int saveTableImpl(const std::string & sSql, const std::vector<std::vector<std::string> > & rows, const std::vector<int> * oColumnTypes = NULL, bool beginTransaction = false) = 0;

    virtual int execSqlImpl(const std::string & sSql) = 0;

    virtual int execSqlListImpl(const std::vector<std::string> & sqlList) = 0;

    virtual int loadSqlImpl(const std::string & sSql, std::vector<std::vector<std::string> > & rows, std::vector<std::string> * oColumnNames = NULL, int iMaxRowCount = -1) = 0;

private:
    int _lastErrorId;
    std::string _lastErrorString;
    std::string _connectSource;
    std::map<std::string, std::string> _connectParams;

    friend class CxDatabaseManager;

};


typedef bool (*fn_isMyDatabase_t)(const std::string & sConnectSource);
typedef CxDatabase * (*fn_createDatabase_t)(void);


class CxDatabaseManager
{
public:
    static CxDatabase * createDatabase(const std::string & sConnectSource);

    static CxDatabase * findDb(const std::string & sSource);

    static CxDatabase * getDefaultDb();

    static void start( );

    static void stop( );

//    inline static int openDatabase(const std::string & sConnectSource) {
//        return openDatabase(sConnectSource, std::string(), true);
//    }

//    static int openDatabase(const std::string & sConnectSource, const std::string & sConnectParams, bool bCreate = true);

//    static int openDatabase(const std::string & sConnectSource, const std::map<std::string, std::string> & sConnectParams, bool bCreate = true);

    static void closeDatabase(const std::string & sConnectSource);

    static const std::map<std::string, CxDatabase* > & dbs();

    static void registDatabaseConstructor(fn_isMyDatabase_t fn_isMyDatabase, fn_createDatabase_t fn_createDatabase);

};


template<typename T1>
class CxTableTemple1
{
public:



};

template<typename T1, typename T2>
class CxTableTemple2
{
public:
    struct DataStruct{
        T1 f1;
        T2 f2;
    };

    std::vector<DataStruct> load();

    std::vector<DataStruct> load(const std::string & sTableName);

    int save(const std::vector<DataStruct> & datas);

};

template<typename T1, typename T2, typename T3>
class CxTableTemple3
{
public:
    struct DataStruct{
        T1 f1;
        T2 f2;
        T3 f3;
    };

    std::vector<DataStruct> load();

    std::vector<DataStruct> load(const std::string & sTableName);

    int save(const std::vector<DataStruct> & datas);

};

class CxDbDriverBase
{

};

class CxDbDriverSqlite : public CxDbDriverBase
{

};

class CxDbDriverOtl : public CxDbDriverBase
{

};

class CxField
{
public:
    CxField() {}

    const std::string & fieldName() const;

    int valueType();

    int toInt();

    bool toBoolean();

};

class CxRow
{
public:
};

class CxTable
{
public:
    const std::vector<CxField> & fields() const;

    void * s;
};

class CxDb{

};

#endif // CXDATABASE_H
