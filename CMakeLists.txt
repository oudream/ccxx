
cmake_minimum_required(VERSION 3.0)

if(APPLE)
# http://kescoode.com/cmake-rpath-problem/
# https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
set(CMAKE_MACOSX_RPATH 0)
endif()

set(gs_project_path ${CMAKE_CURRENT_SOURCE_DIR})

# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
include(./gtest/googletest/googletest/cmake/internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.

add_subdirectory(./gtest)

add_subdirectory(./build/cmake)

add_subdirectory(./3rd)
