### RespeQt CMAKE version
cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.8)
project(RespeQt VERSION 5.3 LANGUAGES CXX)
set(RC_VERSION " RC1B1")
cmake_policy(SET CMP0071 NEW)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-DQT_DEPRECATED_WARNINGS -DQT_DISABLE_DEPRECATED_BEFORE=0x050000)
add_definitions(-DVERSION=\"r${PROJECT_VERSION}${RC_VERSION}\")

option(MAKE_TEST "Build the test application" OFF)
message(STATUS "Build tests ${MAKE_TEST}")
if (MAKE_TEST)
    message(STATUS "Build test application")
    set(TARGET_SOURCES tests/units/siorecordertest.cpp tests/units/tests.cpp)
    set(TARGET RespeQtTest)
    add_definitions(-DRESPEQT_TEST)
else()
    message(STATUS "Build main application")
    set(TARGET_SOURCES main.cpp)
    set(TARGET RespeQt)
endif()

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

set(SOURCES siorecorder.cpp mainwindow.cpp rcl.cpp sdxprotocol.cpp sioworker.cpp diskimagepro.cpp diskimageatx.cpp
    optionsdialog.cpp aboutdialog.cpp diskimage.cpp folderimage.cpp createimagedialog.cpp diskeditdialog.cpp
    autoboot.cpp autobootdialog.cpp atarifilesystem.cpp miscutils.cpp printers/textprinterwindow.cpp
    cassettedialog.cpp docdisplaywindow.cpp bootoptionsdialog.cpp network.cpp logdisplaydialog.cpp respeqtsettings.cpp
    pclink.cpp printers/baseprinter.cpp printers/atari1027.cpp atascii.cpp drivewidget.cpp infowidget.cpp
    atasciiinternational.cpp printerwidget.cpp printers/atariprinter.cpp printers/atari1020.cpp smartdevice.cpp
    printers/nativeoutput.cpp printers/outputwindow.cpp printers/centronics.cpp printers/escp.cpp printers/atari1029.cpp
    diskimageatr.cpp disassembly810.cpp disassembly1050.cpp printers/printerfactory.cpp printers/atari1025.cpp
    printers/passthrough.cpp printers/graphicsprimitive.cpp crc16.cpp cpu6502.cpp)

set(HEADERS mainwindow.h printers/outputwindow.h printers/rawoutput.h siorecorder.h
    tools/make_unique.h printers/printers.h rcl.h sdxprotocol.h serialport.h sioworker.h optionsdialog.h aboutdialog.h
    diskimage.h folderimage.h createimagedialog.h diskeditdialog.h autoboot.h autobootdialog.h atarifilesystem.h
    miscutils.h printers/textprinterwindow.h cassettedialog.h docdisplaywindow.h bootoptionsdialog.h network.h
    logdisplaydialog.h respeqtsettings.h pclink.h printers/baseprinter.h printers/atari1027.h atascii.h
    drivewidget.h infowidget.h printerwidget.h atasciiinternational.h printers/atariprinter.h
    printers/atari1020.h printers/nativeoutput.h printers/centronics.h printers/escp.h printers/atari1029.h
    disassembly810.h disassembly1050.h printers/printerfactory.h printers/atari1025.h printers/passthrough.h
    smartdevice.h printers/graphicsprimitive.h crc16.h cpu6502.h)

set(FORMS mainwindow.ui optionsdialog.ui aboutdialog.ui createimagedialog.ui diskeditdialog.ui autobootdialog.ui
    cassettedialog.ui docdisplaywindow.ui bootoptionsdialog.ui logdisplaydialog.ui drivewidget.ui infowidget.ui
    printers/outputwindow.ui printerwidget.ui printers/textprinterwindow.ui)

set(RESOURCES icons.qrc atarifiles.qrc i18n.qrc documentation.qrc fonts.qrc)

if (UNIX)
    set(SOURCES ${SOURCES} serialport-unix.cpp printers/rawoutput_cups.cpp)
    set(HEADERS ${HEADERS} serialport-unix.h)
endif()
if (WINDOWS)
    set(SOURCES ${SOURCES} serialport-win32.cpp printers/rawoutput_win.cpp)
    set(HEADERS ${HEADERS} serialport-win32.h)
    set(RESOURCES ${RESOURCES} RespeQt.rc)
endif()

find_package(Qt5 5.0...<5.4 REQUIRED
        COMPONENTS Core Widgets Gui Network PrintSupport SerialPort Svg LinguistTools)
if (MAKE_TEST)
    find_package(Qt5 5.0...<5.4 REQUIRED COMPONENTS Test)
endif()

# Translations
file(GLOB TS_FILES i18n/*.ts)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR})
qt5_add_translation(QM_FILES ${TS_FILES})

# Resources
qt5_wrap_ui(UI_HEADERS ${FORMS})
qt5_add_resources(UI_RESOURCES ${RESOURCES})

add_executable(${TARGET} ${SOURCES} ${HEADERS} ${QM_FILES} ${UI_HEADERS} ${UI_RESOURCES} ${TARGET_SOURCES})
if (APPLE)
    set(${TARGET}_ICON RespeQt.icns)
    set_source_files_properties(${TARGET}_ICON PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(${TARGET} PROPERTIES MACOSX_BUNDLE_ICON_FILE RespeQt.icns)
    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE TRUE
        BUNDLE TRUE)
endif()

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries(${TARGET} ${ZLIB_LIBRARIES})
endif()

target_link_libraries(${TARGET} Qt5::Core)
target_link_libraries(${TARGET} Qt5::Widgets)
target_link_libraries(${TARGET} Qt5::Gui)
target_link_libraries(${TARGET} Qt5::Network)
target_link_libraries(${TARGET} Qt5::PrintSupport)
target_link_libraries(${TARGET} Qt5::SerialPort)
target_link_libraries(${TARGET} Qt5::Svg)
if (MAKE_TEST)
    target_link_libraries(${TARGET} Qt5::Test)
endif()

if (UNIX)
    target_link_libraries(${TARGET} z cups)
endif()
if (WINDOWS)
    target_link_libraries(${TARGET} winmm z winspool)
endif()
