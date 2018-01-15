
HEADERS += \
        $$PWD/cxglobal.h                     \
        $$PWD/cxstring.h                     \
        $$PWD/cxtime.h                       \
        $$PWD/cxfile.h                       \
        $$PWD/cxappenv.h                     \
        $$PWD/cxprofile.h                    \
        $$PWD/cxjson.h                       \
        $$PWD/cxprofile_skv_json.h           \
        $$PWD/cxthread.h                     \
        $$PWD/cxinterinfo.h                  \
        $$PWD/cxlog.h                        \
        $$PWD/cxsystem.h                     \
        $$PWD/cxcontainer.h                  \
        $$PWD/cxmath.h                       \
        $$PWD/cxbuffer.h                     \
        $$PWD/cxcrc.h                        \
        $$PWD/cxxml.h                        \
        $$PWD/cxprofile_skv_xml.h            \
        $$PWD/cxsocket.h                     \
        $$PWD/cxprocess.h                    \
        $$PWD/cxdatabase.h                   \
        $$PWD/cxencoding.h                   \
        $$PWD/cxuuid.h                       \
        $$PWD/cxprocmutex.h                  \
        $$PWD/cxwinreg.h                     \


SOURCES += \
        $$PWD/cxglobal.cpp                   \
        $$PWD/cxstring.cpp                   \
        $$PWD/cxtime.cpp                     \
        $$PWD/cxfile.cpp                     \
        $$PWD/cxappenv.cpp                   \
        $$PWD/cxprofile.cpp                  \
        $$PWD/cxjson.cpp                     \
        $$PWD/cxprofile_skv_json.cpp         \
        $$PWD/cxprofile_factorymanager.cpp   \
        $$PWD/cxthread.cpp                   \
        $$PWD/cxinterinfo.cpp                \
        $$PWD/cxlog.cpp                      \
        $$PWD/cxsystem.cpp                   \
        $$PWD/cxmath.cpp                     \
        $$PWD/cxbuffer.cpp                   \
        $$PWD/cxcrc.cpp                      \
        $$PWD/cxxml.cpp                      \
        $$PWD/tinyxml/tinyxml.cpp            \
        $$PWD/tinyxml/tinyxmlerror.cpp       \
        $$PWD/tinyxml/tinyxmlparser.cpp      \
        $$PWD/cxprofile_skv_xml.cpp          \
        $$PWD/cxsocket.cpp                   \
        $$PWD/cxprocess.cpp                  \
        $$PWD/cxdatabase.cpp                 \
        $$PWD/cxencoding.cpp                 \
        $$PWD/cxuuid.cpp                     \
        $$PWD/cxprocmutex.cpp                \
        $$PWD/cxwinreg.cpp                   \

win32: LIBS += -lpsapi


DEFINES += GM_BUILD_CCXX_CORE_CODE
DEFINES += GM_PROJECT_CXAPPLICATION_NULL

