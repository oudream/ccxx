
#include <ccxx/ccxx.h>

using namespace std;

void fn_test(int, int, const void *, int, void *, void *)
{
}

void fn_timer1(int)
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

//main
int main(int argc, const char *argv[])
{
    CxApplication::init(argc, argv);

    CxApplication::pushProcessCallBack(fn_test);

    CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

    CxTimerManager::startTimer(fn_timer1, 2000);

    CxApplication::exec();

    return 0;
}
