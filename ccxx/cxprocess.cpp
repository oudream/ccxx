#include "cxprocess.h"

#include "cxappenv.h"
#include "cxstring.h"
#include "cxfile.h"
#include "cxcontainer.h"

#ifdef GM_OS_WIN
#include <tlhelp32.h>
#include <psapi.h>
#endif

using namespace std;

vector<char *> fn_joinPcharList(const vector<string> &sList)
{
    vector<char *> r;
    for (size_t i = 0; i < sList.size(); ++i)
    {
        const string &sEnvironment = sList.at(i);
        r.push_back((char *) sEnvironment.data());
    }
    if (r.size() > 0)
    {
        r.push_back(NULL);
    }
    return r;
}

std::vector<fn_void_process_notify_t> *fn_fnProcessNotifies()
{
    static vector<fn_void_process_notify_t> fnInits;
    return &fnInits;
}

#define GM_EVENT_PROCESS_END (1)

/*
//* Thread Receive Message
class ProcessWaitThread : public CxJoinableThread {
public:
    pid_os_t m_pid;
    string m_args;

    friend class CxProcess;

public:
    inline void waitExit()
    { join(); }

protected:
    void run()
    {
#ifdef GM_OS_WIN
        ::WaitForSingleObject(m_pid.hProcess, INFINITE); // wait process exit
        CxProcess::threadEventNotify(NULL, GM_EVENT_PROCESS_END, 0, (void *) &m_pid, sizeof(m_pid), NULL);
#endif
    }

    void exit()
    {
        CxJoinableThread::exit();
    }

};
static std::vector<ProcessWaitThread *> f_sProcesses;

*/

CxProcess::CxProcess(const string &sProgram, const map<string, string> &sArguments, const string &sWorkingDirectory, const map<string, string> &sEnvironments)
    :
    m_program(sProgram),
    m_arguments(sArguments),
    m_workingDirectory(sWorkingDirectory),
    m_environments(sEnvironments)
{
    memset(&m_pid, 0, sizeof(m_pid));
}

CxProcess::~CxProcess()
{
    stop();
}

void CxProcess::start()
{
    m_pid = exec(m_program, m_arguments, m_workingDirectory, m_environments);
}

void CxProcess::stop()
{
    if (isvalid(m_pid) && CxProcess::kill(m_pid))
    {
        wait(m_pid);
        release(m_pid);
        memset(&m_pid, 0, sizeof(m_pid));
    }
}

#ifdef GM_OS_WIN

pid_os_t CxProcess::exec(const string &sProgram,
                         const map<string, string> &arguments,
                         const string &sWorkingDirectory,
                         const map<string, string> &environments)
{
    string sArguments = CxAppEnv::argumentsToString(arguments);
    return CxProcess::exec(sProgram, sArguments, sWorkingDirectory, environments);
}

pid_os_t CxProcess::exec(const std::string &sProgram,
                         const std::string &sArguments,
                         const string &sWorkingDirectory,
                         const map<string, string> &environments)
{
    //prepare data
    pid_os_t r;
    //    memset(&r, 0, sizeof(r));
    ZeroMemory(&r, sizeof(r));

    if (sProgram.empty()) return r;

    LPSTR pCmdLine = NULL;
    PVOID pEnvs = NULL;
    LPCSTR pDir = (sWorkingDirectory.size() > 0) ? sWorkingDirectory.c_str() : NULL;

    vector<string> environmentList = CxString::joinToStringList(environments, "=");
    vector<char *> environmentArray = fn_joinPcharList(environmentList);
    pEnvs = ((environmentArray.size() > 0)) ? (&environmentArray.front()) : NULL;

    //process
    bool bSuccess = false;
#if defined(GM_OS_WINCE)
    LPCSTR pAppName =  sProgram.c_str();
    pCmdLine = sArguments.c_str();
    bSuccess = CreateProcessA(   pAppName,
                                 pCmdLine,
                                 0, 0, false, 0,
                                 pEnvs,
                                 pDir,
                                 0,
                                 &r );
#else
    string sCommand = sProgram + " " + sArguments;
    pCmdLine = (char *) sCommand.c_str();
    DWORD dwCreationFlags = 0;
    dwCreationFlags |= CREATE_NEW_CONSOLE;
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    bSuccess = ::CreateProcessA(
        NULL,
        const_cast<char *>(sCommand.data()),
        NULL, NULL, FALSE, dwCreationFlags,
        NULL, NULL, &si,
        &r);
#endif
    if (bSuccess)
    {
//        ProcessWaitThread *o = new ProcessWaitThread();
//        o->m_pid = r;
//        o->m_args = string(sCommand.data(), sCommand.size());
//        o->start();
//        f_sProcesses.push_back(o);
    }
    else
    {
        memset(&r, 0, sizeof(r));
    }
    return r;
}

BOOL GetTokenByExplorer(HANDLE &hToken)
{
    char lpName1[] = "explorer.exe";
    char lpName2[] = "EXPLORER.EXE";
    HANDLE hProcessSnap = NULL;
    BOOL r = FALSE;
    PROCESSENTRY32 pe32 = {0};

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return r;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            if((!strcmp(pe32.szExeFile, lpName1)) || (!strcmp(pe32.szExeFile, lpName2)))
            {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,
                                              FALSE,pe32.th32ProcessID);
                r = OpenProcessToken(hProcess,TOKEN_ALL_ACCESS,&hToken);
                ::CloseHandle (hProcessSnap);
                return r;
            }
        }
        while (Process32Next(hProcessSnap, &pe32));
        r = FALSE;
    }
    else
    {
        r = FALSE;
    }

    ::CloseHandle (hProcessSnap);
    return r;
}

//need lib userenv
//#include <userenv.h>

pid_os_t CxProcess::execAsDetached(const std::string &sProgram,
                                   const map<string, string> &arguments,
                                   const string &sWorkingDirectory,
                                   const map<string, string> &environments)
{
    pid_os_t r;
    ZeroMemory(&r, sizeof(r));

    HANDLE primaryToken;
    if(!GetTokenByExplorer(primaryToken))
    {
//        OutputDebugString("2");
        return r;
    }

//    primaryToken = GetCurrentUserToken();
//    if (primaryToken == 0)
//    {
//        return FALSE;
//    }
    STARTUPINFO StartupInfo = {0};
//    PROCESS_INFORMATION processInfo;
    StartupInfo.cb = sizeof(STARTUPINFO);

    std::string sCommand = sProgram;

//    string sShow = CxString::toLower(CxAppEnv::findArgument(string("show")));
//    if (sShow.size() > 0)
//    {
//        sCommand += " " + sShow;
//    }
//
//    void *lpEnvironment = NULL;
//    BOOL resultEnv = ::CreateEnvironmentBlock(&lpEnvironment, primaryToken, FALSE);
//    if (resultEnv == 0)
//    {
//        long nError = GetLastError();
//    }
    BOOL result = ::CreateProcessAsUser(primaryToken, 0, (LPSTR) (sCommand.c_str()), NULL, NULL, FALSE,
                                        CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS
                                        | CREATE_UNICODE_ENVIRONMENT, NULL, 0, &StartupInfo, &r/*&processInfo*/);
//    ::DestroyEnvironmentBlock(lpEnvironment);
    ::CloseHandle(primaryToken);
    return r;
}


pid_os_t CxProcess::allocEmpty()
{
    pid_os_t r;
    ZeroMemory(&r, sizeof(r));
    return r;
}

bool CxProcess::isvalid(pid_os_t pid)
{
    return pid.dwProcessId != 0 && pid.dwThreadId != 0 && pid.hProcess != NULL && pid.hThread != NULL;
}

bool CxProcess::isSame(pid_os_t pid1, pid_os_t pid2)
{
    return pid1.hProcess == pid2.hProcess;
}

bool CxProcess::kill(pid_os_t pid)
{
    return TerminateProcess(pid.hProcess, 0);
}

void CxProcess::wait(pid_os_t pid)
{
    WaitForSingleObject(pid.hProcess, INFINITE);
}

void CxProcess::release(pid_os_t pid)
{
    CloseHandle(pid.hThread);
    CloseHandle(pid.hProcess);
}

#else

void sighandler(int sig)
{
    /* wait() is the key to acknowledging the SIGCHLD */
    ::wait(0);
}


pid_os_t CxProcess::exec(const std::string &sProgram,
                         const std::string &sArguments,
                         const string &sWorkingDirectory,
                         const map<string, string> &environments)
{
    map<string, string> arguments = CxAppEnv::argumentsFromString(sArguments);
    return CxProcess::exec(sProgram, arguments, sWorkingDirectory, environments);
}

pid_os_t CxProcess::exec(const string & sProgram,
                         const map<string, string> & arguments,
                         const string & sWorkingDirectory,
                         const map<string, string> & environments)
{
    sigset(SIGCHLD, &sighandler);
    pid_os_t r = 0;
    pid_t pid = fork();
    if (pid < 0)
    {
        r = 0;
    }
    else if (pid == 0)
    {
        //prepare data
        vector<string> argumentList = CxString::joinToStringList(arguments, " ");
        vector<char *> argumentArray = fn_joinPcharList(argumentList);
        char * * argv = ((argumentArray.size()>0)) ? (& argumentArray.front()) : NULL;
        //environment
        vector<string> environmentList = CxString::joinToStringList(environments, "=");
        vector<char *> environmentArray = fn_joinPcharList(environmentList);
        char * * envp = ((environmentArray.size()>0)) ? (& environmentArray.front()) : NULL;

        if (sWorkingDirectory.size()>0)
            CxFileSystem::setCurrentDir(sWorkingDirectory);

        execve(sProgram.c_str(), argv, envp);
        exit(0);
    }
    else
    {
        r = pid;
    }
    return r;
}

pid_os_t CxProcess::execAsDetached(const string & sProgram,
                                   const map<string, string> & arguments,
                                   const string & sWorkingDirectory,
                                   const map<string, string> & environments)
{
    return pid_os_t();
}

pid_os_t CxProcess::allocEmpty()
{
    return 0;
}

bool CxProcess::isvalid(pid_os_t pid)
{
    return pid != 0;
}

bool CxProcess::isSame(pid_os_t pid1, pid_os_t pid2)
{
    return pid1 == pid2;
}

bool CxProcess::kill(pid_os_t pid)
{
    return ! ::kill(pid, SIGABRT);
}

void CxProcess::wait(pid_os_t pid)
{
    ::waitpid(pid, NULL, 0);
}

void CxProcess::release(pid_os_t pid)
{
}

#endif

pid_os_t CxProcess::getCurrentPid()
{
    pid_os_t r;
#ifdef GM_OS_WIN
    r.hProcess = ::GetCurrentProcess();
    r.dwProcessId = ::GetCurrentProcessId();
    r.hThread = ::GetCurrentThread();
    r.dwThreadId = ::GetCurrentThreadId();
#else
    r = ::getpid();
#endif
    return r;
}

void CxProcess::threadEventNotify(void *oTarget, int iEvent, int iTag, const void *pData, int iLength, void *oSource)
{
//    CxApplication::pushProcessCallBack(CxProcess::dealThreadNofity, iEvent, iTag, pData, iLength, oSource, oTarget);
//    CxApplication::signalMainThread();
}
/*
void CxProcess::dealThreadNofity(int iEvent, int iTag, const void *pData, int iLength, void *oSource, void *oTarget)
{
    if (iEvent == GM_EVENT_PROCESS_END)
    {
        pid_os_t mPid;
        if (pData && iLength >= sizeof(mPid))
            memcpy(&mPid, pData, sizeof(mPid));
        else
            memset(&mPid, 0, sizeof(mPid));

        ProcessWaitThread *oProcessWaitThread = NULL;
        for (size_t i = 0; i < f_sProcesses.size(); ++i)
        {
            ProcessWaitThread *oThread = f_sProcesses.at(i);
            if (CxProcess::isSame(oThread->m_pid, mPid))
            {
                oProcessWaitThread = oThread;
                f_sProcesses.erase(f_sProcesses.begin() + i);
                break;
            }
        }
        string sCommand = oProcessWaitThread ? string(oProcessWaitThread->m_args.data(), oProcessWaitThread->m_args.size())
                                          : string();

        std::vector<fn_void_process_notify_t> *oFns = fn_fnProcessNotifies();
        for (size_t i = 0; i < oFns->size(); ++i)
        {
            fn_void_process_notify_t fn = oFns->at(i);
            fn(mPid, sCommand);
        }

        if (oProcessWaitThread)
        {
            delete oProcessWaitThread;
        }
    }
}
*/

void CxProcess::registProcessNotify(fn_void_process_notify_t fn)
{
    std::vector<fn_void_process_notify_t> *oFns = fn_fnProcessNotifies();
    if (std::find(oFns->begin(), oFns->end(), fn) == oFns->end())
        oFns->push_back(fn);
}

void CxProcess::unregistProcessNotify(fn_void_process_notify_t fn)
{
    std::vector<fn_void_process_notify_t> *oFns = fn_fnProcessNotifies();
    CxContainer::remove(*oFns, fn);
}

std::vector<CxProcess::ProcessInfo> CxProcess::getRunningProcesses()
{
    std::vector<CxProcess::ProcessInfo> r;
#ifdef GM_OS_WIN
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return r;
    }
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hSnapShot,&pe))
    {
        while (Process32Next(hSnapShot, &pe))
        {
            ProcessInfo processInfo;
            processInfo.name = pe.szExeFile;
            pid_os_t pid;
            pid.dwProcessId = pe.th32ProcessID;
            processInfo.pid = pid;
            r.push_back(processInfo);
        }
    }
    ::CloseHandle(hSnapShot);
#else
    //todo oudream
#endif
    return r;
}

bool CxProcess::isRunning(const std::string &sProcessName)
{
    bool r = false;
#ifdef GM_OS_WIN
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return r;
    }
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hSnapShot,&pe))
    {
        while (Process32Next(hSnapShot,&pe))
        {
            if(strcmp(sProcessName.c_str(), pe.szExeFile) == 0)
            {
                r = true;
                break;
            }
        }
    }
    ::CloseHandle(hSnapShot);
    return r;
#else
    //needtodo
    return -1;
#endif
}

bool CxProcess::isRunning(pid_os_t pid)
{
    bool r = false;
#ifdef GM_OS_WIN
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return r;
    }
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hSnapShot,&pe))
    {
        while (Process32Next(hSnapShot,&pe))
        {
            if(pid.dwProcessId == pe.th32ProcessID)
            {
                r = true;
                break;
            }
        }
    }
    ::CloseHandle(hSnapShot);
    return r;
#else
    //needtodo
    return -1;
#endif
}

int CxProcess::kill(const std::string &sProcessName)
{
    int r = -1;
#ifdef GM_OS_WIN
    const char *szProcessName = sProcessName.c_str();
    HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return r;
    }
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (::Process32First(hSnapShot, &pe))
    {

        r = 0;
        while (::Process32Next(hSnapShot, &pe))
        {
            if (strcmp(szProcessName, pe.szExeFile) == 0)
            {
                DWORD dwProcessID = pe.th32ProcessID;
                HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
                if (::TerminateProcess(hProcess, 0) == 0)
                {
                    DWORD dwError = ::GetLastError();
                    cout << "kill process error: " << pe.th32ProcessID << " Name: " << pe.szExeFile << " errorId: "
                         << dwError << endl;
                }
                else
                {
                    r++;
                }
                ::CloseHandle(hProcess);
                //::Sleep(1000);
            }
        }
    }
    ::CloseHandle(hSnapShot);
    return r;
#else
    return -1;
#endif
}


#ifdef GM_OS_WIN
msepoch_t cxprocess_filetime2msepoch(const FILETIME &ft)
{
    const uint64 iShift = 116444736000000000ULL; // (27111902 << 32) + 3577643008
    union {
        FILETIME  as_file_time;
        msepoch_t as_integer;   // 100-nanos since 1601-Jan-01
    } caster;
    caster.as_file_time = ft;
    caster.as_integer -= iShift; // filetime is now 100-nanos since 1970-Jan-01
    return caster.as_integer / 10000LL; // truncate to microseconds
}
#endif

int CxProcess::getMemoryUsage()
{
    int r = 0;
#ifdef GM_OS_WIN
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (::GlobalMemoryStatusEx(&statex))
    {
        return statex.dwMemoryLoad;
    }
    else
    {
        return r;
    }
#else
    return -1;
#endif
}

int CxProcess::getMemoryUsage(pid_os_t pid)
{
#ifdef GM_OS_WIN
    PROCESS_MEMORY_COUNTERS pmc;
    if (::GetProcessMemoryInfo(pid.hProcess, &pmc, sizeof(pmc)))
    {
        return pmc.WorkingSetSize;
    }
	return -1;
#else
    return -1;
#endif
}

nsepoch_t CxProcess::getCpuUsage(pid_os_t pid, int *pUsage)
{
    nsepoch_t r = 0;
#ifdef GM_OS_WIN
    LARGE_INTEGER creation_time;
    LARGE_INTEGER exit_time;
    LARGE_INTEGER kernel_time;
    LARGE_INTEGER user_time;
    if (!GetProcessTimes(pid.hProcess, (LPFILETIME) &creation_time, (LPFILETIME) &exit_time, (LPFILETIME) &kernel_time, (LPFILETIME) &user_time))
    {
        return r;
    }
    return kernel_time.QuadPart + user_time.QuadPart;
#else
    return -1;
#endif
}

nsepoch_t CxProcess::getCpuUsage()
{
    nsepoch_t r = 0;
#ifdef GM_OS_WIN
    LARGE_INTEGER idleTime;
    LARGE_INTEGER kernelTime;
    LARGE_INTEGER userTime;
    GetSystemTimes((LPFILETIME) &idleTime, (LPFILETIME) &kernelTime, (LPFILETIME) &userTime);
    return kernelTime.QuadPart + userTime.QuadPart;
#else
    return -1;
#endif
}

