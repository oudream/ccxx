
#include <ccxx/ccxx.h>

using namespace std;


void testDql1()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    string sSql = "Select AlarmNo, NeID, AlarmType, ModuleNo, CardNo, AlarmValue, \
			AlarmTime, Status, ResumeTime, Confirm, ConfirmTime, ConfirmUser, \
			Eliminate, EliminateTime, EliminateUser from omc_alarmrec where Status=1 order by AlarmNo limit 0, 3000";

    if (pDb == NULL)
    {
        return;
    }
    int nRst = 0;
    while (1)
    {
        vector<std::vector<std::string> > tRows;
        vector<std::string> tFields;
        nRst += pDb->loadSql(sSql, tRows, &tFields);
        if (nRst < 0)
        {
            return;
        }
        cout << nRst << endl;
    }
}

void testDql2()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

//	string sSql = "select * from omc_neconfig";
    //string sSql = "select * from Table1";
    string sSql = "SELECT omc_neconfig.NeNo AS id , omc_neconfig.NeParentNo AS pId , omc_neconfig.NeAlias AS name FROM omc_neconfig;";

    if (pDb == NULL)
    {
        return;
    }
    int nRst = 0;
    while (1)
    {
        vector<std::vector<std::string> > tRows;
        vector<std::string> tFields;
        nRst += pDb->loadSql(sSql, tRows, &tFields);
        if (nRst < 0)
        {
            return;
        }
        cout << nRst << endl;
        break;
    }
}

void testDql3()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

//	string sSql = "select * from omc_neconfig";
    //string sSql = "select * from Table1";
    string sSql = "select * from table1;";

    if (pDb == NULL)
    {
        return;
    }
    int nRst = 0;
    while (1)
    {
        vector<std::vector<std::string> > tRows;
        vector<std::string> tFields;
        nRst += pDb->loadSql(sSql, tRows, &tFields);
        if (nRst < 0)
        {
            return;
        }
        cout << nRst << endl;
        break;
    }

}

void testDql4()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

//	string sSql = "select * from omc_neconfig";
    //string sSql = "select * from Table1";
    string sSql = "select * from table1;";

    if (pDb == NULL)
    {
        return;
    }
    for (int i = 0; i < 10; ++i)
    {
        vector<string> sSqls;
        for (int j = 0; j < 5; ++j)
        {
            string sSql = "INSERT INTO table1\n"
                "(column1, column2, column3)\n"
                "VALUES(%d, 2969567233, 1.123);";
            sSql = CxString::format(sSql.c_str(), j*i+10);
            sSqls.push_back(sSql);
        }
        int iResult = pDb->execSqlList(sSqls);
        //iResult : TRUE==1 | FALSE==0
        cout << iResult << endl;
    }

}

void testDql5()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    string sSql = "select * from V_ICS_TCK_ZY_MAIN ;";

    if (pDb == NULL)
    {
        return;
    }
    int nRst = 0;
    while (1)
    {
        vector<std::vector<std::string> > tRows;
        vector<std::string> tFields;
        nRst += pDb->loadSql(sSql, tRows, &tFields);
        if (nRst < 0)
        {
            return;
        }
        cout << nRst << endl;
        break;
    }
}

void timerDql1(int iInterval)
{
    static int iIndex = 0;
    ++ iIndex;
    msepoch_t dtNow = CxTime::currentSystemTime();
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();
    if (pDb == NULL)
    {
        cxPrompt() << "can not open database!";
        return;
    }

    string sSql = "select * from ha_appconfig";
    vector<std::vector<std::string> > tRows;
    vector<std::string> tFields;
    int nRst = pDb->loadSql(sSql, tRows, &tFields);
    if (nRst < 0)
    {
        cxPrompt() << "locad sql data empty!";
        return;
    }

    int jIndex = iIndex % tRows.size();
    const vector<std::string> & tRow = tRows.at(jIndex);

    vector<string> row = CxEncoding::utf8ToGb2312(tRow);

    cxPrompt() << "locad sql data row count : " << tRows.size()
               << " , print row index: " << jIndex
               << " , print line new-old: " << tRow.size() << " - " << row.size();
    cxPrompt() << row;

    cxPrompt() << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << " --- --- --- --- ---";
}

void testDql6()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    string sSql = "select * from ICS.\"t1\"";

    if (pDb == NULL)
    {
        return;
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
}

void testDql7()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    if (pDb == NULL)
    {
        return;
    }
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            string sSql = "INSERT INTO table1\n"
                "(column1, column2, column3)\n"
                "VALUES(%d, 2969567233, 1.123);";
            sSql = CxString::format(sSql.c_str(), j*i+10);
            int iResult = pDb->execSql(sSql);
            //iResult : TRUE==1 | FALSE==0
            cout << iResult << endl;
        }
    }

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            string sSql = "INSERT INTO table2\n"
                "(column1, column2, column3)\n"
                "VALUES(%d, 2969567233, 1.123);";
            sSql = CxString::format(sSql.c_str(), j*i+10);
            int iResult = pDb->execSql(sSql);
            //iResult : TRUE==1 | FALSE==0
            cout << iResult << endl;
        }
    }

}
