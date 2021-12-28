CONFIG += qtestlib debug
TEMPLATE = app
TARGET = testWeekTimer
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestWeekTimer.cpp

# Code to test
DEPENDPATH  += ../../src/
INCLUDEPATH += ../../src/

SOURCES += ../../src/WeekTimer.cpp
SOURCES += ../../src/WeekTimerLine.cpp

# Stubs
SOURCES += stub/UnixTime.cpp
HEADERS += stub/UnixTime.h
