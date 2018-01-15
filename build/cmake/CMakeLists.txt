#cmake . -G "Visual Studio 12 2013" --build "C:\ddd\ccpp\ccxx\build\cmake" -B"C:\ddd\ccpp\ccxx\build\cmake-vs2013"
#cmake . -G "Visual Studio 14 2015" --build "C:\ddd\ccpp\ccxx\build\cmake" -B"C:\ddd\ccpp\ccxx\build\cmake-vs2015"
#cmake . --build "C:\ddd\ccpp\ccxx\build\cmake" -B"C:\ddd\ccpp\ccxx\build\cmake-vs-local"
#cd /d "C:\ddd\ccpp\ccxx\build\cmake-vs-local"
#cmake --build .
#-DQT_QMAKE_EXECUTABLE:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32/bin/qmake.exe
#-DCMAKE_PREFIX_PATH:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32

cmake_minimum_required(VERSION 3.0)

project(CCXX VERSION 1.0.1 LANGUAGES CXX C)

###qt
###if enable qt, then config "build/ccpro/local.config.cmake" 's CMAKE_PREFIX_PATH to your dir of qt
set(gs_project_enable_qt 1)
#set(gs_project_enable_qt 0)
#set(gs_project_qt_version 4)

###depend
#CMAKE_CURRENT_SOURCE_DIR (current file path)
#set(gs_project_ccpro_path ${CMAKE_CURRENT_SOURCE_DIR})
#set(gs_project_ccpro_script_path ${CMAKE_CURRENT_SOURCE_DIR}/script)
set(gs_project_deploy_path ${CMAKE_CURRENT_SOURCE_DIR}/../deploy)
set(gs_project_path ${CMAKE_CURRENT_SOURCE_DIR}/../..)
#set(gs_project_basic_path ${gs_project_path}/basic)
#set(gs_project_build_path ${gs_project_path}/build)
#set(gs_project_example_path ${gs_project_path}/example)
set(gs_project_include_path ${gs_project_path}/include)
#set(gs_project_lib_path ${gs_project_path}/lib)
#set(gs_project_res_path ${gs_project_path}/res)
#set(gs_project_src_path ${gs_project_path}/src)
set(gs_project_3rd_path ${gs_project_path}/3rd)
#set(gs_project_temp_path ${gs_project_path}/temp)
#set(gs_project_test_path ${gs_project_path}/test)

set(gs_ccxx_path ${gs_project_path})

if (UNIX)
    set(gs_project_output_path_debug ${gs_project_deploy_path}/unix/bin_d)
    set(gs_project_output_path_release ${gs_project_deploy_path}/unix/bin)
endif()
if (WIN32)
    set(gs_project_output_path_debug ${gs_project_deploy_path}/win32/bin_d)
    set(gs_project_output_path_release ${gs_project_deploy_path}/win32/bin)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++")
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
        set(gs_project_output_path_debug ${gs_project_deploy_path}/win32/bin_gnu_d)
        set(gs_project_output_path_release ${gs_project_deploy_path}/win32/bin_gnu)
        message("bbbbbbbbbbbbbbbbbbbb")
    endif()
endif()

message(${CMAKE_C_COMPILER_ID})
message(${CMAKE_CXX_COMPILER_ID})
message(${gs_project_output_path_debug})
message(${gs_project_output_path_release})

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")

set(CMAKE_CXX_STANDARD 11)

include(common.cmake)
include(local.config.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(${gs_project_include_path})

###lib exe
add_subdirectory(ccxx)

message(bging 001)
#message(MSVC)
message(end 001)
