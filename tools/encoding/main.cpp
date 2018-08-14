
#include <ccxx/ccxx.h>

using namespace std;


void testLoadDir()
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

void testFileGb2312ToUtf8()
{
    string sFilePath = "C:\\ddd\\ygct\\nodejs3\\assets\\xxx\\gis\\tmp2.txt";
    string sFilePath2 = "C:\\ddd\\ygct\\nodejs3\\assets\\xxx\\gis\\tmp2-utf8.txt";
    string sText = CxFile::load(sFilePath);
    string sUtf8Text = CxEncoding::gb2312ToUtf8(sText);
    CxFile::save(sFilePath2, sUtf8Text);
}

void testFileUtf8ToGb2312()
{
    string sFilePath = "C:\\ddd\\ygct\\nodejs3\\assets\\xxx\\gis\\tmp1.txt";
    string sFilePath2 = "C:\\ddd\\ygct\\nodejs3\\assets\\xxx\\gis\\tmp1-gbk.txt";
    string sText = CxFile::load(sFilePath);
    string sGbkText = CxEncoding::utf8ToGb2312(sText);
    CxFile::save(sFilePath2, sGbkText);

    system("set");
}

void fn_test(int, int, const void *, int, void *, void *)
{
    testFileGb2312ToUtf8();

    testFileUtf8ToGb2312();
}

void fn_timer1(int)
{
//    testLoadDir();
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

//int main(int argc, char **argv, char** envp)
//{
//    char** env;
//    for (env = envp; *env != 0; env++)
//    {
//        char* thisEnv = *env;
//        printf("%s\n", thisEnv);
//    }
//}

//int main(int argc, char **argv, char** envp)
//{
//    char** env;
//    for (env = envp; *env != 0; env++)
//    {
//        char* thisEnv = *env;
//        printf("%s\n", thisEnv);
//    }
//    system("PAUSE");
//}

//extern char **environ;

int main(int argc, const char *argv[])
{
//    char **p = environ;
//    while (*p != NULL)
//    {
//        printf("%s (%p)\n", *p, *p);
//        *p++;
//    }

    CxApplication::init(argc, argv);

    CxApplication::pushProcessCallBack(fn_test);

    CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

    CxTimerManager::startTimer(fn_timer1, 2000);

    CxApplication::exec();

    return 0;
}
