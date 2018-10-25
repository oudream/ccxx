#include <ccxx/ccxx.h>
#include <cxlua/lua_run.h>

using namespace std;

int main(int argc, const char*argv[])
{
    if(!CxApplication::singleInstance(argc, argv))
    {
        return -1;
    }

    cxPrompt() << "begin cxlua sample: ";

    CxApplication::init(argc, argv);

    CxLuaRun::start();

    CxTimerManager::startTimer(CxDatabaseManager::connectCheck,60000);

    int iResult = CxApplication::exec();

    CxTimerManager::stopTimer(CxDatabaseManager::connectCheck);

    CxLuaRun::stop();

    return iResult;
}

