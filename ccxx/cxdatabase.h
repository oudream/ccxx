#ifndef CXDATABASE_H
#define CXDATABASE_H

#include "cxglobal.h"

static const std::string CS_SectionDataBase = "DataBase";

class GM_CCXX_CORE_API CxDatabase {
public:
    enum column_type_enum {
        column_type_none = 0,
        column_type_boolean,
        column_type_int,
        column_type_longint,
        column_type_double,
        column_type_datetime,
        column_type_string,
        column_type_blob,
        column_type_res1,
        column_type_res2,
        column_type_res3,
        column_type_res4,
        column_type_res5
    };

public:
    static CxDatabase *getDefaultDb();

    CxDatabase();

    virtual ~CxDatabase();

    inline const std::string &lastErrorString() const
    {
        return _lastErrorString;
    }

    inline int lastErrorId() const
    {
        return _lastErrorId;
    }

    inline const std::string &connectSource() const
    {
        return _connectSource;
    }

    inline const std::string &connectType() const
    {
        return _connectType;
    }

    inline const std::map<std::string, std::string> &connectParams() const
    {
        return _connectParams;
    }

    inline const std::map<std::string, std::string> &databaseAttrs() const
    {
        return _databaseAttrs;
    }

    inline bool isOpen() const
    {
        return isOpenImpl();
    }

    bool
    openDatabase(const std::string &sConnectSource, std::string sConnectType = "", bool bCreate = true, const std::map<std::string, std::string> *oParams = NULL);

    bool openDatabase();

    void closeDatabase();

    inline bool createTable(const std::string &sTableName, const std::map<std::string, std::string> &columns)
    {
        return createTableImpl(sTableName, columns);
    }

    inline bool alterPrimaryKeyAdd(const std::string &sTableName, const std::vector<std::string> &columns)
    {
        return alterPrimaryKeyAddImpl(sTableName, columns);
    }

    int
    loadTable(const std::string &sTableName, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1);

    int
    saveTable(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool bTransaction = true);

    int
    saveTable(const std::string &sSql, const std::vector<std::vector<std::string> > &rows, const std::vector<int> *oColumnTypes = NULL, bool bTransaction = false);

    int
    updateTable(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool bTransaction = true);

    int execSql(const std::string &sSql);

//    inline int callProc()

    int execSqlList(const std::vector<std::string> &sqlList);

    int
    loadSql(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL);

    int
    loadSql1(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1);

    int
    loadSql2(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, std::vector<int> *oColumnTypes = NULL);

    // get one value data
    inline std::string queryValue(const std::string &sSql)
    {
        std::vector<std::vector<std::string> > rows;
        loadSqlImpl(sSql, rows, NULL, 1);
        if (rows.size())
        {
            const std::vector<std::string> &row = rows.at(0);
            if (row.size() > 0)
            {
                return row.at(0);
            }
        }
        return std::string();
    }

    // get one value data
    inline bool queryValue(const std::string &sSql, std::string &sValue)
    {
        std::vector<std::vector<std::string> > rows;
        loadSql1(sSql, rows, NULL, 1);
        if (rows.size())
        {
            const std::vector<std::string> &row = rows.at(0);
            if (row.size() > 0)
            {
                sValue = row.at(0);
                return true;
            }
        }
        return false;
    }

    // get one row data
    inline std::vector<std::string> queryValueToList(const std::string &sSql)
    {
        std::vector<std::vector<std::string> > rows;
        loadSql1(sSql, rows, NULL, 1);
        if (rows.size() > 0)
        {
            return rows.at(0);
        }
        return std::vector<std::string>();
    }

    // get one row data
    inline std::map<std::string, std::string> queryValueToMap(const std::string &sSql)
    {
        std::map<std::string, std::string> r;
        std::vector<std::vector<std::string> > rows;
        std::vector<std::string> columns;
        loadSql1(sSql, rows, &columns, 1);
        if (rows.size() > 0)
        {
            const std::vector<std::string> &row = rows.at(0);
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

    // get mult row key=value data
    inline std::vector<std::map<std::string, std::string> > queryToMapVector(const std::string &sSql)
    {
        std::vector<std::map<std::string, std::string> > v;
        std::map<std::string, std::string> r;
        std::vector<std::vector<std::string> > rows;
        std::vector<std::string> columns;

        loadSql(sSql, rows, &columns);
        if (rows.size() > 0)
        {
            for (size_t j = 0; j < rows.size(); ++j)
            {
                const std::vector<std::string> &row = rows.at(j);
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

    std::string queryToJsonString(const std::string &sSql);

    std::map<std::string, std::string> loadVerticalTableObject(const std::string & sSql);

    inline void * getDb() { return getDbImpl(); }

protected:
    inline void setLastError(int iErrorId, const std::string &sErrorString)
    {
        _lastErrorId = iErrorId;
        _lastErrorString = sErrorString;
    }

    void setSqlLastSuccessTime(int rSqlExec);

protected:
    virtual bool
    openDatabaseImpl(const std::string &sDatabase, const std::string &sDatabaseType, bool bCreate, const std::map<std::string, std::string> *oParams) = 0;

    virtual void closeDatabaseImpl() = 0;

    virtual bool isOpenImpl() const = 0;

    virtual bool createTableImpl(const std::string &sTableName, const std::map<std::string, std::string> &columns) = 0;

    virtual bool alterPrimaryKeyAddImpl(const std::string &sTableName, const std::vector<std::string> &columns) = 0;

    virtual int
    loadTableImpl(const std::string &sTableName, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1) = 0;

    virtual int
    saveTableImpl(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool beginTransaction = false) = 0;

    virtual int
    saveTableImpl(const std::string &sSql, const std::vector<std::vector<std::string> > &rows, const std::vector<int> *oColumnTypes = NULL, bool beginTransaction = false) = 0;

    virtual int
    updateTableImpl(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool beginTransaction = false) = 0;

    virtual int execSqlImpl(const std::string &sSql) = 0;

    virtual int execSqlListImpl(const std::vector<std::string> &sqlList) = 0;

    virtual int
    loadSqlImpl(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1) = 0;

    virtual int
    loadSql2Impl(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, std::vector<int> *oColumnTypes = NULL, int iMaxRowCount = -1) = 0;

    virtual void * getDbImpl() = 0;

protected:
    std::map<std::string, std::string> _databaseAttrs;
    long long _sqlLastSuccessTime;
    long long _reconnectTime;

private:
    int _lastErrorId;
    std::string _lastErrorString;
    std::string _connectSource;
    std::string _connectType;
    std::map<std::string, std::string> _connectParams;

    friend class CxDatabaseManager;

};

typedef bool (*fn_isMyDatabase_t)(const std::string &sConnectSource);
typedef CxDatabase *(*fn_createDatabase_t)(void);

class GM_CCXX_CORE_API CxDatabaseManager {
public:
    static CxDatabase *createDatabase(const std::string &sConnectSource, std::string sConnectType = "");

    static CxDatabase *findDb(const std::string &sSource);

    static CxDatabase *getDefaultDb();
    
    static std::string getDefaultDatabasePath();
    
    static CxDatabase *getDbByIndex(int index);

    static void start();

    static void stop();

    static bool closeDatabase(const std::string &sConnectSource);

    static const std::map<std::string, CxDatabase *> &dbs();

    static void registDatabaseConstructor(fn_isMyDatabase_t fn_isMyDatabase, fn_createDatabase_t fn_createDatabase);

    static void connectCheck(int iInterval);
    
};

template<typename T1>
class CxTableTemple1 {
public:

};

template<typename T1, typename T2>
class CxTableTemple2 {
public:
    struct DataStruct {
        T1 f1;
        T2 f2;
    };

    std::vector<DataStruct> load();

    std::vector<DataStruct> load(const std::string &sTableName);

    int save(const std::vector<DataStruct> &datas);

};

template<typename T1, typename T2, typename T3>
class CxTableTemple3 {
public:
    struct DataStruct {
        T1 f1;
        T2 f2;
        T3 f3;
    };

    std::vector<DataStruct> load();

    std::vector<DataStruct> load(const std::string &sTableName);

    int save(const std::vector<DataStruct> &datas);

};

class CxDbDriverBase {

};

class CxDbDriverSqlite : public CxDbDriverBase {

};

class CxDbDriverOtl : public CxDbDriverBase {

};

class CxField {
public:
    CxField()
    {}

    const std::string &fieldName() const;

    int valueType();

    int toInt();

    bool toBoolean();

};

class CxRow {
public:
};

class CxTable {
public:
    const std::vector<CxField> &fields() const;

    void *s;
};

class CxDb {

};

#endif // CXDATABASE_H
