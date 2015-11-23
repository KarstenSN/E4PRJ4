#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T22:06:46
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = MultiServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Au2Server.cpp \
    Au2Thread.cpp

HEADERS += \
    Au2Server.h \
    Au2Thread.h
