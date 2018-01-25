
cmake_minimum_required(VERSION 3.0)

if(APPLE)
# http://kescoode.com/cmake-rpath-problem/
# https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
set(CMAKE_MACOSX_RPATH 0)
endif()

add_subdirectory(./gtest/googletest)

add_subdirectory(./build/cmake)

