CONFIG += qtestlib debug
TEMPLATE = app
QT += sql
TARGET =
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestSQLiteWrapper.cpp

# Code to test
DEPENDPATH  += ../../src/
INCLUDEPATH += ../../src/

SOURCES += SQLiteWrapper.cpp

# Stubs
SOURCES += stub/UnixTime.cpp
HEADERS += stub/UnixTime.h
