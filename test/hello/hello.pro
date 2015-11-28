TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_LFLAGS   += -static-libgcc
QMAKE_LFLAGS   += -static-libstdc++


DESTDIR = $$PWD/../bin


SOURCES += main.cpp \
#    filesystem1.cpp \
#    filetest1.cpp

