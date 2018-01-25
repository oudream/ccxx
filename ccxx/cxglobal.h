#ifndef CXGLOBAL_H
#define CXGLOBAL_H

/* ---------------------------------------------------------------------------
list:
part001 platform dist
part002 data type           : define data type
part003 lib dll             : define lib dll
part004 platform func
part005 endian functions
part006 CxGlobal            : string resource
part007 CxValueType
part008 CxException
part009 CxFactory
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
typedef signed char int8;   /* 8 bit signed */
typedef unsigned char uint8;   /* 8 bit unsigned */
typedef short int16;   /* 16 bit signed */
typedef unsigned short uint16;   /* 16 bit unsigned */
typedef int int32;   /* 32 bit signed */
typedef unsigned int uint32;   /* 32 bit unsigned */

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
typedef unsigned char uchar;   /* 8 bit unsigned */
typedef unsigned short ushort;   /* 16 bit unsigned */
typedef unsigned int uint;   /* 32 bit unsigned */
#endif
typedef unsigned long ulong;   /* 32 bit unsigned */
typedef long long longlong;   /* 64 bit signed */
typedef unsigned long long ulonglong;   /* 64 bit unsigned */
typedef unsigned char byte;   /* 8 bit unsigned */
typedef unsigned short word;   /* 16 bit unsigned */
typedef unsigned int dword;   /* 32 bit unsigned */
typedef void *pvoid;

//1970.1.1 0:0:0:00
//second do not use time_t , _USE_32BIT_TIME_T
typedef int sepoch_t;
typedef long long msepoch_t;//millisecond
typedef long long usepoch_t;//microsecond
typedef long long nsepoch_t;//nanosecond
typedef unsigned long timems_t;//time size millisecond
typedef long long mslong_t;//time size millisecond
typedef long long secondlong_t;//time size second

typedef void (*fn_void_t)(void);
typedef void (*fn_void_int_t)(int);
typedef void (*fn_void_pchar_t)(const char *, int);
typedef void (*fn_void_tlv_t)(int, const void *, int);
typedef void (*fn_void_msg_tlv_t)(int, int, const void *, int, void *, void *);
typedef int (*fn_int_void_t)(void);
typedef int (*fn_int_int_t)(int);
typedef int (*fn_int_pchar_t)(const char *, int);
typedef int (*fn_int_tlv_t)(int, const void *, int);
typedef int (*fn_int_object_tlv_t)(void *, int, int, const void *);
typedef void (*fn_void_queue_msg_tlv_t)(fn_void_msg_tlv_t, int, int, const void *, int, void *, void *, bool); //bool bHandleOnce  = false


#ifndef    TRUE
#define    TRUE    1
#endif
#ifndef    FALSE
#define    FALSE 0
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
# if defined (__SUNPRO_C) && (__SUNPRO_C >= 0x550)
#  define GM_DECL_EXPORT __global
# elif defined __GNUC__
#  define GM_DECL_EXPORT __attribute__((visibility("default")))
# elif defined(_MSC_VER)
#  define GM_DECL_EXPORT __declspec(dllexport)
# else
#  define GM_DECL_EXPORT /* unknown compiler */
# endif
#endif

#ifndef GM_DECL_IMPORT
# if defined(_MSC_VER)
#  define GM_DECL_IMPORT __declspec(dllimport)
# else
#  define GM_DECL_IMPORT
# endif
#endif

#ifndef GM_CALL_MODE
#  if defined(GM_OS_WIN)
#    define GM_CALL_MODE __cdecl
#  else
#    define GM_CALL_MODE
#  endif
#endif

#if defined(GM_BUILD_CCXX_CORE_LIB)
#define GM_CCXX_CORE_API  GM_DECL_EXPORT
#elif defined(GM_BUILD_CCXX_CORE_CODE)
#define GM_CCXX_CORE_API
#else
#define GM_CCXX_CORE_API  GM_DECL_IMPORT
// GM_BUILD_CCXX_CORE_INCLUDE
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
#define __PRINTF(x, y)   __attribute__ ((format (printf, x, y)))
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
typedef char *caddr_t;
typedef HANDLE fd_t;
typedef SOCKET socket_t;
typedef PROCESS_INFORMATION pid_os_t;

#if !defined(_MSC_VER) || (_MSC_VER < 1900)
#ifndef _TIMESPEC_DEFINED
#define _TIMESPEC_DEFINED
// #define _CRT_NO_TIME_T
typedef struct timespec {
    time_t  tv_sec;	/* Seconds */
    long    tv_nsec;	/* Nanoseconds */
} timespec;

struct itimerspec {
    struct timespec  it_interval;	/* Timer period */
    struct timespec  it_value;	/* Timer expiration */
};
#endif
#endif

extern "C" {

#define SERVICE_MAIN(id, argc, argv) void WINAPI service_##id(DWORD argc, LPSTR *argv)

#if (_WIN32_WINNT > 0x0500)
#define cpr_service_t LPSERVICE_MAIN_FUNCTION
#endif

inline void cx_pthread_exit(void *p)
{ _endthreadex((DWORD) 0); }

inline cx_pthread_t cx_pthread_self(void)
{ return (cx_pthread_t) GetCurrentThreadId(); }

inline int cx_pthread_mutex_init(cx_pthread_mutex_t *mutex, void *x)
{
    InitializeCriticalSection(mutex);
    return 0;
}

inline void cx_pthread_mutex_destroy(cx_pthread_mutex_t *mutex)
{ DeleteCriticalSection(mutex); }

inline void cx_pthread_mutex_lock(cx_pthread_mutex_t *mutex)
{ EnterCriticalSection(mutex); }

inline void cx_pthread_mutex_unlock(cx_pthread_mutex_t *mutex)
{ LeaveCriticalSection(mutex); }

inline char *cx_strdup(const char *s)
{ return _strdup(s); }

inline int cx_stricmp(const char *s1, const char *s2)
{ return _stricmp(s1, s2); }

inline int cx_strnicmp(const char *s1, const char *s2, size_t l)
{ return _strnicmp(s1, s2, l); }
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

#ifdef __APPLE__
#include <sys/_endian.h>
#else
#include <byteswap.h>
#endif

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
#if _MSC_VER < 1900
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif
//*warning
#pragma once
#pragma warning(push)
#pragma warning(disable : 4005)
#pragma warning(disable : 4100) /* -- Disable warning 'unreferenced formal parameter' -- */
#pragma warning(disable : 4251)
#pragma warning(disable : 4355)
#pragma warning(disable : 4290)
#pragma warning(disable : 4291)
#pragma warning(disable : 4661)
#pragma warning(disable : 4819)
#pragma warning(disable : 4996)
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

//-----------------------------------------------------------------------------------------
//
//part005 endian functions
//
//-----------------------------------------------------------------------------------------
inline void gfn_bswap_helper(const uchar *src, uchar *dest, int size)
{ for (int i = 0; i < size; ++i) dest[i] = src[size - 1 - i]; }

/*
 * gfn_bswap(const T src, const uchar *dest);
 * Changes the byte order of \a src from big endian to little endian or vice versa
 * and stores the result in \a dest.
 * There is no alignment requirements for \a dest.
*/
template<typename T>
inline void gfn_bswap(const T src, uchar *dest)
{ gfn_bswap_helper(reinterpret_cast<const uchar *>(&src), dest, sizeof(T)); }

// Used to implement a type-safe and alignment-safe copy operation
// If you want to avoid the memcopy, you must write specializations for this function
template<typename T>
inline void gfn_toUnaligned(const T src, uchar *dest)
{ memcpy(dest, &src, sizeof(T)); }

/* T gfn_fromLittleEndian(const uchar *src)
 * This function will read a little-endian encoded value from \a src
 * and return the value in host-endian encoding.
 * There is no requirement that \a src must be aligned.
*/
#if defined GM_CC_SUN
inline uint64 gfn_fromLittleEndian(const uchar *src, uint64 *dest)
{
    return 0
        | src[0]
        | src[1] * GM_UINT64_C(0x0000000000000100)
        | src[2] * GM_UINT64_C(0x0000000000010000)
        | src[3] * GM_UINT64_C(0x0000000001000000)
        | src[4] * GM_UINT64_C(0x0000000100000000)
        | src[5] * GM_UINT64_C(0x0000010000000000)
        | src[6] * GM_UINT64_C(0x0001000000000000)
        | src[7] * GM_UINT64_C(0x0100000000000000);
}

inline uint32 gfn_fromLittleEndian(const uchar *src, uint32 *dest)
{ return 0 | src[0] | src[1] * uint32(0x00000100) | src[2] * uint32(0x00010000) | src[3] * uint32(0x01000000); }

inline uint16 gfn_fromLittleEndian(const uchar *src, uint16 *dest)
{ return 0 | src[0] | src[1] * 0x0100; }

inline int64 gfn_fromLittleEndian(const uchar *src, int64 * dest)
{ return static_cast<int64>(gfn_fromLittleEndian(src, reinterpret_cast<uint64*>(0))); }
inline int32 gfn_fromLittleEndian(const uchar *src, int32 * dest)
{ return static_cast<int32>(gfn_fromLittleEndian(src, reinterpret_cast<uint32*>(0))); }
inline int16 gfn_fromLittleEndian(const uchar *src, int16 * dest)
{ return static_cast<int16>(gfn_fromLittleEndian(src, reinterpret_cast<uint16*>(0))); }
template <class T> inline T gfn_fromLittleEndian(const uchar *src)
{ return gfn_fromLittleEndian(src, reinterpret_cast<T*>(0)); }

#else
template<typename T>
inline T gfn_fromLittleEndian(const uchar *src);
template<>
inline uint64 gfn_fromLittleEndian<uint64>(const uchar *src)
{
    return 0
           | src[0]
           | src[1] * GM_UINT64_C(0x0000000000000100)
           | src[2] * GM_UINT64_C(0x0000000000010000)
           | src[3] * GM_UINT64_C(0x0000000001000000)
           | src[4] * GM_UINT64_C(0x0000000100000000)
           | src[5] * GM_UINT64_C(0x0000010000000000)
           | src[6] * GM_UINT64_C(0x0001000000000000)
           | src[7] * GM_UINT64_C(0x0100000000000000);
}

template<>
inline uint32 gfn_fromLittleEndian<uint32>(const uchar *src)
{ return 0 | src[0] | src[1] * uint32(0x00000100) | src[2] * uint32(0x00010000) | src[3] * uint32(0x01000000); }

template<>
inline uint16 gfn_fromLittleEndian<uint16>(const uchar *src)
{ return uint16(0 | src[0] | src[1] * 0x0100); }

// signed specializations
template<>
inline int64 gfn_fromLittleEndian<int64>(const uchar *src)
{ return static_cast<int64>(gfn_fromLittleEndian<uint64>(src)); }

template<>
inline int32 gfn_fromLittleEndian<int32>(const uchar *src)
{ return static_cast<int32>(gfn_fromLittleEndian<uint32>(src)); }

template<>
inline int16 gfn_fromLittleEndian<int16>(const uchar *src)
{ return static_cast<int16>(gfn_fromLittleEndian<uint16>(src)); }
#endif

/* This function will read a big-endian (also known as network order) encoded value from \a src
 * and return the value in host-endian encoding.
 * There is no requirement that \a src must be aligned.
*/
#if defined GM_CC_SUN
inline uint64 gfn_fromBigEndian(const uchar *src, uint64 *dest)
{
    return 0
        | src[7]
        | src[6] * GM_UINT64_C(0x0000000000000100)
        | src[5] * GM_UINT64_C(0x0000000000010000)
        | src[4] * GM_UINT64_C(0x0000000001000000)
        | src[3] * GM_UINT64_C(0x0000000100000000)
        | src[2] * GM_UINT64_C(0x0000010000000000)
        | src[1] * GM_UINT64_C(0x0001000000000000)
        | src[0] * GM_UINT64_C(0x0100000000000000);
}

inline uint32 gfn_fromBigEndian(const uchar *src, uint32 * dest)
{
    return 0 | src[3] | src[2] * uint32(0x00000100) | src[1] * uint32(0x00010000)
              | src[0] * uint32(0x01000000);
}

inline uint16 gfn_fromBigEndian(const uchar *src, uint16 * des)
{ return 0 | src[1] | src[0] * 0x0100; }

inline int64 gfn_fromBigEndian(const uchar *src, int64 * dest)
{ return static_cast<int64>(gfn_fromBigEndian(src, reinterpret_cast<uint64*>(0))); }
inline int32 gfn_fromBigEndian(const uchar *src, int32 * dest)
{ return static_cast<int32>(gfn_fromBigEndian(src, reinterpret_cast<uint32*>(0))); }
inline int16 gfn_fromBigEndian(const uchar *src, int16 * dest)
{ return static_cast<int16>(gfn_fromBigEndian(src, reinterpret_cast<uint16*>(0))); }
template <class T> inline T gfn_fromBigEndian(const uchar *src)
{ return gfn_fromBigEndian(src, reinterpret_cast<T*>(0)); }

#else
template<class T>
inline T gfn_fromBigEndian(const uchar *src);
template<>
inline uint64 gfn_fromBigEndian<uint64>(const uchar *src)
{
    return 0
           | src[7]
           | src[6] * GM_UINT64_C(0x0000000000000100)
           | src[5] * GM_UINT64_C(0x0000000000010000)
           | src[4] * GM_UINT64_C(0x0000000001000000)
           | src[3] * GM_UINT64_C(0x0000000100000000)
           | src[2] * GM_UINT64_C(0x0000010000000000)
           | src[1] * GM_UINT64_C(0x0001000000000000)
           | src[0] * GM_UINT64_C(0x0100000000000000);
}

template<>
inline uint32 gfn_fromBigEndian<uint32>(const uchar *src)
{ return 0 | src[3] | src[2] * uint32(0x00000100) | src[1] * uint32(0x00010000) | src[0] * uint32(0x01000000); }

template<>
inline uint16 gfn_fromBigEndian<uint16>(const uchar *src)
{ return uint16(0 | src[1] | src[0] * uint16(0x0100)); }

// signed specializations
template<>
inline int64 gfn_fromBigEndian<int64>(const uchar *src)
{ return static_cast<int64>(gfn_fromBigEndian<uint64>(src)); }

template<>
inline int32 gfn_fromBigEndian<int32>(const uchar *src)
{ return static_cast<int32>(gfn_fromBigEndian<uint32>(src)); }

template<>
inline int16 gfn_fromBigEndian<int16>(const uchar *src)
{ return static_cast<int16>(gfn_fromBigEndian<uint16>(src)); }
#endif
/*
 * T gfn_bswap(T source).
 * Changes the byte order of a value from big endian to little endian or vice versa.
 * This function can be used if you are not concerned about alignment issues,
 * and it is therefore a bit more convenient and in most cases more efficient.
*/
template<typename T>
T gfn_bswap(T source);

#ifdef __GLIBC__
template <> inline uint64 gfn_bswap<uint64>(uint64 source)
{ return bswap_64(source); }
template <> inline uint32 gfn_bswap<uint32>(uint32 source)
{ return bswap_32(source); }
template <> inline uint16 gfn_bswap<uint16>(uint16 source)
{ return bswap_16(source); }
#else
template<>
inline uint64 gfn_bswap<uint64>(uint64 source)
{
    return 0
           | ((source & GM_UINT64_C(0x00000000000000ff)) << 56)
           | ((source & GM_UINT64_C(0x000000000000ff00)) << 40)
           | ((source & GM_UINT64_C(0x0000000000ff0000)) << 24)
           | ((source & GM_UINT64_C(0x00000000ff000000)) << 8)
           | ((source & GM_UINT64_C(0x000000ff00000000)) >> 8)
           | ((source & GM_UINT64_C(0x0000ff0000000000)) >> 24)
           | ((source & GM_UINT64_C(0x00ff000000000000)) >> 40)
           | ((source & GM_UINT64_C(0xff00000000000000)) >> 56);
}

template<>
inline uint32 gfn_bswap<uint32>(uint32 source)
{
    return 0 | ((source & 0x000000ff) << 24) | ((source & 0x0000ff00) << 8) | ((source & 0x00ff0000) >> 8)
           | ((source & 0xff000000) >> 24);
}

template<>
inline uint16 gfn_bswap<uint16>(uint16 source)
{ return uint16(0 | ((source & 0x00ff) << 8) | ((source & 0xff00) >> 8)); }
#endif // __GLIBC__

// signed specializations
template<>
inline int64 gfn_bswap<int64>(int64 source)
{ return gfn_bswap<uint64>(uint64(source)); }

template<>
inline int32 gfn_bswap<int32>(int32 source)
{ return gfn_bswap<uint32>(uint32(source)); }

template<>
inline int16 gfn_bswap<int16>(int16 source)
{ return gfn_bswap<uint16>(uint16(source)); }

#if GM_BYTE_ORDER == GM_BIG_ENDIAN

template<typename T>
inline T gfn_toBigEndian(T source)
{ return source; }
template<typename T>
inline T gfn_fromBigEndian(T source)
{ return source; }
template<typename T>
inline T gfn_toLittleEndian(T source)
{ return gfn_bswap<T>(source); }
template<typename T>
inline T gfn_fromLittleEndian(T source)
{ return gfn_bswap<T>(source); }
template<typename T>
inline void gfn_toBigEndian(T src, uchar *dest)
{ gfn_toUnaligned<T>(src, dest); }
template<typename T>
inline void gfn_toLittleEndian(T src, uchar *dest)
{ gfn_bswap<T>(src, dest); }
#else // GM_LITTLE_ENDIAN

template <typename T> inline T gfn_toBigEndian(T source)
{ return gfn_bswap<T>(source); }
template <typename T> inline T gfn_fromBigEndian(T source)
{ return gfn_bswap<T>(source); }
template <typename T> inline T gfn_toLittleEndian(T source)
{ return source; }
template <typename T> inline T gfn_fromLittleEndian(T source)
{ return source; }
template <typename T> inline void gfn_toBigEndian(T src, uchar *dest)
{ gfn_bswap<T>(src, dest); }
template <typename T> inline void gfn_toLittleEndian(T src, uchar *dest)
{ gfn_toUnaligned<T>(src, dest); }

#endif // GM_BYTE_ORDER == GM_BIG_ENDIAN

template<>
inline uint8 gfn_bswap<uint8>(uint8 source)
{ return source; }

//-----------------------------------------------------------------------------------------
//
//part006 CxGlobal
//
//-----------------------------------------------------------------------------------------
class GM_CCXX_CORE_API CxGlobal {
public:
    static const timems_t inf; /**< A value to use for infinite time */
    static const msepoch_t invalidMsepoch;
    static const msepoch_t invalidUsepoch;
    static const double deadZone;
    static const char spaceChar;//空格
    static const char *spaceString;
    static const char equalChar;//等号
    static const char colonChar;//分号
    static const char commaChar;//逗号
    static const char middleChar;//键值对的中间字符
    static const char *middleString;
    static const char splitChar;//多个字符串的分隔字符（多键值对的分隔字符）
    static const char *splitString;
    static const std::map<std::string, std::string> emptyMapString;
    static const char pathCharacter;
    static const char lineCharacter;
    static const char argumentCharacter;
    static const size_t npos;
    static const char *lineString;
    static const char *warningString;
    static const char *promptString;
    static const char *debugString;
    static const char *trackString;
    static fn_void_msg_tlv_t assertCallBack;

};
#define cs_line_string (std::string(CxGlobal::lineString))
#define cs_middle_string (std::string(CxGlobal::middleString))
#define cs_split_string (std::string(CxGlobal::splitString))
#define cs_space_string (std::string(CxGlobal::spaceString))

#define GM_CHECK_TO_STRING(p) ( std::string( #p ) + (p ? " = true" : " = false") )
#define GM_VALID_RETURE(p)          if (p) { return; }
#define GM_VALID_RETURE_(p, r)      if (p) { return r; }
#define GM_VALID_BREAK(p)           if (p) { break; }
#define GM_VALID_CONTINUE(p)        if (p) { continue; }
#define GM_INVALID_RETURE(p)        if (! (p)) { return; }
#define GM_INVALID_RETURE_(p, r)    if (! (p)) { return r; }
#define GM_INVALID_BREAK(p)         if (! (p)) { break; }
#define GM_INVALID_CONTINUE(p)      if (! (p)) { continue; }

#ifdef GM_DEBUG
#undef assert
#define assert(_Expression) \
    (void) \
    ((!!(_Expression)) || \
     ( \
      ((!(CxGlobal::assertCallBack)) || (CxGlobal::assertCallBack(0,0,#_Expression,__LINE__,(void *)__FILE__,0),1)) \
     ) \
    )
#endif

#define GM_ASSER(x)                 assert(x)
#define GM_ASSERT_IFNOT(x, text)    if (!(x)) assert(text);
#define GM_ASSER_POINTER(x)         assert(x != 0)
#define GM_ASSER_OBJECT(x)          assert(x != NULL)
#define GM_ASSERT_EQ(a, b)          assert((a) == (b))
#define GM_ASSERT_GE(a, b)          assert((a) >= (b))
#define GM_ASSERT_GT(a, b)          assert((a) > (b))
#define GM_ASSERT_LE(a, b)          assert((a) <= (b))
#define GM_ASSERT_LT(a, b)          assert((a) < (b))
#define GM_ASSERT_NE(a, b)          assert((a) != (b))


//-----------------------------------------------------------------------------------------
//
//part007 CxValueType
//
//-----------------------------------------------------------------------------------------
class CxValueType {
public:
    enum ValueTypeEnum {
        ValueType_Integer, ValueType_Enum, ValueType_Boolean,
        ValueType_Double, ValueType_DateTime, ValueType_String
    };

    template<typename T>
    static ValueTypeEnum id();

    template<typename T>
    static std::string simpleName();

    template<typename T>
    static T defaultValue();

    //std::numeric_limits<T>::min()
    template<typename T>
    static T minValue();

    //std::numeric_limits<T>::max()
    template<typename T>
    static T maxValue();

    template<typename T>
    static T originalValue();

    template<typename T1, typename T2>
    static T2 valueTo(const T1 &t1, const T2 &defaultT2);

};

template<typename T>
inline CxValueType::ValueTypeEnum CxValueType::id()
{ return ValueType_Enum; }
template<>
inline CxValueType::ValueTypeEnum CxValueType::id<int>()
{ return ValueType_Integer; }
template<>
inline CxValueType::ValueTypeEnum CxValueType::id<bool>()
{ return ValueType_Boolean; }
template<>
inline CxValueType::ValueTypeEnum CxValueType::id<double>()
{ return ValueType_Double; }
template<>
inline CxValueType::ValueTypeEnum CxValueType::id<std::string>()
{ return ValueType_String; }
template<>
inline CxValueType::ValueTypeEnum CxValueType::id<msepoch_t>()
{ return ValueType_DateTime; }

template<typename T>
inline std::string CxValueType::simpleName()
{ return std::string(); }
template<>
inline std::string CxValueType::simpleName<int>()
{ return "Integer"; }
template<>
inline std::string CxValueType::simpleName<bool>()
{ return "Boolean"; }
template<>
inline std::string CxValueType::simpleName<double>()
{ return "Double"; }
template<>
inline std::string CxValueType::simpleName<std::string>()
{ return "String"; }
template<>
inline std::string CxValueType::simpleName<msepoch_t>()
{ return "DateTime"; }
//                                                                             "Enum";

template<typename T>
inline T CxValueType::defaultValue()
{
    T t;
    return t;
}
template<>
inline int CxValueType::defaultValue<int>()
{ return 0; }
template<>
inline bool CxValueType::defaultValue<bool>()
{ return false; }
template<>
inline uchar CxValueType::defaultValue<uchar>()
{ return 0x00; }
template<>
inline double CxValueType::defaultValue<double>()
{ return 0; }
template<>
inline msepoch_t CxValueType::defaultValue<msepoch_t>()
{ return 0; }
template<>
inline std::string CxValueType::defaultValue<std::string>()
{ return std::string(); }
template<>
inline void *CxValueType::defaultValue<void *>()
{ return NULL; }

template<typename T>
inline T CxValueType::minValue()
{
    T t;
    return t;
}
template<>
inline bool CxValueType::minValue<bool>()
{ return false; }
template<>
inline char CxValueType::minValue<char>()
{ return (std::numeric_limits<char>::min)(); }
template<>
inline uchar CxValueType::minValue<uchar>()
{ return (std::numeric_limits<unsigned char>::min)(); }
template<>
inline short CxValueType::minValue<short>()
{ return (std::numeric_limits<short>::min)(); }
template<>
inline ushort CxValueType::minValue<ushort>()
{ return (std::numeric_limits<unsigned short>::min)(); }
template<>
inline int CxValueType::minValue<int>()
{ return (std::numeric_limits<int>::min)(); }
template<>
inline uint CxValueType::minValue<uint>()
{ return (std::numeric_limits<unsigned int>::min)(); }
template<>
inline float CxValueType::minValue<float>()
{ return (std::numeric_limits<float>::min)(); }
template<>
inline double CxValueType::minValue<double>()
{ return (std::numeric_limits<double>::min)(); }
template<>
inline msepoch_t CxValueType::minValue<msepoch_t>()
{ return GM_MSEPOCH_MIN; }
template<>
inline std::string CxValueType::minValue<std::string>()
{ return std::string(); }

template<typename T>
inline T CxValueType::maxValue()
{
    T t;
    return t;
}
template<>
inline bool CxValueType::maxValue<bool>()
{ return false; }
template<>
inline char CxValueType::maxValue<char>()
{ return (std::numeric_limits<char>::max)(); }
template<>
inline uchar CxValueType::maxValue<uchar>()
{ return (std::numeric_limits<unsigned char>::max)(); }
template<>
inline short CxValueType::maxValue<short>()
{ return (std::numeric_limits<short>::max)(); }
template<>
inline ushort CxValueType::maxValue<ushort>()
{ return (std::numeric_limits<unsigned short>::max)(); }
template<>
inline int CxValueType::maxValue<int>()
{ return (std::numeric_limits<int>::max)(); }
template<>
inline uint CxValueType::maxValue<uint>()
{ return (std::numeric_limits<unsigned int>::max)(); }
template<>
inline float CxValueType::maxValue<float>()
{ return (std::numeric_limits<float>::max)(); }
template<>
inline double CxValueType::maxValue<double>()
{ return (std::numeric_limits<double>::max)(); }
template<>
inline msepoch_t CxValueType::maxValue<msepoch_t>()
{ return GM_MSEPOCH_MAX; }
template<>
inline std::string CxValueType::maxValue<std::string>()
{ return std::string(); }

template<typename T>
inline T CxValueType::originalValue()
{
    T t;
    return t;
}
template<>
inline int CxValueType::originalValue<int>()
{ return -1; }
template<>
inline bool CxValueType::originalValue<bool>()
{ return false; }
template<>
inline uchar CxValueType::originalValue<uchar>()
{ return 0x00; }
template<>
inline double CxValueType::originalValue<double>()
{ return -1; }
template<>
inline msepoch_t CxValueType::originalValue<msepoch_t>()
{ return 0; }
template<>
inline std::string CxValueType::originalValue<std::string>()
{ return std::string(); }

//-----------------------------------------------------------------------------------------
//
//part008 CxException
//
//-----------------------------------------------------------------------------------------
class CxException : public std::exception {
public:
    CxException(const std::string &message = "",
                long lineNumber = -1,
                const std::string &fileName = "<unknown>");
    CxException(const std::string &message,
                long lineNumber,
                long data1lineNumber,
                const std::string &fileName);
    CxException(const std::string &message,
                long lineNumber,
                long data1lineNumber,
                long data2lineNumber,
                const std::string &fileName);
    CxException(const CxException &other);
    virtual ~CxException() throw();
    CxException &operator=(const CxException &other);
    const char *what() const throw();
    long lineNumber() const;
    long data1LineNumber() const;
    long data2LineNumber() const;
    const std::string &fileName() const;

private:
    std::string _message;
    long _lineNumber;
    long _data1lineNumber;
    long _data2lineNumber;
    std::string _fileName;

};

inline CxException::CxException(const CxException &other) : exception(other)
{
    _message = other._message;
    _lineNumber = other._lineNumber;
    _data1lineNumber = other._data1lineNumber;
    _data2lineNumber = other._data2lineNumber;
    _fileName = other._fileName;
}
inline CxException::CxException(const std::string &message, long lineNumber, const std::string &fileName)
    : _message(message), _lineNumber(lineNumber), _data1lineNumber(-1), _data2lineNumber(-1), _fileName(fileName)
{}
inline CxException::CxException(const std::string &message, long lineNumber, long data1lineNumber, const std::string &fileName)
    : _message(message), _lineNumber(lineNumber), _data1lineNumber(data1lineNumber), _data2lineNumber(-1), _fileName(fileName)
{}
inline CxException::CxException(const std::string &message, long lineNumber, long data1lineNumber, long data2lineNumber, const std::string &fileName)
    : _message(message), _lineNumber(lineNumber), _data1lineNumber(data1lineNumber), _data2lineNumber(data2lineNumber), _fileName(fileName)
{}
inline CxException::~CxException() throw()
{}
inline CxException &CxException::operator=(const CxException &other)
{
    exception::operator=(other);
    if (&other != this)
    {
        _message = other._message;
        _lineNumber = other._lineNumber;
        _data1lineNumber = other._data1lineNumber;
        _data2lineNumber = other._data2lineNumber;
        _fileName = other._fileName;
    }
    return *this;
}
inline const char *CxException::what() const throw()
{ return _message.c_str(); }
inline long CxException::lineNumber() const
{ return _lineNumber; }
inline long CxException::data1LineNumber() const
{ return _data1lineNumber; }
inline long CxException::data2LineNumber() const
{ return _data2lineNumber; }
// The file in which the error occurred
inline const std::string &CxException::fileName() const
{ return _fileName; }

//-----------------------------------------------------------------------------------------
//
//part009 CxFactory
//
//-----------------------------------------------------------------------------------------
template<class ClassBase>
class CxFactoryTemplate {
public:
    CxFactoryTemplate()
    {}

    virtual ~CxFactoryTemplate()
    {}

    virtual std::string factoryName() =0;

    virtual ClassBase *createObject() = 0;

};

template<class ClassBase>
class CxFactoryManagerTemplate {
public:
    typedef CxFactoryTemplate<ClassBase> CxFactoryBase;

    template<class FactorySon>
    static FactorySon *createAndRegister()
    {
        FactorySon *oNewFactory = new FactorySon();
        CxFactoryBase *oFactory = NULL;
        std::vector<CxFactoryBase *> *oFactorys = ClassBase::factoriesContainer();
        for (size_t i = 0; i < oFactorys->size(); ++i)
        {
            CxFactoryBase *o = oFactorys->at(i);
            if (o->factoryName() == oNewFactory->factoryName())
            {
                oFactory = o;
                break;
            }
        }
        if (oFactory)
        {
            delete oNewFactory;
            return reinterpret_cast<FactorySon *>(oFactory);
        }
        else
        {
            ClassBase::factoriesContainer()->push_back(oNewFactory);
            return oNewFactory;
        }
    }

    static const std::vector<CxFactoryBase *> *factorys()
    {
        ClassBase::factoriesCreateAndRegister();
        return ClassBase::factoriesContainer();
    }

    static std::vector<std::string> factoryNames()
    {
        ClassBase::factoriesCreateAndRegister();

        std::vector<std::string> slResult;
        std::vector<CxFactoryBase *> *oFactorys = ClassBase::factoriesContainer();
        for (size_t i = 0; i < oFactorys->size(); ++i)
        {
            CxFactoryBase *o = oFactorys->at(i);
            slResult.push_back(o->factoryName());
        }
        return slResult;
    }
    static std::string toLower(const std::string &s)
    {
        std::string r(s.data(), s.size());
        char *pchBegin = const_cast<char *>( r.data());
        char *pchEnd = pchBegin + r.size();
        char *pch = pchBegin;
        while (pch < pchEnd)
        {
            if (*pch >= 0x41 && *pch <= 0x5A)
                *pch += 0x20;
            pch++;
        }
        return r;
    }

    static CxFactoryBase *factory(const std::string &sFactoryName)
    {
        ClassBase::factoriesCreateAndRegister();

        std::vector<CxFactoryBase *> *oFactorys = ClassBase::factoriesContainer();
        std::string sName2 = toLower(sFactoryName);
        for (size_t i = 0; i < oFactorys->size(); ++i)
        {
            CxFactoryBase *o = oFactorys->at(i);
            std::string sName1 = toLower(o->factoryName());
            if (sName1 == sName2)
                return o;
        }
        return NULL;
    }

    template<class ClassSon>
    static ClassSon *createInstance(const std::string &sFactoryName)
    {
        ClassBase::factoriesCreateAndRegister();

        CxFactoryBase *oFactoryBase = factory(sFactoryName);
        if (oFactoryBase)
        {
            return reinterpret_cast<ClassSon *>(oFactoryBase->createObject());
        }
        return NULL;
    }

    static ClassBase *createObject(const std::string &sFactoryName)
    {
        ClassBase::factoriesCreateAndRegister();

        CxFactoryBase *oFactoryBase = factory(sFactoryName);
        if (oFactoryBase)
        {
            return oFactoryBase->createObject();
        }
        return NULL;
    }
};
//template<class ClassBase>
//void* CxFactoryManagerTemplate<ClassBase>::s_factorys = NULL;

//***type full name : use in class
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
