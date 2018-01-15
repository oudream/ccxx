
DEFINES += PROJECT_TEMPLATE_APP_GUI

#
include($$PWD/../../environment.pri)
#
include($$CCXX_PATH/ccxx/ccxx_qt.pri)
#
include($$CCXX_TEST_PATH/channel/channel.pri)

TARGET = ccxx_test_channel

LIBS += -llibccxx1
