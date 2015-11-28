
DEFINES += PROJECT_DATABASE_SQLITE
#DEFINES += PROJECT_DATABASE_ORACLE

include($$PWD/../../ccxx_qt_projects.pri)

TARGET = cxtest_database

SOURCES += \
    main.cpp \
    test_database1.cpp \
    test_database2.cpp \
#    test_database3.cpp \
#    test_database4.cpp \
#    test_odbc_oracle.cpp \
#    test_odbc_access.cpp \
#    test_odbc_mysql.cpp
