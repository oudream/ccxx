#
include($$PWD/../../environment.pri)
#
include($$CCXX_TEST_PATH/regexp/regexp.pri)
#
include($$CCXX_PATH/ccxx/regexp/re2.pri)

TARGET = ccxx_test_regexp

LIBS += -llibccxx1

