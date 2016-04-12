#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T13:39:45
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 595-mosquito
TEMPLATE = app


SOURCES += main.cpp\
    glwidget.cpp \
    window.cpp \
    mosquito.cpp \
    helper.cpp \
    light.cpp \
    board.cpp \
    wall.cpp \
    frog.cpp \
    player.cpp \
    myplayer.cpp

HEADERS  += \
    glwidget.h \
    window.h \
    mosquito.h \
    helper.h \
    light.h \
    board.h \
    wall.h \
    frog.h \
    player.h \
    myplayer.h

FORMS    += \
    mainwindow.ui

INCLUDEPATH += include

DISTFILES += \
    wall_setup.txt
