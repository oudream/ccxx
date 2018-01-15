#include "cxprocmutex.h"


using namespace std;


#ifdef GM_OS_WIN32


CxProcMutex::CxProcMutex(const std::string& name):
    _name(name)
{
    _mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _name.c_str());
    if (_mutex == NULL)
        _mutex = CreateMutex(NULL, FALSE, _name.c_str());
    if (_mutex == NULL)
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot create named mutex!", _name.c_str()));
}


CxProcMutex::~CxProcMutex()
{
    CloseHandle(_mutex);
}


void CxProcMutex::lock()
{
    switch (WaitForSingleObject(_mutex, INFINITE))
    {
    case WAIT_OBJECT_0:
        return;
    case WAIT_ABANDONED:
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot lock named mutex (abadoned)!", _name.c_str()));
    default:
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot lock named mutex!", _name.c_str()));
    }
}


bool CxProcMutex::tryLock()
{
    switch (WaitForSingleObject(_mutex, 0))
    {
    case WAIT_OBJECT_0:
        return true;
    case WAIT_TIMEOUT:
        return false;
    case WAIT_ABANDONED:
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot lock named mutex (abadoned)!", _name.c_str()));
    default:
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot lock named mutex!", _name.c_str()));
    }
}


void CxProcMutex::unlock()
{
    ReleaseMutex(_mutex);
}








#else //unix linux bsd


#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
#include <semaphore.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#endif

#if defined(hpux) || defined(_hpux) || defined(__hpux)
    union semun
    {
        int              val;
        struct semid_ds* buf;
        ushort*          array;
    };
#else
    union semun
    {
        int                 val;
        struct semid_ds*    buf;
        unsigned short int* array;
        struct seminfo*     __buf;
    };
#endif

CxProcMutex::CxProcMutex(const std::string& name):
    _name(name)
{
    std::string fileName = getFileName();
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
    _sem = sem_open(fileName.c_str(), O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 1);
    if ((long) _sem == (long) SEM_FAILED)
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot create named mutex %s (sem_open() failed, errno=%d)!", _name.c_str(), fileName.c_str(), errno));
#else
    int fd = open(fileName.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd != -1)
        close(fd);
    else
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot create named mutex %s (lockfile)!", _name.c_str(), fileName.c_str()));
    key_t key = ftok(fileName.c_str(), 0);
    if (key == -1)
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot create named mutex %s (ftok() failed, errno=%d)!", _name.c_str(), fileName.c_str(), errno));
    _semid = semget(key, 1, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | IPC_CREAT | IPC_EXCL);
    if (_semid >= 0)
    {
        union semun arg;
        arg.val = 1;
        semctl(_semid, 0, SETVAL, arg);
    }
    else if (errno == EEXIST)
    {
        _semid = semget(key, 1, 0);
    }
    else throw CxException(CxString::format("CxProcMutex Exception. %s . cannot create named mutex %s (semget() failed, errno=%d)!", _name.c_str(), fileName.c_str(), errno));
#endif // defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
}


CxProcMutex::~CxProcMutex()
{
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
    sem_close(_sem);
#endif
}


void CxProcMutex::lock()
{
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
    int err;
    do
    {
        err = sem_wait(_sem);
    }
    while (err && errno == EINTR);
    if (err) throw CxException(CxString::format("CxProcMutex Exception. %s . cannot lock named mutex!", _name.c_str()));
#else
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op  = -1;
    op.sem_flg = SEM_UNDO;
    int err;
    do
    {
        err = semop(_semid, &op, 1);
    }
    while (err && errno == EINTR);
    if (err) throw CxException(CxString::format("CxProcMutex Exception. %s . cannot lock named mutex!", _name.c_str()));
#endif
}


bool CxProcMutex::tryLock()
{
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
    return sem_trywait(_sem) == 0;
#else
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op  = -1;
    op.sem_flg = SEM_UNDO | IPC_NOWAIT;
    return semop(_semid, &op, 1) == 0;
#endif
}


void CxProcMutex::unlock()
{
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
    if (sem_post(_sem) != 0)
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot unlock named mutex!", _name.c_str()));
#else
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op  = 1;
    op.sem_flg = SEM_UNDO;
    if (semop(_semid, &op, 1) != 0)
        throw CxException(CxString::format("CxProcMutex Exception. %s . cannot unlock named mutex!", _name.c_str()));
#endif
}


std::string CxProcMutex::getFileName()
{
#if defined(sun) || defined(__APPLE__) || defined(__QNX__)
    std::string fn = "/";
#else
    std::string fn = "/tmp/";
#endif
    fn.append(_name);
    fn.append(".mutex");
    return fn;
}




#endif

