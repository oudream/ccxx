#QT       -= core
#QT       -= gui


#QMAKE_LFLAGS   += -static-libgcc


#TEMPLATE = lib
#CONFIG += staticlib


#INCLUDEPATH += $$PWD/../include

#DESTDIR = $$PWD/../lib


#win32: LIBS += -l$$DESTDIR
#win32: LIBS += -lwsock32 -lws2_32 -lodbc32 -Lucommon


SOURCES += \
    $$PWD/udp.cpp \
    $$PWD/tokenizer.cpp \
    $$PWD/thread.cpp \
    $$PWD/tcp.cpp \
    $$PWD/strchar.cpp \
    $$PWD/socket.cpp \
    $$PWD/slog.cpp \
    $$PWD/serial.cpp \
    $$PWD/process.cpp \
    $$PWD/pointer.cpp \
    $$PWD/mime.cpp \
    $$PWD/map.cpp \
    $$PWD/linked.cpp \
    $$PWD/file.cpp \
    $$PWD/exception.cpp \
    $$PWD/dso.cpp \
    $$PWD/dccp.cpp \
    $$PWD/applog.cpp \
    $$PWD/address.cpp

