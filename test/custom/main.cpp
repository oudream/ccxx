
#include <ccxx/ccxx.h>

//#include <ucommon/file.h>
//#include <ucommon/fsys.h>

using namespace std;

#include "appenv1.hpp"
#include "filesystem1.hpp"
#include "skver_json1.hpp"
#include "uuid1.hpp"
#include "time1.hpp"
#include "timer1.hpp"
#include "string1.hpp"


int fn_interinfo_in_cmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag)
{
    if (sCommand == "exit")
    {
        CxApplication::exit();
        return TRUE;
    }
    else
    {
        cxPrompt() << "in : " << sCommand;
    }
    return FALSE;
}

void fn_test_windows_AttachConsole()
{
#ifdef GM_OS_WIN
    DWORD pid;
    HANDLE hProcess;

    pid = 2848;
    hProcess = OpenProcess(SYNCHRONIZE, FALSE, pid);

    if (!hProcess) {
        printf("Process %ul not found!\n", pid);
        return;
    }

    FreeConsole();

    if (!AttachConsole(pid)) {
        printf("Failed to attach to console of Process %ul\n", pid);
        CloseHandle(hProcess);
        return;
    }

    SetConsoleCtrlHandler(NULL, TRUE);
    GenerateConsoleCtrlEvent(0, 0);
    WaitForSingleObject(hProcess, INFINITE);
    CloseHandle(hProcess);
    FreeConsole();

    if (!AttachConsole(CxProcess::getCurrentPid().dwProcessId))
    {
        printf("Failed to attach to console of Process %ul\n", pid);
    }
        return;
//    testString12();
    CxProcess::kill("cxtest_pure.exe");
#endif
}

void fn_test_encoding1()
{
    vector<string> sFileStrings;
    string sFilePath = "/ddd/ccpp/ccxx/ccxx/cxchannel.h";
    string sFilePath2 = "/ddd/ccpp/ccxx/ccxx/cxchannel2.h";
    if (CxFile::load(sFilePath, sFileStrings, (int)1024))
    {
        vector<string> sFileString2 = CxEncoding::utf8ToGb2312(sFileStrings);
        bool bResult = CxFile::save(sFilePath2, sFileStrings, "");
        cxPrompt() << "Size:" << sFileString2.size() << ", bResult: " << bResult;
    }
}

void fn_test_encoding2()
{
    string sFileStrings;
    string sFilePath = "/ddd/ccpp/ccxx/ccxx/cxchannel.h";
    string sFilePath2 = "/ddd/ccpp/ccxx/ccxx/cxchannel3.h";
    sFileStrings = CxFile::load(sFilePath);
    string sFileString2 = CxEncoding::utf8ToGb2312(sFileStrings);
    bool bResult = CxFile::save(sFilePath2, sFileStrings);
    cxPrompt() << "Size:" << sFileString2.size() << ", bResult: " << bResult;
}

void fn_test_encoding3()
{
    string string1 = "CICS申请结束下发.s";
    string sFileString2 = CxEncoding::utf8ToGb2312(string1);
    cxPrompt() << string1 << " - " << sFileString2;
    CxFile::save("/fff/901.txt", sFileString2);
}

void fn_test_alert()
{
    assert(1 < 0);
}

void fn_test(int, int, const void *, int, void *, void *)
{
    fn_test_alert();
}

void fn_testScan1(int)
{
    cxPrompt() << "begin time1:";
    vector<CxFilePathInfo> pathInfos;
    CxFileSystem::scanDir("x:/", pathInfos);

    cxPrompt() << pathInfos.size();
    for (size_t i = 0; i < pathInfos.size(); ++i)
    {
        const CxFilePathInfo & filePathInfo = pathInfos.at(i);
        const string sFileName = filePathInfo.fileName;
        if (filePathInfo.pathType == CxFileSystem::PathTypeFile)
        {
            string s = CxFile::load(filePathInfo.filePath());
            cxPrompt() << s;
        }
    }
    cxPrompt() << CxTime::currentSystemTimeString();
}

void fn_timer1(int)
{
    cxPrompt() << "begin time1:";

    testStringFormat1();
}

int main(int argc, const char * argv[])
{
    CxApplication::init(argc, argv);

    CxApplication::pushProcessCallBack(fn_test);

    CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

    CxTimerManager::startTimer(fn_timer1, 2000);

    CxApplication::exec();

    return 0;
}
