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

VERSION = r5.3
DEFINES += VERSION=\\\"$$VERSION\\\"

#debug {
#  QMAKE_CXXFLAGS+="-fsanitize=thread -fno-omit-frame-pointer -stdlib=libc++"
#  QMAKE_LFLAGS+="-fsanitize=thread"
#}

test {
  SOURCES += tests/units/tests.cpp \
    tests/units/siorecordertest.cpp
  HEADERS += tests/units/siorecordertest.h \
    tests/units/nulloutput.h \
    tests/units/dummyworker.h
  INCLUDEPATH += tests
  TARGET = RespeQtTests
  CONFIG += console testcase
  CONFIG -= app_bundle
  QT += testlib

  # We want to have the testfiles in the build directory, so we have to this.
  copytestdata.commands = $(COPY_DIR) $$PWD/tests/units/testdata $$OUT_PWD
  first.depends = $(first) copytestdata
  export(first.depends)
  QMAKE_EXTRA_TARGETS += first copytestdata
  DEFINES += RESPEQT_TEST
}

!test {o
  SOURCES += main.cpp
  TARGET = RespeQt
  TEMPLATE = app
}

CONFIG += qt c++11 strict_c++
QT += core gui network widgets printsupport serialport svg
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib


# Warnings for Deprecated functions
DEFINES += QT_DEPRECATED_WARNINGS
# Errors for QT4 deprecated functions
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000

SOURCES += tests/siorecorder.cpp \
    mainwindow.cpp \
    printers/outputwindow.cpp \
    rcl.cpp \
    sdxprotocol.cpp \
    sioworker.cpp \
    optionsdialog.cpp \
    aboutdialog.cpp \
    diskimage.cpp \
    folderimage.cpp \
    createimagedialog.cpp \
    diskeditdialog.cpp \
    autoboot.cpp \
    autobootdialog.cpp \
    atarifilesystem.cpp \
    miscutils.cpp \
    printers/textprinterwindow.cpp \
    cassettedialog.cpp \
    docdisplaywindow.cpp \
    bootoptionsdialog.cpp \
    network.cpp \
    logdisplaydialog.cpp \
    respeqtsettings.cpp \
    pclink.cpp \
    printers/baseprinter.cpp \
    printers/atari1027.cpp \
    atascii.cpp \
    drivewidget.cpp \
    infowidget.cpp \
    atasciiinternational.cpp \
    printerwidget.cpp \
    printers/atariprinter.cpp \
    printers/atari1020.cpp \
    smartdevice.cpp \
    printers/nativeoutput.cpp \
    printers/centronics.cpp \
    printers/escp.cpp \
    printers/atari1029.cpp \
    diskimagepro.cpp \
    diskimageatx.cpp \
    diskimageatr.cpp \
    disassembly810.cpp \
    disassembly1050.cpp \
    printers/printerfactory.cpp \
    printers/atari1025.cpp \
    printers/passthrough.cpp \
    printers/graphicsprimitive.cpp \
    crc16.cpp \
    cpu6502.cpp

HEADERS += mainwindow.h \
    printers/outputwindow.h \
    printers/rawoutput.h \
    tests/siorecorder.h \
    tools/make_unique.h \
    printers/printers.h \
    rcl.h \
    sdxprotocol.h \
    serialport.h \
    sioworker.h \
    optionsdialog.h \
    aboutdialog.h \
    diskimage.h \
    folderimage.h \
    createimagedialog.h \
    diskeditdialog.h \
    autoboot.h \
    autobootdialog.h \
    atarifilesystem.h \
    miscutils.h \
    printers/textprinterwindow.h \
    cassettedialog.h \
    docdisplaywindow.h \
    bootoptionsdialog.h \
    network.h \
    logdisplaydialog.h \
    respeqtsettings.h \
    pclink.h \
    printers/baseprinter.h \
    printers/atari1027.h \
    atascii.h \
    drivewidget.h \
    infowidget.h \
    printerwidget.h \
    atasciiinternational.h \
    printers/atariprinter.h \
    printers/atari1020.h \
    printers/nativeoutput.h \
    printers/centronics.h \
    printers/escp.h \
    printers/atari1029.h \
    disassembly810.h \
    disassembly1050.h \
    printers/printerfactory.h \
    printers/atari1025.h \
    printers/passthrough.h \
    smartdevice.h  \
    printers/graphicsprimitive.h \
    crc16.h \
    cpu6502.h

FORMS += mainwindow.ui \
    optionsdialog.ui \
    aboutdialog.ui \
    createimagedialog.ui \
    diskeditdialog.ui \
    autobootdialog.ui \
    cassettedialog.ui \
    docdisplaywindow.ui \
    bootoptionsdialog.ui \
    logdisplaydialog.ui \
    drivewidget.ui \
    infowidget.ui \
    printers/outputwindow.ui \
    printerwidget.ui \
    printers/textprinterwindow.ui

RESOURCES += icons.qrc \
    atarifiles.qrc \
    i18n.qrc \
    documentation.qrc \
    fonts.qrc

OTHER_FILES += \
    license.txt \
    history.txt \
    atascii_read_me.txt \
    RespeQt.rc \
    about.html \
    compile.txt

# To update translations:
# lupdate respeqt.pro; lrelease i18n/respeqt_*.ts
TRANSLATIONS = \
    i18n/respeqt_de.ts \
    i18n/respeqt_es.ts \
    i18n/respeqt_pl.ts \
    i18n/respeqt_ru.ts \
    i18n/respeqt_sk.ts \
    i18n/respeqt_tr.ts

RC_FILE = RespeQt.rc
ICON = RespeQt.icns

win32 {
    SOURCES += serialport-win32.cpp \
               printers/rawoutput_win.cpp
    HEADERS += serialport-win32.h
    LIBS += -lwinmm -lz -lwinspool
}

unix {
    SOURCES += serialport-unix.cpp \
               printers/rawoutput_cups.cpp
    HEADERS += serialport-unix.h
    LIBS += -lz -lcups

    #QMAKE_CXXFLAGS += -Werror
}

DISTFILES += \
    tests/units/USAGE.md \
    tests/units/testdata/writeSioCapture.json

