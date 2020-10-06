#include "cxdatabase.h"

#include "cxstring.h"

#include "cxfile.h"
#include "cxinterinfo.h"
#include "cxcontainer.h"
#include "cxencoding.h"
#include "cxappenv.h"

#ifndef GM_PROJECT_CXAPPLICATION_NULL
#include "cxapplication.h"
#endif

using namespace std;

static map<string, CxDatabase*> f_oDatabases;
static string f_sDefaultDatabasePath;
static string f_sDefaultDatabaseSource;

map<fn_isMyDatabase_t, fn_createDatabase_t>&
fn_databaseConstructors()
{
    static map<fn_isMyDatabase_t, fn_createDatabase_t> m;
    return m;
}

CxDatabaseManager*
fn_oDatabaseManager()
{
#ifndef GM_PROJECT_CXAPPLICATION_NULL
    CxApplication::registInitFunction(CxDatabaseManager::start);
    CxApplication::registStopFunction(CxDatabaseManager::stop);
#endif
    static CxDatabaseManager m;
    return &m;
}
static CxDatabaseManager* f_oDatabaseManager = fn_oDatabaseManager();

CxDatabase*
CxDatabase::getDefaultDb()
{
    return CxDatabaseManager::getDefaultDb();
}

CxDatabase::CxDatabase()
{
    _sqlLastSuccessTime = CxTime::currentSystemTime();
    _reconnectTime = 0;
}

CxDatabase::~CxDatabase()
{
}

bool
CxDatabase::openDatabase(const string& sConnectSource,
                         string sConnectType,
                         bool bCreate,
                         const std::map<string, string>* oParams)
{
    bool r = openDatabaseImpl(sConnectSource, sConnectType, bCreate, oParams);
    if (r)
    {
        _connectSource = sConnectSource;
        _connectType = sConnectType;
        _connectParams = *oParams;
    }
    return r;
}

bool
CxDatabase::openDatabase()
{
    bool r = false;
    if (_connectSource.size() > 0)
    {
        r = openDatabaseImpl(_connectSource, _connectType, true, &_connectParams);
    }
    return r;
}

void
CxDatabase::closeDatabase()
{
    closeDatabaseImpl();
//    _connectSource.clear();
//    _connectType.clear();
}

int
CxDatabase::loadTable(const string& sTableName,
                      std::vector<std::vector<string> >& rows,
                      std::vector<string>* oColumnNames,
                      int iMaxRowCount)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = loadTableImpl(sTableName, rows, oColumnNames);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = loadTableImpl(sTableName, rows, oColumnNames);
    }
    return r;
}

int
CxDatabase::saveTable(const string& sTableName,
                      const std::vector<string>& columnNames,
                      const std::vector<std::vector<string> >& rows,
                      const std::vector<int>& columnTypes,
                      bool bTransaction)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = saveTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = saveTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    }
    return r;
}

int
CxDatabase::saveTable(const string& sSql,
                      const std::vector<std::vector<string> >& rows,
                      const std::vector<int>* oColumnTypes,
                      bool bTransaction)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = saveTableImpl(sSql, rows, oColumnTypes, bTransaction);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = saveTableImpl(sSql, rows, oColumnTypes, bTransaction);
    }
    return r;
}

int
CxDatabase::updateTable(const string& sTableName,
                        const std::vector<string>& columnNames,
                        const std::vector<std::vector<string> >& rows,
                        const std::vector<int>& columnTypes,
                        bool bTransaction)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = updateTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = updateTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    }
    return r;
}

int
CxDatabase::execSql(const string& sSql)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = execSqlImpl(sSql);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = execSqlImpl(sSql);
    }
    return r;
}

int
CxDatabase::execSqlList(const std::vector<string>& sqlList)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = execSqlListImpl(sqlList);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = execSqlListImpl(sqlList);
    }
    return r;
}

int
CxDatabase::loadSql(const string& sSql, std::vector<std::vector<string> >& rows, std::vector<string>* oColumnNames)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = loadSqlImpl(sSql, rows, oColumnNames);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = loadSqlImpl(sSql, rows, oColumnNames);
    }
    return r;
}

int
CxDatabase::loadSql1(const string& sSql,
                     std::vector<std::vector<string> >& rows,
                     std::vector<string>* oColumnNames,
                     int iMaxRowCount)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = loadSqlImpl(sSql, rows, oColumnNames, iMaxRowCount);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = loadSqlImpl(sSql, rows, oColumnNames, iMaxRowCount);
    }
    return r;
}

int
CxDatabase::loadSql2(const string& sSql,
                     std::vector<std::vector<string> >& rows,
                     std::vector<string>* oColumnNames,
                     std::vector<int>* oColumnTypes)
{
    // Check open and open it
    if (!isOpen())
        openDatabase();
    if (!isOpen())
        return -1;
    int r = loadSql2Impl(sSql, rows, oColumnNames);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0)
            return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen())
            continue;
        r = loadSql2Impl(sSql, rows, oColumnNames);
    }
    return r;
}

string
CxDatabase::queryToJsonString(const string& sSql)
{
    if (sSql.empty())
        return std::string();

    vector<string> sLines;
    string sLine = "[";
    sLines.push_back(sLine);

    std::vector<std::map<std::string, std::string> > v;
    std::vector<std::vector<std::string> > rows;
    std::vector<std::string> columns;
    std::vector<int> columnTypes;

    loadSql2Impl(sSql, rows, &columns, &columnTypes);

    for (size_t i = 0; i < rows.size(); ++i)
    {
        const std::vector<std::string>& row = rows.at(i);
        if (columns.size() < row.size())
            continue;
        if (columnTypes.size() < row.size())
            continue;
        sLine = "{";
        for (size_t j = 0; j < row.size(); ++j)
        {
            int iValueType = columnTypes.at(j);
            const string& sKey = columns.at(j);
            const string& sValue = row.at(j);
            if (iValueType == ColumnTypeString || iValueType == ColumnTypeDatetime)
            {
                string sValue2 = CxString::replace(sValue, '\"', '\'');
                sLine += CxString::format("\"%s\":\"%s\",", sKey.c_str(), sValue2.c_str());
            }
            else if (iValueType == ColumnTypeBlob)
            {
                string sValue2 = CxEncoding::base64Encode(sValue);
                sLine += CxString::format("\"%s\":\"%s\",", sKey.c_str(), sValue2.c_str());
            }
            else
            {
                if (sValue.size() > 0)
                {
                    sLine += CxString::format("\"%s\":%s,", sKey.c_str(), sValue.c_str());
                }
                else
                {
                    sLine += CxString::format("\"%s\":null,", sKey.c_str());
                }
            }
        }
        if (sLine.size() > 1)
        {
            sLine[sLine.size() - 1] = '}';
            if (i < rows.size() - 1)
                sLine.push_back(',');
            sLines.push_back(sLine);
        }
    }
    sLine = "]";
    sLines.push_back(sLine);

    return CxString::join(sLines, 0);
}

std::map<std::string, std::string>
CxDatabase::loadVerticalTableObject(const std::string& sSql)
{
    map<string, string> r;
    std::vector<std::vector<std::string> > rows;
    std::vector<std::string> columnNames;
    loadSql(sSql, rows, &columnNames);
    for (int i = 0; i < rows.size(); ++i)
    {
        const vector<string>& row = rows.at(i);
        if (row.size() < 2)
        {
            break;
        }
        r[row[0]] = row[1];
    }
    return r;
}

void
CxDatabase::setSqlLastSuccessTime(int rSqlExec)
{
    if (rSqlExec > 0)
    {
        _sqlLastSuccessTime = CxTime::currentSystemTime();
    }
    else
    {
        // reopen interval
        if (CxTime::milliSecondDifferToNow(_reconnectTime) > 200)
        {
            closeDatabase();
            openDatabase();
            _reconnectTime = CxTime::currentSystemTime();
            cxPrompt() << _connectSource << " [warn:re open database 2]";
        }
    }
}

std::string
CxDatabase::queryValue(const std::string& sSql)
{
    std::vector<std::vector<std::string> > rows;
    loadSqlImpl(sSql, rows, NULL, 1);
    if (rows.size())
    {
        const std::vector<std::string>& row = rows.at(0);
        if (row.size() > 0)
        {
            return row.at(0);
        }
    }
    return std::string();
}

bool
CxDatabase::queryValue(const std::string& sSql, std::string& sValue)
{
    std::vector<std::vector<std::string> > rows;
    loadSql1(sSql, rows, NULL, 1);
    if (rows.size())
    {
        const std::vector<std::string>& row = rows.at(0);
        if (row.size() > 0)
        {
            sValue = row.at(0);
            return true;
        }
    }
    return false;
}

std::vector<std::string>
CxDatabase::queryValueToList(const std::string& sSql)
{
    std::vector<std::vector<std::string> > rows;
    loadSql1(sSql, rows, NULL, 1);
    if (rows.size() > 0)
    {
        return rows.at(0);
    }
    return std::vector<std::string>();
}

std::map<std::string, std::string>
CxDatabase::queryValueToMap(const std::string& sSql)
{
    std::map<std::string, std::string> r;
    std::vector<std::vector<std::string> > rows;
    std::vector<std::string> columns;
    loadSql1(sSql, rows, &columns, 1);
    if (rows.size() > 0)
    {
        const std::vector<std::string>& row = rows.at(0);
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

std::vector<std::map<std::string, std::string> >
CxDatabase::queryToMapVector(const std::string& sSql)
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
            const std::vector<std::string>& row = rows.at(j);
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

CxDatabase::CursorBase*
CxDatabase::cursorLoad(const std::string& sSql, int iPrefetchArraySize)
{
    if (_cursors.size() > 100)
    {
        msepoch_t dtNow = CxTime::currentSystemTime();
        cxPrompt() << "DB[" << _connectSource << "] cursor count > 100, clear and delete all cursor";
        for (int i = 0; i < _cursors.size(); ++i)
        {
            CursorBase* oCursor = _cursors.at(i);
            if (cursorCloseImpl(oCursor) != FALSE)
            {
                delete oCursor;
            }
        }
        _cursors.clear();
        cxPrompt() << "cursor clear complete, cost time : " << CxTime::milliSecondDifferToNow(dtNow);
    }
    CursorBase* r = cursorLoadImpl(sSql, iPrefetchArraySize);
    if (r != NULL)
    {
        _cursors.push_back(r);
    }
    return r;
}

int
CxDatabase::cursorClose(CxDatabase::CursorBase* oCursor)
{
    int r = FALSE;
    if (oCursor && CxContainer::contain(_cursors, oCursor))
    {
        r = cursorCloseImpl(oCursor);
        if (r != FALSE)
        {
            CxContainer::remove(_cursors, oCursor);
            delete oCursor;
        }
    }
    return r;
}

int
CxDatabase::cursorPut(CxDatabase::CursorBase* oCursor, std::vector<std::vector<std::string> >& rows, int iMaxRowCount)
{
    if (std::find(_cursors.begin(), _cursors.end(), oCursor) != _cursors.end())
    {
        return cursorPutImpl(oCursor, rows, iMaxRowCount);
    }
    return 0;
}

std::vector<std::string>
CxDatabase::cursorNext(CxDatabase::CursorBase* oCursor)
{
    std::vector<std::vector<std::string> > rows;
    if (cursorPutImpl(oCursor, rows, 1) > 0)
    {
        return rows.at(0);
    }
    return std::vector<std::string>();
}

bool
CxDatabase::cursorIsEnd(CxDatabase::CursorBase* oCursor)
{
    if (std::find(_cursors.begin(), _cursors.end(), oCursor) != _cursors.end())
    {
        return cursorIsEndImpl(oCursor);
    }
    return true;
}

CxDatabase*
CxDatabaseManager::createDatabase(const string& sConnectSource, std::string sConnectType)
{
    if (sConnectSource.empty())
        return NULL;
    CxDatabase* r = CxDatabaseManager::findDb(sConnectSource);
    if (r != NULL)
        return r;
    map<fn_isMyDatabase_t, fn_createDatabase_t>& databaseConstructors = fn_databaseConstructors();
    for (std::map<fn_isMyDatabase_t, fn_createDatabase_t>::const_iterator it = databaseConstructors.begin();
         it != databaseConstructors.end(); ++it)
    {
        fn_isMyDatabase_t fn_isMyDatabase = it->first;
        fn_createDatabase_t fn_createDatabase = it->second;
        if (fn_isMyDatabase && fn_createDatabase && fn_isMyDatabase(sConnectSource))
        {
            CxDatabase* oDb = fn_createDatabase();
            oDb->_connectSource = sConnectSource;
            oDb->_connectType = sConnectType;
            f_oDatabases[sConnectSource] = oDb;
            r = oDb;
            if (f_sDefaultDatabaseSource.empty())
            {
                f_sDefaultDatabaseSource = sConnectSource;
            }
            break;
        }
    }
    return r;
}

CxDatabase*
CxDatabaseManager::findDb(const string& sSource)
{
    for (std::map<string, CxDatabase*>::const_iterator it = f_oDatabases.begin(); it != f_oDatabases.end(); ++it)
    {
        const string& sConnectSource = it->first;
        if (CxString::equalCase(sConnectSource, sSource))
        {
            return it->second;
        }
    }
    return NULL;
}

CxDatabase*
CxDatabaseManager::getDefaultDb()
{
    CxDatabase* oDb = findDb(f_sDefaultDatabaseSource);
    return oDb;
}

CxDatabase*
CxDatabaseManager::getDbByIndex(int index)
{
    CxDatabase* oDb = NULL;
    string sConnectSourceIndex = CxString::format("ConnectSource%d", index);
    string sConnectTypeIndex = CxString::format("ConnectType%d", index);
    string sConnectSource = CxAppEnv::findConfig(CS_SectionDataBase, sConnectSourceIndex, std::string());
    string sConnectType = CxAppEnv::findConfig(CS_SectionDataBase, sConnectTypeIndex, std::string());
    if (sConnectSource.size() > 0)
    {
        oDb = CxDatabaseManager::findDb(sConnectSource);
        if (!oDb)
        {
            oDb = CxDatabaseManager::createDatabase(sConnectSource, sConnectType);
            f_oDatabases[sConnectSource] = oDb;
            cxPrompt() << "Index=" << index << ";DbName=" << sConnectSource << ";createDatabase=" << (oDb != NULL);
        }
    }
    return oDb;
}

void
CxDatabaseManager::start()
{
    f_sDefaultDatabasePath = CxFileSystem::mergeFilePath(CxAppEnv::applicationDeployPath(), "db");

    CxDatabase* oDb = NULL;
    for (size_t i = 1; i < 30; ++i)
    {
        string sConnectSource =
            CxAppEnv::findConfig(CS_SectionDataBase, CxString::format("ConnectSource%d", i), std::string());
        string sConnectType =
            CxAppEnv::findConfig(CS_SectionDataBase, CxString::format("ConnectType%d", i), std::string());
        string sConnectParams =
            CxAppEnv::findConfig(CS_SectionDataBase, CxString::format("ConnectParams%d", i), std::string());
        if (sConnectSource.empty())
        {
            sConnectSource = CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultDbName"), std::string());
            if (sConnectSource.empty())
            {
                sConnectSource =
                    CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultDatabaseName"), std::string());
            }
            if (sConnectSource.empty())
            {
                sConnectSource =
                    CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultConnectSource"), std::string());
            }
            sConnectType = CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultConnectType"), std::string());
            sConnectParams =
                CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultConnectParams"), std::string());
        }
        if (sConnectSource.size() > 0)
        {
            oDb = CxDatabaseManager::findDb(sConnectSource);
            if (!oDb)
            {
                oDb = CxDatabaseManager::createDatabase(sConnectSource, sConnectType);
                cxPrompt() << "Database ConnectSource=" << sConnectSource << ";createDatabase=" << (oDb != NULL);
                if (oDb)
                {
                    //f_oDatabases[sConnectSource] = oDb;
                    string sFilePath = CxFileSystem::fullFilePath(CxAppEnv::applicationDeployPath(),
                                                                  "db\\" + sConnectSource);
                    if (CxFileSystem::isExist(sFilePath))
                        sConnectSource = sFilePath;
                    map<string, string> sConnectParams2 =
                        CxString::splitToMap(sConnectParams, CxGlobal::middleChar, CxGlobal::splitChar);
                    oDb->openDatabase(sConnectSource, sConnectType, true, &sConnectParams2);
                }
            }
        }
    }
}

void
CxDatabaseManager::stop()
{
    for (std::map<string, CxDatabase*>::iterator iter = f_oDatabases.begin(); iter != f_oDatabases.end();)
    {
        CxDatabase* oDb = iter->second;
        if (oDb)
        {
            oDb->closeDatabase();
            f_oDatabases.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

void
CxDatabaseManager::connectCheck(int iInterval)
{
    for (std::map<string, CxDatabase*>::const_iterator it = f_oDatabases.begin(); it != f_oDatabases.end(); ++it)
    {
//        const string & sConnectSource = it->first;
        CxDatabase* oDb = it->second;
        if (oDb)
        {

            if (!oDb->isOpen())
            {
                oDb->openDatabase();
            }
            // out time reopen
            if (CxTime::milliSecondDifferToNow(oDb->_sqlLastSuccessTime) > 1000 * 60 * 3)
            {
                if (oDb->isOpen())
                {
                    oDb->closeDatabase();
                    oDb->openDatabase();
                    cxPrompt() << oDb->connectSource() << " [warn:re open database 1]";
                    oDb->_reconnectTime = CxTime::currentSystemTime();
                }
            }
            //
            if (oDb->isOpen())
            {
                string sHeartJumpSql = CxContainer::value(oDb->_databaseAttrs, string("HeartJumpSql"));
                if (sHeartJumpSql.size() > 0)
                {
                    oDb->execSql(sHeartJumpSql);
                }
                else
                {
                    cxPrompt() << oDb->connectSource() << " [Error:HeartJumpSql is empty!!!]";
                }
            }
        }
    }
}

bool
CxDatabaseManager::closeDatabase(const string& sConnectSource)
{
    for (std::map<string, CxDatabase*>::iterator iter = f_oDatabases.begin(); iter != f_oDatabases.end();)
    {
        const string& sSource = iter->first;
        if (CxString::equalCase(sSource, sConnectSource))
        {
            CxDatabase* oDb = iter->second;
            if (oDb)
            {
                oDb->closeDatabase();
                f_oDatabases.erase(iter++);
                return true;
            }
        }
        else
        {
            ++iter;
        }
    }
    return false;
}

const std::map<string, CxDatabase*>&
CxDatabaseManager::dbs()
{
    return f_oDatabases;
}

void
CxDatabaseManager::registDatabaseConstructor(fn_isMyDatabase_t fn_isMyDatabase, fn_createDatabase_t fn_createDatabase)
{
    map<fn_isMyDatabase_t, fn_createDatabase_t>& databaseConstructors = fn_databaseConstructors();
    databaseConstructors[fn_isMyDatabase] = fn_createDatabase;
}

std::string
CxDatabaseManager::getDefaultDatabasePath()
{
    return f_sDefaultDatabasePath;
}

