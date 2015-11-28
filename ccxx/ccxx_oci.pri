
INCLUDEPATH += $$PWD/../include/oci

win32:LIBS += -L$$PWD/../lib/win32/oci -loci

SOURCES += \
    $$PWD/cxdatabase_oci.cpp

