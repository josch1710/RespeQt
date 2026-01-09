// diskbrowserdlg.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: diskBrowserDialog
    title: qsTr("Disk Image Browser")
    modal: false
    
    width: 800
    height: 600
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 5
        
        ToolBar {
            Layout.fillWidth: true
            
            RowLayout {
                ToolButton {
                    text: qsTr("Back")
                    icon.source: "qrc:/icons/silk-icons/icons/arrow_left.png"
                }
                ToolButton {
                    text: qsTr("Forward")
                    icon.source: "qrc:/icons/silk-icons/icons/arrow_right.png"
                }
                ToolButton {
                    text: qsTr("Up")
                    icon.source: "qrc:/icons/silk-icons/icons/arrow_up.png"
                }
                ToolButton {
                    text: qsTr("Home")
                    icon.source: "qrc:/icons/silk-icons/icons/house.png"
                }
                
                TextField {
                    Layout.fillWidth: true
                    placeholderText: qsTr("Path")
                }
            }
        }
        
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            Rectangle {
                SplitView.preferredWidth: 200
                
                TreeView {
                    anchors.fill: parent
                    // Modell für Verzeichnisstruktur
                }
            }
            
            Rectangle {
                SplitView.fillWidth: true
                
                TableView {
                    anchors.fill: parent
                    // Modell für Dateiliste
                    
                    columnWidthProvider: function(column) {
                        switch(column) {
                            case 0: return 200  // Name
                            case 1: return 100  // Size
                            case 2: return 150  // Date
                            default: return 100
                        }
                    }
                }
            }
        }
        
        StatusBar {
            Layout.fillWidth: true
            
            Label {
                text: qsTr("0 items selected")
            }
        }
    }
}
