// createimagedialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: createImageDialog
    title: qsTr("Create New Image File")
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    
    width: 450
    height: 300
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        GroupBox {
            title: qsTr("File Format")
            Layout.fillWidth: true
            
            ColumnLayout {
                RadioButton {
                    text: qsTr("Standard Single Density (.atr)")
                    checked: true
                }
                RadioButton {
                    text: qsTr("Standard Enhanced Density (.atr)")
                }
                RadioButton {
                    text: qsTr("Standard Double Density (.atr)")
                }
                RadioButton {
                    text: qsTr("Double Sided Double Density (.atr)")
                }
                RadioButton {
                    text: qsTr("Hard Disk (.pro)")
                }
            }
        }
        
        GroupBox {
            title: qsTr("Custom Size")
            Layout.fillWidth: true
            
            GridLayout {
                columns: 2
                
                Label { text: qsTr("Sectors:") }
                SpinBox {
                    id: sectorCount
                    from: 1
                    to: 65535
                    value: 720
                }
                
                Label { text: qsTr("Bytes per Sector:") }
                SpinBox {
                    id: bytesPerSector
                    from: 128
                    to: 256
                    value: 128
                    stepSize: 128
                }
            }
        }
        
        RowLayout {
            Layout.fillWidth: true
            
            Label { text: qsTr("Total Size:") }
            Label { 
                text: (sectorCount.value * bytesPerSector.value / 1024).toFixed(2) + " KB"
            }
        }
    }
}
