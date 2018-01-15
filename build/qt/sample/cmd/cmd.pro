
#
include($$PWD/../../environment.pri)
#
include($$CCXX_SAMPLE_PATH/cmd/cmd.pri)

TARGET = example_ccxx_cmd

LIBS += -llibccxx1
#LIBS += -llibccxx_lua1
LIBS += -llibccxx_database_odbc1
LIBS += -llibccxx_database_sqlite1

