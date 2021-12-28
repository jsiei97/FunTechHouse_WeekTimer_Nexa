TEMPLATE = app
QT += sql
TARGET = WeekTimerGPIO

DEPENDPATH  += src
INCLUDEPATH += src

LIBS += -lmosquittopp -lmosquitto

target.path = /usr/local/bin/
INSTALLS += target

# Input
HEADERS += src/MosqConnect.h \
           src/SQLiteWrapper.h \
           src/UnixTime.h \
           src/WeekTimer.h \
           src/WeekTimerLine.h

SOURCES += src/main.cpp \
           src/MosqConnect.cpp \
           src/SQLiteWrapper.cpp \
           src/UnixTime.cpp \
           src/WeekTimer.cpp \
           src/WeekTimerLine.cpp

# The submodules
include($$PWD/mod/qpiGPIO/mod.pri)
