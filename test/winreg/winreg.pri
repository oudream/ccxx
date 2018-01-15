
DEFINES += PROJECT_DATABASE_SQLITE
#DEFINES += PROJECT_DATABASE_ORACLE

include($$PWD/../../ccxx/ccxx_environment.pri)

TARGET = cxtest_database

SOURCES += \
    main.cpp \


LIBS += -llibccxx

