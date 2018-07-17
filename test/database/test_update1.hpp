
#include <ccxx/ccxx.h>

using namespace std;

void testUpdate1()
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