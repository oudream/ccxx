#include <ccxx/ccxx.h>

using namespace std;

string f_sFileName = "001.txt";

void testLoad( bool bOutInfo = true )
{
    vector<string> ss ;
    CxFile::load(CxApplication::applicationPath() + "/" + f_sFileName, ss);
    for (size_t i = 0; i < ss.size(); ++i)
    {
        if (bOutInfo)
            cout << ss[i] << endl;
    }
}

void testSave()
{
    vector<string> ss;
    string s;
    for (size_t i = 0; i < 100; ++i)
    {
        s += "_" + CxString::toString(i);
        ss.push_back(s);
    }
    bool bSuccess = CxFile::save(CxApplication::applicationPath() + "/" + f_sFileName , ss);
    if (bSuccess) cout << "saveToFile success !!!" << endl; else cout << "saveToFile fail !!!" << endl;
}

void testReplaceAll( bool bOutInfo = true )
{
    string ss[] = {
        "abds,asdf,sdaf,asd",
        " abds,asdf,sdaf,asd",
        "  ",
        "  abds,asdf,sdaf,asd   ",
        "             abds,asdf,sdaf,asd",
        "abds,asdf,sdaf,asd         "};
    for (size_t i = 0; i < 6; ++i)
    {
        const string& s = ss[i];
        string str = CxString::replace(s, ",", "");
        if ( bOutInfo )
        {
            cout << i << " -> : " << str << endl;
            cout << str << endl;
            cout << endl;
        }
    }
}

void testFileSuffixName()
{
    string s1 = "filename";
    string s2 = "/tmp/dir/filename";
    string s3 = "filename.suffix";
    string s4 = "/tmp/dir/filename.suffix";
    cout << CxFileSystem::extractFileName(s1) << endl;
    cout << CxFileSystem::extractFileName(s2) << endl;
    cout << CxFileSystem::extractFileName(s3) << endl;
    cout << CxFileSystem::extractFileName(s4) << endl;
}

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

void fn_test_create(const string& sScanPath)
{
    CxFileSystem::createDirMultiLevel(sScanPath);
    for (int j = 0; j < 20; ++j)
    {
        string sNow = CxTime::currentSystemTimeString('-', 'd', '-');
        string sFilePath = CxFileSystem::mergeFilePath(sScanPath, CxString::format("²âÊÔ-Ö®-%s-%d.log", sNow.c_str(), j));
        string sText;
        for (int i = 0; i < j; ++i)
        {
            sText += "²âÊÔµÄÄÚÈÝabc123-";
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
        fn_test_create("D:\\ics4000-ÂÀÅàÁú\\deploy\\log\\tmp01");
    }
    else
    {
        fn_test_delete("D:\\ics4000-ÂÀÅàÁú\\deploy\\log\\tmp01");
    }
    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << "";
}

void fn_timer_timeout_scan(int iInterval)
{
    msepoch_t dtNow = CxTime::currentSystemTime();
//    fn_test_scandir("D:\\ics4000-ÂÀÅàÁú\\deploy\\history");
//    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
//    cxPrompt() << "";
//    fn_test_scandir("D:\\ics4000-ÂÀÅàÁú\\deploy\\log");
//    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
//    cxPrompt() << "";
    fn_test_scandir("C:\\ddd\\ygct\\nodejs3\\node_modules\\oracledb");
    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << "";
    CxTimerManager::stopTimer(fn_timer_timeout_scan);
}

int main(int argc,const char *argv[])
{
    CxApplication::init(argc, argv);

    cxPrompt() << CxTime::currentSystemTime();
    cxPrompt() << CxTime::currentSystemTimeString();
    cxPrompt() << CxTime::toString(CxTime::currentSystemTime());
//    string s1 = "c:/temp/a/b/e\\f/g";
//    string s2 = "./h";
//    string s3 = "./../k";
//    string s4 = "../l";
//    string s5 = "..m";
//
//    string sMsg = CxFileSystem::cd(s2, s1);
//    cout << sMsg << endl;
//    sMsg = CxFileSystem::cd(s3, s1);
//    cout << sMsg << endl;
//    sMsg = CxFileSystem::cd(s4, s1);
//    cout << sMsg << endl;
//    sMsg = CxFileSystem::cd(s5, s1);
//    cout << sMsg << endl;
// 	  cout << CxFileSystem::cd(s3, s1) << endl;
//    cout << CxFileSystem::cd(s4, s1) << endl;
//    cout << CxFileSystem::cd(s5, s1) << endl;

//    CxTimerManager::startTimer(fn_timer_timeout_create_delete, 1000);
    CxTimerManager::startTimer(fn_timer_timeout_scan, 1000);

    CxApplication::exec();

    return 0;
}
