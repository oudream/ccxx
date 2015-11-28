#ifndef CXTHREAD_H
#define CXTHREAD_H

#include "cxglobal.h"

class CxThreadGlobal
{
public:

};

/**
 * The conditional is a common base for other thread synchronizing classes.
 * Many of the complex sychronization objects, including barriers, semaphores,
 * and various forms of read/write locks are all built from the conditional.
 * This assures that the minimum functionality to build higher order thread
 * synchronizing objects is a pure conditional, and removes dependencies on
 * what may be optional features or functions that may have different
 * behaviors on different pthread implimentations and platforms.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxConditional
{
private:
    friend class CxSingleWait;
    friend class CxConditionalAccess;

#if defined(_MSCONDITIONAL_)
    CRITICAL_SECTION mutex;
    CONDITION_VARIABLE cond;
#elif defined(GM_OS_WIN)
    enum {EVENT_SIGNAL = 0, ENENT_BROADCAST = 1};
    HANDLE events[2];
    unsigned waiting;
    CRITICAL_SECTION mlock;
    CRITICAL_SECTION mutex;
#else
#ifndef __PTH__
    class attribute
    {
    public:
        pthread_condattr_t attr;
        attribute();
    };

    static attribute * singleAttribute();
#endif

    pthread_cond_t cond;
    cx_pthread_mutex_t mutex;
#endif

protected:
    /**
     * Conditional wait for signal on millisecond timeout.
     * @param timeout in milliseconds.
     * @return true if signalled, false if timer expired.
     */
    bool wait(timems_t timeout);

    /**
     * Conditional wait for signal on timespec timeout.
     * @param timeout as a high resolution timespec.
     * @return true if signalled, false if timer expired.
     */
    bool wait(struct timespec *timeout);

#ifdef  GM_OS_WIN
    inline void lock(void)
        {EnterCriticalSection(&mutex);}

    inline void unlock(void)
        {LeaveCriticalSection(&mutex);}

    void wait(void);

    void signal(void);

    void broadcast(void);

#else
    /**
     * Lock the conditional's supporting mutex.
     */
    inline void lock(void)
        {cx_pthread_mutex_lock(&mutex);};

    /**
     * Unlock the conditional's supporting mutex.
     */
    inline void unlock(void)
        {cx_pthread_mutex_unlock(&mutex);};

    /**
     * Wait (block) until signalled.
     */
    inline void wait(void)
        {cx_pthread_cond_wait(&cond, &mutex);};

    /**
     * Signal the conditional to release one waiting thread.
     */
    inline void signal(void)
        {cx_pthread_cond_signal(&cond);};

    /**
     * Signal the conditional to release all waiting threads.
     */
    inline void broadcast(void)
        {cx_pthread_cond_broadcast(&cond);};
#endif

    /**
     * Initialize and construct conditional.
     */
    CxConditional();

    /**
     * Destroy conditional, release any blocked threads.
     */
    ~CxConditional();

public:
    /**
     * Convert a millisecond timeout into use for high resolution
     * conditional timers.
     * @param hires timespec representation to set.
     * @param timeout to convert.
     */
    static void set(struct timespec *hires, timems_t timeout);

};


class CxSingleWait
{
private:
    friend class CxConditionalAccess;

#if defined(GM_OS_WIN)
    HANDLE m_event;
    CRITICAL_SECTION mlock;
#else
    volatile int m_bSignaled;
    pthread_cond_t cond;
    cx_pthread_mutex_t mutex;
#endif

public:

    bool wait(timems_t timeout);

    bool wait(struct timespec *timeout);

#ifdef  GM_OS_WIN
    void wait(void);

    void signal(void);
#else
    inline void wait(void)
    {
        cx_pthread_mutex_lock(&mutex);
        if (m_bSignaled)
        {
            m_bSignaled = 0;
            cx_pthread_mutex_unlock(&mutex);
            return;
        }
        cx_pthread_cond_wait(&cond, &mutex);
        cx_pthread_mutex_unlock(&mutex);
    }

    inline void signal(void)
    {
        cx_pthread_mutex_lock(&mutex);
        m_bSignaled = 1;
        cx_pthread_cond_signal(&cond);
        cx_pthread_mutex_unlock(&mutex);
    }

#endif

    CxSingleWait();

    ~CxSingleWait();

};


/**
 * The conditional rw seperates scheduling for optizming behavior or rw locks.
 * This varient of conditonal seperates scheduling read (broadcast wakeup) and
 * write (signal wakeup) based threads.  This is used to form generic rwlock's
 * as well as the specialized condlock.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxConditionalAccess : private CxConditional
{
protected:
#if defined _MSCONDITIONAL_
    CONDITION_VARIABLE bcast;
#elif !defined(GM_OS_WIN)
    pthread_cond_t bcast;
#endif

    unsigned pending, waiting, sharing;

    /**
     * Conditional wait for signal on millisecond timeout.
     * @param timeout in milliseconds.
     * @return true if signalled, false if timer expired.
     */
    bool waitSignal(timems_t timeout);

    /**
     * Conditional wait for broadcast on millisecond timeout.
     * @param timeout in milliseconds.
     * @return true if signalled, false if timer expired.
     */
    bool waitBroadcast(timems_t timeout);


    /**
     * Conditional wait for signal on timespec timeout.
     * @param timeout as a high resolution timespec.
     * @return true if signalled, false if timer expired.
     */
    bool waitSignal(struct timespec *timeout);

    /**
     * Conditional wait for broadcast on timespec timeout.
     * @param timeout as a high resolution timespec.
     * @return true if signalled, false if timer expired.
     */
    bool waitBroadcast(struct timespec *timeout);

    /**
     * Convert a millisecond timeout into use for high resolution
     * conditional timers.
     * @param hires timespec representation to set.
     * @param timeout to convert.
     */
    inline static void set(struct timespec *hires, timems_t timeout)
        {CxConditional::set(hires, timeout);};


#ifdef  GM_OS_WIN
    inline void lock(void)
        {EnterCriticalSection(&mutex);};

    inline void unlock(void)
        {LeaveCriticalSection(&mutex);};

    void waitSignal(void);
    void waitBroadcast(void);

    inline void signal(void)
        {CxConditional::signal();};

    inline void broadcast(void)
        {CxConditional::broadcast();};

#else
    /**
     * Lock the conditional's supporting mutex.
     */
    inline void lock(void)
        {cx_pthread_mutex_lock(&mutex);};

    /**
     * Unlock the conditional's supporting mutex.
     */
    inline void unlock(void)
        {cx_pthread_mutex_unlock(&mutex);};

    /**
     * Wait (block) until signalled.
     */
    inline void waitSignal(void)
        {cx_pthread_cond_wait(&cond, &mutex);};

    /**
     * Wait (block) until broadcast.
     */
    inline void waitBroadcast(void)
        {cx_pthread_cond_wait(&bcast, &mutex);};


    /**
     * Signal the conditional to release one signalled thread.
     */
    inline void signal(void)
        {cx_pthread_cond_signal(&cond);};

    /**
     * Signal the conditional to release all broadcast threads.
     */
    inline void broadcast(void)
        {cx_pthread_cond_broadcast(&bcast);};
#endif

public:
    /**
     * Initialize and construct conditional.
     */
    CxConditionalAccess();

    /**
     * Destroy conditional, release any blocked threads.
     */
    ~CxConditionalAccess();

    /**
     * Access mode shared thread scheduling.
     */
    void access(void);

    /**
     * Exclusive mode write thread scheduling.
     */
    void modify(void);

    /**
     * Release access mode read scheduling.
     */
    void release(void);

    /**
     * Complete exclusive mode write scheduling.
     */
    void commit(void);

    /**
     * Specify a maximum sharing (access) limit.  This can be used
     * to detect locking errors, such as when aquiring locks that are
     * not released.
     * @param max sharing level.
     */
    void limit_sharing(unsigned max);
};


/**
 * A portable implimentation of "barrier" thread sychronization.  A barrier
 * waits until a specified number of threads have all reached the CxBarrier,
 * and then releases all the threads together.  This implimentation works
 * regardless of whether the thread library supports barriers since it is
 * built from conditional.  It also differs in that the number of threads
 * required can be changed dynamically at runtime, unlike pthread barriers
 * which, when supported, have a fixed limit defined at creation time.  Since
 * we use conditionals, another feature we can add is optional support for a
 * wait with timeout.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxBarrier : private CxConditional
{
private:
    unsigned count;
    unsigned waits;

public:
    /**
     * Construct a barrier with an initial size.
     * @param count of threads required.
     */
    CxBarrier(unsigned count);

    /**
     * Destroy barrier and release pending threads.
     */
    ~CxBarrier();

    /**
     * Dynamically alter the number of threads required.  If the size is
     * set below the currently waiting threads, then the barrier releases.
     * @param count of threads required.
     */
    void set(unsigned count);

    /**
     * Dynamically increment the number of threads required.
     */
    void inc(void);

    /**
     * Reduce the number of threads required.
     */
    void dec(void);

    /**
     * Alternative prefix form of the same increment operation.
     * @return the current amount of threads.
     */
    unsigned operator++(void);

    unsigned operator--(void);

    /**
     * Wait at the barrier until the count of threads waiting is reached.
     */
    void wait(void);

    /**
     * Wait at the barrier until either the count of threads waiting is
     * reached or a timeout has occurred.
     * @param timeout to wait in milliseconds.
     * @return true if barrier reached, false if timer expired.
     */
    bool wait(timems_t timeout);

};

/**
 * A portable counting semaphore class.  A semaphore will allow threads
 * to pass through it until the count is reached, and blocks further threads.
 * Unlike pthread semaphore, our semaphore class supports it's count limit
 * to be altered during runtime and the use of timed waits.  This class also
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxSemaphore : protected CxConditional
{
protected:
    unsigned count, waits, used;

    virtual void _unlock(void);

public:
    /**
     * Construct a semaphore with an initial count of threads to permit.
     */
    CxSemaphore(unsigned count = 0);

    /**
     * Wait until the semphore usage count is less than the thread limit.
     * Increase used count for our thread when unblocked.
     */
    void wait(void);

    /**
     * Wait until the semphore usage count is less than the thread limit.
     * Increase used count for our thread when unblocked, or return without
     * changing if timed out.
     * @param timeout to wait in millseconds.
     * @return true if success, false if timeout.
     */
    bool wait(timems_t timeout);

    /**
     * Alter semaphore limit at runtime
     * @param count of threads to allow.
     */
    void set(unsigned count);

    /**
     * Release the semaphore after waiting for it.
     */
    void release(void);

    /**
     * Convenience operator to wait on a counting semaphore.
     */
    inline void operator++(void)
        {wait();};

    /**
     * Convenience operator to release a counting semaphore.
     */
    inline void operator--(void)
        {release();};

};

/**
 * Generic non-recursive exclusive lock class.  This class also impliments
 * the exclusive_lock protocol.  In addition, an interface is offered to
 * support dynamically managed mutexes which are internally pooled.  These
 * can be used to protect and serialize arbitrary access to memory and
 * objects on demand.  This offers an advantage over embedding mutexes to
 * serialize access to individual objects since the maximum number of
 * mutexes will never be greater than the number of actually running threads
 * rather than the number of objects being potentially protected.  The
 * ability to hash the pointer address into an indexed table further optimizes
 * access by reducing the chance for collisions on the primary index mutex.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxMutex
{
protected:
    cx_pthread_mutex_t mlock;

    virtual void _lock(void);

    virtual void _unlock(void);

    friend class CxMutexScope;

public:
    /**
     * Guard class to apply scope based mutex locking to objects.  The mutex
     * is located from the mutex pool rather than contained in the target
     * object, and the lock is released when the guard object falls out of
     * scope.  This is essentially an automation mechanism for mutex::protect.
     * @author David Sugar <dyfet@gnutelephony.org>
     */
    class guard
    {
    private:
        const void *object;

    public:
        /**
          * Create an unitialized instance of guard.  Usually used with a
          * guard = operator.
          */
        guard();

        /**
         * Construct a guard for a specific object.
         * @param object to guard.
         */
        guard(const void *object);

        /**
         * Release mutex when guard falls out of scope.
         */
        ~guard();

        /**
         * Set guard to mutex lock a new object.  If a lock is currently
         * held, it is released.
         * @param object to guard.
         */
        void set(const void *object);

        /**
         * Prematurely release a guard.
         */
        void release(void);

        /**
         * Set guard to mutex lock a new object.  If a lock is currently
         * held, it is released.
         * @param pointer to object to guard.
         */
        inline void operator=(void *pointer)
            {set(pointer);};

    };


    /**
     * Create a mutex lock.
     */
    CxMutex();

    /**
     * Destroy mutex lock, release waiting threads.
     */
    ~CxMutex();

    /**
     * Acquire mutex lock.  This is a blocking operation.
     */
    inline void acquire(void)
        {cx_pthread_mutex_lock(&mlock);};

    /**
     * Acquire mutex lock.  This is a blocking operation.
     */
    inline void lock(void)
        {cx_pthread_mutex_lock(&mlock);};

    /**
     * Release acquired lock.
     */
    inline void unlock(void)
        {cx_pthread_mutex_unlock(&mlock);};

    /**
     * Release acquired lock.
     */
    inline void release(void)
        {cx_pthread_mutex_unlock(&mlock);};

    /**
     * Convenience function to acquire os native mutex lock directly.
     * @param lock to acquire.
     */
    inline static void acquire(cx_pthread_mutex_t *lock)
        {cx_pthread_mutex_lock(lock);};

    /**
     * Convenience function to release os native mutex lock directly.
     * @param lock to release.
     */
    inline static void release(cx_pthread_mutex_t *lock)
        {cx_pthread_mutex_unlock(lock);};

    /**
     * Specify hash table size for guard protection.  The default is 1.
     * This should be called at initialization time from the main thread
     * of the application before any other threads are created.
     * @param size of hash table used for guarding.
     */
    static void indexing(unsigned size);

    /**
     * Specify pointer/object/resource to guard protect.  This uses a
     * dynamically managed mutex.
     * @param pointer to protect.
     */
    static void protect(const void *pointer);

    /**
     * Specify a pointer/object/resource to release.
     * @param pointer to release.
     */
    static void release(const void *pointer);

};

class CxMutexScope
{
private:
    CxMutex * m_mutex;

public:
    inline CxMutexScope(CxMutex * mutex) : m_mutex(mutex) {
        m_mutex->lock();
    }

    inline CxMutexScope(CxMutex & mutex) : m_mutex(& mutex) {
        m_mutex->lock();
    }

    inline ~CxMutexScope() { m_mutex->unlock(); }

};


/**
 * An abstract class for defining classes that operate as a thread.  A derived
 * thread class has a run method that is invoked with the newly created
 * thread context, and can use the derived object to store all member data
 * that needs to be associated with that context.  This means the derived
 * object can safely hold thread-specific data that is managed with the life
 * of the object, rather than having to use the clumsy thread-specific data
 * management and access functions found in thread support libraries.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxThread
{
protected:
// may be used in future if we need cancelable threads...
#ifdef  GM_OS_WIN
    HANDLE cancellor;
#else
    void *cancellor;
#endif

    enum {} reserved;   // cancel mode?
    cx_pthread_t tid;
    size_t stack;
    int priority;

    /**
     * Create a thread object that will have a preset stack size.  If 0
     * is used, then the stack size is os defined/default.
     * @param stack size to use or 0 for default.
     */
    CxThread(size_t stack = 0);

    /**
     * Map thread for get method.  This should be called from start of the
     * run() method of a derived class.
     */
    void map(void);

    /**
     * Check if running.
     */
    virtual bool is_active(void);

public:
    /**
     * Set thread priority without disrupting scheduling if possible.
     * Based on scheduling policy.  It is recommended that the process
     * is set for realtime scheduling, and this method is actually for
     * internal use.
     */
    void setPriority(void);

    /**
     * Yield execution context of the current thread. This is a static
     * and may be used anywhere.
     */
    static void yield(void);

    /**
     * Sleep current thread for a specified time period.
     * @param timeout to sleep for in milliseconds.
     */
    static void sleep(timems_t timeout);

    /**
     * Get mapped thread object.  This returns the mapped base class of the
     * thread object of the current executing context.  You will need to
     * cast to the correct derived class to access derived thread-specific
     * storage.  If the current thread context is not mapped NULL is returned.
     */
    static CxThread *get(void);

    /**
     * Used to initialize threading library.  May be needed for some platforms.
     */
    static void init(void);

    /**
     * Used to specify scheduling policy for threads above priority "0".
     * Normally we apply static realtime policy SCHED_FIFO (default) or
     * SCHED_RR.  However, we could apply SCHED_OTHER, etc.
     */
    static void policy(int polid);

    /**
     * Set concurrency level of process.  This is essentially a portable
     * wrapper for pthread_setconcurrency.
     */
    static void concurrency(int level);

    /**
     * Determine if two thread identifiers refer to the same thread.
     * @param thread1 to test.
     * @param thread2 to test.
     * @return true if both are the same context.
     */
    static bool equal(cx_pthread_t thread1, cx_pthread_t thread2);

    /**
     * Abstract interface for thread context run method.
     */
    virtual void run(void) = 0;

    /**
     * Destroy thread object, thread-specific data, and execution context.
     */
    virtual ~CxThread();

    /**
     * Exit the thread context.  This function should NO LONGER be called
     * directly to exit a running thread.  Instead this method will only be
     * used to modify the behavior of the thread context at thread exit,
     * including detached threads which by default delete themselves.  This
     * documented usage was changed to support Mozilla NSPR exit behavior
     * in case we support NSPR as an alternate thread runtime in the future.
     */
    virtual void exit(void);

    inline operator bool()
        {return is_active();}

    inline bool operator!()
        {return !is_active();}

    inline bool isRunning(void)
        {return is_active();}

};

/**
 * A child thread object that may be joined by parent.  A child thread is
 * a type of thread in which the parent thread (or process main thread) can
 * then wait for the child thread to complete and then delete the child object.
 * The parent thread can wait for the child thread to complete either by
 * calling join, or performing a "delete" of the derived child object.  In
 * either case the parent thread will suspend execution until the child thread
 * exits.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxJoinableThread : public CxThread
{
protected:
#ifdef  GM_OS_WIN
    HANDLE running;
#else
    volatile bool running;
#endif
    volatile bool joining;

    /**
     * Create a joinable thread with a known context stack size.
     * @param size of stack for thread context or 0 for default.
     */
    CxJoinableThread(size_t size = 0);

    /**
     * Delete child thread.  Parent thread suspends until child thread
     * run method completes or child thread calls it's exit method.
     */
    virtual ~CxJoinableThread();

    /**
     * Join thread with parent.  Calling from a child thread to exit is
     * now depreciated behavior and in the future will not be supported.
     * Threads should always return through their run() method.
     */
    void join(void);

    bool is_active(void);

    virtual void run(void) = 0;

public:

    /**
     * start execution of child context.  This must be called after the
     * child object is created (perhaps with "new") and before it can be
     * joined.  This method actually begins the new thread context, which
     * then calls the object's run method.  Optionally raise the priority
     * of the thread when it starts under realtime priority.
     * @param priority of child thread.
     */
    void start(int priority = 0);

    /**
     * start execution of child context as background thread.  This is
     * assumed to be off main thread, with a priority lowered by one.
     */
    inline void background(void)
        {start(-1);};

};

/**
 * A detached thread object that is stand-alone.  This object has no
 * relationship with any other running thread instance will be automatically
 * deleted when the running thread instance exits, either by it's run method
 * exiting, or explicity calling the exit member function.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxDetachedThread : public CxThread
{
protected:
    bool active;

    /**
     * Create a detached thread with a known context stack size.
     * @param size of stack for thread context or 0 for default.
     */
    CxDetachedThread(size_t size = 0);

    /**
     * Destroys object when thread context exits.  Never externally
     * deleted.  Derived object may also have destructor to clean up
     * thread-specific member data.
     */
    ~CxDetachedThread();

    /**
     * Exit context of detached thread.  Thread object will be deleted.
     * This function should NO LONGER be called directly to exit a running
     * thread.  Instead, the thread should only "return" through the run()
     * method to exit.  The documented usage was changed so that exit() can
     * still be used to modify the "delete this" behavior of detached threads
     * while merging thread exit behavior with Mozilla NSPR.
     */
    void exit(void);

    bool is_active(void);

    virtual void run(void) = 0;

public:
    /**
     * start execution of detached context.  This must be called after the
     * object is created (perhaps with "new"). This method actually begins
     * the new thread context, which then calls the object's run method.
     * @param priority to start thread with.
     */
    void start(int priority = 0);

};

#define ENTER_EXCLUSIVE \
    do { static cx_pthread_mutex_t __sync__ = PTHREAD_MUTEX_INITIALIZER; \
        cx_pthread_mutex_lock(&__sync__);

#define LEAVE_EXCLUSIVE \
    cx_pthread_mutex_unlock(&__sync__);} while(0);


#endif // CXTHREAD_H
