#ifndef CXTIMER_H
#define CXTIMER_H


#include "cxglobal.h"
#include "cxcontainer.h"
#include "cxtime.h"
#include "cxthread.h"


class CxTimer;

class ICxTimerNotify
{
protected:
    virtual void timer_timeOut(const CxTimer* oTimer) = 0;

    friend class CxTimer;

};


//*timer function

typedef void (*fn_timer_timeout_t)(int iInterval);



//*timer object
class GM_CCXX_CORE_API CxTimer
{
public:
    CxTimer() :
        m_interval(0),
        m_oTimerTimeOut(NULL),
        m_isStarted(false)
    {}

    CxTimer(ICxTimerNotify * oTimerTimeOut) :
        m_interval(0),
        m_oTimerTimeOut(oTimerTimeOut),
        m_isStarted(false)
    {}

    CxTimer(ICxTimerNotify * oTimerTimeOut, int iInterval) :
        m_interval(iInterval),
        m_oTimerTimeOut(oTimerTimeOut),
        m_isStarted(false)
    {}

    ~CxTimer()
    {
        stop();
    }

    inline void init(ICxTimerNotify * oTimerTimeOut, int iInterval)
    {
        if (m_isStarted)
            stop();
        m_interval = iInterval;
        m_oTimerTimeOut = oTimerTimeOut;
    }

    inline int interval() const { return m_interval; }

    inline void setInterval(int iInterval) {
        if (m_isStarted)
            stop();
        m_interval = iInterval;
    }

    inline const ICxTimerNotify * timerTimeOut() const { return m_oTimerTimeOut; }

    inline void setTimerTimeOut(ICxTimerNotify * oTimerTimeOut)
    {
        if (m_isStarted)
            stop();
        m_oTimerTimeOut = oTimerTimeOut;
    }

    inline bool isStarted() { return m_isStarted; }

    inline void doTimeOut() { if (m_oTimerTimeOut) m_oTimerTimeOut->timer_timeOut(this); }

    void start();

    void stop();

private:
    int m_interval;
    ICxTimerNotify * m_oTimerTimeOut;
    bool m_isStarted;

};



//attention : only one : iInterval and bIndependence == true
//attention : host is alway start
//main thread : startTimer stopTimer stopAll
class GM_CCXX_CORE_API CxTimerManager
{
public:
    static void startTimer(CxTimer * timer);

    static void stopTimer(CxTimer * timer);

    static void startTimer(fn_timer_timeout_t timer, int iInterval);

    static void stopTimer(fn_timer_timeout_t timer, int iInterval);

    static void stopTimer(fn_timer_timeout_t timer);

    static void start();

    static void stop();

};



/***
 * windows timer
#include <windows.h>
SetTimer
KillTimer

TimerQueueTimer
CreateTimerQueue
CreateTimerQueueTimer
DeleteTimerQueue
DeleteTimerQueueTimer
ChangeTimerQueueTimer

CreateWaitableTimer
OpenWaitableTimerA
OpenWaitableTimerW
SetWaitableTimer
WaitableTimer
 */

/***
 * setimer 的精度是 ms，POSIX Timer 是针对有实时要求的应用所设计的，接口支持 ns 级别的时钟精度。
 * 首先，一个进程同一时刻只能有一个 timer。假如应用需要同时维护多个 Interval 不同的计时器，必须自己写代码来维护。这非常不方便。使用 POSIX Timer，一个进程可以创建任意多个 Timer。
//
setitimer
getitimer

//POSIX Timer:
timer_create 	创建一个新的 Timer；并且指定定时器到时通知机制
timer_delete	删除一个 Timer
timer_gettime	Get the time remaining on a POSIX.1b interval timer
timer_settime	开始或者停止某个定时器。
timer_getoverrun	获取丢失的定时通知个数
 */

#endif // CXTIMER_H
