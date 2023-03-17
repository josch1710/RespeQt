# -------------------------------------------------
# Project created by QtCreator 2009-11-22T14:13:00
# Last Update: Nov 29, 2014
# -------------------------------------------------
#
# Copyright 2015,2016 Joseph Zatarski
#
# This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
# However, the years for these copyrights are unfortunately unknown. If you
# know the specific year(s) please let the current maintainer know.
#
#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
VERSION = r5.4
RC_VERSION = "_RC1B1"
DEFINES += VERSION=\\\"$$VERSION$$RC_VERSION\\\"

#debug {
  #QMAKE_CXXFLAGS+="-fsanitize=address -fno-omit-frame-pointer"
  #QMAKE_LFLAGS+="-fsanitize=address"
#}

SOURCES += main.cpp
TARGET = RespeQt
TEMPLATE = app

CONFIG += qt c++11 strict_c++
QT += core gui network widgets printsupport serialport svg
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib

# Warnings for Deprecated functions
DEFINES += QT_DEPRECATED_WARNINGS
# Errors for QT4 deprecated functions
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000

include(sources.pri)

OTHER_FILES += \
    license.txt \
    history.txt \
    atascii_read_me.txt \
    RespeQt.rc \
    about.html \
    compile.html

win32 {
    LIBS += -lwinmm -lz -lwinspool
}

unix {
    LIBS += -lz -lcups
    #QMAKE_CXXFLAGS += -Werror
    CONFIG -= warn_on
    QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_WARN_ON -Wno-deprecated-copy
}

DISTFILES += \
    tests/units/USAGE.md \
    tests/units/testdata/writeSioCapture.json

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.7
    deploy.commands = $$PWD/deployment/macdeploy.sh $$PWD $$OUT_PWD "$$VERSION$$RC_VERSION"
    QMAKE_EXTRA_TARGETS += deploy
}
