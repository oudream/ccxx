#ifndef CXDATABASE_H
#define CXDATABASE_H

#include "cxglobal.h"

static const std::string CS_SectionDataBase = "DataBase";

class GM_CCXX_CORE_API CxDatabase {
public:
    typedef enum {
        ColumnTypeNone = 0,
        ColumnTypeBoolean,
        ColumnTypeInt,
        ColumnTypeLongint,
        ColumnTypeDouble,
        ColumnTypeDatetime,
        ColumnTypeString,
        ColumnTypeBlob,
        ColumnTypeRes1,
        ColumnTypeRes2,
        ColumnTypeRes3,
        ColumnTypeRes4,
        ColumnTypeRes5
    } ColumnTypeEnum;

    typedef enum {
        CursorTypeNone = 0,
        CursorTypeOdbc,
        CursorTypeSqlite,
        CursorTypeRes1,
        CursorTypeRes2,
        CursorTypeRes3,
        CursorTypeRes4,
        CursorTypeRes5
    } CursorTypeEnum;

    class CursorBase {
    public:
        CursorBase(CursorTypeEnum eCursorType=CursorTypeNone, int iPrefetchArraySize=0):_cursorType(eCursorType),_prefetchArraySize(iPrefetchArraySize){}
        virtual ~CursorBase(){}
        inline CursorTypeEnum getCursorType() { return _cursorType; }
        inline int getPrefetchArraySize() { return _prefetchArraySize; }
        inline const std::vector<std::string> & getColumnNames() { return _columnNames; };
        inline const std::vector<int> & getColumnTypes() { return _columnTypes; };
        inline const std::vector<int> & getColumnSizes() { return _columnSizes; };
    protected:
        CursorTypeEnum _cursorType;
        int _prefetchArraySize;
        std::vector<std::string> _columnNames;
        std::vector<int> _columnTypes;
        std::vector<int> _columnSizes;

        friend class CxDatabaseOdbc;
        friend class CxDatabaseSqlite;
    };

public:
    static CxDatabase *getDefaultDb();

    CxDatabase();

    virtual ~CxDatabase();

    inline const std::string &lastErrorString() const { return _lastErrorString; }

    inline int lastErrorId() const { return _lastErrorId; }

    inline const std::string &connectSource() const { return _connectSource; }

    inline const std::string &connectType() const { return _connectType; }

    inline const std::map<std::string, std::string> &connectParams() const { return _connectParams; }

    inline const std::map<std::string, std::string> &databaseAttrs() const { return _databaseAttrs; }

    inline bool isOpen() const { return isOpenImpl(); }

    bool openDatabase(const std::string &sConnectSource, std::string sConnectType = "", bool bCreate = true, const std::map<std::string, std::string> *oParams = NULL);

    bool openDatabase();

    void closeDatabase();

    // ### DDL
    inline bool createTable(const std::string &sTableName, const std::map<std::string, std::string> &columns) { return createTableImpl(sTableName, columns); }

    inline bool alterPrimaryKeyAdd(const std::string &sTableName, const std::vector<std::string> &columns) { return alterPrimaryKeyAddImpl(sTableName, columns); }

    // ### load table
    int loadTable(const std::string &sTableName, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1);

    // ### load data by sql
    int loadSql(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL);

    int loadSql1(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1);

    int loadSql2(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, std::vector<int> *oColumnTypes = NULL);

    // get one value data
    std::string queryValue(const std::string &sSql);

    // get one value data
    bool queryValue(const std::string &sSql, std::string &sValue);

    // get one row data
    std::vector<std::string> queryValueToList(const std::string &sSql);

    // get one row data
    std::map<std::string, std::string> queryValueToMap(const std::string &sSql);

    // get mult row key=value data
    std::vector<std::map<std::string, std::string> > queryToMapVector(const std::string &sSql);

    std::string queryToJsonString(const std::string &sSql);

    std::map<std::string, std::string> loadVerticalTableObject(const std::string & sSql);

    // ### update table
    int saveTable(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool bTransaction = true);

    int saveTable(const std::string &sSql, const std::vector<std::vector<std::string> > &rows, const std::vector<int> *oColumnTypes = NULL, bool bTransaction = false);

    int updateTable(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool bTransaction = true);

    // ### exec sql
    int execSql(const std::string &sSql);

    int execSqlList(const std::vector<std::string> &sqlList);

    // ### GetDb
    inline void * getDb() { return getDbImpl(); }

    // ### Cursor
    CursorBase * cursorLoad(const std::string &sSql, int iPrefetchArraySize = 100);

    bool cursorIsEnd(CursorBase * oCursor);

    std::vector<std::string> cursorNext(CursorBase * oCursor);

    int cursorPut(CursorBase * oCursor, std::vector<std::vector<std::string> > &rows, int iMaxRowCount = -1);

    int cursorClose(CursorBase * oCursor);

    inline std::vector<CursorBase *> cursorGetOnlineAll() { return _cursors; }

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

    virtual void
    closeDatabaseImpl() = 0;

    virtual bool
    isOpenImpl() const = 0;

    virtual bool
    createTableImpl(const std::string &sTableName, const std::map<std::string, std::string> &columns) = 0;

    virtual bool
    alterPrimaryKeyAddImpl(const std::string &sTableName, const std::vector<std::string> &columns) = 0;

    virtual int
    loadTableImpl(const std::string &sTableName, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1) = 0;

    virtual int
    saveTableImpl(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool beginTransaction = false) = 0;

    virtual int
    saveTableImpl(const std::string &sSql, const std::vector<std::vector<std::string> > &rows, const std::vector<int> *oColumnTypes = NULL, bool beginTransaction = false) = 0;

    virtual int
    updateTableImpl(const std::string &sTableName, const std::vector<std::string> &columnNames, const std::vector<std::vector<std::string> > &rows, const std::vector<int> &columnTypes = std::vector<int>(), bool beginTransaction = false) = 0;

    virtual int
    execSqlImpl(const std::string &sSql) = 0;

    virtual int
    execSqlListImpl(const std::vector<std::string> &sqlList) = 0;

    virtual int
    loadSqlImpl(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, int iMaxRowCount = -1) = 0;

    virtual int
    loadSql2Impl(const std::string &sSql, std::vector<std::vector<std::string> > &rows, std::vector<std::string> *oColumnNames = NULL, std::vector<int> *oColumnTypes = NULL, int iMaxRowCount = -1) = 0;

    virtual void *
    getDbImpl() = 0;

    virtual CursorBase *
    cursorLoadImpl(const std::string &sSql, int iPrefetchArraySize) = 0;

    virtual bool
    cursorIsEndImpl(CursorBase * oCursor) = 0;

    virtual int
    cursorPutImpl(CursorBase * oCursor, std::vector<std::vector<std::string> > &rows, int iMaxRowCount) = 0;

    virtual int
    cursorCloseImpl(CursorBase * oCursor) = 0;

protected:
    std::map<std::string, std::string> _databaseAttrs;
    long long _sqlLastSuccessTime;
    long long _reconnectTime;

protected:
    int _lastErrorId;
    std::string _lastErrorString;
    std::string _connectSource;
    std::string _connectType;
    std::map<std::string, std::string> _connectParams;
    std::vector<CursorBase *> _cursors;

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
