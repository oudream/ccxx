#ifndef PROJECT_DAEMON_AGENT_H
#define PROJECT_DAEMON_AGENT_H

#include "daemon_global.h"

class DaemonAgent {
public:

    class ScanRecord {
    public:
        std::string filepath;
        msepoch_t lastModifyTime;
        int64 fileSize;
        int64 scanOffset;
    };

    class AppConfig {
    public:
/*
int AppID            ;
int NodeID           ;
int OrderNo          ;
std::string AppName          ;
std::string AppPath          ;
std::string AppParam         ;
int CanNotRunning    ;
std::string WorkPath         ;
int AllOver          ;
int OperationalCfg   ;
int OperationalTm    ;
int StaySleep        ;
int HeartbeatPort    ;
int HeartbeatTimeout ;
int HeartbeatData    ;
int WithWin          ;
int CpuLimit         ;
int MemLimit         ;
std::string AppAlias         ;
int Supervise        ;
int RunMode          ;
int AppType          ;

        string exeFilePath;
        string workPath;
        string exeFileName;
        int processType;
        int waitFirstMillisecond;
        int localPort;
        string heartHexString;
        string heartBuffer;
        unsigned int timeOut_receive;
        string runParam;
        string commandLine;
        int startMode; //Æô¶¯Ä£Ê½
        int aid;
        int ord;

 */

    };

public:
    static bool init();

    static void scan(int iInterval);

    //*control daemon app
    static int loadConfig2file();

    static std::vector<ProcessConfig> loadDb2ProcessConfigs();

    static std::map<std::string, std::string> getColumnCoverts();

    static bool startDaemon();

    static void checkDaemon(int iInterval);

    static void restartDaemon();

    static void loadConfigAndRestartDaemon();

    //*
    static int getServerid();

    //*
    static std::vector<std::string> reportSelf();

    //*ProcessStatus
    static const std::vector<ProcessStatus> & processStatuses();

    static const ProcessStatus * processStatuse(const std::string & sProcessName);

    static const ProcessStatus * processStatuse(int iAppid);

    //*ProcessAlarm
    static const std::vector<ProcessAlarm> & processAlarms();

    static const ProcessAlarm * processAlarm(const std::string & sProcessName);

    static const ProcessAlarm * processAlarm(int iAppid);

public:
    static std::string daemonFilePath();

    static std::string daemonConfigFilePath();

    static const ScanRecord & currentScanRecord();

private:
    static std::string getDaemonLogFilePath();

    static ScanRecord loadScanRecord();

    static void saveCurrentScanRecord();

    static int publishDaemonData(const std::string & sDaemonData);

    static void addProcessStatus(const ProcessStatus & processStatus);

    static void addProcessAlarm(const ProcessAlarm & processAlarm);

    static ProcessStatus * findProcessStatus(int iAppid);

    static ProcessAlarm * findProcessAlarm(int iAppid);

};

#endif //PROJECT_DAEMON_AGENT_H
