
include($$PWD/../../ccxx/ccxx_environment.pri)

INCLUDEPATH += $$PWD/../../include/oci
LIBS += -L$$PWD/../../lib/win32/oci -loci

TARGET = test_otl

SOURCES += \
    main.cpp \
#    test_otl1.cpp \
#    test_otl2.cpp \
