// diskeditdialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: diskEditDialog
    title: qsTr("Disk Image Editor")
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
                    text: qsTr("Add Files")
                    icon.source: "qrc:/icons/silk-icons/icons/page_add.png"
                }
                ToolButton {
                    text: qsTr("Extract Files")
                    icon.source: "qrc:/icons/silk-icons/icons/page_save.png"
                }
                ToolButton {
                    text: qsTr("Delete Files")
                    icon.source: "qrc:/icons/silk-icons/icons/page_delete.png"
                }
            }
        }
        
        TableView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            columnWidthProvider: function(column) {
                switch(column) {
                    case 0: return 200  // Filename
                    case 1: return 80   // Size
                    case 2: return 100  // Modified
                    case 3: return 80   // Status
                    default: return 100
                }
            }
        }
        
        GroupBox {
            title: qsTr("Disk Properties")
            Layout.fillWidth: true
            
            GridLayout {
                columns: 2
                
                Label { text: qsTr("Format:") }
                Label { text: "ATR" }
                
                Label { text: qsTr("Capacity:") }
                Label { text: "90K" }
                
                Label { text: qsTr("Free Sectors:") }
                Label { text: "707" }
            }
        }
    }
}
