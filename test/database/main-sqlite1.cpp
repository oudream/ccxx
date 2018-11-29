
#include <ccxx/ccxx.h>

#include <ccxx/cxdatabase_sqlite.h>

using namespace std;

void fn_timer_timeout(int iInterval)
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


int main(int argc,const char * argv[])
{
    cout << "begin test CxDatabase : ";

    CxApplication::init(argc, argv);

	string g_sDBSource = "db1.db";
	string g_sDBType = "sqlite";
//
	CxDatabase * pDb = CxDatabaseManager::createDatabase(g_sDBSource, g_sDBType);
	if (pDb == NULL)
	{
		cout << "end test !!!";
		return 0;
	}
	cout << " pDb->openDatabase() : " << pDb->openDatabase();

	CxTimerManager::startTimer(fn_timer_timeout, 200);

//    cout << "end test CxDatabase !!!";

    return CxApplication::exec();
}

