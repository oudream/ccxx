
###setting cmake option:
##-DQT_QMAKE_EXECUTABLE:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32/bin/qmake.exe
##-DCMAKE_PREFIX_PATH:FILEPATH=F:/qt5.5.1mingw4.9/5.5/mingw492_32

# Find the QtWidgets library
if(APPLE)
    set(CMAKE_PREFIX_PATH /usr/local/opt/qt5)
elseif(UNIX)
    set(CMAKE_PREFIX_PATH /fff/qt5.6.3/5.6.3/gcc_64)
elseif(WIN32)
    if(MSVC)
        set(CMAKE_PREFIX_PATH F:/qt5.6.1msvc2015/5.6/msvc2015)
    else()
        set(CMAKE_PREFIX_PATH F:/qt5.5.1mingw4.9/5.5/mingw492_32)
    endif()
endif()