
include($$PWD/../../environment.pri)

TARGET = iec104client

SOURCES += \
    $$CCXX_TOOLS_PATH/iec104client/main.cpp \
    $$CCXX_TOOLS_PATH/iec104client/iec104protocol.cpp \

HEADERS += \
    $$CCXX_TOOLS_PATH/iec104client/iec104protocol.h \
