// Copyright (C) 2001-2005 Open Source Telecom Corporation.
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
 * @file misc.h
 * @short various miscellaneous classes historically used.
 **/

#ifndef COMMONCPP_MISC_H_
#define COMMONCPP_MISC_H_

#ifndef COMMONCPP_CONFIG_H_
#include <commoncpp/config.h>
#endif

#define KEYDATA_INDEX_SIZE  97
#define KEYDATA_PAGER_SIZE  512
#if defined(PATH_MAX)
#if PATH_MAX > 512
#define KEYDATA_PATH_SIZE   512
#else
#define KEYDATA_PATH_SIZE   PATH_MAX
#endif
#else
#define KEYDATA_PATH_SIZE   256
#endif

NAMESPACE_COMMONCPP

class __EXPORT MemPager : private ucommon::memalloc
{
public:
	inline MemPager(size_t pagesize = 4096) : ucommon::memalloc(pagesize) {};

	inline void *alloc(size_t size)
		{return _alloc(size);}

	char *alloc(const char *str);

    inline char *first(const char *str)
        {return alloc(str);}

	inline void *first(size_t size)
		{return _alloc(size);}

	inline int getPages(void)
		{return pages();}

    inline void purge(void)
        {memalloc::purge();}
};

/**
 * The shared mempager uses a mutex to protect key access methods.
 * This class is used when a mempager will be shared by multiple
 * threads.
 *
 * @author David Sugar <dyfet@gnutelephony.org>
 * @short mutex protected memory pager.
 */
class __EXPORT SharedMemPager : public MemPager, public Mutex
{
protected:
    /**
     * Create a mempager mutex pool.
     *
     * @param pagesize page size for allocation.
     * @param name a name for the pool.
     */
    SharedMemPager(size_t pagesize = 4096);
    /**
     * Purge the memory pool while locked.
     */
    void purge(void);

    /**
     * Get the last memory page after locking.
     *
     * @return allocated memory space.
     * @param size of request.
     */
    void* alloc(size_t size);

    inline void *first(size_t size)
        {return alloc(size);}
};


/**
 * This class is used to associate (object) pointers with named strings.
 * A virtual is used to allocate memory which can be overriden in the
 * derived class.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short associate names with pointers.
 */
class __EXPORT Assoc
{
private:
    struct entry {
        const char *id;
        entry *next;
        void *data;
    };

    entry *entries[KEYDATA_INDEX_SIZE];

protected:
    Assoc();
    virtual ~Assoc();

    void clear(void);

    virtual void *getMemory(size_t size) = 0;

public:
    void *getPointer(const char *id) const;
    void setPointer(const char *id, void *data);
};

END_NAMESPACE
#endif
