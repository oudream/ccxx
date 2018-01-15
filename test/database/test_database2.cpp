
#include <ccxx/ccxx.h>

using namespace std;

int main(int argc,const char * argv[])
{
    cout << "begin test database : " << endl;

    CxApplication::init(argc, argv);


//    string sConnectSource = "UID=ygct;PWD=ygct;DSN=dns_mysql_ansi";
    string sConnectSource = "system/oracle@dns_oracle";
    CxDatabase * oDb = CxDatabaseManager::createDatabase(sConnectSource);
    oDb->openDatabase();

    if (! oDb)
        return -1;

    std::vector<std::string> columnNames;
    columnNames.push_back(string("f1"));
    columnNames.push_back(string("f2"));
    columnNames.push_back(string("f3"));
    columnNames.push_back(string("f4"));
    columnNames.push_back(string("f5"));
    columnNames.push_back(string("f6"));
    columnNames.push_back(string("f7"));
    std::vector<std::vector<std::string> > rows;
    for (size_t i = 0; i < 10; ++i)
    {
        std::vector<std::string> row;
        row.push_back(CxString::toString(i*1));
        row.push_back(CxString::toString(i*10));
        row.push_back(CxString::toString(i*100));
        row.push_back(CxString::toString(i*1000.1f));
        row.push_back(CxString::toString(i)+"abc");
        row.push_back(CxString::toString(i)+"abcdefg []12234().,.");
        row.push_back(CxString::toString(i)+"zyx");
        rows.push_back(row);
    }

    int r = oDb->saveTable("t1", columnNames, rows);

    cout << "saveTable result=" << r << endl;
    cout << "end test database !!! " << endl;

    return 0;
}



