
#include <ccxx/ccxx.h>

#include <ccxx/cxdatabase_sqlite.h>

using namespace std;

void testSqlite1()
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

	string sSql = "select * from t1";
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

int fn_interinfo_in_cmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag)
{
	if (sCommand == "exit")
	{
		CxApplication::exit();
		return TRUE;
	}
	else
	{
		cxPrompt() << "in : " << sCommand;
	}
	return FALSE;
}

void fn_timer1(int )
{
	cxPrompt() << "begin time1:";

	testSqlite1();
}

void fn_test(int, int, const void *, int, void *, void *)
{
}

int main(int argc,const char * argv[])
{
	cout << "begin test CxDatabase-Sqlite : ";

	CxApplication::init(argc, argv);

#ifdef GM_OS_WIN
    string sHome = CxAppEnv::findEnv("HOMEPATH");
#else
    string sHome = CxAppEnv::findEnv("HOME");
#endif

    string g_sDBSource = CxFileSystem::mergeFilePath(sHome, "hello1.sqlite3.db");
	string g_sDBType = "Sqlite";

	CxDatabase * pDb = CxDatabaseManager::createDatabase(g_sDBSource, g_sDBType);
	if (pDb == NULL)
	{
		cout << "end test !!!";
		return 0;
	}
	cout << " pDb->openDatabase() : " << pDb->openDatabase();


//    cout << "end test CxDatabase !!!";

	CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

	CxApplication::pushProcessCallBack(fn_test);

	CxTimerManager::startTimer(fn_timer1, 1000);

	return CxApplication::exec();
}

