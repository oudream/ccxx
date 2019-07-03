#include "cxapplication.h"

#include "cxprocess.h"
#include "cxinterinfo.h"
#include "cxlog.h"
#include "cxsystem.h"

using namespace std;

struct ProcessCallBack
{
    ProcessCallBack(fn_void_msg_tlv_t fn1)
        : fn(fn1), msg(0), tag(0), source(0), target(0), data(), hadRun(false) {}
    ProcessCallBack(fn_void_msg_tlv_t fn1, int iMsg, int iTag, void * oSource, void * oTarget)
        : fn(fn1), msg(iMsg), tag(iTag), source(oSource), target(oTarget), data(), hadRun(false) {}
    ProcessCallBack(fn_void_msg_tlv_t fn1, int iMsg, int iTag, int iLength, const void * pData, void * oSource, void * oTarget)
        : fn(fn1), msg(iMsg), tag(iTag), source(oSource), target(oTarget), data((char*)pData, (char*)pData+iLength), hadRun(false) { }
    fn_void_msg_tlv_t fn;
    int msg;
    int tag;
    void * source;
    void * target;
    vector<char> data;
    volatile bool hadRun;
};

#define f_iProcessCallBackCount (1024 * 4 * 3)

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
static ProcessCallBack * * const f_oProcessCallBackEnd = fn_oProcessCallBacks() + f_iProcessCallBackCount;
static CxMutex * f_oProcessCallBackPushLock = fn_oProcessCallBackPushLock();
static int f_iProcessCallBackIndexPush = 0;
static int f_iProcessCallBackIndexPop = 0;
static int f_iProcessCallBackIndexClear = 0;
static ProcessCallBack * f_oRunningProcessCallBack = NULL;

static int f_iProjectType = 0;
static int volatile f_iApplicationStatus = 0;
static bool f_bApplicationHasStop = false;

static int f_iProcessEventsSleep = -1;

static cx_pthread_t f_applicationThreadId = 0;
static pid_os_t f_applicationProcessId;

static msepoch_t f_dtApplicationInit = 0;
static msepoch_t f_dtApplicationStart = 0;
static msepoch_t f_dtApplicationExecute = 0;


//application abort raise signal


#ifdef GM_OS_WIN
long   __stdcall   fn_Windows_Exception_callback(EXCEPTION_POINTERS* excp)
{
    string sMsg = "Windows_Exception(Unhandled) : [CxApplication will exit]";
    sMsg += "\nException_Time= " + CxTime::currentSystemTimeString();
    sMsg += CxString::format("\nException_Address= %x" , excp->ExceptionRecord->ExceptionAddress);
    sMsg += "\nCPU_Register: " + CxTime::currentSystemTimeString();
    sMsg += CxString::format("\nException_Time=eax   %x   ebx   %x   ecx   %x   edx   %x",
        excp->ContextRecord->SegGs,
        excp->ContextRecord->SegFs,
		excp->ContextRecord->SegEs,
        excp->ContextRecord->SegDs);
    cxPrompt() << sMsg;

#ifdef _MSC_VER
	CxThread::createMiniDump(excp);
#endif

    CxApplication::exit();
    return   EXCEPTION_EXECUTE_HANDLER;
}

#ifdef _MSC_VER
int fn_vc_exec(int iTag)
{
    int iResult = 0;
    __try
    {
        CxApplication::doLoopEvents(iTag);
        iResult = TRUE;
    }
    __except (CxThread::createMiniDump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
    {
        iResult = -1;
    }
    return iResult;
}
#endif

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

void CxApplication::pushProcessCallBack(fn_void_msg_tlv_t fn, int iMsg, int iTag, const void * pData, int iLength, void * oSource, void * oTarget, bool bOnlyOnePending)
{
    if (f_iApplicationStatus)
    {
        CxMutexScope lock(f_oProcessCallBackPushLock);
        ProcessCallBack * oProcessCallBack = NULL;
        //*only one waiting run
        if (bOnlyOnePending)
        {
            int iIndex = f_iProcessCallBackIndexClear;
            while (iIndex != f_iProcessCallBackIndexPush)
            {
                oProcessCallBack = f_oProcessCallBacks[iIndex];
                if (!oProcessCallBack->hadRun && oProcessCallBack->fn == fn)
                {
                    //has one in waiting run
                    return;
                }
                iIndex ++;
                if (iIndex >= f_iProcessCallBackCount)
                    iIndex = 0;
            }
        }
        //*clear
        while (f_iProcessCallBackIndexClear != f_iProcessCallBackIndexPush)
        {
            oProcessCallBack = f_oProcessCallBacks[f_iProcessCallBackIndexClear];
            if (oProcessCallBack->hadRun)
            {
                delete oProcessCallBack;
                f_oProcessCallBacks[f_iProcessCallBackIndexClear] = NULL;
                f_iProcessCallBackIndexClear ++;
                if (f_iProcessCallBackIndexClear >= f_iProcessCallBackCount)
                    f_iProcessCallBackIndexClear = 0;
            }
            else
            {
                break;
            }
        }
        //*push
        if (f_oProcessCallBacks[f_iProcessCallBackIndexPush] != NULL)
        {
            cxWarning() << "Error ! Error ! Error ! CxApplication Cache Full. f_iProcessCallBackIndexPush=" << f_iProcessCallBackIndexPush << "; f_iProcessCallBackIndexPop=" << f_iProcessCallBackIndexPop;
            cout        << "Error ! Error ! Error ! CxApplication Cache Full. f_iProcessCallBackIndexPush=" << f_iProcessCallBackIndexPush << "; f_iProcessCallBackIndexPop=" << f_iProcessCallBackIndexPop << endl;
            if (f_oRunningProcessCallBack)
            {
                cxWarning() << "Current CallBack : fn=" << (f_oRunningProcessCallBack->fn!=0) << "; msg=" << f_oRunningProcessCallBack->msg << "; tag=" << f_oRunningProcessCallBack->tag << "; source=" << (f_oRunningProcessCallBack->source!=0) << "; target=" << f_oRunningProcessCallBack->target
                            << "; data_size=" << f_oRunningProcessCallBack->data.size() << "; data=" << CxString::toHexstring(f_oRunningProcessCallBack->data) << ".";
            }
            else
            {
                cxWarning() << "Current CallBack is NULL.";
            }
            CxApplication::exit();
            return;
        }
        else
        {
            // has data and < 4M
            if (iLength>0)
            {
                if (iLength<1024*1024*4 && pData)
                {
                    f_oProcessCallBacks[f_iProcessCallBackIndexPush] = new ProcessCallBack(fn, iMsg, iTag, iLength, pData, oSource, oTarget);
                }
                else
                {
                    cxWarning() << "Error ! Error ! Error ! CxApplication::pushProcessCallBack, but iLength=" << iLength;
                    cout        << "Error ! Error ! Error ! CxApplication::pushProcessCallBack, but iLength=" << iLength << endl;
                    return;
                }
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
}

void CxApplication::nullProcessCallBackSource(void *oSource)
{
    CxMutexScope lock(f_oProcessCallBackPushLock);
    ProcessCallBack * oProcessCallBack = NULL;
    int iIndex = f_iProcessCallBackIndexClear;
    while (iIndex != f_iProcessCallBackIndexPush)
    {
        oProcessCallBack = f_oProcessCallBacks[iIndex];
        if (oProcessCallBack->source == oSource)
        {
            oProcessCallBack->source = NULL;
        }
        iIndex ++;
        if (iIndex >= f_iProcessCallBackCount)
            iIndex = 0;
    }
}

void CxApplication::nullProcessCallBackTarget(void *oTarget)
{
    CxMutexScope lock(f_oProcessCallBackPushLock);
    ProcessCallBack * oProcessCallBack = NULL;
    int iIndex = f_iProcessCallBackIndexClear;
    while (iIndex != f_iProcessCallBackIndexPush)
    {
        oProcessCallBack = f_oProcessCallBacks[iIndex];
        if (oProcessCallBack->target == oTarget)
        {
            oProcessCallBack->target = NULL;
        }
        iIndex ++;
        if (iIndex >= f_iProcessCallBackCount)
            iIndex = 0;
    }
}

void CxApplication::init(int argc, const char *argv[], int iProjectType)
{
    CxGlobal::assertCallBack = CxApplication::assertDeal;

    f_iProjectType = iProjectType;

#ifdef GM_OS_WIN
    ::SetUnhandledExceptionFilter(fn_Windows_Exception_callback);
#endif

    if (f_iApplicationStatus>0)
    {
        return;
    }

    f_dtApplicationInit = CxTime::currentSystemTime();

    f_applicationProcessId = CxProcess::getCurrentPid();

    f_applicationThreadId = cx_pthread_self();

    CxAppEnv::init(argc, argv);

    if (iProjectType != GM_PROJECT_TYPE_LIB_DLL && iProjectType != GM_PROJECT_TYPE_LIB_STATIC)
    {
        CxThread::setRunInTry(true);
#ifdef _MSC_VER
        CxThread::setMiniDumpFilePath(CxAppEnv::applicationFilePath() + ".MiniDump.dmp");
#endif
    }
    f_iProcessEventsSleep = CxAppEnv::findConfig(CS_SectionProgramConfig, CS_EntryProcessEventsSleep, f_iProcessEventsSleep);

    if (f_iProjectType == GM_PROJECT_TYPE_APP_CONSOLE)
    {
        CxInterinfo::startInterInfo(CxApplication::pushProcessCallBack, CxApplication::signalMainThread);
    }

    CxLogManager::startLog();

    //every init function : do
    vector<fn_void_t> * oFnInits = getFnInits();
    for (size_t i = 0; i < oFnInits->size(); ++i)
    {
        fn_void_t fn = oFnInits->at(i);
        fn();
    }

    cxPrompt() << "ApplicationFilePath: " << CxAppEnv::applicationFilePath();

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
	int iResult = 0;

    if (f_iApplicationStatus != 1)
		return iResult;

    f_dtApplicationStart = CxTime::currentSystemTime();

    f_applicationThreadId = cx_pthread_self();

    //every init function : do
    vector<std::pair<fn_void_t, int> > * oFnStarts = getFnStarts();
    for (vector<std::pair<fn_void_t, int> >::const_iterator it = oFnStarts->begin(); it != oFnStarts->end(); ++it)
    {
        const std::pair<fn_void_t, int> & fnInfo = * it;
        fn_void_t fn = fnInfo.first;
        fn();
    }

    f_dtApplicationExecute = CxTime::currentSystemTime();

    //*running
    f_iApplicationStatus = 2;

    cxPrompt() << applicationTargetName() << " begin :";

    cxPrompt() << "init     datetime=" << CxTime::toString(f_dtApplicationInit);
    cxPrompt() << "start    datetime=" << CxTime::toString(f_dtApplicationStart);
    cxPrompt() << "execute  datetime=" << CxTime::toString(f_dtApplicationExecute);

    cxPrompt() << "..." << CxGlobal::lineString << CxGlobal::lineString << CxGlobal::lineString;


//to CxApplication::assertDeal
//to fn_Windows_Exception_callback
    fn_void_int_t fn_sig_handler;
//    fn_sig_handler = signal (SIGABRT, fn_app_SIGABRT);
//    if (fn_sig_handler == SIG_ERR)
//    {
//        std::cout << "error : signal (SIGINT , *)" << std::endl;
//    }
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

#ifdef _MSC_VER
	iResult = fn_vc_exec(iTag);
#else
    try
    {
		iResult = doLoopEvents(iTag);
    }
    catch (...)
    {
    }
#endif
	if (iResult > 0)
	{
		cxDebug() << "CxApplication::doLoopEvents end " << CxTime::currentSystemTimeString();
	}
	else
	{
		cxPrompt() << "Unknow_Exception(by throw and catch) : [CxApplication will exit] at " << CxTime::currentSystemTimeString();
	}
    if (! f_bApplicationHasStop)
    {
        f_bApplicationHasStop = true;
        raiseFnStops();
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
        // push 's ProcessCallBack
        if (f_oProcessCallBacks[f_iProcessCallBackIndexPop] != NULL)
        {
            // copy
            f_oRunningProcessCallBack = f_oProcessCallBacks[f_iProcessCallBackIndexPop];
            f_iProcessCallBackIndexPop ++;
            if (f_iProcessCallBackIndexPop >= f_iProcessCallBackCount)
                f_iProcessCallBackIndexPop = 0;

            if (f_oRunningProcessCallBack->fn)
            {
                if (f_oRunningProcessCallBack->data.size() > 0)
                    f_oRunningProcessCallBack->fn(f_oRunningProcessCallBack->msg, f_oRunningProcessCallBack->tag, & f_oRunningProcessCallBack->data.front(), f_oRunningProcessCallBack->data.size(), f_oRunningProcessCallBack->source, f_oRunningProcessCallBack->target);
                else
                    f_oRunningProcessCallBack->fn(f_oRunningProcessCallBack->msg, f_oRunningProcessCallBack->tag, 0, 0, f_oRunningProcessCallBack->source, f_oRunningProcessCallBack->target);
            }
            f_oRunningProcessCallBack->hadRun = true;
            f_oRunningProcessCallBack = NULL;
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
        f_iApplicationStatus = 0;

        CxInterinfo::stopInterInfo();
        CxLogManager::stopLog();

        std::cout << "CxApplication::doExit()." << std::endl;
        doExit();
    }
}

msepoch_t CxApplication::applicationInitTime()
{
    return f_dtApplicationInit;
}

msepoch_t CxApplication::applicationStartTime()
{
    return f_dtApplicationStart;
}

msepoch_t CxApplication::applicationExecuteTime()
{
    return f_dtApplicationExecute;
}

int CxApplication::applicationStatus()
{
    return f_iApplicationStatus;
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

void CxApplication::assertDeal(int, int, const void *_Expression, int _line, void *_file, void *)
{
    string sFilePath = CxFileSystem::mergeFilePath( CxAppEnv::logPath(), "error.log" );
    string dtNow = CxTime::currentSystemTimeString();
    string sHead = CxString::format("assert\r\n%s\r\n%s\r\n", dtNow.c_str(), CxAppEnv::applicationFilePath().c_str());
    FILE * pFile;
    pFile = fopen (sFilePath.c_str(), "ab+");
    if (pFile==NULL) return;
    size_t iWrote = 0;
    iWrote += fwrite (sHead.data() , 1, sHead.size(), pFile);
    iWrote += fwrite (_Expression , 1, strlen((const char*)_Expression), pFile);
    iWrote += fprintf (pFile, "\r\n%s\r\n%d\r\n\r\n",(const char*)_file,_line);
    fclose (pFile);
}

void CxApplication::raiseFnStops()
{
    vector<fn_void_t> * oFnStops = getFnStops();
    cxDebug() << "CxApplication::stop >> " << "oFnStops.size=" << oFnStops->size();
    cxDebug() << "CxApplication::stop >> " << CxTime::currentSystemTimeString();
    for (size_t i = 0; i < oFnStops->size(); ++i)
    {
        fn_void_t fn = oFnStops->at(i);
        fn();
        cxDebug() << "CxApplication::stop >> " << i << " datetime=" << CxTime::currentSystemTimeString();
    }
    cxDebug() << "CxApplication::stop >> " << CxTime::currentSystemTimeString();
    CxLogManager::stopLog();
    CxInterinfo::stopInterInfo();
}

//*CxApplicationCustom
static CxSingleWait * f_oSingleWait = new CxSingleWait();

int fn_doLoopEvents_custom(int iTag)
{
    while (f_iApplicationStatus)
    {
        CxApplication::runProcessCallBacks();
        f_oSingleWait->wait();
    }
    return true;
}

void fn_doSignalMainThread_custom()
{
    f_oSingleWait->signal();
}

void fn_doProcessEvents_custom()
{
    CxApplication::runProcessCallBacks();
    if (f_iProcessEventsSleep > -1)
    {
        CxThread::sleep(f_iProcessEventsSleep);
    }
}

void fn_doExit_custom()
{
    CxApplication::signalMainThread();
}

void fn_doReset_custom()
{
    delete f_oSingleWait;
    f_oSingleWait = new CxSingleWait();
}

static fn_int_int_t f_fnLoopEvents = fn_doLoopEvents_custom;
static fn_void_t f_fnSignalMainThread = fn_doSignalMainThread_custom;
static fn_void_t f_fnProcessEvents = fn_doProcessEvents_custom;
static fn_void_t f_fnExit = fn_doExit_custom;
static fn_void_t f_fnReset = fn_doReset_custom;

void CxApplication::setFnCore(fn_int_int_t fnDoLoopEvents, fn_void_t fnSignalMainThread, fn_void_t fnDoProcessEvents, fn_void_t fnDoExit, fn_void_t fnReset)
{
    f_fnLoopEvents = fnDoLoopEvents;
    f_fnSignalMainThread = fnSignalMainThread;
    f_fnProcessEvents = fnDoProcessEvents;
    f_fnExit = fnDoExit;
    f_fnReset = fnReset;
}


int CxApplication::doLoopEvents(int iTag)
{
    return f_fnLoopEvents(iTag);
}

void CxApplication::signalMainThread()
{
    f_fnSignalMainThread();
}

void CxApplication::processEvents()
{
    f_fnProcessEvents();
}

void CxApplication::doExit()
{
    f_fnExit();
}

void CxApplication::reset()
{
    f_fnReset();
}

void CxApplication::raiseExit()
{
    if (f_iApplicationStatus !=0)
    {
        cxPrompt() << "CxApplication::raiseExit.";
        cout << "CxApplication::raiseExit." << endl;
        raise(SIGINT);
    }
}

