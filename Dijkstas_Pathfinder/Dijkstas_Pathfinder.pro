#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T21:01:36
#
#-------------------------------------------------

QT       -= gui

TARGET = Dijkstas_Pathfinder
TEMPLATE = lib

DEFINES += DIJKSTAS_PATHFINDER_LIBRARY

SOURCES += dijkstas_pathfinder.cpp

HEADERS += dijkstas_pathfinder.h\
        dijkstas_pathfinder_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
