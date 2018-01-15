
include($$PWD/../../environment.pri)

TARGET = cxKits

SOURCES += \
    $$CCXX_TOOLS_PATH/kits/psmstructcode.cpp \
#    $$CCXX_TOOLS_PATH/process_protector/main.cpp \
#    $$CCXX_TOOLS_PATH/process_protector/gb2312code.cpp \

LIBS += -llibccxx1
