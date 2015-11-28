#ifndef CXPROCESS_H
#define CXPROCESS_H

#include "cxglobal.h"

typedef void (*fn_void_process_notify_t)(pid_os_t pid, const std::string & sArgs);

class CxProcess
{
public:
    CxProcess(const std::string & sProgram,
              const std::map<std::string, std::string> & sArguments = std::map<std::string, std::string>(),
              const std::string & sWorkingDirectory = std::string(),
              const std::map<std::string, std::string> & sEnvironments = std::map<std::string, std::string>());

    ~CxProcess();

    void start();

    void stop();

    inline bool isStarted() const { return isvalid(m_pid); }

    inline pid_os_t pid() { return m_pid; }

    inline const std::map<std::string, std::string>& arguments() const { return m_arguments; }

    inline void setArguments(const std::map<std::string, std::string>& sArguments) { m_arguments = sArguments; }

    inline const std::string& workingDirectory() const { return m_workingDirectory; }

    inline void setWorkingDirectory(const std::string & sWorkingDirectory) { m_workingDirectory = sWorkingDirectory; }

    inline const std::map<std::string, std::string>& environments() const { return m_environments; }

    inline void setEnvironments(const std::map<std::string, std::string>& sEnvironments) { m_environments = sEnvironments; }

    //sArguments : std::map<std::string, std::string> sArguments; sArguments["-name1"] = "value1"; sArguments["-name2"] = "value2";
    //sEnvironments : std::map<std::string, std::string> sEnvironments; sEnvironments["path"] = "\tmp:\user\tmp";
    static pid_os_t exec(const std::string & sProgram,
                                const std::map<std::string, std::string> & arguments = std::map<std::string, std::string>(),
                                const std::string & sWorkingDirectory = std::string(),
                                const std::map<std::string, std::string> & environments = std::map<std::string, std::string>());

    static bool isvalid(pid_os_t pid);

    static bool isSame(pid_os_t pid1, pid_os_t pid2);

    static bool kill(pid_os_t pid);

    static void wait(pid_os_t pid);

    static void release(pid_os_t pid);

    static pid_os_t getCurrentPid();

    static void threadEventNotify(void * oTarget, int iEvent, int iTag = 0, const void * pData = 0, int iLength = 0, void * oSource = 0);

    static void dealThreadNofity(int, int, const void *, int, void *, void *);

    static void registProcessNotify(fn_void_process_notify_t fn);

    static void unregistProcessNotify(fn_void_process_notify_t fn);

private:
    std::string m_program;
    std::map<std::string, std::string> m_arguments;
    std::string m_workingDirectory;
    std::map<std::string, std::string> m_environments;
    pid_os_t m_pid;

};

#endif // CXPROCESS_H
