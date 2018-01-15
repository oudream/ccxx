#
include($$PWD/../../environment.pri)
#
include($$CCXX_TEST_PATH/ucommon/ucommon.pri)

TARGET = ccxx_test_ucommon


QT       -= core
QT       -= gui


TARGET = ucommon


QMAKE_LFLAGS   += -static-libgcc


TEMPLATE = lib
CONFIG += staticlib


#INCLUDEPATH += $$PWD/../include

#DESTDIR = $$PWD/../lib


#win32: LIBS += -lwsock32 -lws2_32 -lodbc32

