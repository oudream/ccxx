#include <ccxx/ccxx.h>

using namespace std;

void fn_test_scandir(const string& sScanPath)
{
    vector<CxFilePathInfo> pathInfos;
    CxFileSystem::scanDir(sScanPath, pathInfos, false, true);
    int64 r = 0;
    cxPrompt() << "sScanPath=" << sScanPath;
    cxPrompt() << "pathInfos.size=" << pathInfos.size();
    cxPrompt() << "print top 20:";
    for (size_t i = 0; i< 100 && i < pathInfos.size(); ++i)
    {
        const CxFilePathInfo &pathInfo = pathInfos.at(i);
        cxPrompt() << pathInfo.filePath();
        cxPrompt() << ", creationTime=" << CxTime::toString(pathInfo.creationTime) << ", lastWriteTime=" << CxTime::toString(pathInfo.lastWriteTime);
        cxPrompt() << "";
    }
}

void fn_timer_timeout_scan(int iInterval)
{
    msepoch_t dtNow = CxTime::currentSystemTime();
    fn_test_scandir("/fff/tmp/中文dir1");
    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << "";
    fn_test_scandir("/fff/tmp/中文dir1/log");
    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << "";
}

int main(int argc,const char *argv[])
{
    CxApplication::init(argc, argv);

    cxPrompt() << CxTime::currentSystemTime();
    cxPrompt() << CxTime::currentSystemTimeString();
    cxPrompt() << CxTime::toString(CxTime::currentSystemTime());

    CxTimerManager::startTimer(fn_timer_timeout_scan, 1000);

    CxApplication::exec();

    return 0;
}
