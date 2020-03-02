
### * setting cmake option:
set(CMAKE_CXX_STANDARD 11)
set(BUILD_SHARED_LIBS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(APPLE)
    # http://kescoode.com/cmake-rpath-problem/
    # https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
    set(CMAKE_MACOSX_RPATH 0)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${gs_project_output_path_release})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRELEASE")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DRELEASE")
else()
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${gs_project_output_path_debug})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
endif()
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY--: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message("local.config.cmake type--: " local.simple.config.cmake)
