#include "cxdatabase.h"

#include "cxstring.h"

#include "cxapplication.h"
#include "cxfile.h"
#include "cxinterinfo.h"
#include "cxcontainer.h"

using namespace std;


map<fn_isMyDatabase_t, fn_createDatabase_t> & fn_databaseConstructors()
{
    static map<fn_isMyDatabase_t, fn_createDatabase_t> m;
    return m;
}

map<string, CxDatabase*> f_oDatabases;

CxDatabaseManager * fn_oDatabaseManager()
{
    CxApplication::registStartFunction(CxDatabaseManager::start);
    CxApplication::registStopFunction(CxDatabaseManager::stop);

    static CxDatabaseManager m;
    return & m;
}

static CxDatabaseManager * f_oDatabaseManager = fn_oDatabaseManager();



CxDatabase *CxDatabase::getDefaultDb()
{
    return CxDatabaseManager::getDefaultDb();
}

CxDatabase::CxDatabase()
{
}

CxDatabase::~CxDatabase()
{
}

bool CxDatabase::openDatabase(const string &sConnectSource, bool bCreate, const std::map<string, string> *oParams)
{
    bool r = openDatabaseImpl(sConnectSource, bCreate, oParams);
    if (r)
    {
        _connectSource = sConnectSource;
    }
    cxPrompt()<<"Database : openDatabase ConnectSource="<<sConnectSource<<";result="<<r;
    return r;
}

bool CxDatabase::openDatabase()
{
    bool r = false;
    if (_connectSource.size()>0)
    {
        r = openDatabaseImpl(_connectSource, true, & _connectParams);
    }
    cxPrompt()<<"Database : openDatabase ConnectSource="<<_connectSource<<";result="<<r;
    return r;
}

void CxDatabase::closeDatabase()
{
    closeDatabaseImpl();
    _connectSource.clear();
}


CxDatabase *CxDatabaseManager::createDatabase(const string &sConnectSource)
{
    map<fn_isMyDatabase_t, fn_createDatabase_t> & databaseConstructors = fn_databaseConstructors();
    for (std::map<fn_isMyDatabase_t, fn_createDatabase_t >::const_iterator it = databaseConstructors.begin(); it != databaseConstructors.end(); ++it)
    {
        fn_isMyDatabase_t fn_isMyDatabase = it->first;
        fn_createDatabase_t fn_createDatabase = it->second;
        if (fn_isMyDatabase && fn_createDatabase && fn_isMyDatabase(sConnectSource))
        {
            CxDatabase * oDb = fn_createDatabase();
            oDb->_connectSource = sConnectSource;
            return oDb;
        }
    }
    return NULL;
}

CxDatabase *CxDatabaseManager::findDb(const string &sSource)
{
    for (std::map<string, CxDatabase* >::const_iterator it = f_oDatabases.begin(); it != f_oDatabases.end(); ++it)
    {
        const string & sConnectSource = it->first;
        if (CxString::equalCase(sConnectSource, sSource))
        {
            return it->second;
        }
    }
    return NULL;
}

CxDatabase *CxDatabaseManager::getDefaultDb()
{
    static CxDatabase * oDb = NULL;
    if (! oDb)
    {
        string sConnectSource = CxApplication::findConfig(CS_SectionProgramConfig, string("DefaultDbName"), std::string());
        if (sConnectSource.empty())
        {
            sConnectSource = CxApplication::findConfig(CS_SectionDataBase, string("ConnectSource1"), std::string());
        }
        if (sConnectSource.size()>0)
        {
            oDb = findDb(sConnectSource);
            if (! oDb)
            {
                oDb = CxDatabaseManager::createDatabase(sConnectSource);
                f_oDatabases[sConnectSource] = oDb;
                cxPrompt()<<"DefaultDbName="<<sConnectSource<<";createDatabase="<<(oDb!=NULL);
            }
        }
    }
    return oDb;
}

void CxDatabaseManager::start()
{
    for (size_t i = 1; i < 30; ++i)
    {
        string sConnectSource = CxApplication::findConfig(CS_SectionDataBase, CxString::format("ConnectSource%d",i), std::string());
        string sConnectParams = CxApplication::findConfig(CS_SectionDataBase, CxString::format("ConnectParams%d",i), std::string());
        if (sConnectSource.size()>0)
        {
            CxDatabase * oDb = findDb(sConnectSource);
            if (! oDb)
            {
                CxDatabase * oDb = CxDatabaseManager::createDatabase(sConnectSource);
                cxPrompt()<<"Database ConnectSource="<<sConnectSource<<";createDatabase="<<(oDb!=NULL);
                if (oDb)
                {
                    f_oDatabases[sConnectSource] = oDb;
                    if (oDb)
                    {
                        string sFilePath = CxFileSystem::fullFilePath(CxApplication::applicationDeployPath(), "db\\"+sConnectSource);
                        if (CxFileSystem::isExist(sFilePath))
                            sConnectSource = sFilePath;
                        map<string, string> sConnectParams2 = CxString::splitToMap(sConnectParams, CxGlobal::middleChar, CxGlobal::splitChar);
                        oDb->openDatabase(sConnectSource, true, &sConnectParams2);
                    }
                }
            }
        }
    }

    CxDatabase * oDb = getDefaultDb();
    if (oDb && ! oDb->isOpen())
    {
        string sDefaultDatabase = CxApplication::findConfig(CS_SectionProgramConfig, string("DefaultDbName"), std::string());
        if (sDefaultDatabase.size()>0)
        {
            oDb->openDatabase(CxFileSystem::mergeFilePath(CxApplication::applicationDeployPath(), "db\\"+sDefaultDatabase));
        }
    }
}

void CxDatabaseManager::stop()
{
    for (std::map<string, CxDatabase* >::const_iterator it = f_oDatabases.begin(); it != f_oDatabases.end(); ++it)
    {
//        const string & sConnectSource = it->first;
        CxDatabase * oDb = it->second;
        if (oDb)
        {
            oDb->closeDatabase();
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

void CxDatabaseManager::closeDatabase(const string &sConnectSource)
{
    CxDatabase * oDb = findDb(sConnectSource);
    if (oDb)
    {
        oDb->closeDatabase();
    }
}

const std::map<string, CxDatabase *> &CxDatabaseManager::dbs()
{
    return f_oDatabases;
}

void CxDatabaseManager::registDatabaseConstructor(fn_isMyDatabase_t fn_isMyDatabase, fn_createDatabase_t fn_createDatabase)
{
    map<fn_isMyDatabase_t, fn_createDatabase_t> & databaseConstructors = fn_databaseConstructors();
    databaseConstructors[fn_isMyDatabase] = fn_createDatabase;
}

