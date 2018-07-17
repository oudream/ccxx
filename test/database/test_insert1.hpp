
#include <ccxx/ccxx.h>

using namespace std;

void testInsert1()
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
