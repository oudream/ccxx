#include "cxapplication.h"

#include "cxprocess.h"
#include "cxfile.h"
#include "cxprofile_skv_ini.h"
#include "cxinterinfo.h"


using namespace std;


struct ProcessCallBack
{
    ProcessCallBack(fn_void_msg_tlv_t fn1) : fn(fn1), msg(0), tag(0), source(0), target(0), data() {}
    ProcessCallBack(fn_void_msg_tlv_t fn1, int iMsg, int iTag, void * oSource, void * oTarget) : fn(fn1), msg(iMsg), tag(iTag), source(oSource), target(oTarget), data() {}
    ProcessCallBack(fn_void_msg_tlv_t fn1, int iMsg, int iTag, int iLength, const void * pData, void * oSource, void * oTarget) :
        fn(fn1), msg(iMsg), tag(iTag), source(oSource), target(oTarget), data((char*)pData, (char*)pData+iLength) { }
    fn_void_msg_tlv_t fn;
    int msg;
    int tag;
    void * source;
    void * target;
    vector<char> data;
};

#define f_iProcessCallBackCount (1024 * 5)

ProcessCallBack * * fn_oProcessCallBacks()
{
    static vector<ProcessCallBack*> m(f_iProcessCallBackCount, (ProcessCallBack*)NULL);
    return & m.front();
}

CxMutex * fn_oProcessCallBackPushLock()
{
    static CxMutex m;
    return & m;
}

static ProcessCallBack * * const f_oProcessCallBacks = fn_oProcessCallBacks();
static ProcessCallBack * * const f_oProcessCallBackEnd = fn_oProcessCallBacks()+f_iProcessCallBackCount;
static CxMutex * f_oProcessCallBackPushLock = fn_oProcessCallBackPushLock();
static int f_iProcessCallBackIndexPush = 0;
static int f_iProcessCallBackIndexPop = 0;


static int f_iProjectType = 0;
static int volatile f_iApplicationStatus = 0;

static string f_sApplicationFilePath = "";
static string f_sApplicationFileName = "";
static string f_sApplicationTargetName = "";
static string f_sApplicationPath = "";
static string f_sApplicationDeployPath = "";
static string f_sApplicationConfigPath = "";
static string f_sApplicationConfigTargetFilePath = "";
static string f_sDefaultConfigFilePath = "";
static string f_sApplicationConfigFilePath = "";
static string f_sApplicationConfigExtendFilePath = "";

static map<string, string> f_arguments;
static map<string, map<string, string> > f_appsConfigs;

static int f_iProcessEventsSleep = -1;

static cx_pthread_t f_applicationThreadId = 0;
static pid_os_t f_applicationProcessId;

static msepoch_t f_dtApplicationInit = 0;
static msepoch_t f_dtApplicationStart = 0;
static msepoch_t f_dtApplicationExecute = 0;


//application abort raise signal


#ifdef GM_OS_WIN

long   __stdcall   fn_Windows_Exception_callback(_EXCEPTION_POINTERS*   excp)
{
    string sMsg = "Windows_Exception(Unhandled) : [CxApplication will exit]";
    sMsg += "\nException_Time= " + CxTime::currentMsepochString();
    sMsg += CxString::format("\nException_Address= %x" , excp->ExceptionRecord->ExceptionAddress);
    sMsg += "\nCPU_Register: " + CxTime::currentMsepochString();
    sMsg += CxString::format("\nException_Time=eax   %x   ebx   %x   ecx   %x   edx   %x" + excp->ContextRecord->Eax,
                             excp->ContextRecord->Ebx,excp->ContextRecord->Ecx,
                             excp->ContextRecord->Edx);
    cxPrompt() << sMsg;

    CxApplication::exit();

    return   EXCEPTION_EXECUTE_HANDLER;
}

#endif

void fn_app_SIGABRT (int param)
{
    cxWarning() << "signal signal signal !!! CxApplication Received SIGABRT , param=" << param;
    cout        << "signal signal signal !!! CxApplication Received SIGABRT , param=" << param << endl;
    CxApplication::exit();
}
void fn_app_SIGFPE (int param)
{
    cxWarning() << "signal signal signal !!! CxApplication Received SIGFPE , param=" << param;
    cout        << "signal signal signal !!! CxApplication Received SIGFPE , param=" << param << endl;
    CxApplication::exit();
}
void fn_app_SIGILL (int param)
{
    cxWarning() << "signal signal signal !!! CxApplication Received SIGILL , param=" << param;
    cout        << "signal signal signal !!! CxApplication Received SIGILL , param=" << param << endl;
    CxApplication::exit();
}
void fn_app_SIGINT (int param)
{
    cxWarning() << "signal signal signal !!! CxApplication Received SIGINT , param=" << param;
    cout        << "signal signal signal !!! CxApplication Received SIGINT , param=" << param << endl;
    CxApplication::exit();
}
void fn_app_SIGSEGV (int param)
{
    cxWarning() << "signal signal signal !!! CxApplication Received SIGSEGV , param=" << param;
    cout        << "signal signal signal !!! CxApplication Received SIGSEGV , param=" << param << endl;
    CxApplication::exit();
}
void fn_app_SIGTERM (int param)
{
    cxWarning() << "signal signal signal !!! CxApplication Received SIGTERM , param=" << param;
    cout        << "signal signal signal !!! CxApplication Received SIGTERM , param=" << param << endl;
    CxApplication::exit();
}




const string &CxApplication::applicationFilePath()
{
    return f_sApplicationFilePath;
}

const string &CxApplication::applicationFileName()
{
    return f_sApplicationFileName;
}

const string &CxApplication::applicationTargetName()
{
    return f_sApplicationTargetName;
}

const string &CxApplication::applicationPath()
{
    return f_sApplicationPath;
}

const string &CxApplication::applicationDeployPath()
{
    return f_sApplicationDeployPath;
}

void CxApplication::setApplicationDeployPath(const string &value)
{
    if (f_sApplicationDeployPath.empty())
        f_sApplicationDeployPath = value;
}

string CxApplication::applicationRelativePath(const string &sDirName)
{
    return CxFileSystem::mergeFilePath(f_sApplicationDeployPath, sDirName);
}

const string &CxApplication::configPath()
{
    return f_sApplicationConfigPath;
}

const string &CxApplication::configTargetFilePath()
{
    return f_sApplicationConfigTargetFilePath;
}

const string &CxApplication::configDefaultFilePath()
{
    return f_sDefaultConfigFilePath;
}

const string &CxApplication::configFilePath()
{
    return f_sApplicationConfigFilePath;
}

const string &CxApplication::configExtendFilePath()
{
    return f_sApplicationConfigExtendFilePath;
}

const map<string, string> &CxApplication::getArguments()
{
    return f_arguments;
}

string CxApplication::findArgument(const string &sArgumentName)
{
    map<string, string>::const_iterator it = f_arguments.find(sArgumentName);
    if (it != f_arguments.end())
        return it->second;
    else
        return std::string();
}

string CxApplication::argumentsToString(const map<string, string> &sArguments)
{
    string r;
    for (map<string, string>::const_iterator it=sArguments.begin(); it!=sArguments.end(); ++it)
    {
        r.push_back(CxGlobal::argumentCharacter);
        r.append(it->first);
        r.push_back(' ');
        r.append(it->second);
        r.push_back(' ');
    }
    if (r.size() > 0) r.resize(r.size()-1);
    return r;
}

map<string, string> CxApplication::argumentsFromString(const string & sArg)
{
    vector<string> sArgs = CxString::split(sArg, ' ');
    map<string, string> r;
    size_t i = 0;
    while (i < sArgs.size() && sArgs[i][0] == CxGlobal::argumentCharacter)
    {
        char * pc = const_cast<char *>(sArgs.at(i).c_str());
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
        sValue.resize(sValue.size()-1);
        r[sName] = sValue;
    }
    return r;
}

map<string, string> CxApplication::argumentsFromArgcv(int argc, char *argv[])
{
    map<string, string> r;
    int i = 1;
    while (i < argc && *argv[i] == CxGlobal::argumentCharacter)
    {
        char * pc = argv[i];
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
        sValue.resize(sValue.size()-1);
        r[sName] = sValue;
    }
    return r;
}

void CxApplication::restartSystem()
{
#ifdef GM_OS_WIN
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    LUID luid;

    // Get version info to determine operation
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (GetVersionEx(&osvi) == 0)
    {
        return;
    }
    // Determine the platform
    if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        // Windows NT 3.51, Windows NT 4.0, Windows 2000,
        // Windows XP, or Windows .NET Server
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        {

        }
        else
        {
            LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);

            tkp.PrivilegeCount = 1;  // one privilege to set
            tkp.Privileges[0].Luid = luid;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, 0);
        }
        ExitWindowsEx(EWX_REBOOT, 0);
    }
#else
    system("reboot");
#endif
}

const std::string & CxApplication::doFindConfig(const string &sSection, const string &sKey, const std::string & sDefault, bool bSaveNew)
{
    for (map<string, map<string, string> >::iterator itSection = f_appsConfigs.begin(); itSection != f_appsConfigs.end(); ++itSection)
    {
        if ( CxString::equalCase(itSection->first ,sSection) )
        {
            map<string, string> & keys = itSection->second;
            for (map<string, string>::const_iterator itKey = keys.begin(); itKey != keys.end() ; ++itKey)
            {
                if ( CxString::equalCase(itKey->first, sKey) )
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
    }
    return sDefault;
}

void CxApplication::saveConfig(const string &sSection, const string &sKey, const std::string & sValue)
{
    map<string, map<string, string> >::iterator itSection = f_appsConfigs.find(sSection);
    if (itSection != f_appsConfigs.end())
    {
        map<string, string> & keys = itSection->second;
        keys[sKey] = sValue;
    }
    else
    {
        map<string, string> keys;
        keys[sKey] = sValue;
        f_appsConfigs[sSection] = keys;
    }
}

bool CxApplication::isApplicationThread()
{
    return cx_pthread_self() == f_applicationThreadId;
}

cx_pthread_t CxApplication::applicationThreadId()
{
    return f_applicationThreadId;
}

pid_os_t CxApplication::applicationProcessId()
{
    return f_applicationProcessId;
}

int CxApplication::getCallBackCount()
{
    return f_iProcessCallBackIndexPush - f_iProcessCallBackIndexPop;
}

std::map<string, std::map<string, string> > CxApplication::toRelativePath(const std::map<string, std::map<string, string> > & sections)
{
    std::map<string, std::map<string, string> > r;
    for (map<string, map<string, string> >::const_iterator itSection = sections.begin(); itSection != sections.end(); ++itSection)
    {
        map<string, string> rKeys;
        const map<string, string> & keys = itSection->second;
        for (map<string, string>::const_iterator itKeys = keys.begin(); itKeys != keys.end(); ++itKeys)
        {
            const string & sValue = itKeys->second;
            bool bOk;
            string sFilePath = CxFileSystem::relativeFilePath(f_sApplicationPath, sValue, & bOk);
            if (bOk && sFilePath.size()>0)
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

const map<string, string> & CxApplication::findConfigs(const string &sSection)
{
    for (map<string, map<string, string> >::iterator itSection = f_appsConfigs.begin(); itSection != f_appsConfigs.end(); ++itSection)
    {
        if ( itSection->first == sSection )
        {
            return itSection->second;
        }
    }
    return CxGlobal::emptyMapString;
}

int CxApplication::saveConfig(const string &sFilePath)
{
    string sSaveFilePath = sFilePath;
    if (sSaveFilePath.empty()) sSaveFilePath = configFilePath();
    std::map<std::string, std::map<std::string, std::string> > sConfigs = toRelativePath(f_appsConfigs);
    return CxSkverManager::save(sConfigs, sSaveFilePath);
}

void CxApplication::pushProcessCallBack(fn_void_msg_tlv_t fn, int iMsg, int iTag, const void * pData, int iLength, void * oSource, void * oTarget)
{
    CxMutexScope lock(f_oProcessCallBackPushLock);
    if (f_oProcessCallBacks[f_iProcessCallBackIndexPush] != NULL)
    {
        cxWarning() << "error error error !!! CxApplication Cache Full (f_oProcessCallBacks[f_iProcessCallBackIndexPush] != NULL) , Can not receive push !!! reset();";
        cout        << "error error error !!! CxApplication Cache Full (f_oProcessCallBacks[f_iProcessCallBackIndexPush] != NULL) , Can not receive push !!! reset();" << endl;
        reset();
    }
    else
    {
        //有数据，并且少于 4 M
        if (iLength>0 && iLength<1024*1024*4 && pData)
        {
            f_oProcessCallBacks[f_iProcessCallBackIndexPush] = new ProcessCallBack(fn, iMsg, iTag, iLength, pData, oSource, oTarget);
        }
        else
        {
            f_oProcessCallBacks[f_iProcessCallBackIndexPush] = new ProcessCallBack(fn, iMsg, iTag, oSource, oTarget);
        }
        f_iProcessCallBackIndexPush ++;
        if (f_iProcessCallBackIndexPush >= f_iProcessCallBackCount)
            f_iProcessCallBackIndexPush = 0;
    }
}

void CxApplication::nullProcessCallBackSource(void *oSource)
{
    //只能运行在 applicationThread
    if (isApplicationThread())
    {
        CxMutexScope lock(f_oProcessCallBackPushLock);
        ProcessCallBack * * pOProcessCallBack = f_oProcessCallBacks;
        ProcessCallBack * oProcessCallBack = NULL;
        while (pOProcessCallBack < f_oProcessCallBackEnd)
        {
            oProcessCallBack = * pOProcessCallBack;
            if (oProcessCallBack && oProcessCallBack->source == oSource)
            {
                oProcessCallBack->source = NULL;
            }
            pOProcessCallBack ++;
        }
    }
}

void CxApplication::nullProcessCallBackTarget(void *oTarget)
{
    //只能运行在 applicationThread
    if (isApplicationThread())
    {
        CxMutexScope lock(f_oProcessCallBackPushLock);
        ProcessCallBack * * pOProcessCallBack = f_oProcessCallBacks;
        ProcessCallBack * oProcessCallBack = NULL;
        while (pOProcessCallBack < f_oProcessCallBackEnd)
        {
            oProcessCallBack = * pOProcessCallBack;
            if (oProcessCallBack && oProcessCallBack->target == oTarget)
            {
                oProcessCallBack->target = NULL;
            }
            pOProcessCallBack ++;
        }
    }
}

void CxApplication::init(int argc, char *argv[], int iProjectType)
{
#ifdef GM_OS_WIN
    ::SetUnhandledExceptionFilter(fn_Windows_Exception_callback);
#endif

    if (f_iApplicationStatus>0)
    {
        return;
    }

    f_dtApplicationInit = CxTime::currentMsepoch();

    f_iProjectType = iProjectType;

    f_applicationThreadId = cx_pthread_self();

    if ( argc > 1 )
    {
        f_arguments = argumentsFromArgcv(argc, argv);
    }
    if ( argc > 0 )
    {
        f_sApplicationFilePath = argv[0];
        f_sApplicationPath = CxFileSystem::extractPath(f_sApplicationFilePath);
        f_sApplicationFileName = CxFileSystem::extractFileName(f_sApplicationFilePath);
        f_sApplicationTargetName = CxFileSystem::extractFilePrefixName(f_sApplicationFilePath);
        if (f_sApplicationDeployPath.empty())
        {
            f_sApplicationDeployPath = CxFileSystem::parentPath( f_sApplicationPath );
        }
        f_sApplicationConfigPath = CxFileSystem::mergeFilePath( f_sApplicationDeployPath , "config" );
        f_sApplicationConfigTargetFilePath = CxFileSystem::mergeFilePath( f_sApplicationConfigPath , f_sApplicationTargetName );
        f_sDefaultConfigFilePath = CxFileSystem::mergeFilePath( f_sApplicationConfigPath, "apps.config.ini");
        if (! CxFileSystem::isExist( f_sDefaultConfigFilePath ) &&
                CxFileSystem::isExist( CxFileSystem::mergeFilePath( f_sApplicationConfigPath, "app.config.ini") ) )
        {
            f_sDefaultConfigFilePath = CxFileSystem::mergeFilePath( f_sApplicationConfigPath, "app.config.ini");
        }
        f_sApplicationConfigFilePath = findArgument("cfp");
        if (f_sApplicationConfigFilePath.empty())
        {
            f_sApplicationConfigFilePath = CxFileSystem::mergeFilePath( f_sApplicationConfigPath, f_sApplicationTargetName + ".config.ini");
        }
        f_sApplicationConfigExtendFilePath = findArgument("sfp");
        if (f_sApplicationConfigExtendFilePath.empty())
        {
            f_sApplicationConfigExtendFilePath = CxFileSystem::mergeFilePath( f_sApplicationConfigPath, f_sApplicationTargetName + ".extend.xml");
        }
        CxSkverIni skvDefault(f_sDefaultConfigFilePath);
        CxSkverIni skvApps(f_sApplicationConfigFilePath);
        skvDefault.updateSectionEntryValues(skvApps.getSectionEntryValues());
        f_appsConfigs = skvDefault.getSectionEntryValues();
        f_iProcessEventsSleep = findConfig(CS_SectionProgramConfig, CS_EntryProcessEventsSleep, f_iProcessEventsSleep);
    }

    //every init function : do
    vector<fn_void_t> * oFnInits = getFnInits();
    for (size_t i = 0; i < oFnInits->size(); ++i)
    {
        fn_void_t fn = oFnInits->at(i);
        fn();
    }

    f_iApplicationStatus = 1;
}

void CxApplication::registInitFunction(fn_void_t fn)
{
    vector<fn_void_t> * oFnInits = getFnInits();
    if (std::find(oFnInits->begin(), oFnInits->end(), fn) == oFnInits->end())
        oFnInits->push_back(fn);
}

void CxApplication::registStartFunction(fn_void_t fn, int iOrder)
{
    vector<std::pair<fn_void_t, int> > * oFnStarts = getFnStarts();
    bool bHas = false;
    for (size_t i = 0; i < oFnStarts->size(); ++i)
    {
        const std::pair<fn_void_t, int> & fnInfo = oFnStarts->at(i);
        if (fnInfo.first == fn)
        {
            bHas = true;
            break;
        }
    }
    if (! bHas)
    {
        bool bInsert = false;
        for (vector<std::pair<fn_void_t, int> >::iterator it = oFnStarts->begin(); it != oFnStarts->end(); ++it)
        {
            const std::pair<fn_void_t, int> & fnInfo = * it;
            if (iOrder < fnInfo.second)
            {
                oFnStarts->insert(it, std::pair<fn_void_t, int>(fn, iOrder));
                bInsert = true;
                break;
            }
        }
        if (! bInsert)
        {
            oFnStarts->insert(oFnStarts->end(), std::pair<fn_void_t, int>(fn, iOrder));
        }
    }
}

void CxApplication::registStopFunction(fn_void_t fn)
{
    vector<fn_void_t> * oFnStops = getFnStops();
    if (std::find(oFnStops->begin(), oFnStops->end(), fn) == oFnStops->end())
        oFnStops->push_back(fn);
}

int CxApplication::projectType()
{
    return f_iProjectType;
}

int CxApplication::exec(int iTag)
{
    if (f_iApplicationStatus != 1)
        return false;

    f_dtApplicationStart = CxTime::currentMsepoch();

    f_applicationThreadId = cx_pthread_self();
    f_applicationProcessId = CxProcess::getCurrentPid();

    //every init function : do
    f_dtApplicationExecute = CxTime::currentMsepoch();

    vector<std::pair<fn_void_t, int> > * oFnStarts = getFnStarts();
    for (vector<std::pair<fn_void_t, int> >::const_iterator it = oFnStarts->begin(); it != oFnStarts->end(); ++it)
    {
        const std::pair<fn_void_t, int> & fnInfo = * it;
        fn_void_t fn = fnInfo.first;
        fn();
    }

    //*running
    f_iApplicationStatus = 2;

    cxPrompt() << f_sApplicationTargetName << " begin :";

    cxPrompt() << "init     datetime=" << CxTime::toString(f_dtApplicationInit);
    cxPrompt() << "start    datetime=" << CxTime::toString(f_dtApplicationStart);
    cxPrompt() << "execute  datetime=" << CxTime::toString(f_dtApplicationExecute);

    cxPrompt() << "..." << CxGlobal::lineString << CxGlobal::lineString << CxGlobal::lineString;

    fn_void_int_t fn_sig_handler;
    fn_sig_handler = signal (SIGABRT, fn_app_SIGABRT);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGINT , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGFPE, fn_app_SIGFPE);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGFPE , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGILL, fn_app_SIGILL);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGILL , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGINT, fn_app_SIGINT);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGINT , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGSEGV, fn_app_SIGSEGV);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGSEGV , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGTERM, fn_app_SIGTERM);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGTERM , *)" << std::endl;
    }

    atexit(CxApplication::exit);

    try
    {
        doLoopEvents(iTag);
    }
    catch (...)
    {
        cxPrompt() << "Unknow_Exception(by throw and catch) : [CxApplication will exit] at " << CxTime::currentMsepochString();
    }

    vector<fn_void_t> * oFnStops = getFnStops();
    for (size_t i = 0; i < oFnStops->size(); ++i)
    {
        fn_void_t fn = oFnStops->at(i);
        fn();
    }

    if (iTag & 0x00000001)
    {
        saveConfig();
    }

    return true;
}

void CxApplication::runProcessCallBacks()
{
    do
    {
        //判断是否收到 push 的 ProcessCallBack
        if (f_oProcessCallBacks[f_iProcessCallBackIndexPop] != NULL)
        {
            //复制指针
            ProcessCallBack * oProcessCallBack = f_oProcessCallBacks[f_iProcessCallBackIndexPop];
            f_oProcessCallBacks[f_iProcessCallBackIndexPop] = NULL;
            f_iProcessCallBackIndexPop ++;
            if (f_iProcessCallBackIndexPop >= f_iProcessCallBackCount)
                f_iProcessCallBackIndexPop = 0;

            if (oProcessCallBack->fn)
            {
                if (oProcessCallBack->data.size() > 0)
                    oProcessCallBack->fn(oProcessCallBack->msg, oProcessCallBack->tag, & oProcessCallBack->data.front(), oProcessCallBack->data.size(), oProcessCallBack->source, oProcessCallBack->target);
                else
                    oProcessCallBack->fn(oProcessCallBack->msg, oProcessCallBack->tag, 0, 0, oProcessCallBack->source, oProcessCallBack->target);
            }
            delete oProcessCallBack;
        }
        else
        {
            break;
        }
    } while (true);
}

void CxApplication::waiting(msepoch_t iTimeOut)
{
    if (f_applicationThreadId == cx_pthread_self())
    {
        msepoch_t dtNow = CxTime::currentSystemTime();
        if(dtNow > 0)
        {
            msepoch_t iG = CxTime::currentSystemTime() - dtNow;
            while(iG < iTimeOut)
            {
                iG = CxTime::currentSystemTime() - dtNow;
                CxApplication::processEvents();
                CxThread::sleep(1);
            }
        }
    }
    else
    {
        msepoch_t dtNow = CxTime::currentSystemTime();
        if(dtNow > 0)
        {
            msepoch_t iG = CxTime::currentSystemTime() - dtNow;
            while(iG < iTimeOut)
            {
                iG = CxTime::currentSystemTime() - dtNow;
                CxThread::sleep(1);
            }
        }
    }
}

int CxApplication::waiting(int iWaitMilliseconds, fn_int_void_t fnCheck, int iSleep)
{
    if (f_applicationThreadId == cx_pthread_self())
    {
        msepoch_t dtNow = CxTime::currentSystemTime();
        msepoch_t iG = 0;
        if(dtNow > 0)
        {
            iG = CxTime::currentSystemTime() - dtNow;
            while(iG < iWaitMilliseconds)
            {
                iG = CxTime::currentSystemTime() - dtNow;
                CxApplication::processEvents();
                if (fnCheck && fnCheck() > 0) break;
                CxThread::sleep(iSleep);
            }
        }
        return iG;
    }
    else
    {
        msepoch_t dtNow = CxTime::currentSystemTime();
        msepoch_t iG = 0;
        if(dtNow > 0)
        {
            iG = CxTime::currentSystemTime() - dtNow;
            while(iG < iWaitMilliseconds)
            {
                iG = CxTime::currentSystemTime() - dtNow;
                if (fnCheck && fnCheck() > 0) break;
                CxThread::sleep(iSleep);
            }
        }
        return iG;
    }
}

void CxApplication::exit()
{
    if (f_iApplicationStatus !=0)
    {
        doExit();
        f_iApplicationStatus = 0;
    }
}

int CxApplication::applicationStatus()
{
    return f_iApplicationStatus;
}

CxMutex *CxApplication::processCallBacksLock()
{
    return f_oProcessCallBackPushLock;
}

std::vector<fn_void_t> * CxApplication::getFnInits()
{
    static vector<fn_void_t> fnInits;
    return & fnInits;
}

std::vector<std::pair<fn_void_t, int> > * CxApplication::getFnStarts()
{
    static vector<std::pair<fn_void_t, int> > fnStarts;
    return & fnStarts;
}

std::vector<fn_void_t> * CxApplication::getFnStops()
{
    static vector<fn_void_t> fnStops;
    return & fnStops;
}



#ifdef CCXX_QT


CxApplicationReceiver f_applicationReceiver;


CxApplicationReceiver::CxApplicationReceiver(QObject *parent)
    : QObject(parent)
{
}

CxApplicationReceiver::~CxApplicationReceiver()
{
}

bool CxApplicationReceiver::event(QEvent * oEvent)
{
    CxApplication::runProcessCallBacks();
    return true;
}



int CxApplication::doLoopEvents(int iTag)
{
    return QApplication::exec();
}

void CxApplication::signalMainThread()
{
    QApplication::postEvent(& f_applicationReceiver, new QEvent(QEvent::User));
}

void CxApplication::processEvents()
{
    QApplication::processEvents();
}

void CxApplication::doExit()
{
    QApplication::exit();
}

void CxApplication::reset()
{
}

#else

static CxSingleWait * f_oSingleWait = new CxSingleWait();

int CxApplication::doLoopEvents(int iTag)
{
    while (f_iApplicationStatus)
    {
        runProcessCallBacks();
        f_oSingleWait->wait();
    }
    return true;
}

void CxApplication::signalMainThread()
{
    f_oSingleWait->signal();
}

void CxApplication::processEvents()
{
    runProcessCallBacks();
    if (f_iProcessEventsSleep > -1)
    {
        CxThread::sleep(f_iProcessEventsSleep);
    }
}

void CxApplication::doExit()
{
    signalMainThread();
}

void CxApplication::reset()
{
    delete f_oSingleWait;
    f_oSingleWait = new CxSingleWait();
}

#endif
