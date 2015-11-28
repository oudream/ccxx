#include "cxthread.h"

#if _POSIX_PRIORITY_SCHEDULING > 0
#include <sched.h>
static int realtime_policy = SCHED_FIFO;
#endif

#undef  _POSIX_SPIN_LOCKS

static unsigned max_sharing = 0;

#if _POSIX_TIMERS > 0 && defined(POSIX_TIMERS)
extern int _posix_clocking;
int _posix_clocking = CLOCK_REALTIME;
#endif

struct mutex_entry
{
    cx_pthread_mutex_t mutex;
    struct mutex_entry *next;
    const void *pointer;
    unsigned count;
};

class mutex_index : public CxMutex
{
public:
    struct mutex_entry *list;

    mutex_index();
};
static mutex_index single_table;
static mutex_index *mutex_table = &single_table;
static unsigned mutex_indexing = 1;

#ifdef  __PTH__
static pth_key_t threadmap;
#else
#ifdef  GM_OS_WIN
static DWORD threadmap;
#else
static pthread_key_t threadmap;
#endif
#endif

mutex_index::mutex_index() : CxMutex()
{
    list = NULL;
}

#if !defined(GM_OS_WIN) && !defined(__PTH__)
CxConditional::attribute * CxConditional::singleAttribute()
{
    static CxConditional::attribute attr;
    return & attr;
}
#endif

#ifdef  __PTH__
static int pthread_cond_timedwait(pthread_cond_t *cond, cx_pthread_mutex_t *mutex, const struct timespec *abstime)
{
    static pth_key_t ev_key = PTH_KEY_INIT;
    pth_event_t ev = pth_event(PTH_EVENT_TIME|PTH_MODE_STATIC, &ev_key,
        pth_time(abstime->tv_sec, (abstime->tv_nsec) / 1000));

    if(!pth_cond_await(cond, mutex, ev))
        return errno;
    return 0;
}

static void pthread_shutdown(void)
{
    pth_kill();
}
#endif

static unsigned hash_address(const void *ptr, unsigned indexing)
{
    assert(ptr != NULL);

    unsigned key = 0;
    unsigned count = 0;
    const unsigned char *addr = (unsigned char *)(&ptr);

    if(indexing < 2)
        return 0;

    // skip lead zeros if little endian...
    while(count < sizeof(const void *) && *addr == 0) {
        ++count;
        ++addr;
    }

    while(count++ < sizeof(const void *) && *addr)
        key = (key << 1) ^ *(addr++);

    return key % indexing;
}

void CxConditional::set(struct timespec *ts, timems_t msec)
{
    assert(ts != NULL);

#if _POSIX_TIMERS > 0 && defined(POSIX_TIMERS)
    clock_gettime(_posix_clocking, ts);
#else
    timeval tv;
    //needtodo
//    gettimeofday(&tv, NULL);
    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = tv.tv_usec * 1000l;
#endif
    ts->tv_sec += msec / 1000;
    ts->tv_nsec += (msec % 1000) * 1000000l;
    while(ts->tv_nsec >= 1000000000l) {
        ++ts->tv_sec;
        ts->tv_nsec -= 1000000000l;
    }
}


#ifdef  GM_OS_WIN

#ifdef  _MSCONDITIONAL_

CxConditional::CxConditional()
{
    InitializeCriticalSection(&mutex);
    InitializeConditionVariable(&cond);
}

CxConditional::~CxConditional()
{
    DeleteCriticalSection(&mutex);
}

void CxConditional::wait(void)
{
    SleepConditionVariableCS(&cond, &mutex, INFINITE);
}

bool CxConditional::wait(timems_t timeout)
{
    if(SleepConditionVariableCS(&cond, &mutex, timeout))
        return true;

    return false;
}

void CxConditional::signal(void)
{
    WakeConditionVariable(&cond);
}

void CxConditional::broadcast(void)
{
    WakeAllConditionVariable(&cond);
}

#else
void CxConditional::wait(void)
{
    int result;

    EnterCriticalSection(&mlock);
    ++waiting;
    LeaveCriticalSection(&mlock);
    LeaveCriticalSection(&mutex);
    result = WaitForMultipleObjects(2, events, FALSE, INFINITE);
    EnterCriticalSection(&mlock);
    --waiting;
    result = ((result == WAIT_OBJECT_0 + ENENT_BROADCAST) && (waiting == 0));
    LeaveCriticalSection(&mlock);
    if(result)
        ResetEvent(events[ENENT_BROADCAST]);
    EnterCriticalSection(&mutex);
}

void CxConditional::signal(void)
{
    EnterCriticalSection(&mlock);
    if(waiting)
    {
        SetEvent(events[EVENT_SIGNAL]);
    }
    LeaveCriticalSection(&mlock);
}

void CxConditional::broadcast(void)
{
    EnterCriticalSection(&mlock);
    if(waiting)
        SetEvent(events[ENENT_BROADCAST]);
    LeaveCriticalSection(&mlock);

}

CxConditional::CxConditional()
{
    waiting = 0;

    InitializeCriticalSection(&mutex);
    InitializeCriticalSection(&mlock);
    events[EVENT_SIGNAL] = CreateEvent(NULL, FALSE, FALSE, NULL);
    events[ENENT_BROADCAST] = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CxConditional::~CxConditional()
{
    DeleteCriticalSection(&mlock);
    DeleteCriticalSection(&mutex);
    CloseHandle(events[EVENT_SIGNAL]);
    CloseHandle(events[ENENT_BROADCAST]);
}

bool CxConditional::wait(timems_t timeout)
{
    int result;
    bool rtn = true;

    if(!timeout)
        return false;

    EnterCriticalSection(&mlock);
    ++waiting;
    LeaveCriticalSection(&mlock);
    LeaveCriticalSection(&mutex);
    result = WaitForMultipleObjects(2, events, FALSE, timeout);
    EnterCriticalSection(&mlock);
    --waiting;
    if(result == WAIT_OBJECT_0 || result == WAIT_OBJECT_0 + ENENT_BROADCAST)
        rtn = true;
    result = ((result == WAIT_OBJECT_0 + ENENT_BROADCAST) && (waiting == 0));
    LeaveCriticalSection(&mlock);
    if(result)
        ResetEvent(events[ENENT_BROADCAST]);
    EnterCriticalSection(&mutex);
    return rtn;
}
#endif

bool CxConditional::wait(struct timespec *ts)
{
    assert(ts != NULL);

    return wait((timems_t)(ts->tv_sec * 1000 + (ts->tv_nsec / 1000000l)));
}

#else

#include <stdio.h>

#ifndef __PTH__
CxConditional::attribute::attribute()
{
    CxThread::init();
    pthread_condattr_init( &attr );
#if _POSIX_TIMERS > 0 && defined(HAVE_PTHREAD_CONDATTR_SETCLOCK) && defined(POSIX_TIMERS)
#if defined(_POSIX_MONOTONIC_CLOCK)
    if(!pthread_condattr_setclock( &attr , CLOCK_MONOTONIC))
        _posix_clocking = CLOCK_MONOTONIC;
#else
    pthread_condattr_setclock( &attr , CLOCK_REALTIME);
#endif
#endif
}
#endif

CxConditional::CxConditional()
{
#ifdef  __PTH__
    CxThread::init();
    pth_cond_init(&cond);
    pth_mutex_init(&mutex);
#else
    GM_ASSERT_IFNOT(pthread_cond_init(&cond, & singleAttribute()->attr) == 0, "conditional init failed");
    GM_ASSERT_IFNOT(cx_pthread_mutex_init(&mutex, NULL) == 0, "mutex init failed");
#endif
}

CxConditional::~CxConditional()
{
#ifndef __PTH__
    pthread_cond_destroy(&cond);
    cx_pthread_mutex_destroy(&mutex);
#endif
}

bool CxConditional::wait(timems_t timeout)
{
    struct timespec ts;
    set(&ts, timeout);
    return wait(&ts);
}

bool CxConditional::wait(struct timespec *ts)
{
    assert(ts != NULL);

    if(pthread_cond_timedwait(&cond, &mutex, ts) == ETIMEDOUT)
        return false;

    return true;
}

#endif





#ifdef  GM_OS_WIN

void CxSingleWait::wait(void)
{
    WaitForSingleObject(m_event, INFINITE);
}

void CxSingleWait::signal(void)
{
    EnterCriticalSection(&mlock);
    SetEvent(m_event);
    LeaveCriticalSection(&mlock);
}

CxSingleWait::CxSingleWait()
{
    InitializeCriticalSection(&mlock);
    m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CxSingleWait::~CxSingleWait()
{
    DeleteCriticalSection(&mlock);
    CloseHandle(m_event);
}

bool CxSingleWait::wait(timems_t timeout)
{
    int result;
    bool rtn = true;

    if(!timeout)
        return false;

    result = WaitForSingleObject(m_event, timeout);
    if(result == WAIT_OBJECT_0)
        rtn = true;
    return rtn;
}

bool CxSingleWait::wait(struct timespec *ts)
{
    assert(ts != NULL);

    return wait((timems_t)(ts->tv_sec * 1000 + (ts->tv_nsec / 1000000l)));
}

#else

CxSingleWait::CxSingleWait()
{
    m_bSignaled = 0;
#ifdef  __PTH__
    CxThread::init();
    pth_cond_init(&cond);
    pth_mutex_init(&mutex);
#else
    GM_ASSERT_IFNOT(pthread_cond_init(&cond, & CxConditional::singleAttribute()->attr) == 0, "conditional init failed");
    GM_ASSERT_IFNOT(cx_pthread_mutex_init(&mutex, NULL) == 0, "mutex init failed");
#endif
}

CxSingleWait::~CxSingleWait()
{
#ifndef __PTH__
    pthread_cond_destroy(&cond);
    cx_pthread_mutex_destroy(&mutex);
#endif
}

bool CxSingleWait::wait(timems_t timeout)
{
    struct timespec ts;
    CxConditional::set(&ts, timeout);
    return wait(&ts);
}

bool CxSingleWait::wait(struct timespec *ts)
{
    assert(ts != NULL);

    if(pthread_cond_timedwait(&cond, &mutex, ts) == ETIMEDOUT)
        return false;

    return true;
}

#endif








#if defined(_MSCONDITIONAL_)

CxConditionalAccess::CxConditionalAccess()
{
    waiting = pending = sharing = 0;
    InitializeConditionVariable(&bcast);
}

CxConditionalAccess::~CxConditionalAccess()
{
}

bool CxConditionalAccess::waitBroadcast(timems_t timeout)
{
    assert(ts != NULL);

    if(SleepConditionVariableCS(&bcast, &mutex, timeout))
        return true;

    return false;
}

bool CxConditionalAccess::waitSignal(timems_t timeout)
{
    assert(ts != NULL);

    if(SleepConditionVariableCS(&cond, &mutex, timeout))
        return true;

    return false;
}

bool CxConditionalAccess::waitBroadcast(struct timespec *ts)
{
    assert(ts != NULL);

    return waitBroadcast((timems_t)(ts->tv_sec * 1000 + (ts->tv_nsec / 1000000l)));
}

bool CxConditionalAccess::waitSignal(struct timespec *ts)
{
    assert(ts != NULL);

    return waitSignal((timems_t)(ts->tv_sec * 1000 + (ts->tv_nsec / 1000000l)));
}

#elif defined(GM_OS_WIN)

void CxConditionalAccess::waitSignal(void)
{
    LeaveCriticalSection(&mutex);
    WaitForSingleObject(&events[EVENT_SIGNAL], INFINITE);
    EnterCriticalSection(&mutex);
}

void CxConditionalAccess::waitBroadcast(void)
{
    int result;

    EnterCriticalSection(&mlock);
    ++waiting;
    LeaveCriticalSection(&mlock);
    LeaveCriticalSection(&mutex);
    result = WaitForSingleObject(&events[ENENT_BROADCAST], INFINITE);
    EnterCriticalSection(&mlock);
    --waiting;
    result = ((result == WAIT_OBJECT_0) && (waiting == 0));
    LeaveCriticalSection(&mlock);
    if(result)
        ResetEvent(events[ENENT_BROADCAST]);
    EnterCriticalSection(&mutex);
}

CxConditionalAccess::CxConditionalAccess() : CxConditional()
{
    pending = waiting = sharing = 0;
}

CxConditionalAccess::~CxConditionalAccess()
{
}

bool CxConditionalAccess::waitSignal(timems_t timeout)
{
    int result;
    bool rtn = true;

    if(!timeout)
        return false;

    LeaveCriticalSection(&mutex);
    result = WaitForSingleObject(events[EVENT_SIGNAL], timeout);
    if(result == WAIT_OBJECT_0)
        rtn = true;
    EnterCriticalSection(&mutex);
    return rtn;
}

bool CxConditionalAccess::waitSignal(struct timespec *ts)
{
    assert(ts != NULL);

    return waitSignal((timems_t)(ts->tv_sec * 1000 + (ts->tv_nsec / 1000000l)));
}


bool CxConditionalAccess::waitBroadcast(timems_t timeout)
{
    int result;
    bool rtn = true;

    if(!timeout)
        return false;

    EnterCriticalSection(&mlock);
    ++waiting;
    LeaveCriticalSection(&mlock);
    LeaveCriticalSection(&mutex);
    result = WaitForSingleObject(events[ENENT_BROADCAST], timeout);
    EnterCriticalSection(&mlock);
    --waiting;
    if(result == WAIT_OBJECT_0)
        rtn = true;
    result = ((result == WAIT_OBJECT_0) && (waiting == 0));
    LeaveCriticalSection(&mlock);
    if(result)
        ResetEvent(events[ENENT_BROADCAST]);
    EnterCriticalSection(&mutex);
    return rtn;
}

bool CxConditionalAccess::waitBroadcast(struct timespec *ts)
{
    assert(ts != NULL);

    return waitBroadcast((timems_t)(ts->tv_sec * 1000 + (ts->tv_nsec / 1000000l)));
}

#else

CxConditionalAccess::CxConditionalAccess()
{
    waiting = pending = sharing = 0;
#ifdef  __PTH__
    pth_cond_init(&bcast);
#else
    GM_ASSERT_IFNOT(pthread_cond_init(&bcast, &singleAttribute()->attr) == 0, "conditional init failed");
#endif
}

CxConditionalAccess::~CxConditionalAccess()
{
#ifndef __PTH__
    pthread_cond_destroy(&bcast);
#endif
}

bool CxConditionalAccess::waitSignal(timems_t timeout)
{
    struct timespec ts;
    set(&ts, timeout);
    return waitSignal(&ts);
}

bool CxConditionalAccess::waitBroadcast(struct timespec *ts)
{
    assert(ts != NULL);

    if(pthread_cond_timedwait(&bcast, &mutex, ts) == ETIMEDOUT)
        return false;

    return true;
}

bool CxConditionalAccess::waitBroadcast(timems_t timeout)
{
    struct timespec ts;
    set(&ts, timeout);
    return waitBroadcast(&ts);
}

bool CxConditionalAccess::waitSignal(struct timespec *ts)
{
    assert(ts != NULL);

    if(pthread_cond_timedwait(&cond, &mutex, ts) == ETIMEDOUT)
        return false;

    return true;
}

#endif

void CxConditionalAccess::modify(void)
{
    lock();
    while(sharing) {
        ++pending;
        waitSignal();
        --pending;
    }
}

void CxConditionalAccess::commit(void)
{
    if(pending)
        signal();
    else if(waiting)
        broadcast();
    unlock();
}

void CxConditionalAccess::access(void)
{
    lock();
    assert(!max_sharing || sharing < max_sharing);
    while(pending) {
        ++waiting;
        waitBroadcast();
        --waiting;
    }
    ++sharing;
    unlock();
}

void CxConditionalAccess::release(void)
{
   lock();
    assert(sharing);

    --sharing;
    if(pending && !sharing)
        signal();
    else if(waiting && !pending)
        broadcast();
    unlock();
}

void CxConditionalAccess::limit_sharing(unsigned max)
{
    max_sharing = max;
}






CxSemaphore::CxSemaphore(unsigned limit) :
CxConditional()
{
    assert(limit > 0);

    count = limit;
    waits = 0;
    used = 0;
}

void CxSemaphore::_unlock(void)
{
    release();
}

bool CxSemaphore::wait(timems_t timeout)
{
    bool result = true;
    struct timespec ts;
    CxConditional::set(&ts, timeout);

    lock();
    while(used >= count && result) {
        ++waits;
        result = CxConditional::wait(&ts);
        --waits;
    }
    if(result)
        ++used;
    unlock();
    return result;
}

void CxSemaphore::wait(void)
{
    lock();
    if(used >= count) {
        ++waits;
        CxConditional::wait();
        --waits;
    }
    ++used;
    unlock();
}

void CxSemaphore::release(void)
{
    lock();
    if(used)
        --used;
    if(waits)
        signal();
    unlock();
}

void CxSemaphore::set(unsigned value)
{
    assert(value > 0);

    unsigned diff;

    lock();
    count = value;
    if(used >= count || !waits) {
        unlock();
        return;
    }
    diff = count - used;
    if(diff > waits)
        diff = waits;
    unlock();
    while(diff--) {
        lock();
        signal();
        unlock();
    }
}






CxBarrier::CxBarrier(unsigned limit) :
CxConditional()
{
    count = limit;
    waits = 0;
}

CxBarrier::~CxBarrier()
{
    lock();
    if(waits)
        broadcast();
    unlock();
}

void CxBarrier::set(unsigned limit)
{
    assert(limit > 0);

    lock();
    count = limit;
    if(count <= waits) {
        waits = 0;
        broadcast();
    }
    unlock();
}

void CxBarrier::dec(void)
{
    lock();
    if(count)
        --count;
    unlock();
}

unsigned CxBarrier::operator--(void)
{
    unsigned result;
    lock();
    if(count)
        --count;
    result = count;
    unlock();
    return result;
}

void CxBarrier::inc(void)
{
    lock();
    count++;
    if(count <= waits) {
        waits = 0;
        broadcast();
    }
    unlock();
}

unsigned CxBarrier::operator++(void)
{
    unsigned result;
    lock();
    count++;
    if(count <= waits) {
        waits = 0;
        broadcast();
    }
    result = count;
    unlock();
    return result;
}

bool CxBarrier::wait(timems_t timeout)
{
    bool result;

    CxConditional::lock();
    if(!count) {
        CxConditional::unlock();
        return true;
    }
    if(++waits >= count) {
        waits = 0;
        CxConditional::broadcast();
        CxConditional::unlock();
        return true;
    }
    result = CxConditional::wait(timeout);
    CxConditional::unlock();
    return result;
}

void CxBarrier::wait(void)
{
    CxConditional::lock();
    if(!count) {
        CxConditional::unlock();
        return;
    }
    if(++waits >= count) {
        waits = 0;
        CxConditional::broadcast();
        CxConditional::unlock();
        return;
    }
    CxConditional::wait();
    CxConditional::unlock();
}






CxMutex::guard::guard()
{
    object = NULL;
}

CxMutex::guard::guard(const void *obj)
{
    object = obj;
    if(obj)
        CxMutex::protect(object);
}

CxMutex::guard::~guard()
{
    release();
}

void CxMutex::guard::set(const void *obj)
{
    release();
    object = obj;
    if(obj)
        CxMutex::protect(object);
}

void CxMutex::guard::release(void)
{
    if(object) {
        CxMutex::release(object);
        object = NULL;
    }
}

CxMutex::CxMutex()
{
#ifdef  __PTH__
    pth_mutex_init(&mlock);
#else
    GM_ASSERT_IFNOT(cx_pthread_mutex_init(&mlock, NULL) == 0, "mutex init failed");
#endif
}

CxMutex::~CxMutex()
{
    cx_pthread_mutex_destroy(&mlock);
}

void CxMutex::indexing(unsigned index)
{
    if(index > 1) {
        mutex_table = new mutex_index[index];
        mutex_indexing = index;
    }
}

void CxMutex::protect(const void *ptr)
{
    mutex_index *index = &mutex_table[hash_address(ptr, mutex_indexing)];
    mutex_entry *entry, *empty = NULL;

    if(!ptr)
        return;

    index->acquire();
    entry = index->list;
    while(entry) {
        if(entry->count && entry->pointer == ptr)
            break;
        if(!entry->count)
            empty = entry;
        entry = entry->next;
    }
    if(!entry) {
        if(empty)
            entry = empty;
        else {
            entry = new struct mutex_entry;
            entry->count = 0;
            cx_pthread_mutex_init(&entry->mutex, NULL);
            entry->next = index->list;
            index->list = entry;
        }
    }
    entry->pointer = ptr;
    ++entry->count;
//  printf("ACQUIRE %p, THREAD %d, POINTER %p, COUNT %d\n", entry, Thread::self(), entry->pointer, entry->count);
    index->release();
    cx_pthread_mutex_lock(&entry->mutex);
}

void CxMutex::release(const void *ptr)
{
    mutex_index *index = &mutex_table[hash_address(ptr, mutex_indexing)];
    mutex_entry *entry;

    if(!ptr)
        return;

    index->acquire();
    entry = index->list;
    while(entry) {
        if(entry->count && entry->pointer == ptr)
            break;
        entry = entry->next;
    }

    assert(entry);
    if(entry) {
//      printf("RELEASE %p, THREAD %d, POINTER %p COUNT %d\n", entry, Thread::self(), entry->pointer, entry->count);
        cx_pthread_mutex_unlock(&entry->mutex);
        --entry->count;
    }
    index->release();
}

void CxMutex::_lock(void)
{
    cx_pthread_mutex_lock(&mlock);
}

void CxMutex::_unlock(void)
{
    cx_pthread_mutex_unlock(&mlock);
}





extern "C" {
#ifdef  GM_OS_WIN
    static unsigned __stdcall exec_thread(void *obj) {
        assert(obj != NULL);

        CxThread *th = static_cast<CxThread *>(obj);
        th->setPriority();
        th->run();
        th->exit();
        return 0;
    }
#else
    static void *exec_thread(void *obj)
    {
        assert(obj != NULL);

        CxThread *th = static_cast<CxThread *>(obj);
        th->setPriority();
        th->run();
        th->exit();
        return NULL;
    }
#endif
}



bool CxThread::equal(cx_pthread_t t1, cx_pthread_t t2)
{
#ifdef GM_OS_WIN
    return t1 == t2;
#else
#ifdef  __PTH__
    return (t1 == t2);
#else
    return pthread_equal(t1, t2) != 0;
#endif
#endif
}

// abstract class never runs...
bool CxThread::is_active(void)
{
    return false;
}

CxThread::CxThread(size_t size)
{
    stack = size;
    priority = 0;
#ifdef  GM_OS_WIN
    cancellor = INVALID_HANDLE_VALUE;
#else
    cancellor = NULL;
#endif
    init();
}

#if defined(GM_OS_WIN)

void CxThread::setPriority(void)
{
    HANDLE hThread = GetCurrentThread();
    priority += THREAD_PRIORITY_NORMAL;
    if(priority < THREAD_PRIORITY_LOWEST)
        priority = THREAD_PRIORITY_LOWEST;
    else if(priority > THREAD_PRIORITY_HIGHEST)
        priority = THREAD_PRIORITY_HIGHEST;

    SetThreadPriority(hThread, priority);
}
#elif _POSIX_PRIORITY_SCHEDULING > 0

void CxThread::setPriority(void)
{
#ifndef __PTH__
    int policy;
    struct sched_param sp;
    cx_pthread_t ptid = cx_pthread_self();
    int pri = 0;

    if(!priority)
        return;

    if(pthread_getschedparam(ptid, &policy, &sp))
        return;

    if(priority > 0) {
        policy = realtime_policy;
        if(realtime_policy == SCHED_OTHER)
            pri = sp.sched_priority + priority;
        else
            pri = sched_get_priority_min(policy) + priority;
        policy = realtime_policy;
        if(pri > sched_get_priority_max(policy))
            pri = sched_get_priority_max(policy);
    } else if(priority < 0) {
        pri = sp.sched_priority - priority;
        if(pri < sched_get_priority_min(policy))
            pri = sched_get_priority_min(policy);
    }

    sp.sched_priority = pri;
    pthread_setschedparam(ptid, policy, &sp);
#endif
}

#else
void CxThread::setPriority(void) {}
#endif

void CxThread::concurrency(int level)
{
#if defined(HAVE_PTHREAD_SETCONCURRENCY) && !defined(GM_OS_WIN)
    pthread_setconcurrency(level);
#endif
}

void CxThread::policy(int polid)
{
#if _POSIX_PRIORITY_SCHEDULING > 0
    realtime_policy = polid;
#endif
}


void CxThread::sleep(timems_t timeout)
{
#if defined(__PTH__)
    pth_usleep(timeout * 1000);
#elif defined(HAVE_PTHREAD_DELAY)
    timespec ts;
    ts.tv_sec = timeout / 1000l;
    ts.tv_nsec = (timeout % 1000l) * 1000000l;
    pthread_delay(&ts);
#elif defined(HAVE_PTHREAD_DELAY_NP)
    timespec ts;
    ts.tv_sec = timeout / 1000l;
    ts.tv_nsec = (timeout % 1000l) * 1000000l;
    pthread_delay_np(&ts);
#elif defined(GM_OS_WIN)
    ::Sleep(timeout);
#else
    usleep(timeout * 1000);
//    nanosleep(timeout * 1000);
#endif
}

void CxThread::yield(void)
{
#if defined(GM_OS_WIN)
    SwitchToThread();
#elif defined(__PTH__)
    pth_yield(NULL);
#elif defined(HAVE_PTHREAD_YIELD_NP)
    pthread_yield_np();
#elif defined(HAVE_PTHREAD_YIELD)
    pthread_yield();
#else
    sched_yield();
#endif
}

CxThread::~CxThread()
{
}

void CxThread::exit(void)
{
    cx_pthread_exit(NULL);
}

void CxThread::map(void)
{
    CxThread::init();
#ifdef  __PTH__
    pth_key_setdata(threadmap, this);
#else
#ifdef  GM_OS_WIN
    TlsSetValue(threadmap, this);
#else
    pthread_setspecific(threadmap, this);
#endif
#endif
}

CxThread *CxThread::get(void)
{
#ifdef  __PTH__
    return (CxThread *)pth_key_setdata(threadmap);
#else
#ifdef  GM_OS_WIN
    return (CxThread *)TlsGetValue(threadmap);
#else
    return (CxThread *)pthread_getspecific(threadmap);
#endif
#endif
}

void CxThread::init(void)
{
    static volatile bool initialized = false;

    if(!initialized) {
#ifdef  __PTH__
        pth_init();
        pth_key_create(&threadmap, NULL);
        atexit(pthread_shutdown);
#else
#ifdef  GM_OS_WIN
        threadmap = TlsAlloc();
#else
        pthread_key_create(&threadmap, NULL);
#endif
#endif
        initialized = true;
    }
}




CxJoinableThread::CxJoinableThread(size_t size)
    : CxThread(size)
{
#ifdef  GM_OS_WIN
    running = INVALID_HANDLE_VALUE;
    joining = false;
#else
    joining = false;
    running = false;
#endif
}

CxJoinableThread::~CxJoinableThread()
{
    join();
}

bool CxJoinableThread::is_active(void)
{
#ifdef  GM_OS_WIN
    return (running != INVALID_HANDLE_VALUE) && !joining;
#else
    return running && !joining;
#endif
}

#ifdef  GM_OS_WIN
void CxJoinableThread::start(int adj)
{
    if(running != INVALID_HANDLE_VALUE)
        return;

    priority = adj;

    if(stack == 1)
        stack = 1024;

    joining = false;
    running = (HANDLE)_beginthreadex(NULL, stack, &exec_thread, this, 0, (unsigned int *)&tid);
    if(!running)
        running = INVALID_HANDLE_VALUE;
}

void CxJoinableThread::join(void)
{
    cx_pthread_t self = cx_pthread_self();
    int rc;

    // already joined, so we ignore...
    if(running == INVALID_HANDLE_VALUE)
        return;

    // self join does cleanup...
    if(equal(tid, self)) {
        CloseHandle(running);
        running = INVALID_HANDLE_VALUE;
        CxThread::exit();
    }

    joining = true;
    rc = WaitForSingleObject(running, INFINITE);
    if(rc == WAIT_OBJECT_0 || rc == WAIT_ABANDONED) {
        CloseHandle(running);
        running = INVALID_HANDLE_VALUE;
    }
}

#else

void CxJoinableThread::start(int adj)
{
    int result;

    if(running)
        return;

    joining = false;
    priority = adj;

#ifndef __PTH__
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
#endif
// we typically use "stack 1" for min stack...
#ifdef  PTHREAD_STACK_MIN
    if(stack && stack < PTHREAD_STACK_MIN)
        stack = PTHREAD_STACK_MIN;
#else
    if(stack && stack < 2)
        stack = 0;
#endif
#ifdef  __PTH__
    pth_attr_t attr = PTH_ATTR_DEFAULT;
    pth_attr_set(attr, PTH_ATTR_JOINABLE);
    tid = pth_spawn(attr, &exec_thread, this);
#else
    if(stack)
        pthread_attr_setstacksize(&attr, stack);
    result = pthread_create(&tid, &attr, &exec_thread, this);
    pthread_attr_destroy(&attr);
    if(!result)
        running = true;
#endif
}

void CxJoinableThread::join(void)
{
    cx_pthread_t self = cx_pthread_self();

    // already joined, so we ignore...
    if(!running)
        return;

    if(equal(tid, self)) {
        running = false;
        CxThread::exit();
    }

    joining = true;

#ifdef  __PTH__
    if(pth_join(tid, NULL))
        running = false;
#else
    if(!pthread_join(tid, NULL))
        running = false;
#endif
}

#endif





bool CxDetachedThread::is_active(void)
{
    return active;
}

CxDetachedThread::CxDetachedThread(size_t size)
{
#ifdef  GM_OS_WIN
    cancellor = INVALID_HANDLE_VALUE;
#else
    cancellor = NULL;
#endif
    active = false;
    stack = size;
}

void CxDetachedThread::exit(void)
{
    delete this;
    cx_pthread_exit(NULL);
}

CxDetachedThread::~CxDetachedThread()
{
}

void CxDetachedThread::start(int adj)
{
#ifdef GM_OS_WIN
    HANDLE hThread;;

    priority = adj;

    if(stack == 1)
        stack = 1024;

    hThread = (HANDLE)_beginthreadex(NULL, stack, &exec_thread, this, 0, (unsigned int *)&tid);
    if(hThread != INVALID_HANDLE_VALUE)
        active = true;
    CloseHandle(hThread);
#else
    priority = adj;
#ifndef __PTH__
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
#endif
// we typically use "stack 1" for min stack...
#ifdef  PTHREAD_STACK_MIN
    if(stack && stack < PTHREAD_STACK_MIN)
        stack = PTHREAD_STACK_MIN;
#else
    if(stack && stack < 2)
        stack = 0;
#endif
#ifdef  __PTH__
    tid = pth_spawn(PTH_ATTR_DEFAULT, &exec_thread, this);
#else
    if(stack)
        pthread_attr_setstacksize(&attr, stack);
    pthread_create(&tid, &attr, &exec_thread, this);
    pthread_attr_destroy(&attr);
#endif
    active = true;
#endif
}
