#ifndef TEST_VDI_LIB_H
#define TEST_VDI_LIB_H

#include <ccxx/ccxx.h>

#if defined(GM_BUILD_GCL_VDI_LIB)
#define GM_GCL_VDI_API  GM_DECL_EXPORT
#elif defined(GM_BUILD_GCL_VDI_CODE)
#define GM_GCL_VDI_API
#else
#define GM_GCL_VDI_API  GM_DECL_IMPORT
// GM_BUILD_GCL_VDI_INCLUDE
#endif

class GM_GCL_VDI_API TestVdi
{
public:
    static int init(int argc, const char * argv[]);

    static int start();

    static void stop();

};


#endif // TEST_VDI_LIB_H
