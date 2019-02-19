
#include <ccxx/ccxx.h>

using namespace std;

void testOdbcOracle1()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

//    string sSql = "SELECT * FROM T_RL_CLZCXX T WHERE NVL(T.HMDZT,'00000000')='00000000'";
//    string sSql = "SELECT T.GID FROM T_RL_CLZCXX T WHERE NVL(T.HMDZT,'00000000')='00000000'";
    string sSql = CxEncoding::utf8ToGb2312("SELECT * FROM T_RL_CLZCXX T WHERE T.CHEHAO= '粤C12341' AND  NVL(T.HMDZT,'00000000')='00000000'");

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