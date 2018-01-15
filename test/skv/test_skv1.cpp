
#include <ccxx/ccxx.h>

using namespace std;

int f_argc = 0;
const char * * f_argv = NULL;

int main(int argc, const char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    CxApplication::init(argc, argv);

    string g_sAppName = "OmcServer";
    //string g_sConfigPath = "../../../../../deploy/config";
//    string g_sConfigPath = "E:/Workshop/ICS/deploy/config/";
    string g_sConfigPath = "D:/build.ygct/ygct_ics_cpp/deploy/win32/config";
    string g_sConfigFileName = g_sAppName + ".json";
    string g_sConfigFilePath = g_sConfigPath + "/" + g_sConfigFileName;
    CxSkverJson skvJson(g_sConfigFilePath);
    map<string, string> g_Config;
    map<string, map<string, string> > g_mapConfig = skvJson.getSectionEntryValues();
    map<string, map<string, string> >::iterator iter = g_mapConfig.begin();
    for (; iter != g_mapConfig.end(); ++iter)
    {
        g_Config = iter->second;
    }

    int iResult = CxApplication::exec();

    return iResult;
}
