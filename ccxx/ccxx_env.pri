
HEADERS += \
        $$PWD/cxglobal.h                     \
        $$PWD/cxstring.h                     \
        $$PWD/cxtime.h                       \
        $$PWD/cxfile.h                       \
        $$PWD/cxappenv.h                     \
        $$PWD/cxprofile.h                    \
        $$PWD/cxrapidjson.h                       \
        $$PWD/cxprofile_skv_json.h           \

SOURCES += \
        $$PWD/cxglobal.cpp                   \
        $$PWD/cxstring.cpp                   \
        $$PWD/cxtime.cpp                     \
        $$PWD/cxfile.cpp                     \
        $$PWD/cxappenv.cpp                   \
        $$PWD/cxprofile.cpp                  \
        $$PWD/cxrapidjson.cpp                     \
        $$PWD/cxprofile_skv_json.cpp         \
        $$PWD/cxprofile_factorymanager.cpp   \

DEFINES += GM_BUILD_CCXX_CORE_CODE
DEFINES += GM_PROJECT_XML_NULL
