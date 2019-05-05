
#set(gs_libuv_path ${gs_project_path}/3rd/libuv/libuv)
#set(gs_libuv_src_path ${gs_libuv_path}/src)
#set(gs_libuv_include_path ${gs_libuv_path}/include)
#set(gs_libuv_cxtest_path ${gs_project_path}/test/libuv)

#include_directories(${gs_libuv_include_path})

add_subdirectory(redis-simple)
add_subdirectory(cxhiredis)
