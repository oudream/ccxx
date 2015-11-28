
include($$PWD/../../ccxx_qt_projects.pri)

TARGET = cxtest_socket

SOURCES += \
    main.cpp \
#    test_socket001.cpp \
#    test_socket002.cpp

win32: LIBS += -lwsock32 -lws2_32
unix: LIBS += -ldl -lrt
unix: DEFINES += _FILe_OFFSET_BITS=64

QMAKE_LFLAGS   += -static-libgcc
QMAKE_LFLAGS   += -static-libstdc++
QMAKE_CXXFLAGS += -Wno-unused-parameter


#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

#SOURCES += toolTcpClient1.cpp \

#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

#SOURCES += toolTcpServer1.cpp \
