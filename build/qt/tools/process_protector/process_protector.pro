
include($$PWD/../../environment.pri)

TARGET = process_protector

SOURCES += \
    $$CCXX_TOOLS_PATH/process_protector/main.cpp \

LIBS += -llibccxx1
