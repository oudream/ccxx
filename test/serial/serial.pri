TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD

SOURCES += \
    serial_example.cc \
    serial.cc \
    win.cc \
    list_ports_win.cc

HEADERS += \
    serial.h \
    v8stdint.h \
    win.h

