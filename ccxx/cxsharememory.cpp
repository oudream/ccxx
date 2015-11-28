#include "cxsharememory.h"

#ifdef GM_OS_WIN
#else
#undef  __EXTENSIONS__
#define __EXTENSIONS__
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <sys/mman.h>
#undef  _POSIX_C_SOURCE
#else
#include <sys/mman.h>
#endif
#include <sys/types.h>
#endif

//have in win32(vc,gcc) linux
#include <fcntl.h>

#if _POSIX_PRIORITY_SCHEDULING > 0
#include <sched.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#define INSERT_OFFSET   16
#endif

#ifndef INSERT_OFFSET
#define INSERT_OFFSET   0
#endif

#ifdef __FreeBSD__
#include <sys/ipc.h>
#include <sys/shm.h>

static void ftok_name(const char *name, char *buf, size_t max)
{
    assert(name != NULL && *name != 0);
    assert(buf != NULL);
    assert(max > 0);

    struct stat ino;
    if(*name == '/')
        ++name;

    if(!stat("/var/run/ipc", &ino) && S_ISDIR(ino.st_mode))
        snprintf(buf, max, "/var/run/ipc/%s", name);
    else
        snprintf(buf, max, "/tmp/.%s.ipc", name);
}

static key_t createipc(const char *name, char mode)
{
    assert(name != NULL && *name != 0);

    char buf[65];
    int fd;

    ftok_name(name, buf, sizeof(buf));
    fd = ::open(buf, O_CREAT | O_EXCL | O_WRONLY, 0664);
    if(fd > -1)
        ::close(fd);
    return ftok(buf, mode);
}

static key_t accessipc(const char *name, char mode)
{
    assert(name != NULL && *name != 0);

    char buf[65];

    ftok_name(name, buf, sizeof(buf));
    return ftok(buf, mode);
}

#endif

static  bool use_mapping = true;

void CxShareMemory::disable(void)
{
    use_mapping = false;
}

CxShareMemory::CxShareMemory(const char *fn, size_t len)
{
    assert(fn != NULL && *fn != 0);
    assert(len > 0);

    erase = true;
    set(idname, sizeof(idname), fn);
    create(fn, size);
}

CxShareMemory::CxShareMemory(const char *fn)
{
    erase = false;
    assert(fn != NULL && *fn != 0);
    create(fn, 0);
}

CxShareMemory::CxShareMemory()
{
    erase = false;
    size = 0;
    used = 0;
    map = NULL;
}

#if defined(GM_OS_WIN)

bool CxShareMemory::create(const char *fn, size_t len)
{
    assert(fn != NULL && *fn != 0);

    int share = FILE_SHARE_READ;
//  int prot = FILE_MAP_READ;
    int mode = GENERIC_READ;

    size = 0;
    used = 0;
    map = NULL;

    if(!use_mapping)
    {
        assert(len > 0);    // cannot use dummy for r/o...
        map = (caddr_t)malloc(len);
        if(!map)fault();

        size = len;
        return true;
    }

    if(*fn == '/')
        ++fn;

    if(len)
    {
        mode |= GENERIC_WRITE;
        share |= FILE_SHARE_WRITE;
        fd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, len, fn);
    }
    else
        fd = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fn);

    if(fd == INVALID_HANDLE_VALUE || fd == NULL)
        return false;

    map = (caddr_t)MapViewOfFile(fd, FILE_MAP_ALL_ACCESS, 0, 0, len);
    if(map)
    {
        size = len;
        VirtualLock(map, size);
        return true;
    }
    else
    {
        fault();
        return false;
    }
}

CxShareMemory::~CxShareMemory()
{
    release();
}

void CxShareMemory::remove(const char *id)
{
    assert(id != NULL && *id != 0);
}

void CxShareMemory::release(void)
{
    if(map) {
        if(use_mapping) {
            VirtualUnlock(map, size);
            UnmapViewOfFile(fd);
            CloseHandle(fd);
            map = NULL;
            fd = INVALID_HANDLE_VALUE;
        }
        else {
            free(map);
            map = NULL;
        }
    }
    if(erase) {
        remove(idname);
        erase = false;
    }
    size = 0;
    used = 0;
}


#elif defined(__FreeBSD__)


bool CxShareMemory::create(const char *fn, size_t len)
{
    assert(fn != NULL && *fn != 0);

    int prot = PROT_READ;
    struct stat ino;
    char fbuf[80];

    size = 0;
    used = 0;

    if(!use_mapping) {
        assert(len > 0);
        map = (caddr_t)malloc(size);
        if(!map)
            fault();
        size = mapsize = len;
        return false;
    }

    if(*fn != '/') {
        snprintf(fbuf, sizeof(fbuf), "/%s", fn);
        fn = fbuf;
    }

    if(len) {
        len += INSERT_OFFSET;
        prot |= PROT_WRITE;
        fd = shm_open(fn, O_RDWR | O_CREAT, 0664);
        if(fd > -1) {
            if(ftruncate(fd, len)) {
                ::close(fd);
                fd = -1;
            }
        }
    }
    else {
        fd = shm_open(fn, O_RDONLY, 0664);
        if(fd > -1) {
            fstat(fd, &ino);
            len = ino.st_size;
        }
    }

    if(fd < 0)
        return false;


    map = (caddr_t)mmap(NULL, len, prot, MAP_SHARED, fd, 0);
    if(!map)
        fault();
    ::close(fd);
    if(map != (caddr_t)MAP_FAILED) {
        size = mapsize = len;
        mlock(map, mapsize);
#if INSERT_OFFSET > 0
        if(prot & PROT_WRITE) {
            size -= INSERT_OFFSET;
            snprintf(map, INSERT_OFFSET, "%ld", size);
        }
        else
            size = atol(map);
        map += INSERT_OFFSET;
#endif
    }

    return true;
}

CxShareMemory::~CxShareMemory()
{
    release();
}

void CxShareMemory::release()
{
    if(size) {
        if(use_mapping) {
            map -= INSERT_OFFSET;
            munlock(map, mapsize);
            munmap(map, mapsize);
        }
        else
            free(map);
        size = 0;
    }
    if(erase) {
        remove(idname);
        erase = false;
    }
    size = 0;
    used = 0;
}

void CxShareMemory::remove(const char *fn)
{
    assert(fn != NULL && *fn != 0);

    char fbuf[80];

    if(!use_mapping)
        return;

    if(*fn != '/') {
        snprintf(fbuf, sizeof(fbuf), "/%s", fn);
        fn = fbuf;
    }

    shm_unlink(fn);
}


#else


void CxShareMemory::remove(const char *name)
{
    assert(name != NULL && *name != 0);

    key_t key;
    fd_t fd;

    if(!use_mapping)
        return;

    key = accessipc(name, 'S');
    if(key) {
        fd = shmget(key, 0, 0);
        if(fd > -1)
            shmctl(fd, IPC_RMID, NULL);
    }
}

void CxShareMemory::create(const char *name, size_t len)
{
    assert(name != NULL && *name != 0);

    struct shmid_ds stat;
    size = 0;
    used = 0;
    key_t key;

    if(!use_mapping) {
        assert(len > 0);
        map = (caddr_t)malloc(len);
        if(!map)
            fault();
        size = len;
        return;
    }

    if(len) {
        key = createipc(name, 'S');
remake:
        fd = shmget(key, len, IPC_CREAT | IPC_EXCL | 0664);
        if(fd == -1 && errno == EEXIST) {
            fd = shmget(key, 0, 0);
            if(fd > -1) {
                shmctl(fd, IPC_RMID, NULL);
                goto remake;
            }
        }
    }
    else {
        key = accessipc(name, 'S');
        fd = shmget(key, 0, 0);
    }

    if(fd > -1) {
        if(len)
            size = len;
        else if(shmctl(fd, IPC_STAT, &stat) == 0)
            size = stat.shm_segsz;
        else
            fd = -1;
    }
    map = (caddr_t)shmat(fd, NULL, 0);
    if(!map)
        fault();
#ifdef  SHM_LOCK
    if(fd > -1)
        shmctl(fd, SHM_LOCK, NULL);
#endif
}

CxShareMemory::~CxShareMemory()
{
    release();
}

void CxShareMemory::release(void)
{
    if(size > 0) {
        if(use_mapping) {
#ifdef  SHM_UNLOCK
            shmctl(fd, SHM_UNLOCK, NULL);
#endif
            shmdt(map);
            fd = -1;
        }
        else
            free(map);
        size = 0;
    }
    if(erase) {
        remove(idname);
        erase = false;
    }
    size = 0;
    used = 0;
}

#endif

void CxShareMemory::fault(void) const
{
    abort();
}

void *CxShareMemory::sbrk(size_t len)
{
    assert(len > 0);
    void *mp = (void *)(map + used);
    if(used + len > size)
        fault();
    used += len;
    return mp;
}

bool CxShareMemory::copy(size_t offset, void *buffer, size_t bufsize) const
{
    if(!map || (offset + bufsize > size)) {
        fault();
        return false;
    }

    const void *member = (const void *)(map + offset);

    do {
        memcpy(buffer, member, bufsize);
    } while(memcmp(buffer, member, bufsize));

    return true;
}

void *CxShareMemory::offset(size_t offset) const
{
    if(offset >= size)
        return NULL;
    return (void *)(map + offset);
}

char *CxShareMemory::set(char *str, size_t size, const char *s)
{
    if(!str)
        return NULL;

    if(size < 2)
        return str;

    if(!s)
        s = "";

    size_t l = strlen(s);

    if(l >= size)
        l = size - 1;

    if(!l) {
        *str = 0;
        return str;
    }

    memmove(str, s, l);
    str[l] = 0;
    return str;
}



