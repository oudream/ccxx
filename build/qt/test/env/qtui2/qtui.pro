
DEFINES += PROJECT_TEMPLATE_APP_GUI

include($$PWD/../../ccxx/ccxx_environment.pri)

TARGET = cxtest_channel

SOURCES += \
    main.cpp \
#    testTcpclient1.cpp \
#    testUdp1.cpp \
#    testSerail1.cpp \
#    testServer1.cpp \
#    testSerial0.cpp \
    test_channel_win.cpp \
    serial_config_win.cpp

FORMS += \
    test_channel_win.ui \
    serial_config_win.ui

HEADERS += \
    test_channel_win.h \
    serial_config_win.h
