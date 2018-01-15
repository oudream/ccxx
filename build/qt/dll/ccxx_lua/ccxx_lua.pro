
DEFINES += PROJECT_TEMPLATE_LIB_DLL
#DEFINES += PROJECT_TEMPLATE_LIB_STATIC
CONFIG(debug, debug|release) : DEFINES += LPEG_DEBUG

#
include($$PWD/../../environment.pri)
#
include($$GCL_SRC_PATH/script/script.pri)
#
include($$GCL_SRC_PATH/3party/lua5.3.0/src/lua.pri)

LIBS += -llibccxx1

