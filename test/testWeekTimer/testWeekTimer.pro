CONFIG += qtestlib debug
TEMPLATE = app
TARGET =
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestWeekTimer.cpp

# Code to test
DEPENDPATH  += ../../src/
INCLUDEPATH += ../../src/

SOURCES += WeekTimer.cpp
SOURCES += WeekTimerLine.cpp
