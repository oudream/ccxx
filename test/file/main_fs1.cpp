#include <ccxx/ccxx.h>

using namespace std;

void fn_test_create(const string& sScanPath)
{
    CxFileSystem::createDirMultiLevel(sScanPath);
    for (int j = 0; j < 20; ++j)
    {
        string sNow = CxTime::currentSystemTimeString('-', 'd', '-');
        string sFilePath = CxFileSystem::mergeFilePath(sScanPath, CxString::format("测试-之-%s-%d.log", sNow.c_str(), j));
        string sText;
        for (int i = 0; i < j; ++i)
        {
            sText += "测试的内容abc123-";
        }
        CxFile::save(sFilePath, sText);
        cxPrompt() << "Create File: " << sFilePath;
    }
}

void fn_test_delete(const string& sScanPath)
{
    CxFileSystem::removeDir(sScanPath);
    cxPrompt() << "removeDir: " << sScanPath;
}

void fn_timer_timeout_create_delete(int iInterval)
{
    static int iIndex = 0;
    msepoch_t dtNow = CxTime::currentSystemTime();
    if (iIndex++ % 2)
    {
        fn_test_create("/fff/tmp/中文dir1");
    }
    else
    {
        fn_test_delete("/fff/tmp/中文dir1/log");
    }
    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << "";
}

int main(int argc,const char *argv[])
{
    CxApplication::init(argc, argv);

    cxPrompt() << CxTime::currentSystemTime();
    cxPrompt() << CxTime::currentSystemTimeString();
    cxPrompt() << CxTime::toString(CxTime::currentSystemTime());

    CxTimerManager::startTimer(fn_timer_timeout_create_delete, 1000);

    CxApplication::exec();

    return 0;
}
