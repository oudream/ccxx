#include "cxprocess.h"

#include "cxapplication.h"
#include "cxstring.h"
#include "cxfile.h"
#include "cxcontainer.h"

using namespace std;


vector<char *> fn_joinPcharList(const vector<string> & sList)
{
    vector<char *> r;
    for (size_t i = 0; i < sList.size(); ++i)
    {
        const string & sEnvironment = sList.at(i);
        r.push_back((char *)sEnvironment.data());
    }
    if (r.size()>0)
    {
        r.push_back(NULL);
    }
    return r;
}

std::vector<fn_void_process_notify_t> * fn_fnProcessNotifies()
{
    static vector<fn_void_process_notify_t> fnInits;
    return & fnInits;
}



#define GM_EVENT_PROCESS_END (1)


//* Thread Receive Message
class ProcessWaitThread : public CxJoinableThread
{
public:
    pid_os_t m_pid;
    string m_args;

    friend class CxProcess;

public:
    inline void waitExit() { join(); }

protected:
    void run()
    {
#ifdef GM_OS_WIN
        ::WaitForSingleObject( m_pid.hProcess, INFINITE );//检测进程是否停止
        CxProcess::threadEventNotify(NULL, GM_EVENT_PROCESS_END, 0, (void *)&m_pid, sizeof(m_pid), NULL);
#endif
    }

    void exit()
    {
        CxJoinableThread::exit();
    }

};
static std::vector<ProcessWaitThread*> f_sProcesses;




CxProcess::CxProcess(const string &sProgram, const map<string, string>& sArguments, const string &sWorkingDirectory, const map<string, string> & sEnvironments) :
    m_program(sProgram) ,
    m_arguments(sArguments) ,
    m_workingDirectory(sWorkingDirectory) ,
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
    if (isvalid(m_pid) && kill(m_pid))
    {
        wait(m_pid);
        release(m_pid);
        memset(&m_pid, 0, sizeof(m_pid));
    }
}

#ifdef GM_OS_WIN

pid_os_t CxProcess::exec(const string & sProgram,
                            const map<string, string> & arguments,
                            const string & sWorkingDirectory,
                            const map<string, string> & environments)
{
//prepare data
    pid_os_t r;
//    memset(&r, 0, sizeof(r));
    ZeroMemory(&r, sizeof(r));

    if (sProgram.empty()) return r;

    LPSTR pCmdLine = NULL;
    PVOID pEnvs = NULL;
    LPCSTR pDir = (sWorkingDirectory.size()>0) ? sWorkingDirectory.c_str() : NULL;

    string sArguments = CxApplication::argumentsToString(arguments);
    vector<string> environmentList = CxString::joinToStringList(environments, "=");
    vector<char *> environmentArray = fn_joinPcharList(environmentList);
    pEnvs = ((environmentArray.size()>0)) ? (& environmentArray.front()) : NULL;

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
    string sArgs = sProgram + " " + sArguments;
    pCmdLine = (char *)sArgs.c_str();
//    bSuccess = CreateProcess(   0,
//                                pCmdLine,
//                                0, 0, false, 0,
//                                pEnvs,
//                                pDir,
//                                0,
//                                &r );

    DWORD dwCreationFlags = 0;
//        dwCreationFlags |= CREATE_NEW_CONSOLE;
//    if(startMode==0)dwCreationFlags |= CREATE_NO_WINDOW;
//    else if(startMode==1)dwCreationFlags |= CREATE_NEW_CONSOLE;

//    PROCESS_INFORMATION pi; //进程信息
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    bSuccess = ::CreateProcessA(
                NULL,
                const_cast<char *>(sArgs.data()),
                NULL,NULL,FALSE,dwCreationFlags,
                NULL,NULL,&si,
                &r);
#endif
    if (bSuccess)
    {
        ProcessWaitThread * o = new ProcessWaitThread();
        o->m_pid = r;
        o->m_args = string(sArgs.data(), sArgs.size());
        o->start();
        f_sProcesses.push_back(o);
    }
    else
    {
        memset(&r, 0, sizeof(r));
    }
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
    WaitForSingleObject( pid.hProcess, INFINITE );
}

void CxProcess::release(pid_os_t pid)
{
    CloseHandle(pid.hThread);
    CloseHandle(pid.hProcess);
}

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
    //把数据加入队列中
    CxApplication::pushProcessCallBack(CxProcess::dealThreadNofity, iEvent, iTag, pData, iLength, oSource, oTarget);
    //通知
    CxApplication::signalMainThread();
}

void CxProcess::dealThreadNofity(int iEvent, int iTag, const void * pData, int iLength, void * oSource, void * oTarget)
{
    if (iEvent == GM_EVENT_PROCESS_END)
    {
        pid_os_t mPid;
        if (pData && iLength>=sizeof(mPid))
            memcpy(&mPid, pData, sizeof(mPid));
        else
            memset(&mPid, 0, sizeof(mPid));

        ProcessWaitThread * oProcessWaitThread = NULL;
        for (size_t i = 0; i < f_sProcesses.size(); ++i)
        {
            ProcessWaitThread * oThread = f_sProcesses.at(i);
            if (CxProcess::isSame(oThread->m_pid, mPid))
            {
                oProcessWaitThread = oThread;
                f_sProcesses.erase(f_sProcesses.begin()+i);
                break;
            }
        }
        string sArgs = oProcessWaitThread ? string(oProcessWaitThread->m_args.data(), oProcessWaitThread->m_args.size()) : string();

        std::vector<fn_void_process_notify_t> * oFns = fn_fnProcessNotifies();
        for (size_t i = 0; i < oFns->size(); ++i)
        {
            fn_void_process_notify_t fn = oFns->at(i);
            fn(mPid, sArgs);
        }

        if (oProcessWaitThread)
        {
            delete oProcessWaitThread;
        }
    }
}

void CxProcess::registProcessNotify(fn_void_process_notify_t fn)
{
    std::vector<fn_void_process_notify_t> * oFns = fn_fnProcessNotifies();
    if (std::find(oFns->begin(), oFns->end(), fn) == oFns->end())
        oFns->push_back(fn);
}

void CxProcess::unregistProcessNotify(fn_void_process_notify_t fn)
{
    std::vector<fn_void_process_notify_t> * oFns = fn_fnProcessNotifies();
    CxContainer::remove(*oFns, fn);
}

#else

void sighandler(int sig)
{
    /* wait() is the key to acknowledging the SIGCHLD */
    ::wait(0);
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
