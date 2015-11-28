#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T09:55:20
#
#-------------------------------------------------

#QT       -= core gui

#TARGET = cmd
#TEMPLATE = lib

#DEFINES += CMD_LIBRARY

#SOURCES += cmd.cpp

#HEADERS += cmd.h

DEFINES += PROJECT_TEMPLATE_SUBDIRS

include($$PWD/../../ccxx_qt_projects.pri)

OTHER_FILES += \
    $$PWD/cmd.bat \
    $$PWD/cmd.sh \


unix {
    message("sh cmd.sh begin...")
    system(sudo sh cmd.sh $$PROJECT_BIN_PATH)
}

win32 | win64 {
    system(cmd cmd.bat $$PROJECT_BIN_PATH)
}
