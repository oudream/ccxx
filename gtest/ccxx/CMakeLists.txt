
include(./../../build/cmake/ccxx/ccxx.cmake)

include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${gmock_SOURCE_DIR}/include)

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

add_subdirectory(./cxstring)
#add_subdirectory(./cxappenv)

