
cmake_minimum_required(VERSION 3.0)

project(ccxx)

set(gs_project_path ${CMAKE_CURRENT_SOURCE_DIR})

if (NOT gs_project_deploy_path)
    set(gs_project_deploy_path ${gs_project_path}/build/deploy)
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

    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")
    SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
    SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")
    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")

    message(------gs_project_output_path_debug: ${gs_project_output_path_debug})
    message(------gs_project_output_path_release: ${gs_project_output_path_release})
endif()

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/modules")

if (CCXX_BUILD_TYPE STREQUAL all)
    include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/local.all.config.cmake)
else()
    include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/local.simple.config.cmake)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/common.cmake)

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(${gs_project_include_path})

add_subdirectory(./3rd)

add_subdirectory(./gtest)

if (${gs_project_enable_opencv_sample})
    add_subdirectory(./sample/opencv)
endif ()

add_subdirectory(./build/cmake)
