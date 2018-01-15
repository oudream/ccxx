
DEFINES += PROJECT_TEMPLATE_LIB_DLL

#
include($$PWD/../../environment.pri)

TARGET = libccxx_database_odbc

DEFINES += GM_BUILD_CCXX_ODBC_LIB

#
include($$CCXX_PATH/ccxx/ccxx_odbc.pri)

LIBS += -llibccxx1
