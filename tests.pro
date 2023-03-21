
VERSION = r5.3
DEFINES += VERSION=\\\"$$VERSION\\\"

QT += testlib
QT += core network widgets printsupport serialport svg testlib
CONFIG += console qt warn_on depend_includepath testcase c++11 strict_c++
CONFIG -= app_bundle

debug {
  QMAKE_CXXFLAGS+="-fsanitize=address,undefined -fno-omit-frame-pointer -stdlib=libc++"
  QMAKE_LFLAGS+="-fsanitize=address,undefined"
}

TEMPLATE = app

SOURCES +=  tests/units/siorecordertest.cpp \
    tests/units/tests.cpp

HEADERS += tests/units/siorecordertest.h \
    tests/units/nulloutput.h \
    tests/units/dummyworker.h
INCLUDEPATH += tests $$[QT_INSTALL_HEADERS]/QtZlib include headers tools

TARGET = RespeQtTests

# Warnings for Deprecated functions
DEFINES += QT_DEPRECATED_WARNINGS
# Errors for QT4 deprecated functions
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000


# We want to have the testfiles in the build directory, so we have to this.
copytestdata.commands = $(COPY_DIR) $$PWD/tests/units/testdata $$OUT_PWD
first.depends = $(first) copytestdata
export(first.depends)
QMAKE_EXTRA_TARGETS += first copytestdata
DEFINES += RESPEQT_TEST

DISTFILES += \
    tests/units/USAGE.md \
    tests/units/testdata/writeSioCapture.json

win32 {
    LIBS += -lwinmm -lz -lwinspool
}

unix {
    LIBS += -lz -lcups
    CONFIG -= warn_on
    QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_WARN_ON -Wno-deprecated-copy
}

include(sources.pri)
