
#include <ccxx/ccxx.h>

#include <ccxx/cxdatabase_odbc.h>

using namespace std;

CxDatabase::CursorBase * f_oCursor1 = NULL;
CxDatabase::CursorBase * f_oCursor2 = NULL;

void timerCursor1(int iInterval)
{
    static int iIndex = 0;
    ++ iIndex;
    msepoch_t dtNow = CxTime::currentSystemTime();
    CxDatabase * oDb = CxDatabaseManager::getDefaultDb();
    if (oDb == NULL)
    {
        cxPrompt() << "can not open database!";
        return;
    }

    cxPrompt() << "DB has cursor count : " << oDb->cursorGetOnlineAll().size();

    if (f_oCursor1 == NULL)
    {
        string sSql = "select * from ha_appconfig";
        f_oCursor1 = oDb->cursorLoad(sSql, 10);
    }

    if (f_oCursor2 == NULL)
    {
        string sSq2 = "select * from omc_neconfig";
        f_oCursor2 = oDb->cursorLoad(sSq2, 10);
    }

    if (! oDb->cursorIsEnd(f_oCursor1))
    {
        vector<std::vector<std::string> > rows;
        oDb->cursorPut(f_oCursor1, rows);
        if (rows.size() > 0)
        {
            vector<string> row = CxEncoding::utf8ToGb2312(rows[0]);
            cxPrompt() << "cursor1 cursorPut row count : " << rows.size()
                       << " , print line0: ";
            cxPrompt() << row;
        }
    }
    if (oDb->cursorIsEnd(f_oCursor1))
    {
        oDb->cursorClose(f_oCursor1);
        f_oCursor1 = NULL;
    }

    if (! oDb->cursorIsEnd(f_oCursor2))
    {
        vector<std::vector<std::string> > rows;
        oDb->cursorPut(f_oCursor2, rows);
        if (rows.size() > 0)
        {
            vector<string> row = CxEncoding::utf8ToGb2312(rows[0]);
            cxPrompt() << "cursor2 cursorPut row count : " << rows.size()
                       << " , print line0: ";
            cxPrompt() << row;
        }
    }
    if (oDb->cursorIsEnd(f_oCursor2))
    {
        oDb->cursorClose(f_oCursor2);
        f_oCursor2 = NULL;
    }

    cxPrompt() << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << " --- --- --- --- ---";
}

void timerCursor2(int iInterval)
{
    static int iIndex = 0;
    ++ iIndex;
    msepoch_t dtNow = CxTime::currentSystemTime();
    CxDatabase * oDb = CxDatabaseManager::getDefaultDb();
    if (oDb == NULL)
    {
        cxPrompt() << "can not open database!";
        return;
    }

    cxPrompt() << "DB has cursor count : " << oDb->cursorGetOnlineAll().size();

    string sSql = "select * from omc_performancedata";
    CxDatabase::CursorBase * oCursor = oDb->cursorLoad(sSql, 100);

    cxPrompt() << "oDb->cursorLoad(sSql, 100); " << CxTime::milliSecondDifferToNow(dtNow);

    int iCount = 0;
    while (! oDb->cursorIsEnd(oCursor))
    {
        vector<string> row = oDb->cursorNext(oCursor);
        ++iCount;
    }
    oDb->cursorClose(oCursor);

    cxPrompt() << "oDb->cursorNext(sSql).count :  " << iCount;
    cxPrompt() << "oDb->cursorNext(sSql).all : " << CxTime::milliSecondDifferToNow(dtNow);

    cxPrompt() << " --- --- --- --- ---";
}

void testCursor1()
{
    cxPrompt() << "TEST Cursor1 start : " << CxTime::currentSystemTimeString();
    CxTimerManager::startTimer(timerCursor1, 200);

    CxTimerManager::startTimer(timerCursor2, 1000);
}

void timerCursor3(int iInterval)
{
    static int iIndex = 0;
    ++ iIndex;
    msepoch_t dtNow = CxTime::currentSystemTime();
    CxDatabase * oDb = CxDatabaseManager::getDefaultDb();
    if (oDb == NULL)
    {
        cxPrompt() << "can not open database!";
        return;
    }

    cxPrompt() << "DB has cursor count : " << oDb->cursorGetOnlineAll().size();

    string sSql = "select * from t1";
    CxDatabase::CursorBase * oCursor1 = oDb->cursorLoad(sSql, 10);

    if (oCursor1)
    {
        cxPrompt() << "getColumnNames: ";
        cxPrompt() << oCursor1->getColumnNames();
        cxPrompt() << "getColumnTypes: ";
        cxPrompt() << oCursor1->getColumnTypes();
        cxPrompt() << "getColumnSizes: ";
        cxPrompt() << oCursor1->getColumnSizes();
    }
    if (! oDb->cursorIsEnd(oCursor1)){}
    {
        vector<std::vector<std::string> > rows;
        oDb->cursorPut(oCursor1, rows);
        for (int i = 0; i < rows.size(); ++i)
        {
            vector<string> row = CxEncoding::utf8ToGb2312(rows[i]);
            cxPrompt() << "cursor1 cursorPut row count : " << rows.size()
                       << " , print line" << i << " :";
            cxPrompt() << row;
        }
    }

    cxPrompt() << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << " --- --- --- --- ---";
}

void testCursor2()
{
    cxPrompt() << "TEST Cursor2 start : " << CxTime::currentSystemTimeString();
    CxTimerManager::startTimer(timerCursor3, 1000);
}

void testCursor4()
{
    static int iIndex = 0;
    ++ iIndex;
    msepoch_t dtNow = CxTime::currentSystemTime();
    CxDatabase * oDb = CxDatabaseManager::getDefaultDb();
    if (oDb == NULL)
    {
        cxPrompt() << "can not open database!";
        return;
    }

    cxPrompt() << "DB has cursor count : " << oDb->cursorGetOnlineAll().size();

    string sSql = "Select AlarmNo, NeID, AlarmType, ModuleNo, CardNo, AlarmValue, AlarmTime, Confirm, ConfirmTime, ConfirmUser from omc_alarmrec where Status=0 order by AlarmNo desc limit 0, 1000";
    CxDatabase::CursorBase * oCursor = oDb->cursorLoad(sSql, 100);

    cxPrompt() << "oDb->cursorLoad(sSql, 100); " << CxTime::milliSecondDifferToNow(dtNow);

    int iCount = 0;
    while (! oDb->cursorIsEnd(oCursor))
    {
        vector<string> row = oDb->cursorNext(oCursor);
        ++iCount;
    }
    oDb->cursorClose(oCursor);

    cxPrompt() << "oDb->cursorNext(sSql).count :  " << iCount;
    cxPrompt() << "oDb->cursorNext(sSql).all : " << CxTime::milliSecondDifferToNow(dtNow);

    cxPrompt() << " --- --- --- --- ---";
}

int fn_interinfo_in_cmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag)
{
    if (sCommand == "exit")
    {
        CxApplication::exit();
        return TRUE;
    }
    else if (sCommand == "open")
    {
        string sDsn = CxContainer::value(sParams, string("dsn"), string());
        CxDatabase * pDb = CxDatabaseManager::getDefaultDb();
        pDb->openDatabase();
    }
    else if (sCommand == "select")
    {
        string sSql = CxContainer::value(sParams, string("sql"), string());
        CxDatabase * pDb = CxDatabaseManager::getDefaultDb();
        if (pDb == NULL)
        {
            return TRUE;
        }
        int nRst = 0;
        vector<std::vector<std::string> > tRows;
        vector<std::string> tFields;
        nRst += pDb->loadSql(sSql, tRows, &tFields);
        cxPrompt() << "tRows.size() : " << tRows.size();
        for (int i = 0; i < tRows.size() && i < 10; ++i)
        {
            cxPrompt() << tRows[i];
        }
        return TRUE;
    }
    else
    {
        cxPrompt() << "in : " << sCommand;
    }
    return FALSE;
}

void fn_timer1(int)
{
//	cxPrompt() << "begin time1:";
    msepoch_t dtNow = CxTime::currentSystemTime();
    testCursor4();
}

void fn_test(int, int, const void *, int, void *, void *)
{
}

int main(int argc,const char * argv[])
{
    cout << "begin test CxDatabase-ODBC : ";

    CxApplication::init(argc, argv);

//	string g_sDBSource = "UID=FMIS9999;PWD=FMIS9999;DSN=cics_oracle_dsn";
//	string g_sDBType = "Oracle";

    string g_sDBSource = "UID=root;PWD=123456;DSN=mysql_cursor_dsn";
    string g_sDBType = "MySQL";

//	string g_sDBSource = "UID=root;PWD=123456;DSN=ics_mysql_dsn";
//	string g_sDBType = "MySQL";
//
//	string g_sDBSource = "DSN=ics_access_dsn";
//	string g_sDBType = "access";

    CxDatabase * pDb = CxDatabaseManager::createDatabase(g_sDBSource, g_sDBType);
    if (pDb == NULL)
    {
        cout << "error : createDatabase fail! end test !!!";
        return 0;
    }
    if (!pDb->openDatabase())
    {
        cout << "error : openDatabase fail! end test !!!";
        return 0;
    }

//    cout << "end test CxDatabase !!!";

    CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

    CxApplication::pushProcessCallBack(fn_test);

    CxTimerManager::startTimer(fn_timer1, 300);

    return CxApplication::exec();
}
