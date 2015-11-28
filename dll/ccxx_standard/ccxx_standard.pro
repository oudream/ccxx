
DEFINES += PROJECT_TEMPLATE_LIB_DLL
#DEFINES += PROJECT_TEMPLATE_LIB_STATIC
#DEFINES += GCL_NO_CCXX

TARGET = ccxx

include($$PWD/../../ccxx_qt_projects.pri)

include($$CCXX_PATH/ccxx/ccxx.pri)
#include($$CCXX_PATH/ccxx/ccxx_oci.pri)
include($$CCXX_PATH/ccxx/regexp/regexp.pri)
include($$CCXX_PATH/ccxx/fastcgi/fastcgi.pri)



