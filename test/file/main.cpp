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

void fn_timer_timeout(int iInterval)
{
    cxLog() << CxTime::currentSystemTimeString();
}

int main(int argc,const char *argv[])
{
    CxApplication::init(argc, argv);

    string s1 = "c:/temp/a/b/e\\f/g";
    string s2 = "./h";
    string s3 = "./../k";
    string s4 = "../l";
    string s5 = "..m";

    cout << CxFileSystem::cd(s2, s1) << endl;
    cout << CxFileSystem::cd(s3, s1) << endl;
    cout << CxFileSystem::cd(s4, s1) << endl;
    cout << CxFileSystem::cd(s5, s1) << endl;

    CxTimerManager::startTimer(fn_timer_timeout, 100);

    CxApplication::exec();

    return 0;
}
