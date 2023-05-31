set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

### RespeQt CMAKE version
cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.8)
project(RespeQt VERSION 5.4 LANGUAGES CXX)
set(RC_VERSION "RC2")
cmake_policy(SET CMP0071 NEW)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ui ui/printers ui/filesystems)
add_definitions(-DQT_DEPRECATED_WARNINGS -DQT_DISABLE_DEPRECATED_BEFORE=0x050000)
add_definitions(-DVERSION=\"v${PROJECT_VERSION}_${RC_VERSION}\")



message(STATUS "Build main application")
set(TARGET_SOURCES src/main.cpp)
set(TARGET RespeQt)

message(STATUS "Build test application")
set(TARGETTEST_SOURCES tests/units/siorecordertest.cpp tests/units/tests.cpp)
set(TARGETTEST RespeQtTest)

include_directories(include headers tests tools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer -stdlib=libc++")
if (NOT WIN32)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
endif()

set(SOURCES
    src/siorecorder.cpp
    src/mainwindow.cpp
    src/rcl.cpp
    src/sdxprotocol.cpp
    src/sioworker.cpp
    src/diskimages/diskimagepro.cpp
    src/diskimages/diskimageatx.cpp
    src/optionsdialog.cpp
    src/aboutdialog.cpp
    src/diskimages/diskimage.cpp
    src/diskimages/folderimage.cpp
    src/createimagedialog.cpp
    src/diskeditdialog.cpp
    src/autoboot.cpp
    src/autobootdialog.cpp
    src/miscutils.cpp
    src/printers/textprinterwindow.cpp
    src/cassettedialog.cpp
    src/docdisplaywindow.cpp
    src/bootoptionsdialog.cpp
    src/network.cpp
    src/logdisplaydialog.cpp
    src/respeqtsettings.cpp
    src/pclink.cpp
    src/printers/baseprinter.cpp
    src/printers/atari1027.cpp
    src/atascii.cpp
    src/drivewidget.cpp
    src/atasciiinternational.cpp
    src/printerwidget.cpp
    src/printers/atariprinter.cpp
    src/printers/atari1020.cpp
    src/smartdevice.cpp
    src/printers/nativeoutput.cpp
    src/printers/outputwindow.cpp
    src/printers/centronics.cpp
    src/printers/escp.cpp
    src/printers/atari1029.cpp
    src/diskimages/simplediskimage.cpp
    src/diskimages/disassembly810.cpp
    src/diskimages/disassembly1050.cpp
    src/printers/printerfactory.cpp
    src/printers/atari1025.cpp
    src/printers/passthrough.cpp
    src/printers/graphicsprimitive.cpp
    src/crc16.cpp
    src/diskimages/cpu6502.cpp
    src/filesystems/ataridirentry.cpp
    src/filesystems/atarifilesystem.cpp
    src/filesystems/dos10filesystem.cpp
    src/filesystems/dos20filesystem.cpp
    src/filesystems/dos25filesystem.cpp
    src/filesystems/mydosfilesystem.cpp
    src/filesystems/spartadosfilesystem.cpp
    src/diskimages/diskgeometry.cpp
    src/diskimages/prosectorinfo.cpp
    src/diskimages/atxsectorinfo.cpp
    src/diskimages/atxtrackinfo.cpp
    src/diskimages/board.cpp
    src/diskimages/diskimagefactory.cpp
    src/RespeQtApplication.cpp
)

set(HEADERS
    include/mainwindow.h
    include/printers/outputwindow.h
    include/printers/rawoutput.h
    include/siorecorder.h
    tools/make_unique.h
    include/printers/printers.h
    include/rcl.h
    include/sdxprotocol.h
    include/serialport.h
    include/sioworker.h
    include/optionsdialog.h
    include/aboutdialog.h
    include/diskimages/diskimage.h
    include/diskimages/folderimage.h
    include/createimagedialog.h
    include/diskeditdialog.h
    include/autoboot.h
    include/autobootdialog.h
    include/miscutils.h
    include/printers/textprinterwindow.h
    include/cassettedialog.h
    include/docdisplaywindow.h
    include/bootoptionsdialog.h
    include/network.h
    include/logdisplaydialog.h
    include/respeqtsettings.h
    include/pclink.h
    include/printers/baseprinter.h
    include/printers/atari1027.h
    include/atascii.h
    include/drivewidget.h
    include/printerwidget.h
    include/atasciiinternational.h
    include/printers/atariprinter.h
    include/printers/atari1020.h
    include/printers/nativeoutput.h
    include/printers/centronics.h
    include/printers/escp.h
    include/printers/atari1029.h
    include/diskimages/disassembly810.h
    include/diskimages/disassembly1050.h
    include/printers/printerfactory.h
    include/printers/atari1025.h
    include/printers/passthrough.h
    include/smartdevice.h
    include/printers/graphicsprimitive.h
    include/crc16.h
    include/diskimages/cpu6502.h
    headers/atarisio.h
    include/filesystems/ataridirentry.h
    include/filesystems/atarifilesystem.h
    include/filesystems/dos10filesystem.h
    include/filesystems/dos20filesystem.h
    include/filesystems/dos25filesystem.h
    include/filesystems/mydosfilesystem.h
    include/filesystems/spartadosfilesystem.h
    include/diskimages/diskgeometry.h
    include/diskimages/prosectorinfo.h
    include/diskimages/atxsectorinfo.h
    include/diskimages/atxtrackinfo.h
    include/diskimages/board.h
    include/diskimages/simplediskimage.h
    include/diskimages/diskimagepro.h
    include/diskimages/diskimageatx.h
    include/diskimages/diskimagefactory.h
    include/RespeQtApplication.h
        include/diskimages/constvalues.h)

set(FORMS
    ui/mainwindow.ui
    ui/optionsdialog.ui
    ui/aboutdialog.ui
    ui/createimagedialog.ui
    ui/diskeditdialog.ui
    ui/autobootdialog.ui
    ui/cassettedialog.ui
    ui/docdisplaywindow.ui
    ui/bootoptionsdialog.ui
    ui/logdisplaydialog.ui
    ui/drivewidget.ui
    ui/printers/outputwindow.ui
    ui/printerwidget.ui
    ui/printers/textprinterwindow.ui
)

set(RESOURCES
    resources/icons.qrc
    resources/atarifiles.qrc
    resources/i18n.qrc
    resources/documentation.qrc
    resources/fonts.qrc
)

if (UNIX)
    set(SOURCES ${SOURCES} src/serialport-unix.cpp src/printers/rawoutput_cups.cpp)
    set(HEADERS ${HEADERS} include/serialport-unix.h)
endif()
if (WIN32)
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT
            "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    set(SOURCES ${SOURCES} src/serialport-win32.cpp src/printers/rawoutput_win.cpp
            resources/RespeQt.rc)
    set(HEADERS ${HEADERS} include/serialport-win32.h)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
endif()

find_package(Qt5 5.0...<5.7 REQUIRED
    COMPONENTS Core Widgets Gui Network PrintSupport SerialPort Svg LinguistTools
)

find_package(Qt5 5.0...<5.7 REQUIRED COMPONENTS Test)

if (APPLE)
    find_package(Qt5 5.0...<5.7 COMPONENTS MacExtras)
elseif(UNIX)
    find_package(Qt5 5.0...<5.7 COMPONENTS X11Extras)
elseif(WIN32)
    find_package(Qt5 5.0...<5.7 COMPONENTS WinExtras)
endif()

# Translations
file(GLOB TS_FILES i18n/*.ts)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR})
qt5_add_translation(QM_FILES ${TS_FILES})

# Resources
qt5_wrap_ui(UI_HEADERS ${FORMS})
qt5_add_resources(UI_RESOURCES ${RESOURCES})

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE RespeQt.icns)
    set(APP_ICON resources/RespeQt.icns src/diskimages/diskimagefactory.cpp)
    set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(${TARGET} MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${QM_FILES} ${UI_HEADERS} ${UI_RESOURCES} ${TARGET_SOURCES} ${APP_ICON})
    add_executable(${TARGETTEST} MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${QM_FILES} ${UI_HEADERS} ${UI_RESOURCES} ${TARGETTEST_SOURCES} ${APP_ICON})

    add_custom_target(deploy ${CMAKE_SOURCE_DIR}/deployment/macdeploy.sh "\"${CMAKE_SOURCE_DIR}\"" "\"${CMAKE_BINARY_DIR}\"" "${PROJECT_VERSION}" "${RC_VERSION}")
else()
    add_executable(${TARGET} ${SOURCES} ${HEADERS} ${QM_FILES} ${UI_HEADERS} ${UI_RESOURCES} ${TARGET_SOURCES})
    add_executable(${TARGETTEST} ${SOURCES} ${HEADERS} ${QM_FILES} ${UI_HEADERS} ${UI_RESOURCES} ${TARGETTEST_SOURCES})
endif()


find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries(${TARGET} ${ZLIB_LIBRARIES})
    target_link_libraries(${TARGETTEST} ${ZLIB_LIBRARIES})
endif()

target_link_libraries(${TARGET} Qt5::Core)
target_link_libraries(${TARGET} Qt5::Widgets)
target_link_libraries(${TARGET} Qt5::Gui)
target_link_libraries(${TARGET} Qt5::Network)
target_link_libraries(${TARGET} Qt5::PrintSupport)
target_link_libraries(${TARGET} Qt5::SerialPort)
target_link_libraries(${TARGET} Qt5::Svg)

target_link_libraries(${TARGETTEST} Qt5::Core)
target_link_libraries(${TARGETTEST} Qt5::Widgets)
target_link_libraries(${TARGETTEST} Qt5::Gui)
target_link_libraries(${TARGETTEST} Qt5::Network)
target_link_libraries(${TARGETTEST} Qt5::PrintSupport)
target_link_libraries(${TARGETTEST} Qt5::SerialPort)
target_link_libraries(${TARGETTEST} Qt5::Svg)
target_link_libraries(${TARGETTEST} Qt5::Test)

# Include platform extras
if (APPLE AND Qt5MacExtras_FOUND)
    message(STATUS "Found Mac Extras")
    target_link_libraries(${TARGET} Qt5::MacExtras)
    target_link_libraries(${TARGETTEST} Qt5::MacExtras)
endif()
if (UNIX AND Qt5X11Extras_FOUND)
    message(STATUS "Found X11 Extras")
    target_link_libraries(${TARGETTEST} Qt5::X11Extras)
endif()
if (WIN32 AND Qt5WinExtras_FOUND)
    message(STATUS "Found Win Extras")
    target_link_libraries(${TARGETTEST} Qt5::WinExtras)
endif()

if (UNIX)
    target_link_libraries(${TARGET} z cups)
endif()
if (WIN32)
    target_link_libraries(${TARGET} winmm z winspool)
endif()

# Deployment
if (LINUX)
    add_custom_target(deploy deployment/lindeploy.sh )
endif()

target_compile_definitions(${TARGETTEST} PUBLIC -DRESPEQT_TEST)
