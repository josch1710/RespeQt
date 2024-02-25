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

SOURCES += \
    src/siorecorder.cpp \
    src/mainwindow.cpp \
    src/printers/outputwindow.cpp \
    src/rcl.cpp \
    src/sdxprotocol.cpp \
    src/sioworker.cpp \
    src/optionsdialog.cpp \
    src/aboutdialog.cpp \
    src/createimagedialog.cpp \
    src/diskeditdialog.cpp \
    src/autoboot.cpp \
    src/autobootdialog.cpp \
    src/miscutils.cpp \
    src/printers/textprinterwindow.cpp \
    src/cassettedialog.cpp \
    src/docdisplaywindow.cpp \
    src/bootoptionsdialog.cpp \
    src/network.cpp \
    src/logdisplaydialog.cpp \
    src/respeqtsettings.cpp \
    src/respeqtapplication.cpp \
    src/pclink.cpp \
    src/printers/baseprinter.cpp \
    src/printers/atari1027.cpp \
    src/atascii.cpp \
    src/drivewidget.cpp \
    src/atasciiinternational.cpp \
    src/printerwidget.cpp \
    src/printers/atariprinter.cpp \
    src/printers/atari1020.cpp \
    src/smartdevice.cpp \
    src/printers/nativeoutput.cpp \
    src/printers/centronics.cpp \
    src/printers/escp.cpp \
    src/printers/atari1029.cpp \
    src/printers/printerfactory.cpp \
    src/printers/atari1025.cpp \
    src/printers/passthrough.cpp \
    src/printers/graphicsprimitive.cpp \
    src/crc16.cpp \
    src/filesystems/ataridirentry.cpp \
    src/filesystems/atarifilesystem.cpp \
    src/filesystems/dos10filesystem.cpp \
    src/filesystems/dos20filesystem.cpp \
    src/filesystems/dos25filesystem.cpp \
    src/filesystems/mydosfilesystem.cpp \
    src/filesystems/spartadosfilesystem.cpp \
    src/diskimages/diskimage.cpp \
    src/diskimages/folderimage.cpp \
    src/diskimages/diskimagepro.cpp \
    src/diskimages/diskimageatx.cpp \
    src/diskimages/simplediskimage.cpp \
    src/diskimages/disassembly810.cpp \
    src/diskimages/disassembly1050.cpp \
    src/diskimages/cpu6502.cpp \
    src/diskimages/atxsectorinfo.cpp \
    src/diskimages/atxtrackinfo.cpp \
    src/diskimages/board.cpp \
    src/diskimages/diskgeometry.cpp \
    src/diskimages/diskimagefactory.cpp \
    src/diskimages/prosectorinfo.cpp \
    src/diskimages/board.cpp \
    src/diskimages/cpu6502.cpp \
    src/diskimages/diskimageatr.cpp \
    src/diskimages/disassembly810.cpp \
    src/diskimages/disassembly1050.cpp \
    src/diskbrowser/picsplitter.cpp \
    src/diskbrowser/picpreview.cpp \
    src/diskbrowser/folderdisks.cpp \
    src/diskbrowser/dbsettings.cpp \
    src/diskbrowser/dbjson.cpp \
    src/diskbrowser/dbini.cpp \
    src/diskbrowser/diskbrowserdlg.cpp

HEADERS += \
    include/mainwindow.h \
    include/printers/outputwindow.h \
    include/printers/rawoutput.h \
    include/siorecorder.h \
    tools/make_unique.h \
    include/printers/printers.h \
    include/rcl.h \
    include/sdxprotocol.h \
    include/serialport.h \
    include/sioworker.h \
    include/optionsdialog.h \
    include/aboutdialog.h \
    include/createimagedialog.h \
    include/diskeditdialog.h \
    include/autoboot.h \
    include/autobootdialog.h \
    include/miscutils.h \
    include/printers/textprinterwindow.h \
    include/cassettedialog.h \
    include/docdisplaywindow.h \
    include/bootoptionsdialog.h \
    include/network.h \
    include/logdisplaydialog.h \
    include/respeqtsettings.h \
    include/respeqtapplication.h \
    include/pclink.h \
    include/printers/baseprinter.h \
    include/printers/atari1027.h \
    include/atascii.h \
    include/drivewidget.h \
    include/printerwidget.h \
    include/atasciiinternational.h \
    include/printers/atariprinter.h \
    include/printers/atari1020.h \
    include/printers/nativeoutput.h \
    include/printers/centronics.h \
    include/printers/escp.h \
    include/printers/atari1029.h \
    include/printers/printerfactory.h \
    include/printers/atari1025.h \
    include/printers/passthrough.h \
    include/smartdevice.h  \
    include/printers/graphicsprimitive.h \
    headers/atarisio.h \
    include/crc16.h \
    include/filesystems/ataridirentry.h \
    include/filesystems/atarifilesystem.h \
    include/filesystems/dos10filesystem.h \
    include/filesystems/dos20filesystem.h \
    include/filesystems/dos25filesystem.h \
    include/filesystems/mydosfilesystem.h \
    include/filesystems/spartadosfilesystem.h \
    include/diskimages/disassembly810.h \
    include/diskimages/disassembly1050.h \
    include/diskimages/diskimage.h \
    include/diskimages/diskimageatx.h \
    include/diskimages/folderimage.h \
    include/diskimages/cpu6502.h \
    include/diskimages/atxsectorinfo.h \
    include/diskimages/atxtrackinfo.h \
    include/diskimages/board.h \
    include/diskimages/constvalues.h \
    include/diskimages/disassembly810.h \
    include/diskimages/disassembly1050.h \
    include/diskimages/cpu6502.h \
    include/diskimages/diskimage.h \
    include/diskimages/folderimage.h \
    include/diskimages/diskgeometry.h \
    include/diskimages/diskimagepro.h \
    include/diskimages/prosectorinfo.h \
    include/diskimages/simplediskimage.h \
    include/diskimages/diskimagefactory.h
    include/diskimages/atxsectorinfo.h \
    include/diskimages/atxtrackinfo.h \
    include/diskimages/board.h \
    include/diskbrowser/picsourcetype.h \
    include/diskbrowser/picsplitter.h \
    include/diskbrowser/picpreview.h \
    include/diskbrowser/folderdisks.h \
    include/diskbrowser/dbsettings.h \
    include/diskbrowser/dbjson.h \
    include/diskbrowser/dbini.h \
    include/diskbrowser/diskbrowserdlg.h

FORMS += \
    ui/mainwindow.ui \
    ui/optionsdialog.ui \
    ui/aboutdialog.ui \
    ui/createimagedialog.ui \
    ui/diskeditdialog.ui \
    ui/autobootdialog.ui \
    ui/cassettedialog.ui \
    ui/docdisplaywindow.ui \
    ui/bootoptionsdialog.ui \
    ui/logdisplaydialog.ui \
    ui/drivewidget.ui \
    ui/diskbrowserdlg.ui \
    ui/printers/outputwindow.ui \
    ui/printerwidget.ui \
    ui/printers/textprinterwindow.ui

RESOURCES += \
    resources/icons.qrc \
    resources/atarifiles.qrc \
    resources/i18n.qrc \
    resources/documentation.qrc \
    resources/fonts.qrc

# To update translations:
# lupdate respeqt.pro; lrelease i18n/respeqt_*.ts
TRANSLATIONS = \
    i18n/respeqt_de.ts \
    i18n/respeqt_es.ts \
    i18n/respeqt_pl.ts \
    i18n/respeqt_ru.ts \
    i18n/respeqt_sk.ts \
    i18n/respeqt_tr.ts

RC_FILE = resources/RespeQt.rc
ICON = resources/RespeQt.icns

win32 {
    SOURCES += src/serialport-win32.cpp \
               src/printers/rawoutput_win.cpp
    HEADERS += include/serialport-win32.h
}

unix {
    SOURCES += src/serialport-unix.cpp \
               src/printers/rawoutput_cups.cpp
    HEADERS += include/serialport-unix.h
}



