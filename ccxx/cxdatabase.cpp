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

static map<string, CxDatabase *> f_oDatabases;
static string f_sDefaultDatabasePath;
static string f_sDefaultDatabaseSource;

map<fn_isMyDatabase_t, fn_createDatabase_t> &fn_databaseConstructors()
{
    static map<fn_isMyDatabase_t, fn_createDatabase_t> m;
    return m;
}

CxDatabaseManager *fn_oDatabaseManager()
{
#ifndef GM_PROJECT_CXAPPLICATION_NULL
    CxApplication::registStartFunction(CxDatabaseManager::start);
    CxApplication::registStopFunction(CxDatabaseManager::stop);
#endif
    static CxDatabaseManager m;
    return &m;
}
static CxDatabaseManager *f_oDatabaseManager = fn_oDatabaseManager();

CxDatabase *CxDatabase::getDefaultDb()
{
    return CxDatabaseManager::getDefaultDb();
}

CxDatabase::CxDatabase()
{
    _sqlLastSuccessTime = CxTime::currentMsepoch();
    _reconnectTime = 0;
}

CxDatabase::~CxDatabase()
{
}

bool
CxDatabase::openDatabase(const string &sConnectSource, string sConnectType, bool bCreate, const std::map<string, string> *oParams)
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

bool CxDatabase::openDatabase()
{
    bool r = false;
    if (_connectSource.size() > 0)
    {
        r = openDatabaseImpl(_connectSource, _connectType, true, &_connectParams);
    }
    return r;
}

void CxDatabase::closeDatabase()
{
    closeDatabaseImpl();
//    _connectSource.clear();
//    _connectType.clear();
}

int
CxDatabase::loadTable(const string &sTableName, std::vector<std::vector<string> > &rows, std::vector<string> *oColumnNames, int iMaxRowCount)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = loadTableImpl(sTableName, rows, oColumnNames);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = loadTableImpl(sTableName, rows, oColumnNames);
    }
    return r;
}

int
CxDatabase::saveTable(const string &sTableName, const std::vector<string> &columnNames, const std::vector<std::vector<string> > &rows, const std::vector<int> &columnTypes, bool bTransaction)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = saveTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = saveTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    }
    return r;
}

int
CxDatabase::saveTable(const string &sSql, const std::vector<std::vector<string> > &rows, const std::vector<int> *oColumnTypes, bool bTransaction)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = saveTableImpl(sSql, rows, oColumnTypes, bTransaction);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = saveTableImpl(sSql, rows, oColumnTypes, bTransaction);
    }
    return r;
}

int
CxDatabase::updateTable(const string &sTableName, const std::vector<string> &columnNames, const std::vector<std::vector<string> > &rows, const std::vector<int> &columnTypes, bool bTransaction)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = updateTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = updateTableImpl(sTableName, columnNames, rows, columnTypes, bTransaction);
    }
    return r;
}

int CxDatabase::execSql(const string &sSql)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = execSqlImpl(sSql);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = execSqlImpl(sSql);
    }
    return r;
}

int CxDatabase::execSqlList(const std::vector<string> &sqlList)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = execSqlListImpl(sqlList);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = execSqlListImpl(sqlList);
    }
    return r;
}

int CxDatabase::loadSql(const string &sSql, std::vector<std::vector<string> > &rows, std::vector<string> *oColumnNames)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = loadSqlImpl(sSql, rows, oColumnNames);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = loadSqlImpl(sSql, rows, oColumnNames);
    }
    return r;
}

int
CxDatabase::loadSql1(const string &sSql, std::vector<std::vector<string> > &rows, std::vector<string> *oColumnNames, int iMaxRowCount)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = loadSqlImpl(sSql, rows, oColumnNames, iMaxRowCount);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = loadSqlImpl(sSql, rows, oColumnNames, iMaxRowCount);
    }
    return r;
}

int
CxDatabase::loadSql2(const string &sSql, std::vector<std::vector<string> > &rows, std::vector<string> *oColumnNames, std::vector<int> *oColumnTypes)
{
    // Check open and open it
    if (!isOpen()) openDatabase();
    if (!isOpen()) return -1;
    int r = loadSql2Impl(sSql, rows, oColumnNames);
    for (int i = 0; i < 1; ++i)
    {
        if (r >= 0) return r;
        // fail, rerun after open
        setSqlLastSuccessTime(r);
        if (!isOpen()) continue;
        r = loadSql2Impl(sSql, rows, oColumnNames);
    }
    return r;
}

string CxDatabase::queryToJsonString(const string &sSql)
{
    if (sSql.empty()) return std::string();

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
        const std::vector<std::string> &row = rows.at(i);
        if (columns.size() < row.size()) continue;
        if (columnTypes.size() < row.size()) continue;
        sLine = "{";
        for (size_t j = 0; j < row.size(); ++j)
        {
            int iValueType = columnTypes.at(j);
            const string &sKey = columns.at(j);
            const string &sValue = row.at(j);
            if (iValueType == column_type_string || iValueType == column_type_datetime)
            {
                string sValue2 = CxString::replace(sValue, '\"', '\'');
                sLine += CxString::format("\"%s\":\"%s\",", sKey.c_str(), sValue2.c_str());
            }
            else if (iValueType == column_type_blob)
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

std::map<std::string, std::string> CxDatabase::loadVerticalTableObject(const std::string &sSql)
{
    map<string, string> r;
    std::vector<std::vector<std::string> > rows;
    std::vector<std::string> columnNames;
    loadSql(sSql, rows, &columnNames);
    for (int i = 0; i < rows.size(); ++i)
    {
        const vector<string> &row = rows.at(i);
        if (row.size() > 1)
        {
            break;
        }
        r[row[0]] = row[1];
    }
    return r;
}

void CxDatabase::setSqlLastSuccessTime(int rSqlExec)
{
    if (rSqlExec > 0)
    {
        _sqlLastSuccessTime = CxTime::currentMsepoch();
    }
    else
    {
        // reopen interval
        if (CxTime::milliSecondDifferToNow(_reconnectTime) > 200)
        {
            closeDatabase();
            openDatabase();
            _reconnectTime = CxTime::currentMsepoch();
            cxPrompt() << _connectSource << " [warn:re open database 2]";
        }
    }
}

CxDatabase *CxDatabaseManager::createDatabase(const string &sConnectSource, std::string sConnectType)
{
    if (sConnectSource.empty()) return NULL;
    CxDatabase *r = CxDatabaseManager::findDb(sConnectSource);
    if (r != NULL) return r;
    map<fn_isMyDatabase_t, fn_createDatabase_t> &databaseConstructors = fn_databaseConstructors();
    for (std::map<fn_isMyDatabase_t, fn_createDatabase_t>::const_iterator it = databaseConstructors.begin();
         it != databaseConstructors.end(); ++it)
    {
        fn_isMyDatabase_t fn_isMyDatabase = it->first;
        fn_createDatabase_t fn_createDatabase = it->second;
        if (fn_isMyDatabase && fn_createDatabase && fn_isMyDatabase(sConnectSource))
        {
            CxDatabase *oDb = fn_createDatabase();
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

CxDatabase *CxDatabaseManager::findDb(const string &sSource)
{
    for (std::map<string, CxDatabase *>::const_iterator it = f_oDatabases.begin(); it != f_oDatabases.end(); ++it)
    {
        const string &sConnectSource = it->first;
        if (CxString::equalCase(sConnectSource, sSource))
        {
            return it->second;
        }
    }
    return NULL;
}

CxDatabase *CxDatabaseManager::getDefaultDb()
{
    CxDatabase *oDb = findDb(f_sDefaultDatabaseSource);
    return oDb;
}

CxDatabase *CxDatabaseManager::getDbByIndex(int index)
{
    CxDatabase *oDb = NULL;
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

void CxDatabaseManager::start()
{
    f_sDefaultDatabasePath = CxFileSystem::mergeFilePath(CxAppEnv::applicationDeployPath(), "db");

    CxDatabase *oDb = NULL;
    for (size_t i = 1; i < 30; ++i)
    {
        string sConnectSource = CxAppEnv::findConfig(CS_SectionDataBase, CxString::format("ConnectSource%d", i), std::string());
        string sConnectType = CxAppEnv::findConfig(CS_SectionDataBase, CxString::format("ConnectType%d", i), std::string());
        string sConnectParams = CxAppEnv::findConfig(CS_SectionDataBase, CxString::format("ConnectParams%d", i), std::string());
        if (sConnectSource.empty())
        {
            sConnectSource = CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultDbName"), std::string());
            if (sConnectSource.empty())
            {
                sConnectSource = CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultDatabaseName"), std::string());
            }
            if (sConnectSource.empty())
            {
                sConnectSource = CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultConnectSource"), std::string());
            }
            sConnectType = CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultConnectType"), std::string());
            sConnectParams = CxAppEnv::findConfig(CS_SectionProgramConfig, string("DefaultConnectParams"), std::string());
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
                    map<string, string> sConnectParams2 = CxString::splitToMap(sConnectParams, CxGlobal::middleChar, CxGlobal::splitChar);
                    oDb->openDatabase(sConnectSource, sConnectType, true, &sConnectParams2);
                }
            }
        }
    }
}

void CxDatabaseManager::stop()
{
    for (std::map<string, CxDatabase *>::iterator iter = f_oDatabases.begin(); iter != f_oDatabases.end();)
    {
        CxDatabase *oDb = iter->second;
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

void CxDatabaseManager::connectCheck(int iInterval)
{
    for (std::map<string, CxDatabase *>::const_iterator it = f_oDatabases.begin(); it != f_oDatabases.end(); ++it)
    {
//        const string & sConnectSource = it->first;
        CxDatabase *oDb = it->second;
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
                    oDb->_reconnectTime = CxTime::currentMsepoch();
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


//int CxDatabaseManager::openDatabase(const string &sConnectSource, const string &sConnectParams, bool bCreate)
//{
//    map<string, string> sConnectParams2 = CxString::splitToMap(sConnectParams, CxGlobal::middleChar, CxGlobal::splitChar);
//    return openDatabase(sConnectSource, sConnectParams2, bCreate);
//}

//int CxDatabaseManager::openDatabase(const string &sConnectSource, const std::map<string, string> &sConnectParams, bool bCreate)
//{
//    CxDatabase * oDb = findDb(sConnectSource);
//    if (! oDb)
//    {
//        oDb = createDatabase(sConnectSource);
//    }
//    if (oDb)
//    {
//        return oDb->openDatabase(sConnectSource, bCreate, &sConnectParams);
//    }
//    return FALSE;
//}

bool CxDatabaseManager::closeDatabase(const string &sConnectSource)
{
    for (std::map<string, CxDatabase *>::iterator iter = f_oDatabases.begin(); iter != f_oDatabases.end();)
    {
        const string &sSource = iter->first;
        if (CxString::equalCase(sSource, sConnectSource))
        {
            CxDatabase *oDb = iter->second;
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

const std::map<string, CxDatabase *> &CxDatabaseManager::dbs()
{
    return f_oDatabases;
}

void
CxDatabaseManager::registDatabaseConstructor(fn_isMyDatabase_t fn_isMyDatabase, fn_createDatabase_t fn_createDatabase)
{
    map<fn_isMyDatabase_t, fn_createDatabase_t> &databaseConstructors = fn_databaseConstructors();
    databaseConstructors[fn_isMyDatabase] = fn_createDatabase;
}

std::string CxDatabaseManager::getDefaultDatabasePath()
{
    return f_sDefaultDatabasePath;
}

