
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS   += -static-libgcc
QMAKE_LFLAGS   += -static-libstdc++

win32: LIBS += -lwsock32 -lws2_32 -lodbc32

TARGET = gcdaemon

DESTDIR = $$PWD/../bin

SOURCES += \
    common.cpp \
    main.cpp \

OTHER_FILES += \
    readme.txt

