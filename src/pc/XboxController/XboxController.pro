#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T10:38:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = XboxController
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

HEADERS += \
    xboxcontroller.h

# Add following line to get XInput9_1_0 library to work witht the apllication
unix|win32: LIBS += -lXinput9_1_0
