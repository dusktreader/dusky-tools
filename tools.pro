#-------------------------------------------------
#
# Project created by QtCreator 2010-09-03T13:33:44
#
#-------------------------------------------------

QT -= gui
QT += xml

TARGET = tools
TEMPLATE = lib

DEFINES += TOOLS_LIBRARY

SOURCES += \
           tools.cpp   \
           cvtools.cpp \
    localassert.cpp \
    randmaster.cpp \
    interval.cpp \

HEADERS += \
           tools.h        \
           cvtools.h      \
           tools_global.h \
    localassert.h \
    randmaster.h \
    interval.h \
    persist.hpp \
