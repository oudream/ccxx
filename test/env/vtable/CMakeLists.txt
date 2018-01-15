cmake_minimum_required(VERSION 3.6)
project(vtable)

#COMPILE_FLAGS(-Wint-to-pointer-cast)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES test1.cpp)
add_executable(vtable ${SOURCE_FILES})