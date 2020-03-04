
### * setting cmake option:
set(CMAKE_CXX_STANDARD 11)
set(BUILD_SHARED_LIBS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (APPLE)
    # http://kescoode.com/cmake-rpath-problem/
    # https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
    set(CMAKE_MACOSX_RPATH 0)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${gs_project_output_path_release})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRELEASE")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DRELEASE")
else ()
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${gs_project_output_path_debug})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
endif ()
message(------CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


###  * qt
### if enable qt, then config "build/ccpro/local.all.config.cmake" 's CMAKE_PREFIX_PATH to your dir of qt
set(gs_project_enable_qt 0)
find_package(Qt5Core)
if (Qt5Core_LIBRARIES)
    set(gs_project_enable_qt 1)

    ##-DQT_QMAKE_EXECUTABLE:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32/bin/qmake.exe
    ##-DCMAKE_PREFIX_PATH:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32
    # Find the QtWidgets library
    if (APPLE)
        list(APPEND CMAKE_PREFIX_PATH /Users/oudream/Qt5.5.1/5.5/clang_64)
        set(gs_prefix_path_python /Library/Frameworks/Python.framework/Versions/3.6)
    elseif (UNIX)
        list(APPEND CMAKE_PREFIX_PATH /fff/qt5.6.3/5.6.3/gcc_64)
    elseif (WIN32)
        message("CMAKE_SIZEOF_VOID_P:" ${CMAKE_SIZEOF_VOID_P})
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(gs_prefix_path_python C:/Python/Python36-64)
        else ()
            set(gs_prefix_path_python C:/Python/Python36-32)
        endif ()
        if (MSVC)
            message(${MSVC_VERSION})
            if (MSVC_VERSION EQUAL 1800) # 1800 is Visual Studio 2013
                list(APPEND CMAKE_PREFIX_PATH F:/qt5.6.3/5.6.3/msvc2013)
            elseif (MSVC_VERSION EQUAL 1900)
                list(APPEND CMAKE_PREFIX_PATH F:/qt5.6.1msvc2015/5.6/msvc2015)
            elseif (MSVC_VERSION GREATER 1910)
                list(APPEND CMAKE_PREFIX_PATH F:/qt5.12.3/5.12.3/msvc2017)
            else ()
                message("can not find qt version!!!")
            endif ()
        else ()
            list(APPEND CMAKE_PREFIX_PATH F:/qt5.5.1mingw4.9/5.5/mingw492_32)
        endif ()
    endif ()
endif ()
#set(gs_project_enable_qt 1)
# set(gs_project_qt_version 4)


### * cpython
set(gs_project_enable_cpython 0)
set(Python_ADDITIONAL_VERSIONS 3.8 3.7 3.6 3.5 3.4)
find_package(PythonLibsNew)
if (PYTHONLIBS_FOUND)
    set(gs_project_enable_cpython 1)
    # Cache variables so pybind11_add_module can be used in parent projects
    set(PYBIND11_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")
    set(PYTHON_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS} CACHE INTERNAL "")
    set(PYTHON_LIBRARIES ${PYTHON_LIBRARIES} CACHE INTERNAL "")
    set(PYTHON_MODULE_PREFIX ${PYTHON_MODULE_PREFIX} CACHE INTERNAL "")
    set(PYTHON_MODULE_EXTENSION ${PYTHON_MODULE_EXTENSION} CACHE INTERNAL "")
    set(PYTHON_VERSION_MAJOR ${PYTHON_VERSION_MAJOR} CACHE INTERNAL "")
    set(PYTHON_VERSION_MINOR ${PYTHON_VERSION_MINOR} CACHE INTERNAL "")
    if (WIN32 AND NOT MSVC AND gs_prefix_path_python)
        if (NOT PYTHON_LIBRARIES)
            #        target_link_libraries( IcsRt2Stack debug ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/libgcl_sdk.dll)
            #        target_link_libraries( IcsRt2Stack optimized ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/libgcl_sdk.dll)
            set(PYTHON_LIBRARIES ${gs_prefix_path_python}/libs/libpython36.a)
        endif ()
    endif ()
    message(------PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS})
    message(------PYTHON_LIBRARIES: ${PYTHON_LIBRARIES})
    message(------PYTHON_DEBUG_LIBRARIES: ${PYTHON_DEBUG_LIBRARIES})
endif ()
#set(gs_project_enable_cpython 1)
# set(gs_project_cpython_version 2)


### * libuv
set(gs_project_enable_uv 1)
#set(gs_project_enable_uv 0)


### * curl
set(gs_project_enable_curl 0)
if (NOT WIN32)
    find_package(CURL)
    if (CURL_LIBRARIES)
        set(gs_project_enable_curl 1)
    endif ()
else ()
    set(gs_project_enable_curl 1)
endif ()
#set(gs_project_enable_curl 1)


### * openssl
set(gs_project_enable_openssl 1)
if (NOT WIN32)
    find_package(OpenSSL)
    if (OPENSSL_SSL_LIBRARY)
        set(gs_project_enable_openssl 1)
    endif ()
else ()
    set(gs_project_enable_openssl 1)
endif ()
#set(gs_project_enable_openssl 0)


### * opencv
set(gs_project_enable_opencv 1)
#set(gs_project_enable_opencv 0)
if (WIN32)
    set(gs_project_enable_opencv 0)
endif ()
if (gs_project_enable_opencv)
    set(gs_project_enable_opencv_sample 1)
    #set(gs_project_enable_opencv_sample 0)
endif ()


### * yamlcpp yaml-cpp
set(gs_project_enable_yamlcpp 1)
#set(gs_project_enable_yamlcpp 0)

### * hiredis
set(gs_project_enable_hiredis 1)
#set(gs_project_enable_hiredis 0)


### * gtest googletest
set(gtest_build_tests 1)
set(gtest_build_samples 1)
set(gmock_build_tests 1)
set(gtest_build_tests 0)
set(gtest_build_samples 0)
set(gmock_build_tests 0)


### * benchmark
set(BENCHMARK_ENABLE_TESTING ON)
set(BENCHMARK_ENABLE_TESTING OFF)


### * htop
set(gs_project_enable_htop 1)
### cd /opt/ddd/ccpp/ccxx/3rd/htop/readme.sh
### ./autogen.sh && ./configure --disable-unicode
#set(gs_project_enable_htop 0)


if (gs_project_enable_cpython AND gs_prefix_path_python)
    list(APPEND CMAKE_PREFIX_PATH ${gs_prefix_path_python})
endif ()

### libuv uv
if (gs_project_enable_uv)
    set(gs_libuv_path ${gs_project_path}/3rd/libuv/libuv)
    set(gs_libuv_src_path ${gs_libuv_path}/src)
    set(gs_libuv_include_path ${gs_libuv_path}/include)
    set(gs_libuv_cxtest_path ${gs_project_path}/test/libuv)
endif ()

### yamlcpp yaml-cpp
if (gs_project_enable_yamlcpp)
    set(gs_yamlcpp_path ${gs_project_path}/3rd/yaml-cpp)
    set(gs_yamlcpp_src_path ${gs_yamlcpp_path}/yaml-cpp/src)
    set(gs_yamlcpp_test_path ${gs_yamlcpp_path}/yaml-cpp/test)
    set(gs_yamlcpp_include_path ${gs_yamlcpp_path}/yaml-cpp/include)
    set(gs_yamlcpp_cxtest_path ${gs_project_path}/test/yamlcpp)
endif ()

### libuv uv
if (gs_project_enable_hiredis)
    set(gs_hiredis_path ${gs_project_path}/3rd/hiredis)
    set(gs_hiredis_src_path ${gs_hiredis_path}/hiredis)
    set(gs_hiredis_include_path ${gs_hiredis_path})
    set(gs_hiredis_cxtest_path ${gs_project_path}/test/hiredis)
endif ()

message(------CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH})
message(------local.config.cmake type: local.all.config.cmake)
