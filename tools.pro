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

SOURCES +=           \
    qtools.cpp       \
    cvtools.cpp      \
    localassert.cpp  \
    qlocalassert.cpp \
    randmaster.cpp   \
    interval.cpp     \
    persist.cpp      \
    utilities.cpp \
    convert.cpp

HEADERS +=           \
    qtools.h         \
    cvtools.h        \
    tools_global.h   \
    localassert.h    \
    qlocalassert.h   \
    randmaster.h     \
    interval.h       \
    persist.h        \
    utilities.hpp    \
    dbrep.hpp \
    convert.hpp
