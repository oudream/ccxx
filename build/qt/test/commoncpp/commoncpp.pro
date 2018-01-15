
#
include($$PWD/../../environment.pri)
#
include($$CCXX_TEST_PATH/commoncpp/commoncpp.pri)

TARGET = ccxx_test_commoncpp

QT       -= core
QT       -= gui


QMAKE_LFLAGS   += -static-libgcc


TEMPLATE = lib
CONFIG += staticlib


#INCLUDEPATH += $$PWD/../include

#DESTDIR = $$PWD/../lib


#win32: LIBS += -l$$DESTDIR
win32: LIBS += -lwsock32 -lws2_32 -lodbc32 -Lccxx_test_ucommon

