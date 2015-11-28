#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt


#QMAKE_LFLAGS   += -static-libgcc
#QMAKE_LFLAGS   += -static-libstdc++


#DESTDIR = $$PWD/../bin


#SOURCES += main.cpp \

include($$PWD/../../ccxx_qt_projects.pri)

TARGET = cxtest_fastcgi

SOURCES += \
    main.cpp \
