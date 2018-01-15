
DEFINES += PROJECT_TEMPLATE_LIB_DLL

#
include($$PWD/../../environment.pri)

TARGET = libccxx_database_sqlite

DEFINES += GM_BUILD_CCXX_SQLITE_LIB

#
include($$CCXX_PATH/ccxx/ccxx_sqlite3.pri)

LIBS += -llibccxx1
