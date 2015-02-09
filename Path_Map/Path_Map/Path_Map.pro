#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T19:40:38
#
#-------------------------------------------------

QT       -= gui

TARGET = Path_Map
TEMPLATE = lib

DEFINES += PATH_MAP_LIBRARY

SOURCES += path_map.cpp

HEADERS += path_map.h\
        path_map_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
