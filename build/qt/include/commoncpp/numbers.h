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

#ifndef COMMONCPP_NUMBERS_H_
#define COMMONCPP_NUMBERS_H_

#ifndef COMMONCPP_CONFIG_H_
#include <commoncpp/config.h>
#endif

#ifndef COMMONCPP_STRING_H_
#include <commoncpp/string.h>
#endif

typedef ucommon::DateTimeString DateTimeString;
typedef ucommon::DateNumber DateNumber;

class __EXPORT Date : public ucommon::Date
{
protected:
    inline void toJulian(long year, long month, long day)
        {ucommon::Date::set(year, month, day);}

    inline void fromJulian(char *buf) const
        {put(buf);}

public:
    inline Date(time_t value) : ucommon::Date(value) {};

    inline Date(struct tm *object) : ucommon::Date(object) {};

    inline Date(const char *ptr, size_t size = 0) : ucommon::Date(ptr, size) {};

    inline Date(int y, unsigned m, unsigned d) : ucommon::Date(y, m, d) {};

    inline Date(const Date& object) : ucommon::Date(object) {};

    inline Date() : ucommon::Date() {};

    inline int getYear(void) const
        {return year();}

    inline unsigned getMonth(void) const
        {return month();}

    inline unsigned getDay(void) const
        {return day();}

    inline unsigned getDayOfWeek(void) const
        {return dow();}

    inline long getJulian(void) const
        {return julian;}

    inline const char *get(char *buffer) const
        {return put(buffer);}

    inline time_t getTime(void) const
        {return timeref();}

    inline bool isValid(void) const
        {return is_valid();}
};

class __EXPORT Time : public ucommon::Time
{
protected:
    inline void toSeconds(int h, int m = 0, int s = 0)
        {set(h, m, s);}

    inline void fromSeconds(char *buf) const
        {put(buf);}

public:
    inline Time(time_t value) : ucommon::Time(value) {};

    inline Time(tm_t *object) : ucommon::Time(object) {};

    inline Time(const char *ptr, size_t size) : ucommon::Time(ptr, size) {};

    inline Time(int h, int m, int s) : ucommon::Time(h, m, s) {};

    inline Time() : ucommon::Time() {};

    inline int getHour(void) const
        {return hour();}

    inline int getMinute(void) const
        {return minute();}

    inline int getSecond(void) const
        {return second();}

    inline const char *get(char *buffer) const
        {return put(buffer);}

    inline bool isValid(void) const
        {return is_valid();}

};

class __EXPORT DateTime : public ucommon::DateTime
{
public:
    inline DateTime(time_t time) : ucommon::DateTime(time) {};

    inline DateTime(struct tm *dt) : ucommon::DateTime(dt) {};


    inline DateTime(int year, unsigned month, unsigned day,
        int hour = 0, int minute = 0, int second = 0) :
            ucommon::DateTime(year, month, day, hour, minute, second) {};

    inline DateTime(const char *ptr, size_t size) :
        ucommon::DateTime(ptr, size) {};

    inline DateTime(const DateTime& obj) : ucommon::DateTime(obj) {};

    inline DateTime() : ucommon::DateTime() {};

    inline int getYear(void) const
        {return year();}

    inline unsigned getMonth(void) const
        {return month();}

    inline unsigned getDay(void) const
        {return day();}

    inline unsigned getDayOfWeek(void) const
        {return dow();}

    inline long getJulian(void) const
        {return julian;}

    inline const char *get(char *buffer) const
        {return ucommon::DateTime::put(buffer);}

    inline time_t getTime(void) const
        {return ucommon::DateTime::timeref();}

    inline bool isValid(void) const
        {return ucommon::DateTime::is_valid();}

    inline int getHour(void) const
        {return hour();}

    inline int getMinute(void) const
        {return minute();}

    inline int getSecond(void) const
        {return second();}

    inline static tm_t *glt(time_t *time = NULL)
        {return ucommon::DateTime::local(time);}
};

#endif

