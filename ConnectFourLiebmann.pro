#-------------------------------------------------
#
# Project created by QtCreator 2013-02-14T19:49:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConnectFourLiebmann
TEMPLATE = app


SOURCES += main.cpp\
        connectfourliebmann.cpp \
    connectfourwidget.cpp \
    graphicobjects.cpp \
    graphicpart.cpp \
    connectfourgame.cpp \
    gameplayer.cpp

HEADERS  += connectfourliebmann.h \
    connectfourwidget.h \
    collections.h \
    settings.h \
    graphicobjects.h \
    graphicpart.h \
    connectfourgame.h \
    gameplayer.h

QT           += opengl
