
cmake_minimum_required(VERSION 3.0)

project(GTESTWORKGROUP VERSION 1.0.1 LANGUAGES CXX C)

### depend path
#CMAKE_CURRENT_SOURCE_DIR (current file path)
if (NOT gs_project_path)
    set(gs_project_path ${CMAKE_CURRENT_SOURCE_DIR}/..)
endif()
message('PROJECT-PATH: ' ${gs_project_path})

set(gs_project_deploy_path ${gs_project_path}/deploy)
set(gs_project_include_path ${gs_project_path}/include)
set(gs_project_3rd_path ${gs_project_path}/3rd)

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

include(./../build/cmake/common.cmake)
include(./../build/cmake/local.config.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(${gs_project_include_path})

add_subdirectory(./googletest)
add_subdirectory(./test)
add_subdirectory(./ccxx)
