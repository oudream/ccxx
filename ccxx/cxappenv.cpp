#include "cxappenv.h"

#include "cxprofile.h"

using namespace std;

//#define app_config_filename "app.config.json"
//#define apps_config_filename "apps.config.json"
//#define config_suffixname ".config.json"
#define app_config_filename "app.config.ini"
#define apps_config_filename "apps.config.ini"
#define config_suffixname ".config.ini"

static string f_sApplicationFilePath = "";
static string f_sApplicationFileName = "";
static string f_sApplicationTargetName = "";
static string f_sApplicationPath = "";
static string f_sApplicationDefaultDeployPath = "";
static string f_sApplicationDeployPath = "";
static string f_sApplicationConfigPath = "";
static string f_sDefaultConfigFilePath = "";
static string f_sApplicationConfigFilePath = "";
static string f_sApplicationConfigExtendFilePath = "";
static string f_sApplicationLogPath = "";
static string f_sApplicationTempPath = "";
static string f_sApplicationDataPath = "";

static string f_argument;
static vector<string> f_argumentList;
static map<string, string> f_arguments;
static map<string, map<string, string> > f_appsConfigs;
static vector<string> f_appsConfigSectionNames;

void fn_scan_result_CxAppEnv(const CxFileSystem::PathInfo &pathInfo, string *sParam, int *iParam)
{
    string sFileName = pathInfo.fileName;
    string sPrefixName = CxFileSystem::extractFilePrefixName(sFileName);
    string sSuffix = CxFileSystem::extractFileSuffixName(sFileName);
    if (CxString::equalCase(sPrefixName, f_sApplicationTargetName)
        || CxString::equalCase(sPrefixName, f_sApplicationTargetName + ".config"))
    {
        std::vector<std::string> sSuffixes = CxSkverFactoryManager::factoryNames();
        if (CxString::containEqualCase(sSuffixes, sSuffix))
        {
            f_sApplicationConfigFilePath = pathInfo.filePath();
        }
    }
}

void CxAppEnv::init(int argc, const char *argv[])
{
    if (argc > 1)
    {
        f_arguments = argumentsFromArgcv(argc, argv);
    }
    if (argc > 0)
    {
        //*1 begin auto deployPath and configPath
        f_sApplicationFilePath = argv[0];
        //may be current dir
        if (!CxFileSystem::hasRootPath(f_sApplicationFilePath)) f_sApplicationFilePath = CxFileSystem::mergeFilePath(CxFileSystem::getCurrentDir(), f_sApplicationFilePath);
        f_sApplicationPath = CxFileSystem::extractPath(f_sApplicationFilePath);
        f_sApplicationFileName = CxFileSystem::extractFileName(f_sApplicationFilePath);
        f_sApplicationTargetName = CxFileSystem::extractFilePrefixName(f_sApplicationFilePath);
        f_sApplicationConfigFilePath = string();
        //scan app config file
        CxFileSystem::scanDir(f_sApplicationPath, fn_scan_result_CxAppEnv);
        //*config is same dir to f_sApplicationFilePath
        if (f_sApplicationConfigFilePath.size() > 0)
        {
            f_sApplicationDeployPath = f_sApplicationPath;
            f_sApplicationConfigPath = f_sApplicationPath;
        }
        else
        {
            string sDefaultDeployPath = CxFileSystem::parentPath(f_sApplicationPath);
            if (CxFileSystem::isExist(sDefaultDeployPath))
            {
                string sPath = CxFileSystem::mergeFilePath(sDefaultDeployPath, "config");
                if (CxFileSystem::isExist(sPath))
                {
                    CxFileSystem::scanDir(sPath, fn_scan_result_CxAppEnv);
                }
            }
			if (f_sApplicationConfigFilePath.empty() && f_sApplicationDefaultDeployPath.size() > 0)
			{
				f_sApplicationDeployPath = f_sApplicationDefaultDeployPath;
				CxFileSystem::scanDir(CxFileSystem::mergeFilePath(f_sApplicationDeployPath, "config"), fn_scan_result_CxAppEnv);
			}
			else
			{
				f_sApplicationDeployPath = sDefaultDeployPath;
			}
			if (f_sApplicationConfigFilePath.empty())
			{
				f_sApplicationConfigPath = CxFileSystem::mergeFilePath(f_sApplicationDeployPath, "config");
			}
            else
            {
                f_sApplicationConfigPath = CxFileSystem::extractPath(f_sApplicationConfigFilePath);
            }
        }
        //*
        f_sDefaultConfigFilePath = CxFileSystem::mergeFilePath(f_sApplicationConfigPath, apps_config_filename);
        if (!CxFileSystem::isExist(f_sDefaultConfigFilePath) &&
            CxFileSystem::isExist(CxFileSystem::mergeFilePath(f_sApplicationConfigPath, app_config_filename)))
        {
            f_sDefaultConfigFilePath = CxFileSystem::mergeFilePath(f_sApplicationConfigPath, app_config_filename);
        }
        if (f_sApplicationConfigFilePath.empty())
        {
            f_sApplicationConfigFilePath = findArgument("cfp");
            if (f_sApplicationConfigFilePath.empty())
            {
                f_sApplicationConfigFilePath = CxFileSystem::mergeFilePath(f_sApplicationConfigPath,
                                                                           f_sApplicationTargetName
                                                                           + config_suffixname);
            }
        }
        f_sApplicationConfigExtendFilePath = findArgument("sfp");
        if (f_sApplicationConfigExtendFilePath.empty())
        {
            f_sApplicationConfigExtendFilePath = CxFileSystem::mergeFilePath(f_sApplicationConfigPath,
                                                                             f_sApplicationTargetName + ".extend.xml");
        }
        CxSkverBase *oSkvDefault = CxSkverManager::beginUse(f_sDefaultConfigFilePath);
        if (oSkvDefault)
        {
            CxSkverBase *oSkvApps = CxSkverManager::beginUse(f_sApplicationConfigFilePath);
            if (oSkvApps)
            {
                oSkvDefault->updateSectionEntryValues(oSkvApps->getSectionEntryValues());
                CxSkverManager::endUse(oSkvApps);
            }
            f_appsConfigs = oSkvDefault->getSectionEntryValues();
            CxSkverManager::endUse(oSkvDefault);
        }

        //log path
        f_sApplicationLogPath = findConfig(CS_SectionProgramConfig, "LogPath", std::string());
        if (f_sApplicationLogPath.size() > 0)
        {
            f_sApplicationLogPath = CxFileSystem::fullFilePath(f_sApplicationDeployPath, f_sApplicationLogPath);
        }
        else
        {
            f_sApplicationLogPath = CxFileSystem::mergeFilePath(CxFileSystem::mergeFilePath(f_sApplicationDeployPath, "log"), f_sApplicationTargetName);
        }
        //temp path
        f_sApplicationTempPath = findConfig(CS_SectionProgramConfig, "TempPath", std::string());
        if (f_sApplicationTempPath.size() > 0)
        {
            f_sApplicationTempPath = CxFileSystem::fullFilePath(f_sApplicationDeployPath, f_sApplicationTempPath);
        }
        else
        {
            f_sApplicationTempPath = CxFileSystem::mergeFilePath(CxFileSystem::mergeFilePath(f_sApplicationDeployPath, "temp"), f_sApplicationTargetName);
        }
        //data path
        f_sApplicationDataPath = findConfig(CS_SectionProgramConfig, "DataPath", std::string());
        if (f_sApplicationDataPath.size() > 0)
        {
            f_sApplicationDataPath = CxFileSystem::fullFilePath(f_sApplicationDeployPath, f_sApplicationDataPath);
        }
        else
        {
            f_sApplicationDataPath = CxFileSystem::mergeFilePath(CxFileSystem::mergeFilePath(f_sApplicationDeployPath, "data"), f_sApplicationTargetName);
        }

        //create dirs
        if (!CxFileSystem::isExist(f_sApplicationLogPath)) CxFileSystem::createDirMultiLevel(f_sApplicationLogPath);
        if (!CxFileSystem::isExist(f_sApplicationTempPath)) CxFileSystem::createDirMultiLevel(f_sApplicationTempPath);
        if (!CxFileSystem::isExist(f_sApplicationDataPath)) CxFileSystem::createDirMultiLevel(f_sApplicationDataPath);
        if (!CxFileSystem::isExist(f_sApplicationConfigPath)) CxFileSystem::createDirMultiLevel(f_sApplicationConfigPath);
    }
    argumentSaveToConfig();

    cout << "f_sApplicationFilePath: "            << f_sApplicationFilePath << endl;
    cout << "f_sApplicationFileName: "            << f_sApplicationFileName << endl;
    cout << "f_sApplicationTargetName: "          << f_sApplicationTargetName << endl;
    cout << "f_sApplicationPath: "                << f_sApplicationPath << endl;
    cout << "f_sApplicationDefaultDeployPath: "   << f_sApplicationDefaultDeployPath << endl;
    cout << "f_sApplicationDeployPath: "          << f_sApplicationDeployPath << endl;
    cout << "f_sApplicationConfigPath: "          << f_sApplicationConfigPath << endl;
    cout << "f_sDefaultConfigFilePath: "          << f_sDefaultConfigFilePath << endl;
    cout << "f_sApplicationConfigFilePath: "      << f_sApplicationConfigFilePath << endl;
    cout << "f_sApplicationConfigExtendFilePath: "<< f_sApplicationConfigExtendFilePath << endl;
    cout << "f_sApplicationLogPath: "             << f_sApplicationLogPath << endl;
    cout << "f_sApplicationTempPath: "            << f_sApplicationTempPath << endl;
    cout << "f_sApplicationDataPath: "            << f_sApplicationDataPath << endl;
}

const string &CxAppEnv::applicationFilePath()
{
    return f_sApplicationFilePath;
}

const string &CxAppEnv::applicationFileName()
{
    return f_sApplicationFileName;
}

const string &CxAppEnv::applicationTargetName()
{
    return f_sApplicationTargetName;
}

const string &CxAppEnv::applicationPath()
{
    return f_sApplicationPath;
}

const string &CxAppEnv::applicationDeployPath()
{
    return f_sApplicationDeployPath;
}

string CxAppEnv::applicationDeployPathMerge(const string &sMerge)
{
    return CxFileSystem::mergeFilePath(f_sApplicationDeployPath, sMerge);
}

void CxAppEnv::setApplicationDefaultDeployPath(const string &value)
{
    f_sApplicationDefaultDeployPath = value;
}

const string &CxAppEnv::configPath()
{
    return f_sApplicationConfigPath;
}

const string &CxAppEnv::configDefaultFilePath()
{
    return f_sDefaultConfigFilePath;
}

const string &CxAppEnv::configFilePath()
{
    return f_sApplicationConfigFilePath;
}

const string &CxAppEnv::configExtendFilePath()
{
    return f_sApplicationConfigExtendFilePath;
}

const string &CxAppEnv::logPath()
{
    return f_sApplicationLogPath;
}

const string &CxAppEnv::tempPath()
{
    return f_sApplicationTempPath;
}

const string &CxAppEnv::dataPath()
{
    return f_sApplicationDataPath;
}

const map<string, string> &CxAppEnv::arguments()
{
    return f_arguments;
}

string CxAppEnv::findArgument(const string &sArgumentName)
{
    map<string, string>::const_iterator it = f_arguments.find(sArgumentName);
    if (it != f_arguments.end())
        return it->second;
    else
        return std::string();
}

bool CxAppEnv::containArgument(const string &sArgumentName)
{
    map<string, string>::const_iterator it = f_arguments.find(sArgumentName);
    return it != f_arguments.end();
}

string CxAppEnv::argumentsToString(const map<string, string> &sArguments)
{
    string r;
    for (map<string, string>::const_iterator it = sArguments.begin(); it != sArguments.end(); ++it)
    {
        r.push_back(CxGlobal::argumentCharacter);
        r.append(it->first);
        r.push_back(' ');
        r.append(it->second);
        r.push_back(' ');
    }
    if (r.size() > 0) r.resize(r.size() - 1);
    return r;
}

map<string, string> CxAppEnv::argumentsFromString(const string &sArg)
{
    vector<string> sArgs = CxString::split(sArg, ' ');
    map<string, string> r;
    size_t i = 0;
    while (i < sArgs.size() && sArgs[i][0] == CxGlobal::argumentCharacter)
    {
        char *pc = const_cast<char *>(sArgs.at(i).c_str());
        ++i;
        ++pc;
        string sName = pc;
        string sValue;
        while (i < sArgs.size() && sArgs[i][0] == CxGlobal::argumentCharacter)
        {
            sValue += sArgs[i];
            ++i;
            sValue.push_back(' ');
        }
        sValue.resize(sValue.size() - 1);
        r[sName] = sValue;
    }
    return r;
}

std::map<string, std::map<string, string> >
CxAppEnv::toRelativePath(const std::map<string, std::map<string, string> > &sections)
{
    std::map<string, std::map<string, string> > r;
    for (map<string, map<string, string> >::const_iterator itSection = sections.begin();
         itSection != sections.end(); ++itSection)
    {
        map<string, string> rKeys;
        const map<string, string> &keys = itSection->second;
        for (map<string, string>::const_iterator itKeys = keys.begin(); itKeys != keys.end(); ++itKeys)
        {
            const string &sValue = itKeys->second;
            bool bOk;
            string sFilePath = CxFileSystem::relativeFilePath(f_sApplicationPath, sValue, &bOk);
            if (bOk && sFilePath.size() > 0)
            {
                rKeys[itKeys->first] = sFilePath;
            }
            else
            {
                rKeys[itKeys->first] = sValue;
            }
        }
        r[itSection->first] = rKeys;
    }
    return r;
}

map<string, string> CxAppEnv::argumentsFromArgcv(int argc, const char *argv[])
{
    map<string, string> r;
    int i = 1;
    f_argumentList.clear();
    f_argument.clear();
    while (i < argc && i < 1000)
    {
        f_argumentList.push_back(string(argv[i]));
        f_argument.append(argv[i]);
        f_argument.push_back(' ');
        if (*argv[i] == CxGlobal::argumentCharacter)
        {
            const char *pc = argv[i];
            ++i;
            ++pc;
            string sName = pc;
            string sValue;
            while (i < argc && *argv[i] != CxGlobal::argumentCharacter)
            {
                sValue += argv[i];
                ++i;
                sValue.push_back(' ');
            }
            if (sValue.size() > 0)
            {
                sValue.resize(sValue.size() - 1);
                r[sName] = sValue;
                f_argument.append(sValue);
                f_argument.push_back(' ');
            }
        }
        else
        {
            ++i;
        }
    }
    return r;
}

const std::string &
CxAppEnv::doFindConfig(const string &sSection, const string &sKey, const std::string &sDefault, bool bSaveNew)
{
    for (map<string, map<string, string> >::iterator itSection = f_appsConfigs.begin();
         itSection != f_appsConfigs.end(); ++itSection)
    {
        if (CxString::equalCase(itSection->first, sSection))
        {
            map<string, string> &keys = itSection->second;
            for (map<string, string>::const_iterator itKey = keys.begin(); itKey != keys.end(); ++itKey)
            {
                if (CxString::equalCase(itKey->first, sKey))
                {
                    return itKey->second;
                }
            }
            if (bSaveNew)
            {
                keys[sKey] = sDefault;
                return sDefault;
            }
        }
    }
    if (bSaveNew)
    {
        map<string, string> keys;
        keys[sKey] = sDefault;
        f_appsConfigs[sSection] = keys;
        updateConfigSectionNames();
    }
    return sDefault;
}

const std::map<string, std::map<string, string> > &CxAppEnv::getConfigsData()
{
    return f_appsConfigs;
}

const std::vector<string> &CxAppEnv::getConfigSectionNames()
{
    return f_appsConfigSectionNames;
}

const map<string, string> &CxAppEnv::findConfigs(const string &sSection)
{
    for (map<string, map<string, string> >::iterator itSection = f_appsConfigs.begin();
         itSection != f_appsConfigs.end(); ++itSection)
    {
        if (itSection->first == sSection)
        {
            return itSection->second;
        }
    }
    return CxGlobal::emptyMapString;
}

int CxAppEnv::saveConfig(const string &sFilePath)
{
    string sSaveFilePath = sFilePath;
    if (sSaveFilePath.empty()) sSaveFilePath = configFilePath();
    std::map<std::string, std::map<std::string, std::string> > sConfigs = toRelativePath(f_appsConfigs);
    return CxSkverManager::save(sConfigs, sSaveFilePath);
}

void CxAppEnv::saveConfig(const string &sSection, const string &sKey, const std::string &sValue)
{
    map<string, map<string, string> >::iterator itSection = f_appsConfigs.find(sSection);
    if (itSection != f_appsConfigs.end())
    {
        map<string, string> &keys = itSection->second;
        keys[sKey] = sValue;
    }
    else
    {
        map<string, string> keys;
        keys[sKey] = sValue;
        f_appsConfigs[sSection] = keys;
        updateConfigSectionNames();
    }
}

bool CxAppEnv::singleInstance(int argc, const char *argv[])
{
    bool bRet = true;

#ifdef GM_OS_WIN
    string sMode = findArgument("runMode");
    if(sMode.size()==0 || CxString::equalIgnoreAll(sMode,string("single")))
    {
        if ( argc > 0 )
        {
            string sApplicationFilePath = argv[0];
            string sName = CxFileSystem::extractFileName(sApplicationFilePath);
            HANDLE m_hMutex = ::CreateMutex(NULL, TRUE, sName.c_str());

            DWORD dwRet = ::GetLastError();
            ::cout<<sName<<dwRet<<endl;
            if (m_hMutex)
            {
                if (ERROR_ALREADY_EXISTS == dwRet)
                {
                    string sInfo = sApplicationFilePath+string(" is running!!!");
                    ::cout<<"SingleInstance error: "<<sInfo<<endl; 
//                    ::MessageBox(NULL,sInfo.c_str(),"SingleInstance error",MB_OKCANCEL);
                    ::CloseHandle(m_hMutex);
                    bRet = false;
                }
            }
        }
    }

#endif
    return bRet;
}

void CxAppEnv::updateConfigSectionNames()
{
    f_appsConfigSectionNames.clear();
    for (map<string, map<string, string> >::iterator itSection = f_appsConfigs.begin();
         itSection != f_appsConfigs.end(); ++itSection)
    {
        f_appsConfigSectionNames.push_back(itSection->first);
    }
}

void CxAppEnv::argumentSaveToConfig()
{
    for (std::map<string, string>::const_iterator itArgument = f_arguments.begin();
         itArgument != f_arguments.end(); ++itArgument)
    {
        string sArgumentKey = itArgument->first;
        string sArgumentValue = itArgument->second;
        if (sArgumentKey.size() > 0 && sArgumentValue.size() > 0)
        {
            map<string, string> sConfigs = CxString::splitToMap(sArgumentValue, CxGlobal::middleChar, CxGlobal::splitChar);
            for (std::map<string, string>::const_iterator it = sConfigs.begin();
                 it != sConfigs.end(); ++it)
            {
                CxAppEnv::saveConfig(sArgumentKey, it->first, it->second);
            }
        }
    }
}

const std::vector<std::string> &CxAppEnv::argumentList()
{
    return f_argumentList;
}

std::string CxAppEnv::argumentsString()
{
    return f_argument;
}

CxAppEnv::CommandLineArguments::CommandLineArguments(int argc, const char *argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        _args.push_back(string(argv[i]));
    }
    updateArgs();
}

CxAppEnv::CommandLineArguments::~CommandLineArguments()
{
}

void CxAppEnv::CommandLineArguments::push(const std::string &sKey, const std::string &sValue)
{
    if (sKey.empty())
    {
        return;
    }
    if (CxString::beginWith(sKey, "-"))
    {
        _args.push_back(sKey);
    }
    else
    {
        _args.push_back("-" + sKey);
    }
    _args.push_back(sValue);
    updateArgs();
}

void CxAppEnv::CommandLineArguments::updateArgs()
{
    _argPchars.resize(_args.size());
    for (int i = 0; i < _args.size(); ++i)
    {
        const string &sArg = _args.at(i);
        _argPchars[i] = sArg.c_str();
    }
    argc = _argPchars.size();
    argv = _argPchars.size() > 0 ? (const char **) (&(_argPchars.front())) : NULL;
}
