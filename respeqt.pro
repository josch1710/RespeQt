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
VERSION = 5.4
RC_VERSION = "RC3"
DEFINES += VERSION=\\\"v$$VERSION_$$RC_VERSION\\\"

#debug {
  #QMAKE_CXXFLAGS+="-fsanitize=address -fno-omit-frame-pointer"
  #QMAKE_LFLAGS+="-fsanitize=address"
#}

SOURCES += src/main.cpp
TARGET = RespeQt
TEMPLATE = app

CONFIG += qt c++11 strict_c++
QT += core gui network widgets printsupport serialport svg
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib include headers tools

# Warnings for Deprecated functions
DEFINES += QT_DEPRECATED_WARNINGS
# Errors for QT4 deprecated functions
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000
# No unknown pragmas
QMAKE_CXXFLAGS += -Wno-unknown-pragmas

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

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.7
    bundle.files = \
        i18n/respeqt_de.qm \
        i18n/respeqt_es.qm \
        i18n/respeqt_pl.qm \
        i18n/respeqt_ru.qm \
        i18n/respeqt_sk.qm \
        i18n/respeqt_tr.qm
    bundle.path = Contents/Resources
    QMAKE_BUNDLE_DATA += bundle

    deploy.commands = $$PWD/deployment/macdeploy.sh $$PWD $$OUT_PWD "$$VERSION" "$$RC_VERSION"
    QMAKE_EXTRA_TARGETS += deploy
}
win32 {
    deploy.commands = $$PWD/deployment/windeploy.bat "$$OUT_PWD/release/$$TARGET" "$$QMAKE_QMAKE" "$$PWD" "$${VERSION}_$${RC_VERSION}"
    QMAKE_EXTRA_TARGETS += deploy
}
linux {
    deploy.commands = $$PWD/deployment/lindeploy.sh "$$OUT_PWD/$$TARGET" "$$QMAKE_QMAKE" "$$PWD" "$${VERSION}$${RC_VERSION}"
    QMAKE_EXTRA_TARGETS += deploy
}

DISTFILES += \
    deployment/lindeploy.sh
