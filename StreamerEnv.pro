#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T10:46:34
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StreamerEnv
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    viewerdesc.cpp \
    library.cpp

HEADERS  += mainwindow.h \
    viewerdesc.h \
    library.h

FORMS    += mainwindow.ui
