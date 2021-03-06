QT       += testlib

QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

include(../src/src.pri)

INCLUDEPATH = $$PWD/../src/

HEADERS += TestImageProcessor.h \
    TestHeightMapMesh.h \
    TestLvlPlanMesh.h

SOURCES += main.cpp\
    TestImageProcessor.cpp \
    TestHeightMapMesh.cpp \
    TestLvlPlanMesh.cpp

RESOURCES = $$PWD/tests.qrc
