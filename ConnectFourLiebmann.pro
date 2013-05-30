#-------------------------------------------------
#
# Project created by QtCreator 2013-02-14T19:49:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConnectFourLiebmann
TEMPLATE = app


SOURCES += main.cpp\
        connectfourliebmann.cpp \
    connectfourwidget.cpp \
    graphicobjects.cpp \
    graphicpart.cpp \
    connectfourgame.cpp \
    gameplayer.cpp \
    settings.cpp \
    netcommand.cpp \
    connectfoursettings.cpp \
    netclientcom.cpp \
    netservercom.cpp \
    connectfourmanagement.cpp

HEADERS  += connectfourliebmann.h \
    connectfourwidget.h \
    collections.h \
    settings.h \
    graphicobjects.h \
    graphicpart.h \
    connectfourgame.h \
    gameplayer.h \
    netcommand.h \
    connectfoursettings.h \
    netclientcom.h \
    netservercom.h \
    connectfourmanagement.h

QT           += opengl

OTHER_FILES += \
    README.txt

RESOURCES += \
    images.qrc

QMAKE_CXXFLAGS += -std=c++11
