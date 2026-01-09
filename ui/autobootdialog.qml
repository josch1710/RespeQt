// autobootdialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Dialog {
    id: autobootDialog
    title: qsTr("Boot executable")
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    
    width: 500
    height: 400
    
    GridLayout {
        anchors.fill: parent
        columns: 2
        
        Label {
            text: qsTr("Executable file:")
        }
        
        RowLayout {
            TextField {
                id: executablePath
                Layout.fillWidth: true
            }
            
            Button {
                text: qsTr("Browse...")
                onClicked: fileDialog.open()
            }
        }
        
        Label {
            text: qsTr("Boot address:")
        }
        
        TextField {
            id: bootAddress
            text: "0x0000"
            inputMask: "\\0\\xHHHH"
        }
        
        GroupBox {
            title: qsTr("Options")
            Layout.columnSpan: 2
            Layout.fillWidth: true
            
            ColumnLayout {
                CheckBox {
                    text: qsTr("Use high speed executable loader")
                    checked: true
                }
                
                CheckBox {
                    text: qsTr("Clear memory before loading")
                    checked: true
                }
            }
        }
    }
    
    FileDialog {
        id: fileDialog
        title: qsTr("Select executable file")
        nameFilters: ["Executable files (*.xex *.com *.exe)"]
        onAccepted: executablePath.text = fileDialog.fileUrl
    }
}
