#ifndef CXSHAREMEMORY_H
#define CXSHAREMEMORY_H


#include "cxglobal.h"

#define ci_sharememory_status_invalid (0)
#define ci_sharememory_status_good (1)
#define ci_sharememory_status_initing (2)
#define ci_sharememory_status_closing (3)

#define ci_sharememory_head1 (0xEB90EB90)
#define ci_sharememory_head2 (0xA55AA55A)


/**
 * Construct or access a named section of memory.  A logical name is used
 * which might map to something that is invoked from a call like shm_open
 * or a named w32 mapped swap segment.  This is meant to support mapping a
 * vector onto shared memory and is often used as a supporting class for our
 * shared memory access templates.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class GM_CCXX_CORE_API CxShareMemory
{
protected:
    size_t mapsize;
    caddr_t map;
    fd_t fd;

    size_t size, used;
    char idname[65];
    bool erase;

    /**
     * Supporting function to construct a new or access an existing
     * shared memory segment.  Used by primary constructors.
     * @param name of segment to create or access.
     * @param size of segment if creating new.  Use 0 for read-only access.
     */
    bool create(const char *name, size_t size = (size_t)0);

    /**
     * Handler for failure to map (allocate) memory.
     */
    virtual void fault(void) const;

public:
    /**
     * @brief CxShareMemory
     */
    CxShareMemory();

    /**
     * Provide read-only mapped access to an existing named shared memory
     * segment.  The size of the map is found by the size of the already
     * existing segment.
     * @param name of existing segment.
     */
    CxShareMemory(const char *name);

    /**
     * Construct a read/write access mapped shared segment of memory of a
     * known size.  This constructs a new memory segment.
     * @param name of segment.
     * @param size of segment.
     */
    CxShareMemory(const char *name, size_t size);

    /**
     * Unmap memory segment.
     */
    virtual ~CxShareMemory();

    /**
     * @brief openWrite
     * @param name
     * @param size
     * @return
     */
    inline bool openModify(const char *name, size_t size) {
        create(name, size);
        return map;
    }

    /**
     * @brief openRead
     * @param name
     * @return
     */
    inline bool openView(const char *name) {
        create(name);
        return map;
    }

    /**
     * @brief close
     */
    inline void close(void) { release(); }

    /**
     * Unmap memory segment.
     */
    void release(void);

    /**
     * @brief isValid
     * @return
     */
    inline bool isValid() const {
        return (size != 0);
    }

    /**
     * Test if map active.
     * @return true if active map.
     */
    inline operator bool() const
        {return (size != 0);}

    /**
     * Test if map is inactive.
     * @return true if map inactive.
     */
    inline bool operator!() const
        {return (size == 0);}

    /**
     * Extend size of managed heap on shared memory segment.  This does not
     * change the size of the mapped segment in any way, only that of any
     * heap space that is being allocated and used from the mapped segment.
     * @return start of space from map.
     * @param size of space requested.  Will fault if past end of segment.
     */
    void * sbrk(size_t size);

    /**
     * Get memory from a specific offset within the mapped memory segment.
     * @param offset from start of segment.  Will fault if past end.
     * @return address of offset.
     */
    void * offset(size_t offset) const;

    /**
     * Copy memory from specific offset within the mapped memory segment.
     * This function assures the copy is not in the middle of being modified.
     * @param offset from start of segment.
     * @param buffer to copy into.
     * @param size of object to copy.
     * @return true on success.
     */
    bool copy(size_t offset, void *buffer, size_t size) const;

    /**
     * Get size of mapped segment.
     * @return size of mapped segment.
     */
    inline size_t len(void)
        {return size;}

    /**
     * Get starting address of mapped segment.
     * @return starting address of mapped segment.
     */
    inline caddr_t addr(void)
        {return map;}

    /**
     * Destroy a previously existing memory segment under the specified name.
     * This is used both before creating a new one, and after a publishing
     * process unmaps the segment it created.
     * @param name of segment to remove.
     */
    static  void remove(const char *name);

    /**
     * An API that allows "disabling" of publishing shared memory maps.
     * This may be useful when an app doesn't want to use shared memory
     * as a runtime or build option, but does not want to have to be "recoded"
     * explicitly for non-shared memory either.  Basically it substitutes a
     * dummy map running on the local heap.
     */
    static void disable(void);

    /**
     * Safely set a null terminated string buffer in memory.  If the text
     * is too large to fit into the buffer, it is truncated to the size.
     * @param buffer to set.
     * @param size of buffer.  Includes null byte at end of string.
     * @param text to set in buffer.
     * @return pointer to text buffer.
     */
    static char *set(char *buffer, size_t size, const char *text);

};








#endif // CXSHAREMEMORY_H
