
#应用程序类型
DEFINES += PROJECT_TEMPLATE_APP_GUI
#DEFINES += GM_TRACK
#PROJECT_TARGET_DEPLOY = 1

#
include($$PWD/../../environment.pri)
#
include($$CCXX_PATH/ccxx/ccxx_qt.pri)
#
include($$CCXX_SAMPLE_PATH/qt/qt.pri)

TARGET = example_ccxx_qt

LIBS += -llibccxx1
#LIBS += -llibccxx_lua1
LIBS += -llibccxx_database_odbc1
LIBS += -llibccxx_database_sqlite1

RC_FILE = $$PWD/qt.rc

