#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T14:03:31
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = power_acoustic
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp \
    udpchange.cpp \
    commandline.cpp \
    mad.cpp \
    collectordatabag.cpp \
    bag.cpp

HEADERS += \
    udpchange.h \
    commandline.h \
    mad.h \
    command_for_bag.h \
    collectordatabag.h \
    bag.h
