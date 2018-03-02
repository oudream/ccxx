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
 * The accuracy of the setimer is ms. The POSIX Timer is designed for real-time applications. The interface supports ns-level clock accuracy.
 First, a process can have only one timer at a time. If the application needs to maintain multiple Interval different timers, you must write the code to maintain. This is very inconvenient. With POSIX Timer, a process can create as many Timer as you want.

 setitimer
 getitimer

// POSIX Timer:
 timer_create Creates a new Timer; and specifies timer notification mechanism
 timer_delete delete a Timer
 timer_gettime Get the time remaining on a POSIX.1b interval timer
 timer_settime start or stop a timer.
 timer_getoverrun Get the number of missing timing notifications
 */

#endif // CXTIMER_H
