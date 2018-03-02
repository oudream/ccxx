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
/// CxTimerHost : With CxTimer
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

#define ci_timerhostes_size (100)
static CxTimerHost f_mTimerHostes[ci_timerhostes_size];

static int f_timer_type_object = GM_TIMER_TYPE_OBJECT;
static int f_timer_type_funtion = GM_TIMER_TYPE_FUNTION;


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
            cxWarning() << "Error ! Error ! Error ! CxTimer Cache Full , Can not start timer !!!";
            cout        << "Error ! Error ! Error ! CxTimer Cache Full , Can not start timer !!!";
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
        CxApplication::pushProcessCallBack(fn_processTimers, 0, 0, NULL, 0, NULL, NULL, true);
        CxApplication::signalMainThread();
    }
}

class UpdateTimersThread : public CxJoinableThread
{
public:
    static void startUpdateTimers();

    static void stopUpdateTimers();

    UpdateTimersThread()
    {
        _isStarted = false;
    }
    ~UpdateTimersThread()
    {
    }

    inline void stop() { _isStarted=false; join(); }

protected:
    void run()
    {
        while ( _isStarted )
        {
            fn_updateTimers();
            //todo : sleep 's ms can = (The greatest common divisor) gcd div 10
            CxThread::sleep(10);
        }
    }

private:
    volatile bool _isStarted;

};

UpdateTimersThread * fn_threadTimerInit()
{
    CxApplication::registStartFunction(UpdateTimersThread::startUpdateTimers, 19000);
    CxApplication::registStopFunction(UpdateTimersThread::stopUpdateTimers);
    static UpdateTimersThread m;
    return & m;
}
static UpdateTimersThread * f_oThreadTimer = fn_threadTimerInit();

void UpdateTimersThread::startUpdateTimers()
{
    if (f_oThreadTimer && ! f_oThreadTimer->_isStarted)
    {
#ifdef GM_OS_WIN
        //THREAD_PRIORITY_TIME_CRITICAL
        int iPriority = THREAD_PRIORITY_HIGHEST;
#else
        //SCHED_OTHER,SCHED_FIFO,SCHED_RR
        int iPriority = 2;
#endif
        f_oThreadTimer->_isStarted = true;
        f_oThreadTimer->start(iPriority);
    }
}

void UpdateTimersThread::stopUpdateTimers()
{
    if (f_oThreadTimer && f_oThreadTimer->_isStarted)
    {
        f_oThreadTimer->stop();
    }
}

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
        if (CxApplication::applicationStatus())
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
    //todo : m_isStarted = CxTimerObjectManager::isExist(this); if (! m_isStarted) : interne error
    m_isStarted = true;
}

void CxTimer::stop()
{
    CxTimerManager::stopTimer(this);
    m_isStarted = false;
}

