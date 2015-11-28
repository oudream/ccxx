#ifndef CXGLOBAL_H
#define CXGLOBAL_H

/* ---------------------------------------------------------------------------
list:
part001 platform dist
part002 data type : define
part003 lib dll   : define
part004 platform func
--------------------------------------------------------------------------- */

//stl
#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

//clib
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>


//-----------------------------------------------------------------------------------------
//
//part001 platform dist
//
//-----------------------------------------------------------------------------------------
#if defined(MSDOS) || defined(_MSDOS)
#  define GM_OS_MSDOS
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define GM_OS_WIN32
#  define GM_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINCE) || defined(_WIN32_WCE)
#    define GM_OS_WINCE
#  else
#    define GM_OS_WIN32
#  endif
#endif

#if defined(GM_OS_WIN32) || defined(GM_OS_WIN64) || defined(GM_OS_WINCE)
#  define GM_OS_WIN
#endif



//-----------------------------------------------------------------------------------------
//
//part002 data type : define
//
//-----------------------------------------------------------------------------------------

typedef signed char         int8        ;   /* 8 bit signed */
typedef unsigned char       uint8       ;   /* 8 bit unsigned */
typedef short               int16       ;   /* 16 bit signed */
typedef unsigned short      uint16      ;   /* 16 bit unsigned */
typedef int                 int32       ;   /* 32 bit signed */
typedef unsigned int        uint32      ;   /* 32 bit unsigned */

#ifdef _MSC_VER
#  define GM_INT64_C(c) c ## i64    /* signed 64 bit constant */
#  define GM_UINT64_C(c) c ## ui64   /* unsigned 64 bit constant */
typedef __int64 int64;            /* 64 bit signed */
typedef unsigned __int64 uint64;  /* 64 bit unsigned */
#else
#  define GM_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define GM_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long int64;           /* 64 bit signed */
typedef unsigned long long uint64; /* 64 bit unsigned */
#endif

#ifndef QT_BEGIN_INCLUDE_NAMESPACE
typedef unsigned char       uchar       ;   /* 8 bit unsigned */
typedef unsigned short      ushort      ;   /* 16 bit unsigned */
typedef unsigned int        uint        ;   /* 32 bit unsigned */
#endif

typedef unsigned long       ulong       ;   /* 32 bit unsigned */

typedef long long           longlong    ;   /* 64 bit signed */
typedef unsigned long long  ulonglong   ;   /* 64 bit unsigned */

typedef unsigned char       byte        ;   /* 8 bit unsigned */
typedef unsigned short      word        ;   /* 16 bit unsigned */
typedef unsigned int        dword       ;   /* 32 bit unsigned */

typedef    void* 			pvoid;



//1970.1.1 0:0:0:00
//second
typedef time_t sepoch_t;
//millisecond
typedef long long msepoch_t;
//microsecond
typedef long long usepoch_t;
//time size millisecond
typedef unsigned long timems_t;



typedef void (*fn_void_t)(void);
typedef void (*fn_void_int_t)(int);
typedef void (*fn_void_pchar_t)(const char*, int);
typedef void (*fn_void_tlv_t)(int, const void*, int);
typedef void (*fn_void_msg_tlv_t)(int, int, const void*, int, void*, void*);
typedef int (*fn_int_void_t)(void);
typedef int (*fn_int_int_t)(int);
typedef int (*fn_int_pchar_t)(const char*, int);
typedef int (*fn_int_tlv_t)(int, const void*, int);
typedef int (*fn_int_object_tlv_t)(void*, int, int, const void*);



#ifndef    TRUE
#define    TRUE	1
#endif
#ifndef    FALSE
#define	   FALSE 0
#endif
#ifndef    NULL
#define	   NULL 0
#endif



//*complier define
#ifndef __SCHAR_MAX__
#define __SCHAR_MAX__ (127)
#endif
#ifndef __SHRT_MAX__
#define __SHRT_MAX__ (32767)
#endif
#ifndef __INT_MAX__
#define __INT_MAX__ (2147483647)
#endif
#ifndef __LONG_LONG_MAX__
#define __LONG_LONG_MAX__ (9223372036854775807LL)
#endif
#ifndef __FLT_MIN__
#define __FLT_MIN__ (1.17549435e-38F)
#endif
#ifndef __FLT_MAX__
#define __FLT_MAX__ (3.40282347e+38F)
#endif
#ifndef __FLT_MAX__
#define __DBL_MIN__ (2.2250738585072014e-308)
#endif
#ifndef __DBL_MAX__
#define __DBL_MAX__ (1.7976931348623157e+308)
#endif

#define GM_BOOLEAN_MIN (false)
#define GM_BOOLEAN_MAX (true)

#define GM_CHAR_MIN (-__SCHAR_MAX__-1)
#define GM_CHAR_MAX (__SCHAR_MAX__)

#define GM_SHORT_MIN (-__SHRT_MAX__-1)
#define GM_SHORT_MAX (__SHRT_MAX__)

#define GM_INT_MIN (-__INT_MAX__-1)
#define GM_INT_MAX (__INT_MAX__)

#define GM_LONGLONG_MIN (-__LONG_LONG_MAX__-1)
#define GM_LONGLONG_MAX (__LONG_LONG_MAX__)

#define GM_UCHAR_MIN (0)
#define GM_UCHAR_MAX (__SCHAR_MAX__ * 2U + 1)

#define GM_USHORT_MIN (0)
#define GM_USHORT_MAX (__SHRT_MAX__ * 2U + 1)

#define GM_UINT_MIN (-__INT_MAX__-1)
#define GM_UINT_MAX (__INT_MAX__ * 2U + 1)

#define GM_ULONGLONG_MIN (0)
#define GM_ULONGLONG_MAX (__LONG_LONG_MAX__ * 2ULL + 1)

#define GM_FLOAT_MIN (__FLT_MIN__)
#define GM_FLOAT_MAX (__FLT_MAX__)

#define GM_DOUBLE_MIN (__DBL_MIN__)
#define GM_DOUBLE_MAX (__DBL_MAX__)


//UTC  : 1970-01-01 00:00:000
#define GM_MSEPOCH_MIN (0LL)
//UTC  : 9999-12-12 23:59:999
#define GM_MSEPOCH_MAX (253400659239000LL)

//UTC  : 0001-01-01 00:00:000
#define GM_MSEPOCH_MIN_AGO (371023038777600000LL)
//UTC  : 1969-12-12 23:59:999
#define GM_MSEPOCH_MAX_AGO (371085172732839000LL)


#define cb_bool_true ((bool)true)
#define cb_bool_false ((bool)false)
#define ci_int_zero ((int)0)
#define ci_int_minus_one ((int)-1)
#define ci_uint_zero ((unsigned int)0)
#define ci_uint_minus_one ((unsigned int)-1)
#define ci_longlong_zero ((long long)0)
#define ci_longlong_minus_one ((long long)-1)
#define ci_ulonglong_zero ((unsigned long long)0)
#define ci_ulonglong_minus_one ((unsigned long long)-1)
#define cf_float_zero ((float)0)
#define cd_double_zero ((double)0)


//-----------------------------------------------------------------------------------------
//
//part003 lib dll   : define
//
//-----------------------------------------------------------------------------------------
#ifndef GM_DECL_EXPORT
#  if defined(GM_OS_WIN)
#    define GM_DECL_EXPORT extern "C" __declspec(dllexport)
#  else
#    define GM_DECL_EXPORT __attribute__((visibility("default")))
#    define GM_DECL_HIDDEN __attribute__((visibility("hidden")))
#    define GM_CALL_MODE
#  endif
#  ifndef GM_DECL_EXPORT
#    define GM_DECL_EXPORT
#  endif
#endif

#ifndef GM_DECL_IMPORT
#  if defined(GM_OS_WIN)
#    define GM_DECL_IMPORT extern "C" __declspec(dllimport)
#  else
#    define GM_DECL_IMPORT
#  endif
#endif

#ifndef GM_DECL_HIDDEN
#  define GM_DECL_HIDDEN
#endif

#ifndef GM_CALL_MODE
#  if defined(GM_OS_WIN)
#    define GM_CALL_MODE __cdecl
#  else
#    define GM_CALL_MODE
#  endif
#endif

#ifdef GM_BUILD_LIB
#define GM_CORE_API  GM_DECL_EXPORT
#else
#define GM_CORE_API  GM_DECL_IMPORT
#endif



//-----------------------------------------------------------------------------------------
//
//part004 platform func
//
//-----------------------------------------------------------------------------------------
#ifndef __PTH__
#ifndef _THREADSAFE
#define _THREADSAFE 1
#endif
#endif

#ifndef _POSIX_PTHREAD_SEMANTICS
#define _POSIX_PTHREAD_SEMANTICS
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && (__GNU_MINOR__ > 3))
#define __PRINTF(x,y)   __attribute__ ((format (printf, x, y)))
#define __SCANF(x, y) __attribute__ ((format (scanf, x, y)))
#define __MALLOC      __attribute__ ((malloc))
#endif

#ifndef __MALLOC
#define __PRINTF(x, y)
#define __SCANF(x, y)
#define __MALLOC
#endif


//*win32
#ifdef GM_OS_WIN



#if defined(__BORLANDC__) && !defined(__MT__)
#error Please enable multithreading
#endif

#if defined(_MSC_VER) && !defined(_MT)
#error Please enable multithreading (Project -> Settings -> C/C++ -> Code Generation -> Use Runtime Library)
#endif

// Require for compiling with critical sections.
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

// Make sure we're consistent with _WIN32_WINNT
#ifndef WINVER
#define WINVER _WIN32_WINNT
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#   define NOMINMAX
#   undef UNICODE
//#   undef _UWIN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <io.h>

typedef DWORD cx_pthread_t;
typedef CRITICAL_SECTION cx_pthread_mutex_t;
typedef char* caddr_t;
typedef HANDLE fd_t;
typedef SOCKET socket_t;
typedef PROCESS_INFORMATION pid_os_t;

#ifndef _TIMESPEC_DEFINED
#define _TIMESPEC_DEFINED
struct timespec {
  time_t  tv_sec;	/* Seconds */
  long    tv_nsec;	/* Nanoseconds */
};

struct itimerspec {
  struct timespec  it_interval;	/* Timer period */
  struct timespec  it_value;	/* Timer expiration */
};
#endif

extern "C" {

    #define SERVICE_MAIN(id, argc, argv) void WINAPI service_##id(DWORD argc, LPSTR *argv)

    typedef LPSERVICE_MAIN_FUNCTION cpr_service_t;

    inline void cx_pthread_exit(void *p)
        {_endthreadex((DWORD)0);}

    inline cx_pthread_t cx_pthread_self(void)
        {return (cx_pthread_t)GetCurrentThreadId();}

    inline int cx_pthread_mutex_init(cx_pthread_mutex_t *mutex, void *x)
        {InitializeCriticalSection(mutex); return 0;}

    inline void cx_pthread_mutex_destroy(cx_pthread_mutex_t *mutex)
        {DeleteCriticalSection(mutex);}

    inline void cx_pthread_mutex_lock(cx_pthread_mutex_t *mutex)
        {EnterCriticalSection(mutex);}

    inline void cx_pthread_mutex_unlock(cx_pthread_mutex_t *mutex)
        {LeaveCriticalSection(mutex);}

    inline char *cx_strdup(const char *s)
        {return _strdup(s);}

    inline int cx_stricmp(const char *s1, const char *s2)
        {return _stricmp(s1, s2);}

    inline int cx_strnicmp(const char *s1, const char *s2, size_t l)
        {return _strnicmp(s1, s2, l);}
}



#elif defined(__PTH__)



#include <pth.h>

typedef int socket_t;
typedef int fd_t;
typedef pid_t pid_os_t;
#define INVALID_SOCKET -1
#define INVALID_HANDLE_VALUE -1

#define cx_pthread_mutex_t pth_mutex_t
#define pthread_cond_t pth_cond_t
#define cx_pthread_t pth_t

inline int cx_pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
    {return pth_sigmask(how, set, oset);};

inline void cx_pthread_exit(void *p)
    {pth_exit(p);};

inline cx_pthread_t cx_pthread_self(void)
    {return pth_self();}

inline void cx_pthread_kill(cx_pthread_t tid, int sig)
    {pth_raise(tid, sig);};

inline int cx_pthread_mutex_init(cx_pthread_mutex_t *mutex, void *x)
    {return pth_mutex_init(mutex) != 0;};

inline void cx_pthread_mutex_destroy(cx_pthread_mutex_t *mutex)
    {};

inline void cx_pthread_mutex_lock(cx_pthread_mutex_t *mutex)
    {pth_mutex_acquire(mutex, 0, NULL);};

inline void cx_pthread_mutex_unlock(cx_pthread_mutex_t *mutex)
    {pth_mutex_release(mutex);};

inline void cx_pthread_cond_wait(pthread_cond_t *cond, cx_pthread_mutex_t *mutex)
    {pth_cond_await(cond, mutex, NULL);};

inline void cx_pthread_cond_signal(pthread_cond_t *cond)
    {pth_cond_notify(cond, FALSE);};

inline void cx_pthread_cond_broadcast(pthread_cond_t *cond)
    {pth_cond_notify(cond, TRUE);};


#define cx_strdup                   strdup
#define cx_stricmp                  stricmp
#define cx_strnicmp                 strnicmp



#else



#include <pthread.h>

#define cx_pthread_t pthread_t
#define cx_pthread_mutex_t pthread_mutex_t

typedef int socket_t;
typedef int fd_t;
typedef pid_t pid_os_t;
#define INVALID_SOCKET -1
#define INVALID_HANDLE_VALUE -1

#define cx_pthread_sigmask          pthread_sigmask
#define cx_pthread_exit             pthread_exit
#define cx_pthread_self             pthread_self
#define cx_pthread_kill             pthread_kill
#define cx_pthread_mutex_init       pthread_mutex_init
#define cx_pthread_mutex_destroy    pthread_mutex_destroy
#define cx_pthread_mutex_lock       pthread_mutex_lock
#define cx_pthread_mutex_unlock     pthread_mutex_unlock
#define cx_pthread_cond_wait        pthread_cond_wait
#define cx_pthread_cond_signal      pthread_cond_signal
#define cx_pthread_cond_broadcast   pthread_cond_broadcast

#define cx_strdup                   strdup
#define cx_stricmp                  stricmp
#define cx_strnicmp                 strnicmp



#endif

//*error id
#ifdef  GM_OS_WIN
#ifndef ENETDOWN
#define ENETDOWN        ((int)(WSAENETDOWN))
#endif
#ifndef EINPROGRESS
#define EINPROGRESS     ((int)(WSAEINPROGRESS))
#endif
#ifndef ENOPROTOOPT
#define ENOPROTOOPT     ((int)(WSAENOPROTOOPT))
#endif
#ifndef EADDRINUSE
#define EADDRINUSE      ((int)(WSAEADDRINUSE))
#endif
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL   ((int)(WSAEADDRNOTAVAIL))
#endif
#ifndef ENETUNREACH
#define ENETUNREACH     ((int)(WSAENETUNREACH))
#endif
#ifndef EHOSTUNREACH
#define EHOSTUNREACH    ((int)(WSAEHOSTUNREACH))
#endif
#ifndef EHOSTDOWN
#define EHOSTDOWN       ((int)(WSAEHOSTDOWN))
#endif
#ifndef ENETRESET
#define ENETRESET       ((int)(WSAENETRESET))
#endif
#ifndef ECONNABORTED
#define ECONNABORTED    ((int)(WSAECONNABORTED))
#endif
#ifndef ECONNRESET
#define ECONNRESET      ((int)(WSAECONNRESET))
#endif
#ifndef EISCONN
#define EISCONN         ((int)(WSAEISCONN))
#endif
#ifndef ENOTCONN
#define ENOTCONN        ((int)(WSAENOTCONN))
#endif
#ifndef ESHUTDOWN
#define ESHUTDOWN       ((int)(WSAESHUTDOWN))
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT       ((int)(WSAETIMEDOUT))
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED    ((int)(WSAECONNREFUSED))
#endif
#endif

#ifdef _MSC_VER
typedef signed long ssize_t;
#define snprintf _snprintf
#define vsnprintf _vsnprintf
//*warning
#pragma once
#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#pragma warning(disable: 4355)
#pragma warning(disable: 4290)
#pragma warning(disable: 4291)
#pragma warning(disable: 4100) /* -- Disable warning 'unreferenced formal parameter' -- */
#pragma warning(disable: 4661)
#pragma warning(pop)
#else
#ifndef GM_OS_WIN
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif
#include <stdint.h>
#include <unistd.h>
#endif // _MSC_VER



#ifdef GM_DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#ifndef _DEBUG
#define _DEBUG
#endif
#ifndef __DEBUG
#define __DEBUG
#endif
#ifndef DEBUG_
#define DEBUG_
#endif
#ifndef DEBUG__
#define DEBUG__
#endif
#ifndef _DEBUG_
#define _DEBUG_
#endif
#ifndef __DEBUG__
#define __DEBUG__
#endif
#endif

#if defined(_DEBUG) || defined(__DEBUG) || defined(DEBUG) || defined(DEBUG_) || defined(DEBUG__) || defined(_DEBUG_) || defined(__DEBUG__)
#ifndef GM_DEBUG
#define GM_DEBUG
#endif
#endif

#if defined(_debug) || defined(__debug) || defined(debug) || defined(debug_) || defined(debug__) || defined(_debug_) || defined(__debug__)
#ifndef GM_DEBUG
#define GM_DEBUG
#endif
#endif

#ifndef NDEBUG
#ifndef GM_DEBUG
#define GM_DEBUG
#endif
#endif



class CxGlobal
{
public:
    static const timems_t inf; /**< A value to use for infinite time */

    static const msepoch_t invalidMsepoch;

    static const msepoch_t invalidUsepoch;

//目前std::string不支持多线程
//    static const std::string emptyString;
//    static const std::string spaceString;
//    static const std::string  midString;
//    static const std::string  splitString;

    static const double deadZone;
    //空格
    static const char  spaceChar;
    static const char * spaceString;
    //等号
    static const char  equalChar;
    //分号
    static const char  colonChar;
    //逗号
    static const char  commaChar;

    //键值对的中间字符
    static const char  middleChar;
    static const char * middleString;
    //多个字符串的分隔字符（多键值对的分隔字符）
    static const char  splitChar;
    static const char * splitString;

    static const std::map<std::string, std::string> emptyMapString;

    static const char pathCharacter;

    static const char lineCharacter;

    static const char argumentCharacter;    

    static const size_t npos;

    static const char * lineString;

    static const char * warningString;

    static const char * promptString;

    static const char * debugString;

    static const char * trackString;

};
#define cs_line_string (std::string(CxGlobal::lineString))
#define cs_middle_string (std::string(CxGlobal::middleString))
#define cs_split_string (std::string(CxGlobal::splitString))
#define cs_space_string (std::string(CxGlobal::spaceString))


#define GM_CHECK_TO_STRING( p ) ( std::string( #p ) + (p ? " = true" : " = false") )

#define GM_INVALID_RETURE( p ) \
    if (! (p)) { return; }

#define GM_INVALID_RETURE_( p , r ) \
    if (! (p)) { return r; }

#define GM_VALID_RETURE( p ) \
    if (p) { return; }

#define GM_VALID_RETURE_( p , r ) \
    if (p) { return r; }

#define GM_INVALID_BREAK( p ) \
    if (! (p)) { break; }

#define GM_VALID_BREAK( p ) \
    if (p) { break; }

#define GM_INVALID_CONTINUE( p ) \
    if (! (p)) { continue; }

#define GM_VALID_CONTINUE( p ) \
    if (p) { continue; }

#define GM_ASSER( x ) assert(x)

#define GM_ASSERT_IFNOT( x , text ) if (!(x)) assert(text);

#define GM_ASSER_POINTER( x ) assert(x != 0)

#define GM_ASSER_OBJECT( x ) assert(x != NULL)


#define GM_TYPEFULLNAME(BaseClass, sTypeFirstName) \
    public: \
        static std::string TYPESIMPLENAME() { return sTypeFirstName; } \
        std::string typeSimpleName() const { return sTypeFirstName; } \
        static std::string TYPEFIRSTNAME() { return sTypeFirstName; } \
        std::string typeFirstName() const { return sTypeFirstName; } \
        static std::string TYPELASTNAME() { return BaseClass::TYPEFULLNAME(); } \
        std::string typeLastName() const { return BaseClass::TYPEFULLNAME(); } \
        static std::string TYPEFULLNAME() { return (BaseClass::TYPEFULLNAME().empty()) ? sTypeFirstName : sTypeFirstName + "." + BaseClass::TYPEFULLNAME(); } \
        std::string typeFullName() const { return TYPEFULLNAME(); }

#define GM_TYPEFULLNAME_BASE(sTypeBaseName) \
    public: \
        static std::string TYPEFULLNAME() { return sTypeBaseName; } \
        virtual std::string typeFullName() const = 0 ; \
        virtual std::string typeSimpleName() const = 0 ; \
        virtual std::string typeFirstName() const = 0 ; \
        virtual std::string typeLastName() const = 0 ;



#define GM_SINGLETON_DECLARATION(ClassName) \
    public: \
    static ClassName* singleton() { \
        if (! s_instance) \
            s_instance = new ClassName(); \
        return s_instance; \
    } \
    private: \
    static ClassName* s_instance;

#define GM_SINGLETON_REALIZATION(ClassName) \
    ClassName* ClassName::s_instance = NULL;



#define GM_DELETEANDNULL_OBJECT(oObject) \
    if (oObject) { delete oObject; oObject = NULL; }

#define GM_DELETEANDNULL_ARRAY(oObject) \
    if (oObject) { delete[] oObject; oObject = NULL; }


#endif // CXGLOBAL_H
