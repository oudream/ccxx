
include(${gs_ccxx_path}/ccxx/ccxx.cmake)

set(gs_ccxx_sample_path ${gs_ccxx_path}/sample)
set(gs_ccxx_test_path ${gs_ccxx_path}/test)
set(gs_ccxx_tools_path ${gs_ccxx_path}/tools)

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

add_subdirectory(libs)
add_subdirectory(sample)
add_subdirectory(test)
add_subdirectory(tools)
add_subdirectory(lua)
add_subdirectory(gtest)
add_subdirectory(benchmark)

if (gs_project_enable_cpython)
    add_subdirectory(python)
endif()
