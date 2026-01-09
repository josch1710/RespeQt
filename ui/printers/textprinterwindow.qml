// ui/printers/textprinterwindow.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: textPrinterWindow
    title: qsTr("Text Printer Window")
    
    width: 700
    height: 500
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        ToolBar {
            Layout.fillWidth: true
            
            RowLayout {
                ToolButton {
                    text: qsTr("Save As...")
                    icon.source: "qrc:/icons/silk-icons/icons/disk.png"
                }
                
                ToolButton {
                    text: qsTr("Clear")
                    icon.source: "qrc:/icons/silk-icons/icons/page_white.png"
                }
                
                ToolSeparator {}
                
                ToolButton {
                    text: qsTr("Font...")
                    icon.source: "qrc:/icons/silk-icons/icons/font.png"
                }
                
                Item { Layout.fillWidth: true }
                
                CheckBox {
                    text: qsTr("Word Wrap")
                }
                
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
                id: printerText
                readOnly: true
                font.family: "Courier"
                font.pointSize: 12
                wrapMode: TextArea.NoWrap
            }
        }
        
        StatusBar {
            Layout.fillWidth: true
            
            RowLayout {
                Label {
                    text: qsTr("Characters: 0")
                }
                
                Item { Layout.fillWidth: true }
                
                Label {
                    text: qsTr("Lines: 0")
                }
            }
        }
    }
    
    FontDialog {
        id: fontDialog
        title: qsTr("Select Font")
    }
}
