// ui/printers/outputwindow.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: outputWindow
    title: qsTr("Printer Output")
    
    width: 600
    height: 400
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        ToolBar {
            Layout.fillWidth: true
            
            RowLayout {
                ToolButton {
                    text: qsTr("Save")
                    icon.source: "qrc:/icons/silk-icons/icons/disk.png"
                }
                
                ToolButton {
                    text: qsTr("Clear")
                    icon.source: "qrc:/icons/silk-icons/icons/page_white.png"
                }
                
                Item { Layout.fillWidth: true }
                
                ToolButton {
                    text: qsTr("Print")
                    icon.source: "qrc:/icons/silk-icons/icons/printer.png"
                }
            }
        }
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            TextArea {
                id: outputText
                readOnly: true
                font.family: "Courier"
                wrapMode: TextArea.NoWrap
            }
        }
        
        StatusBar {
            Layout.fillWidth: true
            
            Label {
                text: qsTr("Output Format: Plain Text")
            }
        }
    }
}
