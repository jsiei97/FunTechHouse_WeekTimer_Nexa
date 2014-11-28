TEMPLATE = app
TARGET =

DEPENDPATH  += . src
INCLUDEPATH += . src

LIBS += -ltelldus-core
LIBS += -lmosquittopp -lmosquitto

target.path = /usr/local/bin/
INSTALLS += target

# Input
SOURCES += main.cpp

SOURCES += WeekTimer.cpp
HEADERS += WeekTimer.h

SOURCES += WeekTimerLine.cpp
HEADERS += WeekTimerLine.h

SOURCES += MosqConnect.cpp
HEADERS += MosqConnect.h

SOURCES += UnixTime.cpp
HEADERS += UnixTime.h
