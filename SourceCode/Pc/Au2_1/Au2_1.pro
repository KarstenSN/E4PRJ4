#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T15:36:21
#
#-------------------------------------------------

QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Au2_1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    xboxcontroller.h

FORMS    += mainwindow.ui

LIBS     += -LC:\Qt\libvlc-qt\lib -lvlc-qt -lvlc-qt-widgets -pthread -lXinput9_1_0
INCLUDEPATH += C:\Qt\libvlc-qt\include\

win32: RC_FILE = Au2.rc




