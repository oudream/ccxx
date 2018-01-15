#ifndef CXPROCMUTEX_H
#define CXPROCMUTEX_H

#include "cxstring.h"
#include "cxthread.h"
#include "cxtime.h"

//multi process scenario
class GM_CCXX_CORE_API CxProcMutex
{
public:
    CxProcMutex(const std::string& name);

    ~CxProcMutex();

    void lock();

    bool tryLock();

    void unlock();

private:
    CxProcMutex();
    CxProcMutex(const CxProcMutex&);
    CxProcMutex& operator = (const CxProcMutex&);

    std::string _name;

#ifdef GM_OS_WIN32
private:
    HANDLE      _mutex;

#else
private:
    std::string getFileName();

#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
    sem_t* _sem;
#else
    int _lockfd; // lock file descriptor
    int _semfd;  // file used to identify semaphore
    int _semid;  // semaphore id
#endif
#endif

};

class CxProcMutexScope
{
public:
    inline CxProcMutexScope(CxProcMutex & mutex): _mutex(& mutex)
    {
        _mutex->lock();
    }

    inline CxProcMutexScope(CxProcMutex * mutex): _mutex(mutex)
    {
        _mutex->lock();
    }

    CxProcMutexScope(CxProcMutex& mutex, long milliseconds): _mutex(& mutex)
    {
        msepoch_t dtNow = CxTime::currentMsepoch();
        do
        {
            try
            {
                if (_mutex->tryLock())
                    return;
            }
            catch (...)
            {
                assert("cannot lock mutex");
//                throw std::exception();
            }
            CxThread::sleep(5);
        }
        while (CxTime::milliSecondDifferToNow(dtNow) > milliseconds);
    }

    CxProcMutexScope(CxProcMutex * mutex, long milliseconds): _mutex(mutex)
    {
        msepoch_t dtNow = CxTime::currentMsepoch();
        do
        {
            try
            {
                if (_mutex->tryLock())
                    return;
            }
            catch (...)
            {
                assert("cannot lock mutex");
                //                throw std::exception();
            }
            CxThread::sleep(5);
        }
        while (CxTime::milliSecondDifferToNow(dtNow) > milliseconds);
    }

    ~CxProcMutexScope()
    {
        try
        {
            _mutex->unlock();
        }
        catch (...)
        {
//			unexpected;
        }
    }

private:
    CxProcMutex * _mutex;

    CxProcMutexScope();
    CxProcMutexScope(const CxProcMutexScope&);
    CxProcMutexScope& operator = (const CxProcMutexScope&);
};



#endif // CXPROCMUTEX_H
