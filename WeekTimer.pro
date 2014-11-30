TEMPLATE = app
TARGET =

DEPENDPATH  += src
INCLUDEPATH += src

LIBS += -ltelldus-core
LIBS += -lmosquittopp -lmosquitto

target.path = /usr/local/bin/
INSTALLS += target

# Input
SOURCES += main.cpp
SOURCES += WeekTimer.cpp
SOURCES += WeekTimerLine.cpp
SOURCES += MosqConnect.cpp
SOURCES += UnixTime.cpp
