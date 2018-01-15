
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS   += -static-libgcc
QMAKE_LFLAGS   += -static-libstdc++

win32: LIBS += -lwsock32 -lws2_32 -lodbc32

DESTDIR = $$PWD/../bin

SOURCES += \
    $$PWD/../../../ccxx_tools/daemon/common.cpp \
    $$PWD/../../../ccxx_tools/daemon/main.cpp \

OTHER_FILES += \
    readme.txt

#TARGET = gcdaemon
TARGET = daemon
