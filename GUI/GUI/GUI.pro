#-------------------------------------------------
#
# Project created by QtCreator 2015-02-07T01:53:06
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    schedule_dialog.cpp \
    train_dialog.cpp \
    path_map.cpp \
    algorithm.cpp \
    sql.cpp

HEADERS  += mainwindow.h \
    schedule_dialog.h \
    train_dialog.h \
    path_map.h

FORMS    += mainwindow.ui \
    schedule_dialog.ui \
    train_dialog.ui
