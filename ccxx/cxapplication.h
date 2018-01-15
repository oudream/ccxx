#ifndef CXAPPLICATION_H
#define CXAPPLICATION_H

#include "cxappenv.h"
#include "cxthread.h"

class GM_CCXX_CORE_API CxApplication : public CxAppEnv
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

    //*part : depend on function init
    static msepoch_t applicationInitTime();

    static msepoch_t applicationStartTime();

    static msepoch_t applicationExecuteTime();

    static void init(int argc, const char * argv[], int iProjectType = GM_PROJECT_TYPE);

    //1) starts; 2) while(1) {process} isSave(0x00000001)
    static int exec(int iTag = 0);

    static void processEvents();

    static int applicationStatus();

    /**
     * @brief pushProcessCallBack
     * @param fn
     * @param bOnlyOnePending : on handle is only one
     * @param iMsg
     * @param iTag
     * @param pData
     * @param iLength
     * @param oSource
     * @param oTarget
     */
    static void pushProcessCallBack(fn_void_msg_tlv_t fn, int iMsg = 0, int iTag = 0, const void * pData = 0, int iLength = 0, void * oSource = 0, void * oTarget = 0, bool bOnlyOnePending  = false);

    static void nullProcessCallBackSource(void * oSource);

    static void nullProcessCallBackTarget(void * oTarget);

    static void signalMainThread();

    static void runProcessCallBacks();

    static void waiting(msepoch_t iTimeOut);

    static int waiting(int iWaitMilliseconds, fn_int_void_t fnCheck, int iSleep = 1);

    static void raiseExit();

    static void exit();

    static bool isApplicationThread();

    static cx_pthread_t applicationThreadId();

    static pid_os_t applicationProcessId();

    static int getCallBackCount();

    static int doLoopEvents(int iTag);

protected:
	static void doExit();

    static void reset();

private:
    static std::vector<fn_void_t> * getFnInits();

    static std::vector<std::pair<fn_void_t, int> > * getFnStarts();

    static std::vector<fn_void_t> * getFnStops();

    static void assertDeal(int, int, const void * _Expression, int _line, void* _file, void*);

    static void raiseFnStops();

    static void setFnCore(fn_int_int_t fnDoLoopEvents,
                          fn_void_t fnSignalMainThread,
                          fn_void_t fnDoProcessEvents,
                          fn_void_t fnDoExit,
                          fn_void_t fnReset);

    friend class CxApplicationQt;
};


#endif // CXAPPLICATION_H
