
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/fastcgi.h \
    $$PWD/fcgi_config.h \
#    $$PWD/fcgi_config_x86.h \
    $$PWD/fcgi_stdio.h \
    $$PWD/fcgiapp.h \
    $$PWD/fcgimisc.h \
    $$PWD/fcgio.h \
    $$PWD/fcgios.h

SOURCES += \
    $$PWD/fcgio.cpp \
    $$PWD/fcgi_stdio.c \
    $$PWD/fcgiapp.c \

win32 {
    SOURCES += \
        $$PWD/os_win32.c
}
#unix {
#    SOURCES += \
#        $$PWD/os_unix.c
#}

