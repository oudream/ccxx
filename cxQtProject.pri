
CCXX_PATH = $$PWD/ccxx
PROJECT_PATH = $$PWD
PROJECT_INCLUDE_PATH = $$PWD/include


INCLUDEPATH += $$CCXX_PATH
INCLUDEPATH += $$PROJECT_INCLUDE_PATH


### path
#* var
#win32:!wince*:
#mac:!qpa:
#embedded:
#symbian:
#irix-cc*:
#unix|win32-g++*:
#x11:
#win32-msvc*|win32-icc:
#solaris-cc*:
#wince*:
win32 {
    CONFIG(debug, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/win32/ccxx_debug
    CONFIG(release, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/win32/ccxx_release
    PROJECT_LIB_PATH = $$PWD/lib/win32
}
win64 {
    CONFIG(debug, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/win64/ccxx_debug
    CONFIG(release, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/win64/ccxx_release
    PROJECT_LIB_PATH = $$PWD/lib/win64
}
unix {
    CONFIG(debug, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/unix/ccxx_debug
    CONFIG(release, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/unix/ccxx_release
    PROJECT_LIB_PATH = $$PWD/lib/unix
}
x11 {
    CONFIG(debug, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/x11/ccxx_debug
    CONFIG(release, debug|release) : PROJECT_BIN_PATH = $$PWD/../deploy/x11/ccxx_release
    PROJECT_LIB_PATH = $$PWD/lib/xll
}


#### project destdir ###

DESTDIR = $$PROJECT_BIN_PATH
LIBS += -L$$PROJECT_BIN_PATH
LIBS += -L$$PROJECT_LIB_PATH

#contains(DEFINES, PROJECT_DATABASE_SQLITE) {
#LIBS += -lsqlite3odbcnw
#}


QT       -= core
QT       -= gui

#QMAKE_CXXFLAGS += -static-libgcc
QMAKE_LFLAGS   += -static-libgcc
QMAKE_LFLAGS   += -static-libstdc++

QMAKE_LFLAGS += -Wno-unused-parameter
QMAKE_LFLAGS += -Wunused-variable
QMAKE_LFLAGS += -Wsign-compare

QMAKE_CXXFLAGS += -static-libgcc
QMAKE_CXXFLAGS += -static-libstdc++

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wunused-variable
QMAKE_CXXFLAGS += -Wsign-compare

#1):define project template

#PROJECT_TEMPLATE_APP_GUI
#PROJECT_TEMPLATE_APP_CONSOLE
#PROJECT_TEMPLATE_LIB_DLL
#PROJECT_TEMPLATE_LIB_STATIC

! contains(DEFINES, PROJECT_TEMPLATE_APP_GUI) : ! contains(DEFINES, PROJECT_TEMPLATE_APP_CONSOLE) : ! contains(DEFINES, PROJECT_TEMPLATE_LIB_DLL) : ! contains(DEFINES, PROJECT_TEMPLATE_LIB_STATIC) {
DEFINES += PROJECT_TEMPLATE_APP_CONSOLE
}

#2):define process

### project template ###

contains(DEFINES, PROJECT_TEMPLATE_APP_GUI) {
TEMPLATE = app
DEFINES += PROJECT_3RDPARTY_QT
}

contains(DEFINES, PROJECT_TEMPLATE_APP_CONSOLE) {
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
}

contains(DEFINES, PROJECT_TEMPLATE_LIB_DLL) {
TEMPLATE = lib
CONFIG += dll
}

contains(DEFINES, PROJECT_TEMPLATE_LIB_STATIC) {
TEMPLATE = lib
CONFIG += staticlib
}


CONFIG(debug, debug|release):DEFINES += GM_DEBUG


### project 3rdparty qt ###

contains(DEFINES, PROJECT_3RDPARTY_QT)
{
    QT += core
    QT += gui
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
    CODECFORTR = UTF-8
    CODECFORSRC = UTF-8
}


include($$PWD/ccxx/ccxx.pri)
