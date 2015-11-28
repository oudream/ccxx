#ifndef CXAPPLICATION_H
#define CXAPPLICATION_H


#include "cxglobal.h"
#include "cxthread.h"
#include "cxstring.h"


static const std::string CS_SectionProgramConfig = "ProgramConfigs";
static const std::string CS_EntryProcessEventsSleep = "ProcessEventsSleep";

#ifndef GM_PROJECT_TYPE
#define GM_PROJECT_TYPE (1)
#endif

#define GM_PROJECT_TYPE_APP_CONSOLE   (1)
#define GM_PROJECT_TYPE_APP_GUI       (2)
#define GM_PROJECT_TYPE_LIB_DLL       (3)
#define GM_PROJECT_TYPE_LIB_STATIC    (4)

class CxApplication
{
public:
    //*part : do not depend on function init
    static void restartSystem();

public:
    //*init -> start -> run
    static void registInitFunction(fn_void_t fn);

    static void registStartFunction(fn_void_t fn, int iOrder = 10000);

    static void registStopFunction(fn_void_t fn);

    static int projectType();

    static void init(int argc, char * argv[], int iProjectType = GM_PROJECT_TYPE);

    //1) starts; 2) while(1) {process} isSave(0x00000001)
    static int exec(int iTag = 0);

    static void processEvents();

    static int applicationStatus();

    static CxMutex * processCallBacksLock();

    static void pushProcessCallBack(fn_void_msg_tlv_t fn, int iMsg = 0, int iTag = 0, const void * pData = 0, int iLength = 0, void * oSource = 0, void * oTarget = 0);

    static void nullProcessCallBackSource(void * oSource);

    static void nullProcessCallBackTarget(void * oTarget);

    static void signalMainThread();

    static void runProcessCallBacks();

    static void waiting(msepoch_t iTimeOut);

    static int waiting(int iWaitMilliseconds, fn_int_void_t fnCheck, int iSleep = 1);

    static void exit();

    //*part : depend on function init
    static const std::string& applicationFilePath();

    static const std::string& applicationFileName();

    static const std::string& applicationTargetName();

    static const std::string& applicationPath();

    static const std::string& applicationDeployPath();

    static void setApplicationDeployPath(const std::string& value);

    static std::string applicationRelativePath(const std::string& sDirName);

    static const std::string& configPath();

    static const std::string& configTargetFilePath();

    static const std::string& configDefaultFilePath();

    static const std::string& configFilePath();

    static const std::string& configExtendFilePath();

    static const std::map<std::string, std::string>& getArguments();

    static std::string findArgument(const std::string& sArgumentName);

    //sArguments : std::map<std::string, std::string> sArguments; sArguments["-name1"] = "value1"; sArguments["-name2"] = "value2";
    static std::string argumentsToString(const std::map<std::string, std::string>& sArguments);

    //sArg : "-name1 value1 value2 -name2 value3"
    static std::map<std::string, std::string> argumentsFromString(const std::string & sArg);

    static std::map<std::string, std::string> argumentsFromArgcv(int argc, char *argv[]);

    //*part : application config
    static const std::map<std::string, std::string> & findConfigs(const std::string & sSection);

    template<typename TValue>
    static TValue findConfig(const std::string & sSection, const std::string & sKey, const TValue & tDefault, bool bSaveNew = false)
    {
        std::string sDefault = CxString::toString(tDefault);
        std::string sValue = doFindConfig(sSection, sKey, sDefault, bSaveNew);
        return CxString::fromString(sValue, tDefault);
    }

    static int saveConfig(const std::string & sFilePath = std::string());

    static void saveConfig(const std::string &sSection, const std::string &sKey, const std::string &sValue);

    static bool isApplicationThread();

    static cx_pthread_t applicationThreadId();
    static pid_os_t applicationProcessId();

    static int getCallBackCount();

private:
    static int doLoopEvents(int iTag);

    static void doExit();

    static std::vector<fn_void_t> * getFnInits();

    static std::vector<std::pair<fn_void_t, int> > * getFnStarts();

    static std::vector<fn_void_t> * getFnStops();

    static std::map<std::string, std::map<std::string, std::string> > toRelativePath(const std::map<std::string, std::map<std::string, std::string> > & sections);

    static const std::string & doFindConfig(const std::string & sSection, const std::string & sKey, const std::string & sDefault = std::string(), bool bSaveNew = true);

    static void reset();

};

#ifdef CCXX_QT

#include <QApplication>


class CxApplicationReceiver : public QObject
{

    Q_OBJECT

public:
    explicit CxApplicationReceiver(QObject *parent=0);

    ~CxApplicationReceiver();

    bool event(QEvent *);

};

#else
#endif

#endif // CXAPPLICATION_H
