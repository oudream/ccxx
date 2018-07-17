
#include <ccxx/ccxx.h>

#include <ccxx/cxdatabase_odbc.h>

#include "test_cursor1.hpp"

using namespace std;

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

	testCursor1();

//    cout << "end test CxDatabase !!!";

    return CxApplication::exec();
}

