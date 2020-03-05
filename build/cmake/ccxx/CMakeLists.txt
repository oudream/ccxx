
include(ccxx.cmake)

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
