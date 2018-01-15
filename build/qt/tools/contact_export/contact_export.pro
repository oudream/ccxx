
include($$PWD/../../environment.pri)

TARGET = contact_export

SOURCES += \
    $$CCXX_TOOLS_PATH/contact_export/main.cpp \

LIBS += -llibccxx1
