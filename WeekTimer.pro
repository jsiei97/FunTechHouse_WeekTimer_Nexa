TEMPLATE = app
QT += sql
TARGET =

DEPENDPATH  += src
INCLUDEPATH += src

LIBS += -lmosquittopp -lmosquitto

target.path = /usr/local/bin/
INSTALLS += target

# Input
SOURCES += main.cpp
SOURCES += WeekTimer.cpp
SOURCES += WeekTimerLine.cpp
SOURCES += MosqConnect.cpp
SOURCES += UnixTime.cpp
SOURCES += SQLiteWrapper.cpp

# The submodules
include($$PWD/mod/qpiGPIO/mod.pri)
