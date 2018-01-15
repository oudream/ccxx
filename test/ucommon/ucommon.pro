QT       -= core
QT       -= gui


TARGET = ucommon


QMAKE_LFLAGS   += -static-libgcc


TEMPLATE = lib
CONFIG += staticlib


INCLUDEPATH += $$PWD/../include

DESTDIR = $$PWD/../lib


win32: LIBS += -lwsock32 -lws2_32 -lodbc32


include($$PWD/ucommon.pri)

