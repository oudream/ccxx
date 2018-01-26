
win32 {
    ## Windows common build here
    !contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -L$PWD/../lib/win32
        message("x86 build")
        ## Windows x86 (32bit) specific build here
    } else {
        LIBS += -L$PWD/../lib/win64
        message("x86_64 build")
        ## Windows x64 (64bit) specific build here
    }
}
win32: LIBS += -lwsock32 -lws2_32 -lpsapi
unix: LIBS += -ldl -lrt
unix: DEFINES += _FILe_OFFSET_BITS=64

INCLUDEPATH += $$PWD/..
INCLUDEPATH += $$PWD

CONFIG(debug, debug|release) : DEFINES += DEBUG
CONFIG(release, debug|release) : DEFINES += NDEBUG


HEADERS += \
    $$PWD/readme.txt \
    $$PWD/cxglobal.h \
    $$PWD/cxsystem.h \
    $$PWD/cxappenv.h \
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
    $$PWD/cxprofile_skv_xml.h \
    $$PWD/cxprofile_skv_json.h \
    $$PWD/cxchannel_global.h \
    $$PWD/cxchannel.h \
    $$PWD/cxchannel_none.h \
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
    $$PWD/cxbusiness.h \
    $$PWD/cxencoding.h \
    $$PWD/cxresource.h \
    $$PWD/cxjson.h \
    $$PWD/cxuuid.h \
    $$PWD/cxprocmutex.h \
    $$PWD/cxwinreg.h \

SOURCES += \
    $$PWD/cxglobal.cpp \
    $$PWD/cxsystem.cpp \
    $$PWD/cxappenv.cpp \
    $$PWD/cxapplication.cpp \
    $$PWD/cxstring.cpp \
    $$PWD/cxmath.cpp \
    $$PWD/cxbuffer.cpp \
    $$PWD/cxpacket.cpp \
    $$PWD/cxcrc.cpp \
    $$PWD/cxfile.cpp \
    $$PWD/cxthread.cpp \
    $$PWD/cxtime.cpp \
    $$PWD/cxtimer.cpp \
    $$PWD/cxlog.cpp \
    $$PWD/cxcontext.cpp \
    $$PWD/cxdescribe.cpp \
    $$PWD/cxinterinfo.cpp \
    $$PWD/cxprofile.cpp \
    $$PWD/tinyxml/tinyxml.cpp \
    $$PWD/tinyxml/tinyxmlerror.cpp \
    $$PWD/tinyxml/tinyxmlparser.cpp \
    $$PWD/cxxml.cpp \
    $$PWD/cxefile.cpp \
    $$PWD/cxsharememory.cpp \
    $$PWD/cxsocket.cpp \
    $$PWD/cxbitmap.cpp \
    $$PWD/cxprocess.cpp \
    $$PWD/cxprofile_skv_xml.cpp \
    $$PWD/cxprofile_skv_json.cpp \
    $$PWD/cxchannel.cpp \
    $$PWD/cxchannel_none.cpp \
    $$PWD/cxchannel_tcpclient.cpp \
    $$PWD/cxchannel_tcpserver.cpp \
    $$PWD/cxchannel_udp.cpp \
    $$PWD/cxchannel_serial.cpp \
    $$PWD/cxsafequeue.cpp \
    $$PWD/cxsafethread.cpp \
    $$PWD/cxsafebuffer.cpp \
    $$PWD/cxdatabase.cpp \
    $$PWD/cxbusiness.cpp \
    $$PWD/cxencoding.cpp \
    $$PWD/cxchannel_factorymanager.cpp \
    $$PWD/cxprofile_factorymanager.cpp \
    $$PWD/cxdescribe_factorymanager.cpp \
    $$PWD/cxresource.cpp \
    $$PWD/cxjson.cpp \
    $$PWD/cxuuid.cpp \
    $$PWD/cxprocmutex.cpp \
    $$PWD/cxwinreg.cpp \
