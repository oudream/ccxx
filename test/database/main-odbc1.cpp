#include <ccxx/ccxx.h>

#include <ccxx/cxdatabase_odbc.h>

#include "test_cursor1.hpp"
#include "test_dql1.hpp"
#include "test_odbc_oracle1.hpp"

using namespace std;

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

	testDql7();

//	testOdbcOracle1();
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

	string g_sDBSource = "UID=root;PWD=123456;DSN=ics_mysql_dsn";
	string g_sDBType = "MySQL";

//	string g_sDBSource = "DSN=ics_access_dsn";
//	string g_sDBType = "access";


	CxDatabase * pDb = CxDatabaseManager::createDatabase(g_sDBSource, g_sDBType);
	if (pDb == NULL)
	{
		cout << "end test !!!";
		return 0;
	}
	cout << " pDb->openDatabase() : " << pDb->openDatabase();

//	testCursor1();
//	testCursor2();

//    cout << "end test CxDatabase !!!";

	CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

	CxApplication::pushProcessCallBack(fn_test);

	CxTimerManager::startTimer(fn_timer1, 1000);

	return CxApplication::exec();
}
