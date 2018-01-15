// Copyright (C) 1999-2005 Open Source Telecom Corporation.
// Copyright (C) 2006-2014 David Sugar, Tycho Softworks.
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

/**
 * Export interfaces for library interfaces.
 * This is a special header used when we have to build DLL's for dumb
 * platforms which require explicit declaration of imports and exports.
 * The real purpose is to include our local headers in a new library
 * module with external headers referenced as imports, and then to define
 * our own interfaces in our new library as exports.  This allows native
 * construction of new DLL's based on/that use ucommon on Microsoft Windows
 * and perhaps for other similarly defective legacy platforms.  Otherwise
 * this header is not used at all, and not when building applications.
 * @file commoncpp/export.h
 */

#if defined(_MSC_VER) || defined(WIN32) || defined(_WIN32) || defined(_MSWINDOWS_)
#ifdef  __EXPORT
#undef  __EXPORT
#endif

#if defined(UCOMMON_STATIC) || defined(UCOMMON_RUNTIME)
#define __EXPORT
#else
#define __EXPORT __declspec(dllexport)
#endif
#endif

