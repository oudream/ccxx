
DEFINES += PROJECT_TEMPLATE_LIB_DLL
#DEFINES += PROJECT_TEMPLATE_LIB_STATIC
#DEFINES += GCL_NO_CCXX
DEFINES += CCXX_QT

TARGET = ccxx

include($$PWD/../../ccxx_qt_projects.pri)

include($$CCXX_PATH/ccxx/ccxx.pri)
#include($$CCXX_PATH/ccxx/ccxx_oci.pri)
#include($$CCXX_PATH/ccxx/regexp/regexp.pri)

HEADERS += \
    $$CCXX_PATH/ccxx/qtcommon.h \

SOURCES += \
    $$CCXX_PATH/ccxx/qtcommon.cpp \

