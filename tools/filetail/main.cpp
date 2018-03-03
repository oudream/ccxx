
#include <ccxx/ccxx.h>

using namespace std;

vector<string> f_sIgnoreDirNames;

// result file tail is "/r/n"
void fn_checkFiles()
{
    string sFilePathScan = CxFileSystem::mergeFilePath(CxAppEnv::tempPath(), "scan.txt");
    string sFilePathHasReturn = CxFileSystem::mergeFilePath(CxAppEnv::tempPath(), "has_return.txt");
    string sFilePathHasZh = CxFileSystem::mergeFilePath(CxAppEnv::tempPath(), "has_zh.txt");

    vector<string> sScanFilePaths;
    vector<string> sHasReturnFilePaths;
    vector<string> sHasZhFilePaths;
    string sFilePath = CxAppEnv::findArgument("p");
    if (sFilePath.size() > 0)
    {
        vector<CxFileSystem::PathInfo> pathInfos;
        CxFileSystem::scanDir(sFilePath, pathInfos, true, false, f_sIgnoreDirNames);
        for (size_t i = 0; i < pathInfos.size(); ++i)
        {
            const CxFileSystem::PathInfo &pathInfo = pathInfos.at(i);
            if (pathInfo.fileName == ".DS_Store") continue;
            sScanFilePaths.push_back(pathInfo.filePath());
        }
    }
    for (int j = 0; j < sScanFilePaths.size(); ++j)
    {
        const string &sScanFilePath = sScanFilePaths.at(j);

        int iHasReturn = -1;
        int iHasZh = -1;
        int iHasZhCol;
        vector<string> sLines;
        CxFile::load(sScanFilePath, sLines, "\n");
        for (int i = 0; i < sLines.size(); ++i)
        {
            const string sLine = sLines.at(i);
            if (iHasReturn == -1 && CxString::endWith(sLine, "\r")) iHasReturn = i;
            if (iHasZh == -1)
            {
                for (int k = 0; k < sLine.size(); ++k)
                {
                    if (sLine[k] < 0)
                    {
                        iHasZh = i;
                        iHasZhCol = k;
                        break;
                    }
                }
            }
        }
        if (iHasReturn != -1)
        {
            sHasReturnFilePaths.push_back(sScanFilePath + " ( " + CxString::toString(iHasReturn+1) + " ) ");
        }
        if (iHasZh != -1)
        {
            sHasZhFilePaths.push_back(sScanFilePath + " ( " + CxString::toString(iHasZh+1) + " | " + CxString::toString(iHasZhCol+1) + " ) ");
        }
    }
    bool bResult = CxFile::save(sFilePathScan, sScanFilePaths);
    cxPrompt() << "sFilePathScan : " << bResult;
    bResult = CxFile::save(sFilePathHasReturn, sHasReturnFilePaths);
    cxPrompt() << "sFilePathHasReturn : " << bResult;
    bResult = CxFile::save(sFilePathHasZh, sHasZhFilePaths);
    cxPrompt() << "sFilePathHasZh : " << bResult;
}

void fn_test(int, int, const void *, int, void *, void *)
{
    fn_checkFiles();
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

    f_sIgnoreDirNames.push_back(".DS_Store");
    f_sIgnoreDirNames.push_back(".git");
    f_sIgnoreDirNames.push_back(".svn");
    f_sIgnoreDirNames.push_back(".gitignore");
    f_sIgnoreDirNames.push_back(".idea");
    f_sIgnoreDirNames.push_back("cmake-build-debug");
    f_sIgnoreDirNames.push_back("cmake-build-release");

    CxApplication::pushProcessCallBack(fn_test);

    CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

    CxTimerManager::startTimer(fn_timer1, 2000);

    CxApplication::exec();

    return 0;
}
