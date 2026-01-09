// optionsdialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: optionsDialog
    title: qsTr("Options")
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    
    width: 500
    height: 400
    
    TabBar {
        id: tabBar
        width: parent.width
        
        TabButton { text: qsTr("Emulation") }
        TabButton { text: qsTr("Serial Port") }
        TabButton { text: qsTr("User Interface") }
        TabButton { text: qsTr("Shortcuts") }
    }
    
    StackLayout {
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: tabBar.currentIndex
        
        // Emulation Tab
        ScrollView {
            clip: true
            
            ColumnLayout {
                spacing: 10
                
                GroupBox {
                    title: qsTr("Emulation Settings")
                    Layout.fillWidth: true
                    
                    ColumnLayout {
                        CheckBox {
                            text: qsTr("Use High Speed mode")
                            checked: true
                        }
                        
                        CheckBox {
                            text: qsTr("Use Happy mode")
                        }
                        
                        CheckBox {
                            text: qsTr("Use Chip mode")
                        }
                    }
                }
                
                GroupBox {
                    title: qsTr("Disk Drive Settings")
                    Layout.fillWidth: true
                    
                    GridLayout {
                        columns: 2
                        
                        Label { text: qsTr("Default Image Format:") }
                        ComboBox {
                            model: ["ATR", "XFD", "PRO"]
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
        
        // Serial Port Tab
        ScrollView {
            clip: true
            
            ColumnLayout {
                spacing: 10
                
                GroupBox {
                    title: qsTr("Serial Port Settings")
                    Layout.fillWidth: true
                    
                    GridLayout {
                        columns: 2
                        
                        Label { text: qsTr("Port:") }
                        ComboBox {
                            model: ["COM1", "COM2", "COM3", "COM4"]
                            Layout.fillWidth: true
                        }
                        
                        Label { text: qsTr("Speed:") }
                        ComboBox {
                            model: ["19200", "38400", "57600", "115200"]
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
        
        // User Interface Tab
        ScrollView {
            clip: true
            
            ColumnLayout {
                spacing: 10
                
                GroupBox {
                    title: qsTr("Language")
                    Layout.fillWidth: true
                    
                    ComboBox {
                        model: ["English", "Deutsch", "Français", "Español"]
                        Layout.fillWidth: true
                    }
                }
                
                GroupBox {
                    title: qsTr("Display Settings")
                    Layout.fillWidth: true
                    
                    ColumnLayout {
                        CheckBox {
                            text: qsTr("Show minimized to tray")
                        }
                        
                        CheckBox {
                            text: qsTr("Save window positions")
                        }
                    }
                }
            }
        }
        
        // Shortcuts Tab
        TableView {
            clip: true
            
            columnWidthProvider: function(column) {
                switch(column) {
                    case 0: return 200  // Action
                    case 1: return 150  // Shortcut
                    default: return 100
                }
            }
        }
    }
}
