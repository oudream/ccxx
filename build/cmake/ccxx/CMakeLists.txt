
set(gs_ccxx_sample_path ${gs_ccxx_path}/sample)
set(gs_ccxx_test_path ${gs_ccxx_path}/test)
set(gs_ccxx_tools_path ${gs_ccxx_path}/tools)

include(ccxx.cmake)

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

add_subdirectory(ccxx)
add_subdirectory(sample)
add_subdirectory(test)
add_subdirectory(tools)
