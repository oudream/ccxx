
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

void testInsert2(int nAlarmNo)
{
    string sSql2 = "INSERT INTO `omc_alarmrec`(`AlarmNo`, `NeID`, `AlarmType`, `ModuleNo`, `CardNo`, `AlarmValue`, `AlarmTime`, `ResumeTime`, `RepaireMark`, `Status`, `Confirm`, `ConfirmTime`, `ConfirmUser`, `Eliminate`, `EliminateTime`, `EliminateUser`) VALUES(%d, 1, 2, 3, 4, %d, '1558626987752', '0', NULL, 0, 0, '0', '', 0, '0', '');";

    string sSql = "Insert into omc_alarmrec(AlarmNo, NeID, AlarmType, ModuleNo, CardNo, AlarmValue, "
                    "AlarmTime, Status, ResumeTime, Confirm, ConfirmTime, ConfirmUser, "
                    "Eliminate, EliminateTime, EliminateUser) "
                    "values(%d, 1, 2, 3, 4, %d, %d, 0, 0, 0, 0, '', 0, 0, '') ";

    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    if (pDb == NULL)
    {
        return;
    }
    int nRst = 0;
    nRst += pDb->execSql(CxString::format(sSql2.c_str(), nAlarmNo, nAlarmNo, nAlarmNo));
    cout << "execSql=" << nRst << endl;
}

void testInsert3(long long nAlarmNo)
{
    string sSql = "Insert into omc_logrec(ID, UserId, CustomerIP, CustomerPort, OperationNo, RecTime, Details) "
            "values('%lld', '1', '1.1.1.1', 2222, '3333', '%lld', '')";

    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    if (pDb == NULL)
    {
        return;
    }
    int nRst = 0;
    nRst += pDb->execSql(CxString::format(sSql.c_str(), nAlarmNo, nAlarmNo));
    cout << "execSql=" << nRst << endl;
}

void testInsert4(long long nAlarmNo)
{
    string sSql = "Insert into omc_performancedata(ID, NeID, Node, RecTime, Type, Value) "
                    "values('%lld', 1, 'node1', '%lld', 1, 'value%lld') ";

    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    if (pDb == NULL)
    {
        return;
    }
    int nRst = 0;
    nRst += pDb->execSql(CxString::format(sSql.c_str(), nAlarmNo, nAlarmNo, nAlarmNo));
    cout << "execSql=" << nRst << endl;
}

/*
function TestSql()
print("------------------------------ execSql operation ------------------------------")
local nRst = ccOdbc.OpenDatabase("UID=root;PWD=123456;DSN=ics_mysql_dsn", "MySQL", "utf8")
if (nRst == nil) then
    print("open db error")
return "error"
else
print("open db success")
end

    local nTm = ccTime.GetCurrentTimeMS()
local nAlarmNo = nTm % 1000000;

local sSql = "Insert into omc_alarmrec(AlarmNo, NeID, AlarmType, ModuleNo, CardNo, AlarmValue, "
sSql = sSql .. "AlarmTime, Status, ResumeTime, Confirm, ConfirmTime, ConfirmUser, "
sSql = sSql .. "Eliminate, EliminateTime, EliminateUser) "
sSql = sSql .. "values(" .. tostring(nAlarmNo) .. ", 1, 2, 3, 4, " .. tostring(nAlarmNo) .. ", "
sSql = sSql .. tostring(nTm) .. ", 0, 0, 0, 0, '', 0, 0, '') "

ccLog.Write(3, sSql)

nRst = ccOdbc.ExecSql("UID=root;PWD=123456;DSN=ics_mysql_dsn", sSql)
if (nRst == nil) then
    print("execSql db error")
return "error"
else
print("execSql db success")
end

    sSql = "Insert into omc_logrec(ID, UserId, CustomerIP, CustomerPort, OperationNo, RecTime, Details) "
sSql = sSql .. "values('" .. tostring(nTm) .. "', '1', '1.1.1.1', 2222, '3333', '"
sSql = sSql .. tostring(nTm) .. "', '')"

ccLog.Write(3, sSql)

nRst = ccOdbc.ExecSql("UID=root;PWD=123456;DSN=ics_mysql_dsn", sSql)
if (nRst == nil) then
    print("execSql db error")
return "error"
else
print("execSql db success")
end

    sSql = "Insert into omc_performancedata(ID, NeID, Node, RecTime, Type, Value) "
sSql = sSql .. "values('" .. tostring(nTm) .. "', 1, 'node1', '" .. tostring(nTm)
sSql = sSql .. "', 1, 'value" .. tostring(nTm) .. "') "

ccLog.Write(3, sSql)

nRst = ccOdbc.ExecSql("UID=root;PWD=123456;DSN=ics_mysql_dsn", sSql)
if (nRst == nil) then
    print("execSql db error")
return "error"
else
print("execSql db success")
end

return "ok"
end


TestSql()

*/
