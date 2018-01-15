/* ucommon-config.h.  Generated from ucommon-config.h.in by configure.  */
/* ucommon-config.h.in.  Generated from configure.ac by autoheader.  */

/* has posix clock functions */
//no use
//#define HAVE_CLOCK_GETTIME 1

/* has clocked nanosleep */
//use to : Timer::sync()
#define HAVE_CLOCK_NANOSLEEP 1

/* Define to 1 if you have the <dirent.h> header file. */
//win32 no dirent.h
//#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
//win32 no dlfcn.h
//#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <endian.h> header file. */
//win32 no endian.h
//#define HAVE_ENDIAN_H 1

/* has execvp in libc */
//no use
//#define HAVE_EXECVP 1

/* Define to 1 if you have the <fcntl.h> header file. */
//have in win32(vc,gcc) linux
#define HAVE_FCNTL_H 1

/* has ftok */
//only in unix
//#define HAVE_FTOK 1

/* can truncate files */
//only in unix
//#define HAVE_FTRUNCATE 1

/* "cannot test in autoconf safely" */
//#define HAVE_GCC_ATOMICS

/* have getaddrinfo */
#define HAVE_GETADDRINFO 1

/* have gethostbyname2 */
/* #undef HAVE_GETHOSTBYNAME2 */

/* has gettext in glibc */
#define HAVE_GETTEXT 1

/* has localtime_r */
//#define HAVE_GMTIME_R 1

/* have inet_ntop */
//#define HAVE_INET_NTOP 1

/* Define to 1 if you have the <inttypes.h> header file. */
//no use
//#define HAVE_INTTYPES_H 1

/* international headers */
//vc no libintl.h
//#define HAVE_LIBINTL_H 1

/* Define to 1 if you have the <linux/version.h> header file. */
/* #undef HAVE_LINUX_VERSION_H */

/* has localtime_r */
//#define HAVE_LOCALTIME_R 1

/* have lstat */
//only on unix
//#define HAVE_LSTAT 1

/* Define to 1 if you have the <mach/clock.h> header file. */
/* #undef HAVE_MACH_CLOCK_H */

/* Define to 1 if you have the <mach-o/dyld.h> header file. */
/* #undef HAVE_MACH_O_DYLD_H */

/* Define to 1 if you have the <memory.h> header file. */
//no use
#define HAVE_MEMORY_H 1

/* has nanosleep */
#define HAVE_NANOSLEEP 1

/* Define to 1 if you have the <netinet/in.h> header file. */
//only unix
//#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <net/if.h> header file. */
//only unix
//#define HAVE_NET_IF_H 1

/* Define to 1 if you have the <openssl/fips.h> header file. */
/* #undef HAVE_OPENSSL_FIPS_H */

/* Define to 1 if you have the <openssl/ssl.h> header file. */
/* #undef HAVE_OPENSSL_SSL_H */

/* Define to 1 if you have the <poll.h> header file. */
//only unix
//#define HAVE_POLL_H 1

/* can specify access options */
//#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,8)
//#ifdef  HAVE_POSIX_FADVISE
//#undef  HAVE_POSIX_FADVISE
//linux version > 2.6.8
//#define HAVE_POSIX_FADVISE 1

/* posix memory alignment */
//#define HAVE_POSIX_MEMALIGN 1

/* "pthread clocking" */
/* #undef HAVE_PTHREAD_CONDATTR_SETCLOCK */

/* "pthread delay" */
/* #undef HAVE_PTHREAD_DELAY */

/* "pthread delay np" */
/* #undef HAVE_PTHREAD_DELAY_NP */

/* "pthread concurrency" */
/* #undef HAVE_PTHREAD_SETCONCURRENCY */

/* "pthread scheduling" */
/* #undef HAVE_PTHREAD_SETSCHEDPRIO */

/* "pthread yield" */
/* #undef HAVE_PTHREAD_YIELD */

/* can do atomic write with offset */
//no use
//#define HAVE_PWRITE 1

/* has readlink in libc */
//only unix valid
#define HAVE_READLINK 1

/* realpath support */
//ucommon realize
//#define HAVE_REALPATH 1

/* have regex header */
//#define HAVE_REGEX_H 1

/* can set localization */
#define HAVE_SETLOCALE 1

/* process group control */
//only nuix
//#define HAVE_SETPGRP 1

/* alternate dyn loader */
/* #undef HAVE_SHL_LOAD */

/* shared memory open */
#define HAVE_SHM_OPEN 1

/* have socketpair in libsocket */
/* #undef HAVE_SOCKETPAIR */

/* "Socks proxy support" */
/* #undef HAVE_SOCKS */

/* Define to 1 if you have the <socks.h> header file. */
/* #undef HAVE_SOCKS_H */

/* Define to 1 if you have the <stdexcept> header file. */
//#define HAVE_STDEXCEPT 1

/* Define to 1 if you have the <stdint.h> header file. */
//no use
//#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
//no use
//#define HAVE_STDLIB_H 1

/* string collation */
//#define HAVE_STRCOLL 1

/* has strerror_r */
//#define HAVE_STRERROR_R 1

/* string icmp */
#define HAVE_STRICMP 1

/* Define to 1 if you have the <strings.h> header file. */
//#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
//#define HAVE_STRING_H 1

/* string istr */
/* #undef HAVE_STRISTR */

/* has symlink in libc */
//#define HAVE_SYMLINK 1

/* system config */
//#define HAVE_SYSCONF 1

/* Define to 1 if you have the <syslog.h> header file. */
//#define HAVE_SYSLOG_H 1

/* Define to 1 if you have the <sys/event.h> header file. */
/* #undef HAVE_SYS_EVENT_H */

/* Define to 1 if you have the <sys/file.h> header file. */
//#define HAVE_SYS_FILE_H 1

/* Define to 1 if you have the <sys/filio.h> header file. */
/* #undef HAVE_SYS_FILIO_H */

/* Define to 1 if you have the <sys/inotify.h> header file. */
/* #undef HAVE_SYS_INOTIFY_H */

/* Define to 1 if you have the <sys/lockf.h> header file. */
/* #undef HAVE_SYS_LOCKF_H */

/* Define to 1 if you have the <sys/mman.h> header file. */
//#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
//#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
//#define HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
//#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/shm.h> header file. */
//#define HAVE_SYS_SHM_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
//#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/timeb.h> header file. */
#define HAVE_SYS_TIMEB_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
//#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
//#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <termios.h> header file. */
//#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <termio.h> header file. */
//#define HAVE_TERMIO_H 1

///* Define to 1 if you have the <unistd.h> header file. */
//#define HAVE_UNISTD_H 1

/* has wait4 in libc */
//#define HAVE_WAIT4 1

/* has waitpid in libc */
//#define HAVE_WAITPID 1

/* Define to 1 if you have the <wchar.h> header file. */
#define HAVE_WCHAR_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
//#define LT_OBJDIR ".libs/"

/* Name of package */
//#define PACKAGE "ucommon"

/* Define to the address where bug reports for this package should be sent. */
//#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
//#define PACKAGE_NAME "ucommon"

/* Define to the full name and version of this package. */
//#define PACKAGE_STRING "ucommon 6.0.7"

/* Define to the one symbol short name of this package. */
//#define PACKAGE_TARNAME "ucommon"

/* Define to the home page for this package. */
//#define PACKAGE_URL ""

/* Define to the version of this package. */
//#define PACKAGE_VERSION "6.0.7"

/* Define to 1 if you have the ANSI C header files. */
//#define STDC_HEADERS 1

/* config path */
#define UCOMMON_CFGPATH "/etc"

/* locale path */
//#define UCOMMON_LOCALE "/usr/local/share/locale"

/* config path */
#define UCOMMON_PREFIX "/usr/local"

/* config path */
#define UCOMMON_VARPATH "/var"

/* Version number of package */
//#define VERSION "6.0.7"

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#define restrict __restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
   __restrict__, even though the corresponding Sun C compiler ends up with
   "#define restrict _Restrict" or "#define restrict __restrict__" in the
   previous line.  Perhaps some future version of Sun C++ will work with
   restrict; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif

/* Define to empty if the keyword `volatile' does not work. Warning: valid
   code using `volatile' can become incorrect without. Disable with care. */
/* #undef volatile */


#define NEW_STDCPP

#if defined(_MSC_VER) || defined(WIN32) || defined(_WIN32)
#   define NOMINMAX
#   undef UNICODE
#   undef _UWIN
#   undef restrict
#endif

#include <ucommon/platform.h>


