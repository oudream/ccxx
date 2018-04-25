
###setting cmake option:
##-DQT_QMAKE_EXECUTABLE:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32/bin/qmake.exe
##-DCMAKE_PREFIX_PATH:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32

# Find the QtWidgets library
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH /Users/oudream/Qt5.5.1/5.5/clang_64)
elseif(UNIX)
    list(APPEND CMAKE_PREFIX_PATH /fff/qt5.6.3/5.6.3/gcc_64)
elseif(WIN32)
    message("CMAKE_SIZEOF_VOID_P:" ${CMAKE_SIZEOF_VOID_P})
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        list(APPEND CMAKE_PREFIX_PATH C:/Python/Python36-64)
    else()
        list(APPEND CMAKE_PREFIX_PATH C:/Python/Python36-32)
    endif()
    if(MSVC)
        message(${MSVC_VERSION})
        if(MSVC_VERSION EQUAL 1800) # 1800 is Visual Studio 2013
            list(APPEND CMAKE_PREFIX_PATH F:/qt5.8.0msvc2013/5.8/msvc2013)
        elseif(MSVC_VERSION EQUAL 1900)
            list(APPEND CMAKE_PREFIX_PATH F:/qt5.6.1msvc2015/5.6/msvc2015)
        elseif(MSVC_VERSION GREATER 1910)
            list(APPEND CMAKE_PREFIX_PATH F:/qt5.10.1/5.10.1/msvc2017_64)
        else()
            message("can not find qt version!!!")
        endif()
    else()
        list(APPEND CMAKE_PREFIX_PATH F:/qt5.5.1mingw4.9.2x86/5.5/mingw492_32)
#        list(APPEND CMAKE_PREFIX_PATH F:/qt5.5.1mingw4.9/5.5/mingw492_32)
    endif()
endif()

message("CMAKE_PREFIX_PATH:" ${CMAKE_PREFIX_PATH})