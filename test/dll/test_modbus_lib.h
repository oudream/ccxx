#ifndef TEST_MODBUS_LIB_H
#define TEST_MODBUS_LIB_H

#include <ccxx/ccxx.h>

#if defined(VXD_API_MODBUS_IMPL_EXPORT)
    #define VXD_API_MODBUS_IMPL GM_DECL_EXPORT
#elif defined(VXD_API_MODBUS_IMPL_SRC)
    #define VXD_API_MODBUS_IMPL
#else
    #define VXD_API_MODBUS_IMPL GM_DECL_IMPORT
#endif

//#define VXD_API_CALL __stdcall
#define VXD_API_CALL __cdecl

#ifdef __cplusplus
extern "C" {
#endif


VXD_API_MODBUS_IMPL int VXD_API_CALL vxd_modbus_init(int argc, const char * argv[]);


#ifdef __cplusplus
}
#endif


#endif // TEST_MODBUS_LIB_H
