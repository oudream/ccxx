
DEFINES += PROJECT_DATABASE_SQLITE
#DEFINES += PROJECT_DATABASE_ORACLE

#
include($$PWD/../../environment.pri)
#
include($$CCXX_TEST_PATH/database/database.pri)

TARGET = ccxx_test_database

LIBS += -llibccxx1
LIBS += -llibccxx_database_odbc1
LIBS += -llibccxx_database_sqlite1
