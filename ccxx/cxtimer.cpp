#include "cxtimer.h"
#include "cxapplication.h"
#include "cxinterinfo.h"


#define GM_TIMER_EVENT_ADD 1
#define GM_TIMER_EVENT_DELETE 2

#define GM_TIMER_TYPE_OBJECT 1
#define GM_TIMER_TYPE_FUNTION 2


using namespace std;


///
/// \brief The CxTimerHost struct
/// CxTimerHost : 是装着CxTimer的容器
struct CxTimerHost
{
    CxTimerHost()
    {
        m_interval = 0;
        m_isTimeOut = false;
        m_startTime = 0;
        m_timeOutTime = 0;
        m_processTime = 0;
        m_processTimes = 0;
    }

    ~CxTimerHost(){}

    std::map<void*, bool> m_timers;
    volatile int m_interval;
    volatile bool m_isTimeOut;
    msepoch_t m_startTime;
    msepoch_t m_timeOutTime;
    msepoch_t m_processTime;
    int m_processTimes;

};

///
/// \brief f_timerHostes : 是全局CxTimerHost*的容器，装着指向CxTimerHost的指针；启动与停止CxTimer就是从此容器中添加与删除CxTimer指针
/// \brief f_timerHostes_process : 是全局CxTimerHost*的容器，装着指向CxTimerHost的指针，是时钟是否到时的处理子线程中处，在主线程中处理时钟回调用
/// \brief f_mutex_host_size : 是f_timerHostes的锁
/// \brief f_mutex_timer_process : 是f_timerHostes_process的锁

#define ci_timerhostes_size (100)
static CxTimerHost f_mTimerHostes[ci_timerhostes_size];

static volatile int f_iThreadTimerStatus = 1;

static int f_timer_type_object = GM_TIMER_TYPE_OBJECT;
static int f_timer_type_funtion = GM_TIMER_TYPE_FUNTION;


CxTimerManager * fn_timerManagerStart()
{
    CxApplication::registStartFunction(CxTimerManager::start, 19000);
    CxApplication::registStopFunction(CxTimerManager::stop);
    static CxTimerManager timerManager;
    return & timerManager;
}
static CxTimerManager * f_oTimerManager = fn_timerManagerStart();


//
// *** timerHost - add delete
//
CxTimerHost * fn_getTimerHost(int iInterval)
{
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);
        if (oTimerHost->m_interval == iInterval)
        {
            return oTimerHost;
        }
    }
    return NULL;
}

CxTimerHost * fn_getTimerHost(void * timer, int iInterval)
{
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);
        if (oTimerHost->m_interval == iInterval)
        {
            std::map<void*, bool> & timers = oTimerHost->m_timers;
            std::map<void*, bool>::iterator itTimer = timers.find(timer);
            if (itTimer != timers.end())
            {
                return oTimerHost;
            }
        }
    }
    return NULL;
}

CxTimerHost * fn_getTimerHost(void * timer)
{
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);
        std::map<void*, bool> & timers = oTimerHost->m_timers;
        std::map<void*, bool>::iterator itTimer = timers.find(timer);
        if (itTimer != timers.end())
        {
            return oTimerHost;
        }
    }
    return NULL;
}

CxTimerHost * fn_getEmptyTimerHost()
{
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);
        if (oTimerHost->m_interval == 0)
        {
            return oTimerHost;
        }
    }
    return NULL;
}

void fn_separateFromHost(void * timer, int iInterval)
{
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);
        if (oTimerHost->m_interval == iInterval)
        {
            std::map<void*, bool> & timers = oTimerHost->m_timers;
            std::map<void*, bool>::iterator itTimer = timers.find(timer);
            if (itTimer != timers.end())
            {
                timers.erase(itTimer);
            }
            if (timers.size()==0)
            {
                oTimerHost->m_interval = 0;
            }
        }
    }
}

void fn_separateFromHost(void * timer)
{
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);
        std::map<void*, bool> & timers = oTimerHost->m_timers;
        std::map<void*, bool>::iterator itTimer = timers.find(timer);
        if (itTimer != timers.end())
        {
            timers.erase(itTimer);
        }
        if (timers.size()==0)
        {
            oTimerHost->m_interval = 0;
        }
    }
}

void fn_separateFromHost(int iInterval)
{
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);
        if (oTimerHost->m_interval == iInterval)
        {
            oTimerHost->m_interval = 0;
            oTimerHost->m_timers.clear();
        }
    }
}

void fn_addTimer(void * timer, int iInterval, bool bObject)
{
    CxTimerHost * oTimerHost = fn_getTimerHost(iInterval);
    if (oTimerHost)
    {
        oTimerHost->m_timers[timer] = bObject;
    }
    else
    {
        oTimerHost = fn_getEmptyTimerHost();
        if (oTimerHost)
        {
            oTimerHost->m_interval = iInterval;
            oTimerHost->m_timers[timer] = bObject;
            oTimerHost->m_startTime = CxTime::currentSystemTime();
            oTimerHost->m_timeOutTime = oTimerHost->m_startTime + oTimerHost->m_interval;
        }
        else
        {
            cxWarning() << "error error error !!! CxTimer Cache Full , Can not start timer !!!";
            cout        << "error error error !!! CxTimer Cache Full , Can not start timer !!!";
        }
    }
}



//
// *** fn_processTimers
//

void fn_processTimers(int, int, const void *, int, void *, void *)
{
    msepoch_t dtNow = CxTime::currentSystemTime();

    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);

        if (oTimerHost->m_isTimeOut)
        {
            oTimerHost->m_isTimeOut = false;
            oTimerHost->m_processTimes += 1;
            oTimerHost->m_processTime = dtNow;
            map<void*, bool> timerHosts = oTimerHost->m_timers;
            for (map<void*, bool>::iterator it = timerHosts.begin(); it != timerHosts.end(); ++it)
            {
                void * p = (it->first);
                //* maybe delete in doTimeOut
                if (oTimerHost->m_timers.find(p) != oTimerHost->m_timers.end())
                {
                    if (it->second)
                    {
                        CxTimer * timer = (CxTimer *)p;
                        timer->doTimeOut();
                    }
                    else
                    {
                        fn_timer_timeout_t timer = (fn_timer_timeout_t)p;
                        timer(oTimerHost->m_interval);
                    }
                }
            }
        }
    }
}

//attention : timing regularly : thread call
void fn_updateTimers()
{
    msepoch_t dtNow = CxTime::currentSystemTime();

    bool bHasTimeOut = false;
    for (size_t i = 0; i < ci_timerhostes_size; ++i)
    {
        CxTimerHost * oTimerHost = &(f_mTimerHostes[i]);

        if (oTimerHost->m_interval > 0 && dtNow > oTimerHost->m_timeOutTime)
        {
            oTimerHost->m_isTimeOut = true;
            oTimerHost->m_timeOutTime = oTimerHost->m_timeOutTime + oTimerHost->m_interval;
            bHasTimeOut = true;
        }
    }
    if (bHasTimeOut)
    {
        CxApplication::pushProcessCallBack(fn_processTimers);
        CxApplication::signalMainThread();
    }
}

class UpdateTimersThread : public CxJoinableThread
{
public:
    void run()
    {
        while ( f_iThreadTimerStatus )
        {
            fn_updateTimers();
            //besttodo : sleep 的毫秒数可以用最大公约数除10
            CxThread::sleep(10);
        }
    }

    inline void waitExit() { f_iThreadTimerStatus = 0; join(); }

};

UpdateTimersThread f_threadTimer;

void fn_doStopTimer(void *timer, int iInterval, bool bObject)
{
    if (bObject)
    {
        fn_separateFromHost(timer);
    }
    else
    {
        fn_separateFromHost(timer, iInterval);
    }
}

void fn_doStopTimer(void *timer)
{
    fn_separateFromHost(timer);
}

void fn_doStopAllTimer(void *timer)
{
    fn_separateFromHost(timer);
}

void fn_doStopAllTimer(int iInterval)
{
    fn_separateFromHost(iInterval);
}

void fn_doStartTimer(void * timer, int iInterval, bool bObject)
{
    fn_doStopTimer(timer, iInterval, bObject);
    fn_addTimer(timer, iInterval, bObject);
}



void fn_modifyTimer(int iEvent, int iInterval, const void * pData, int iLength, void * timer, void * oTarget)
{
    assert(timer != NULL);
    int iTimerType = 0;
    if (oTarget)
        iTimerType = * (int *)oTarget;

    if (iEvent == GM_TIMER_EVENT_ADD)
    {
        if (iTimerType == GM_TIMER_TYPE_OBJECT)
        {
            CxTimer * oTimer = (CxTimer *)timer;
            fn_doStartTimer(oTimer, oTimer->interval(), true);
        }
        else if (iTimerType == GM_TIMER_TYPE_FUNTION)
        {
            fn_doStartTimer(timer, iInterval, false);
        }
    }
    else if (iEvent == GM_TIMER_EVENT_DELETE)
    {
        if (iTimerType == GM_TIMER_TYPE_OBJECT)
        {
            CxTimer * oTimer = (CxTimer *)timer;
            fn_doStopTimer(oTimer, oTimer->interval(), true);
        }
        else if (iTimerType == GM_TIMER_TYPE_FUNTION)
        {
            if (iInterval > 0)
            {
                fn_doStopTimer(timer, iInterval, false);
            }
            else
            {
                fn_doStopTimer(timer);
            }
        }
    }
}



void CxTimerManager::startTimer(CxTimer *timer)
{
    assert(timer != NULL);
    if (CxApplication::isApplicationThread())
    {
        fn_doStartTimer(timer, timer->interval(), true);
    }
    else
    {
        CxApplication::pushProcessCallBack(fn_modifyTimer, GM_TIMER_EVENT_ADD, 0, 0, 0, timer, & f_timer_type_object);
        CxApplication::signalMainThread();
    }
}

void CxTimerManager::stopTimer(CxTimer *timer)
{
    assert(timer != NULL);
    if (CxApplication::isApplicationThread())
    {
        fn_doStopTimer(timer, timer->interval(), true);
    }
    else
    {
        CxApplication::pushProcessCallBack(fn_modifyTimer, GM_TIMER_EVENT_DELETE, 0, 0, 0, timer, & f_timer_type_object);
        CxApplication::signalMainThread();
        while ( 1 )
        {
            CxThread::sleep(0);
            if (! fn_getTimerHost(timer))
                return;
        }
    }
}

void CxTimerManager::startTimer(fn_timer_timeout_t timer, int iInterval)
{
    assert(timer != NULL);
    assert(iInterval > 0);
    if (CxApplication::isApplicationThread())
    {
        fn_doStartTimer((void *)timer, iInterval, false);
    }
    else
    {
        CxApplication::pushProcessCallBack(fn_modifyTimer, GM_TIMER_EVENT_ADD, iInterval, 0, 0, (void *)timer, & f_timer_type_funtion);
        CxApplication::signalMainThread();
    }
}

void CxTimerManager::stopTimer(fn_timer_timeout_t timer, int iInterval)
{
    assert(timer != NULL);
    if (CxApplication::isApplicationThread())
    {
        fn_doStopTimer((void *)timer, iInterval, false);
    }
    else
    {
        CxApplication::pushProcessCallBack(fn_modifyTimer, GM_TIMER_EVENT_DELETE, iInterval, 0, 0, (void *)timer, & f_timer_type_funtion);
        CxApplication::signalMainThread();
    }
}

void CxTimerManager::stopTimer(fn_timer_timeout_t timer)
{
    assert(timer != NULL);
    if (CxApplication::isApplicationThread())
    {
        fn_doStopTimer((void *)timer);
    }
    else
    {
        CxApplication::pushProcessCallBack(fn_modifyTimer, GM_TIMER_EVENT_DELETE, 0, 0, 0, (void *)timer, & f_timer_type_funtion);
        CxApplication::signalMainThread();
    }
}

void CxTimerManager::start()
{
    static bool bNoStart = true;
    if (bNoStart)
    {
#ifdef GM_OS_WIN
        //THREAD_PRIORITY_TIME_CRITICAL
        int iPriority = THREAD_PRIORITY_HIGHEST;
#else
        //SCHED_OTHER,SCHED_FIFO,SCHED_RR
        int iPriority = 2;
#endif
        bNoStart = true;
        f_threadTimer.start(iPriority);
    }
}

void CxTimerManager::stop()
{
    f_threadTimer.waitExit();
}



//
// *** CxTimer
//

void CxTimer::start()
{
    if (m_isStarted)
        return;
    if (! m_oTimerTimeOut)
        return;
    if (m_interval <= 0)
        return;
    CxTimerManager::startTimer(this);
    //besttodo : m_isStarted = CxTimerObjectManager::isExist(this); if (! m_isStarted) 内部错误
    m_isStarted = true;
}

void CxTimer::stop()
{
    CxTimerManager::stopTimer(this);
    m_isStarted = false;
}

