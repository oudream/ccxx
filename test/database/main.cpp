
#include <ccxx/ccxx.h>

#include <ccxx/cxdatabase_sqlite.h>
#include <ccxx/cxdatabase_odbc.h>

using namespace std;

void test1()
{
	CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

	string sSqlDrop = "DROP TABLE IF EXISTS `table1`;";

	string sSqlCreate = "\
						CREATE TABLE `table1` (\
						`column1` int(11) DEFAULT NULL,\
						`column2` bigint(20) DEFAULT NULL,\
						`column3` double DEFAULT NULL,\
						`column4` text\
						) ENGINE = InnoDB DEFAULT CHARSET = utf8";

	string sSqlInsert = "INSERT INTO db1.table1(column1, column2, column3, column4) VALUES(%d, 1234567890124, 1.246, '2abcde');";

	string sSqlDelete = "DELETE FROM table1 WHERE column1=%d";

	//check sql
	pDb->execSql(sSqlDrop);
	pDb->execSql(sSqlCreate);
	if (pDb->execSql(CxString::format(sSqlInsert.c_str(), 1001)) > 0)
	{
		if (pDb->execSql(CxString::format(sSqlDelete.c_str(), 1001)) <= 0)
		{
			cout << "exit, sSqlDelete execSql error!";
		}
	}
	else
	{
		cout << "exit, sSqlInsert execSql error!";
	}

	while (true)
	{
		for (size_t i = 0; i < 100; i++)
		{
			string sSql2 = CxString::format(sSqlInsert.c_str(), i);
			int iResult = pDb->execSql(sSql2);
			cout << sSql2 << " result=" << iResult;
			cxLogInfo() << sSql2 << " result=" << iResult;
			CxThread::sleep(10);
		}

		for (size_t i = 0; i < 100; i++)
		{
			string sSql2 = CxString::format(sSqlDelete.c_str(), i);
			int iResult = pDb->execSql(sSql2);
			cout << sSql2 << " result=" << iResult;
			cxLogInfo() << sSql2 << " result=" << iResult;
			CxThread::sleep(10);
		}
	}
}

void test2()
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

void test3()
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

void test4()
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

void test5()
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

void test6()
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

void test7()
{
	string sSql = " Insert into omc_neconfig (NeNo, NeAlias, NeType, AccessType, EthernetIP, EthernetPort, \n"
		"EthernetType, NeParentNo, Enable, SysNeed) values(9001, '火车识别装置#1', 28929, 1, '', 0, 0, 0, 1, 1); ";

	CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

	if (pDb == NULL)
	{
		return;
	}
	int nRst = 0;
	nRst += pDb->execSql(sSql);
	cout << "execSql=" << nRst << endl;
}

void test8()
{
	string sSql1 = "UPDATE `ics4000`.`table1` SET `column1` = 1, `column2` = 123456789123, `column3` = 1.23456789, `column4` = 'abcdefghijk123456789a' WHERE `column1` = 1;";
	string sSql2 = "UPDATE `ics4000`.`table2` SET `column1` = 1, `column2` = 123456789123, `column3` = 1.23456789, `column4` = 'abcdefghijk123456789a' WHERE `column1` = 2;";
	string sSql3 = "UPDATE `ics4000`.`table1` SET `column1` = 1, `column2` = 123456789123, `column3` = 1.23456789, `column4` = 'abcdefghijk123456789a' WHERE `column1` = 2;";

	CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

	if (pDb == NULL)
	{
		return;
	}
	int nRst = 0;
	cout << "\ntest8 begin:" << endl;
	nRst = pDb->execSql(sSql1);
	cout << "execSql1=" << nRst << endl;
	nRst = pDb->execSql(sSql2);
	cout << "execSql2=" << nRst << endl;
	nRst = pDb->execSql(sSql3);
	cout << "execSql3=" << nRst << endl;
	cout << "test8 end." << endl;
}

int main(int argc,const char * argv[])
{
    cout << "begin test CxDatabase : ";

    CxApplication::init(argc, argv);

	//	string g_sDBSource = "UID=ICS;PWD=ICS;DSN=ics_oracle_dsn";
	//	string g_sDBType = "Oracle";

	string g_sDBSource = "UID=root;PWD=123456;DSN=ics_mysql_dsn";
	string g_sDBType = "MySQL";
//
	CxDatabase * pDb = CxDatabaseManager::createDatabase(g_sDBSource, g_sDBType);
	if (pDb == NULL)
	{
		cout << "end test !!!";
		return 0;
	}
	cout << " pDb->openDatabase() : " << pDb->openDatabase();

	test8();

    cout << "end test CxDatabase !!!";

    return CxApplication::exec();
}

