#ifndef PROJECT_DAEMON_GLOBAL_H
#define PROJECT_DAEMON_GLOBAL_H

#include <ccxx/cxstring.h>
#include <ccxx/cxtime.h>
#include <ccxx/cxfile.h>
#include <ccxx/cxprocess.h>
#include <ccxx/cxappenv.h>
#include <ccxx/cxprofile.h>
#include <ccxx/cxrapidjson.h>
#include <ccxx/cxencoding.h>

class ProcessConfig {
public:
    static ProcessConfig createProcessConfig(const std::string & sProcessFilePath, const std::string &  sProcessWorkPath, int iWaitMillisecond,
                                             int iProcessType, int iLocalPort, const std::string &  sHeartBuffer, int iTimeOut_receive,
                                             const std::string &  sProcessParam, int iMode, int iAid, int iOrd, int iDm,
                                             int iCpuLimit, int iMemLimit, const std::string & sAppAlias, int iSupervise, int iRunMode, int iAppType) ;

    static std::vector<ProcessConfig> prepareProcessers(const std::vector<std::map<std::string, std::string> > & processConfigLines);

    static std::string getExeFilePath(const std::string &sPf, const std::string &sPpf);

    static bool isUrl(const std::string & sPath);

    static std::vector<std::string> report(const ProcessConfig & processConfig);

    static std::string getTitle(const ProcessConfig & processConfig);

    static bool isValid(const ProcessConfig & processConfig);

public:
    std::string exeFilePath;
    std::string workPath;
    std::string exeFileName;
    int processType;
    int waitFirstMillisecond;

    int localPort;
    std::string heartHexString;
    std::string heartBuffer;
    unsigned int timeOut_receive;

    std::string runParam;
    std::string commandLine;
    int startMode; //启动模式
    int aid;
    int ord;
    int dm;
    int CpuLimit         ;
    int MemLimit         ;
    std::string AppAlias ;
    int Supervise        ;
    int RunMode          ;
    int AppType          ;

    /**
     * dm: 守护方式
0 保留
1 正常守护
2 不许运行
3 不守护
     */
    typedef enum {
        DmNone,
        DmNormalDaemon,
        DmCannotRunning,
        DmCannotDaemon
	} DmEnum;

    /**
     * RunMode: 运行方式
0 保留
1 HA启动
2 手工启动
     */
    typedef enum {
        RunModeNone,
        RunModeAuto,
        RunModePerformance
    } RunModeEnum;

    /**
     * Supervise: 监视方式
0 不监视
1 保活
2 性能
3 1+2
     */
    typedef enum {
        SuperviseNone,
        SuperviseKeepLive,
        SupervisePerformance,
        SuperviseAB
    } SuperviseEnum;

    /**
     * AppType: 程序类型
0 保留
1 后台服务
2 工具
3 客户端
4 服务模块
     */
    typedef enum {
        AppTypeNone,
        AppTypeService,
        AppTypeTools,
        AppTypeClient
    } AppTypeEnum;

};

class ProcessStatus {
public:
    static std::string getLogClass();

    static bool fromJson(const rapidjson::Value * jValue, ProcessStatus & outProcessStatus);

public:
    int aid;
    int ord;
    std::string process;
    int isRunnig;//isThreadWaitFinish, hi.hProcess
    msepoch_t startTime;
    int startTimes;
    long long receiveBytes;
    msepoch_t lastReceiveTime;
    msepoch_t recordTime;
    int syscpu;
    int sysmem;
    int cpu;
    int mem;

    std::string toJson();

    void assignedFrom(const ProcessStatus &pss);

};

class ProcessAlarm {
public:
    static std::string getLogClass();

    static bool fromJson(const rapidjson::Value * jValue, ProcessAlarm & outProcessAlarm);

    enum AlarmTypeEnum {
        AlarmType_None,
        AlarmType_HeartBeatTimeout,
        AlarmType_Finish
    };

public:
    int aid;
    int ord;
    std::string process;
    msepoch_t alarmTime;
    int alarmType;
    std::string alarmMsg;

    //{"process":"%s","alarmType":"%s","alarmMsg":"%s"}
    std::string toJson();

    void assignedFrom(const ProcessAlarm & psa);

};

class DaemonGlobal {
public:
    static std::string getLogFilePath();

    //{"t":"%s","c":"%s","i":"%s","d":""}
    static void log(std::string sLogClass, std::string jsonLogContent, std::string sId = std::string());

    static int fillCommandLine(std::vector<CxProcess::ProcessInfo> & processInfos);

    static const CxProcess::ProcessInfo * findProcessInfo(const std::vector<CxProcess::ProcessInfo> & processInfos, const ProcessConfig *oProcessConfig);

};

typedef int (*fn_add_ProcessStatus_t)(const ProcessStatus & processStatus);
typedef int (*fn_add_ProcessAlarm_t)(const ProcessAlarm & processAlarm);


//callback event
extern fn_add_ProcessStatus_t fn_add_ProcessStatus;
extern fn_add_ProcessAlarm_t fn_add_ProcessAlarm;
extern std::string gs_defaultBinPath;


#endif //PROJECT_DAEMON_GLOBAL_H
