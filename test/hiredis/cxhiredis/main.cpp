#include <ccxx/ccxx.h>

#include <ccxx/cxhiredis.h>

#include <ccxx/ccxx.h>

using namespace std;

void fn_addList(const string & sKeyName, int iCount)
{
    cxPrompt() << CxString::format("getKeyType(\"%s\"): ", sKeyName.c_str()) << CxHiredis::getKeyType(sKeyName);
    //vector<string> listValues;
    //int iRand = CxTime::currentMsepoch() % 100;
    //int iCount2 = iCount > 0 ? iCount : 10;
    //for (int i = 0; i < iCount2; ++i)
    //{
    //    string sValue;
    //    sValue.push_back(char(i + iRand));
    //    listValues.push_back(sValue);
    //}
    msepoch_t dtNow = CxTime::currentSystemTime();
    vector<string> list2; list2.push_back(CxTime::toString(dtNow)); list2.push_back(CxTime::toString(dtNow + 1)); list2.push_back(CxTime::toString(dtNow+2)); 
    cxPrompt() << CxString::format(" --- --- --- LIST %s operation but del --- --- --- ", sKeyName.c_str());
    cxPrompt() << CxString::format("listPush(\"%s\", vector<string>([count=%d]): ", sKeyName.c_str(), iCount) << CxHiredis::listPushMultiple(sKeyName, list2);
}

void fn_timer_timeout1(int iInterval)
{
    static int iIndex = 0;

    if (iIndex > 100)
    {
        string sDelKey = CxString::format("list%d", iIndex-100);

        cxPrompt() << CxString::format(" --- --- --- LIST %s del --- --- --- ", sDelKey.c_str());
        cxPrompt() << CxString::format("deleteKey(\"%s\"): ", sDelKey.c_str()) << CxHiredis::deleteKey(sDelKey);
    }
    else
    {
        fn_addList(CxString::format("list%d", iIndex), iIndex);
    }

    iIndex++;
    if (iIndex > 200)
    {
        iIndex = 0;
    }
}

int main(int argc, const char * argv[])
{
    CxApplication::init(argc, argv);
    if (!CxApplication::singleInstance(argc, argv))
    {
        return -1;
    }

    string sIp = CxString::fromString(CxAppEnv::findArgument("h"), string("127.0.0.1"));
    int iPort = CxString::fromString(CxAppEnv::findArgument("p"), int(6379));
    if (!CxHiredis::loadClient(sIp, iPort))
    {
        return -1;
    }

    cxPrompt() << "hostLoad ok";
    cxPrompt() << "serverIpAddress(): " << CxHiredis::serverIpAddress();
    cxPrompt() << "serverPort(): " << CxHiredis::serverPort();
    cxPrompt() << "hostIsLoaded(): " << CxHiredis::isLoadedClient();
    cxPrompt() << "hostSelect(0): " << CxHiredis::select(0);
    cxPrompt() << "hostSelect(100): " << CxHiredis::select(100);

    //cxPrompt() << "deleteKey(\"willdel1\"): " << CxHiredis::deleteKey("willdel1");
    cxPrompt() << "stringSet(\"willdel1\", \"willdel1value a1234132\"): " << CxHiredis::stringSet("willdel1", "willdel1value a1234132");
    //cxPrompt() << "deleteKey(\"willdel1\"): " << CxHiredis::deleteKey("willdel1");

    cxPrompt() << " --- --- --- STRING 1 --- --- --- ";

    cxPrompt() << "getKeyType(\"bar\"): " << CxHiredis::getKeyType("string1");
    cxPrompt() << "stringSet(\"bar\", \"bar1\"): " << CxHiredis::stringSet("string1", "string1-value1abc123 zxy");
    cxPrompt() << "getKeyType(\"bar\"): " << CxHiredis::getKeyType("string1");
    cxPrompt() << "stringGet(\"bar\"): " << CxHiredis::stringGet("string1");

    cxPrompt() << " --- --- --- LIST 1 --- --- --- ";
    cxPrompt() << "getKeyType(\"list1\"): " << CxHiredis::getKeyType("list1");
    cxPrompt() << "listGetLength(\"list1\"): " << CxHiredis::listGetLength("list1");
    cxPrompt() << "listPush(\"list1\", \"list1-item1abc123 zxy\"): " << CxHiredis::listPush("list1", "list1-item1abc123 zy");
    cxPrompt() << "getKeyType(\"list1\"): " << CxHiredis::getKeyType("list1");
    cxPrompt() << "listGetLength(\"list1\"): " << CxHiredis::listGetLength("list1");
    cxPrompt() << "listGet(\"list1\", 0, -1): " << CxHiredis::listGet("list1", 0, -1);

    cxPrompt() << " --- --- --- LIST 2 --- --- --- ";
    cxPrompt() << "getKeyType(\"list0424v6\"): " << CxHiredis::getKeyType("list0424v6");
    vector<string> list2; list2.push_back("i2a 1"); list2.push_back("i2a 2"); list2.push_back("i2a 3"); list2.push_back("i2a 4"); list2.push_back("i2a 5");
    cxPrompt() << "listPush(\"list0424v6\", vector<string>({string(\"i2a 1\"),string(\"i2a 2\"),string(\"i2a 3\")})): " << CxHiredis::listPushMultiple("list0424v6", list2);
    cxPrompt() << "getKeyType(\"list0424v6\"): " << CxHiredis::getKeyType("list0424v6");
    cxPrompt() << "listGetLength(\"list0424v6\"): " << CxHiredis::listGetLength("list0424v6");
    cxPrompt() << "listGet(\"list0424v6\", 0, -1): " << CxHiredis::listGet("list0424v6", 0, -1);
    cxPrompt() << "listRemove(\"list0424v6\", 0, string(\"i2a 2\")): " << CxHiredis::listRemove("list0424v6", 0, string("i2a 2"));
    cxPrompt() << "listGet(\"list0424v6\", 0, -1): " << CxHiredis::listGet("list0424v6", 0, -1);
    cxPrompt() << "listTrim(\"list0424v6\", 2, 3): " << CxHiredis::listTrim("list0424v6", 2, 3);
    cxPrompt() << "listGet(\"list0424v6\", 0, -1): " << CxHiredis::listGet("list0424v6", 0, -1);


    CxTimerManager::startTimer(fn_timer_timeout1, 100);

    CxApplication::exec();

    return TRUE;
}
