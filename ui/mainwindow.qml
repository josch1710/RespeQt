// mainwindow.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 587
    height: 417
    title: "RespeQt"
    
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            
            MenuItem {
                text: qsTr("&Start emulation")
                //shortcut: "Alt+E"
                icon.source: "qrc:/icons/silk-icons/icons/disconnect.png"
                checkable: true
            }
            
            MenuItem {
                text: qsTr("&Boot Atari executable...")
                //shortcut: "Alt+B"
                icon.source: "qrc:/icons/other-icons/Atari.png"
            }
            
            MenuItem {
                text: qsTr("Playback cassette image...")
                //shortcut: "Alt+C"
                icon.source: "qrc:/icons/oxygen-icons/16x16/devices/media_tape.png"
            }
            
            MenuSeparator {}
            
            MenuItem {
                text: qsTr("&Quit")
                //shortcut: "Alt+Q"
                icon.source: "qrc:/icons/oxygen-icons/16x16/actions/exit.png"
            }
        }
        
        Menu {
            title: qsTr("&Disk")
            
            MenuItem {
                text: qsTr("New disk image...")
                //shortcut: "Ctrl+N"
                icon.source: "qrc:/icons/oxygen-icons/16x16/devices/media_floppy.png"
            }
            
            MenuItem {
                text: qsTr("Mount &disk image...")
                //shortcut: "Ctrl+D"
                icon.source: "qrc:/icons/oxygen-icons/16x16/devices/3floppy_unmount.png"
            }
            
            MenuItem {
                text: qsTr("Mount &folder image...")
                //shortcut: "Ctrl+F"
                icon.source: "qrc:/icons/silk-icons/icons/folder.png"
            }
        }
        
        // ... weitere Menüs
    }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 0
            
            GridLayout {
                id: leftColumn
                // Platzhalter für Drive-Widgets
            }
            
            GridLayout {
                id: rightColumn
                // Platzhalter für weitere Drive-Widgets
            }
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "lightgray"
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 0
            
            GridLayout {
                id: leftColumn2
            }
            
            GridLayout {
                id: rightColumn2
            }
        }
        
        TextArea {
            id: textEdit
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            wrapMode: TextArea.NoWrap
            enabled: false
        }
    }
    
    /*statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            Label {
                text: ""
            }
        }
    }*/
}
