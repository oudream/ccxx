// Copyright (C) 2009-2014 David Sugar, Tycho Softworks.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.
//
// This exception applies only to the code released under the name GNU
// Common C++.  If you copy code from other releases into a copy of GNU
// Common C++, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
//
// If you write modifications of your own for GNU Common C++, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.
//

#ifndef COMMONCPP_CONFIG_H_
#define COMMONCPP_CONFIG_H_

#ifndef _UCOMMON_UCOMMON_H_
#include <ucommon/ucommon.h>
#endif

#ifdef  __EXPORT
#undef  __EXPORT
#endif

#define __EXPORT    __SHARED

// #include <streambuf>
#include <iostream>

#define COMMONCPP_HEADERS
#define CCXX_NAMESPACES
#define COMMONCPP_NAMESPACE ost
#define NAMESPACE_COMMONCPP namespace ost {
#define TIMEOUT_INF ucommon::Timer::inf

#ifdef  _UCOMMON_EXTENDED_
#define CCXX_EXCEPTIONS
#endif

#ifdef  AF_INET6
#define CCXX_IPV6
#endif

#ifdef  AF_INET
#define CCXX_IPV4
#endif

typedef pthread_t   cctid_t;
typedef int8_t      int8;
typedef uint8_t     uint8;
typedef int16_t     int16;
typedef uint16_t    uint16;
typedef int32_t     int32;
typedef uint32_t    uint32;
typedef int64_t     int64;
typedef uint64_t    uint64;

#if !defined(_MSWINDOWS_) && !defined(__QNX__)

/**
 * Convenience function for case insensitive null terminated string compare.
 * @param string1 to compare.
 * @param string2 to compare.
 * @return 0 if equal, > 0 if s2 > s1, < 0 if s2 < s1.
 */
extern "C" inline int stricmp(const char *string1, const char *string2)
    {return ucommon::String::case_compare(string1, string2);}

/**
 * Convenience function for case insensitive null terminated string compare.
 * @param string1 to compare.
 * @param string2 to compare.
 * @param max size of string to compare.
 * @return 0 if equal, > 0 if s2 > s1, < 0 if s2 < s1.
 */
extern "C" inline int strnicmp(const char *string1, const char *string2, size_t max)
    {return ucommon::String::case_compare(string1, string2, max);}

#endif



#endif
