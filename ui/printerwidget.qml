// printerwidget.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: printerWidget
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 5
        
        ToolBar {
            Layout.fillWidth: true
            
            RowLayout {
                ToolButton {
                    text: qsTr("Clear")
                    icon.source: "qrc:/icons/silk-icons/icons/printer_delete.png"
                }
                
                ToolButton {
                    text: qsTr("Save Output")
                    icon.source: "qrc:/icons/silk-icons/icons/printer_add.png"
                }
                
                Item { Layout.fillWidth: true }
                
                ComboBox {
                    model: ["Text Mode", "Graphics Mode"]
                }
            }
        }
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            TextArea {
                readOnly: true
                font.family: "Courier"
                wrapMode: TextArea.NoWrap
            }
        }
        
        StatusBar {
            Layout.fillWidth: true
            
            Label {
                text: qsTr("Printer Ready")
            }
        }
    }
}
