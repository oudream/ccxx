
win32: LIBS += -lwsock32 -lws2_32
unix: LIBS += -ldl -lrt
unix: DEFINES += _FILe_OFFSET_BITS=64

INCLUDEPATH += $$PWD

CONFIG(debug, debug|release) : DEFINES += DEBUG
CONFIG(release, debug|release) : DEFINES += NDEBUG


HEADERS += \
    $$PWD/readme.txt \
    $$PWD/cxglobal.h \
    $$PWD/cxsystem.h \
    $$PWD/cxapplication.h \
    $$PWD/cxcontainer.h \
    $$PWD/cxstring.h \
    $$PWD/cxmath.h \
    $$PWD/cxbuffer.h \
    $$PWD/cxpacket.h \
    $$PWD/cxcrc.h \
    $$PWD/cxfile.h \
    $$PWD/cxthread.h \
    $$PWD/cxtime.h \
    $$PWD/cxtimer.h \
    $$PWD/cxfactory.h \
    $$PWD/cxvalue.h \
    $$PWD/cxlog.h \
    $$PWD/cxinterinfo.h \
    $$PWD/cxcontext.h \
    $$PWD/cxdescribe.h \
    $$PWD/cxprofile.h \
    $$PWD/cxxml.h \
    $$PWD/cxefile.h \
    $$PWD/cxsharememory.h \
    $$PWD/cxsocket.h \
    $$PWD/cxbitmap.h \
    $$PWD/cxprocess.h \
    $$PWD/cxprofile_skv.h \
    $$PWD/cxprofile_skv_txt.h \
    $$PWD/cxprofile_skv_ini.h \
    $$PWD/cxprofile_skv_xml.h \
    $$PWD/cxprofile_loo.h \
    $$PWD/cxchannel_global.h \
    $$PWD/cxchannel.h \
    $$PWD/cxchannel_tcpclient.h \
    $$PWD/cxchannel_tcpserver.h \
    $$PWD/cxchannel_udp.h \
    $$PWD/cxchannel_serial.h \
    $$PWD/ccxx.h \
    $$PWD/cxsafequeue.h \
    $$PWD/cxsafethread.h \
    $$PWD/cxsafebuffer.h \
    $$PWD/cxmacro.h \
    $$PWD/cxdatabase.h \
    $$PWD/sqlite3.h \
    $$PWD/sqlite3ext.h \
    $$PWD/cxbusiness.h \
    $$PWD/cxencoding.h \
    $$PWD/cxresource.h \
    $$PWD/cxjson.h \

SOURCES += \
    $$PWD/cxglobal.cpp \
    $$PWD/cxsystem.cpp \
    $$PWD/cxapplication.cpp \
    $$PWD/cxcontainer.cpp \
    $$PWD/cxstring.cpp \
    $$PWD/cxmath.cpp \
    $$PWD/cxbuffer.cpp \
    $$PWD/cxpacket.cpp \
    $$PWD/cxcrc.cpp \
    $$PWD/cxfile.cpp \
    $$PWD/cxthread.cpp \
    $$PWD/cxtime.cpp \
    $$PWD/cxtimer.cpp \
    $$PWD/cxfactory.cpp \
    $$PWD/cxvalue.cpp \
    $$PWD/cxlog.cpp \
    $$PWD/cxcontext.cpp \
    $$PWD/cxdescribe.cpp \
    $$PWD/cxinterinfo.cpp \
    $$PWD/cxprofile.cpp \
    $$PWD/tinyxml.cpp \
    $$PWD/tinyxmlerror.cpp \
    $$PWD/tinyxmlparser.cpp \
    $$PWD/cxxml.cpp \
    $$PWD/cxefile.cpp \
    $$PWD/cxsharememory.cpp \
    $$PWD/cxsocket.cpp \
    $$PWD/cxbitmap.cpp \
    $$PWD/cxprocess.cpp \
    $$PWD/cxprofile_skv.cpp \
    $$PWD/cxprofile_skv_txt.cpp \
    $$PWD/cxprofile_skv_ini.cpp \
    $$PWD/cxprofile_skv_xml.cpp \
    $$PWD/cxprofile_loo.cpp \
    $$PWD/cxchannel.cpp \
    $$PWD/cxchannel_tcpclient.cpp \
    $$PWD/cxchannel_tcpserver.cpp \
    $$PWD/cxchannel_udp.cpp \
    $$PWD/cxchannel_serial.cpp \
    $$PWD/cxsafequeue.cpp \
    $$PWD/cxsafethread.cpp \
    $$PWD/cxsafebuffer.cpp \
    $$PWD/cxdatabase.cpp \
    $$PWD/cxdatabase_sqlite.cpp \
    $$PWD/cxdatabase_odbc.cpp \
    $$PWD/sqlite3.c \
    $$PWD/cxbusiness.cpp \
    $$PWD/cxencoding.cpp \
    $$PWD/cxchannel_factorymanager.cpp \
    $$PWD/cxprofile_factorymanager.cpp \
    $$PWD/cxdescribe_factorymanager.cpp \
    $$PWD/cxresource.cpp \
    $$PWD/cxjson.cpp \

