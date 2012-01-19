#-------------------------------------------------
#
# Project created by QtCreator 2012-01-09T18:56:36
#
#-------------------------------------------------

QT       += xml testlib

QT       -= gui

TARGET = tst_persistxml
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS +=    \
    persist.h \
    dummy.h   \
    localassert.h \
    dummyparent.h

SOURCES += \
    tst_persistxml.cpp \
    persist.cpp \
    dummy.cpp \
    localassert.cpp \
    dummyparent.cpp

