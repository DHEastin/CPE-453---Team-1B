#-------------------------------------------------
#
# Project created by QtCreator 2015-02-07T18:36:19
#
#-------------------------------------------------

QT       -= gui

TARGET = Map_Object
TEMPLATE = lib

DEFINES += MAP_OBJECT_LIBRARY

SOURCES += map_object.cpp

HEADERS += map_object.h\
        map_object_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
