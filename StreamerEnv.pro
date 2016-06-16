#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T10:46:34
#
#-------------------------------------------------

QT       += core gui concurrent

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StreamerEnv
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    library.cpp \
    entities.cpp \
    singleiterationemulate.cpp \
    streamerenv.cpp \
    streamerview.cpp

HEADERS  += mainwindow.h \
    library.h \
    entities.h \
    singleiterationemulate.h \
    streamerenv.h \
    streamerview.h

FORMS    += mainwindow.ui \
    singleiterationemulate.ui \
    streamerenv.ui \
    streamerview.ui

RESOURCES += \
    res.qrc
