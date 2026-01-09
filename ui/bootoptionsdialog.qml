// bootoptionsdialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: bootOptionsDialog
    title: qsTr("Boot Options")
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    
    width: 400
    height: 300
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        GroupBox {
            title: qsTr("Boot File Settings")
            Layout.fillWidth: true
            
            ColumnLayout {
                spacing: 5
                
                RadioButton {
                    id: basicOption
                    text: qsTr("BASIC")
                    checked: true
                }
                
                RadioButton {
                    id: dosOption
                    text: qsTr("DOS")
                }
                
                RadioButton {
                    id: customOption
                    text: qsTr("Custom")
                }
                
                RowLayout {
                    enabled: customOption.checked
                    
                    TextField {
                        id: customPathField
                        Layout.fillWidth: true
                        placeholderText: qsTr("Enter custom boot file path")
                    }
                    
                    Button {
                        text: qsTr("Browse...")
                        onClicked: fileDialog.open()
                    }
                }
            }
        }
        
        GroupBox {
            title: qsTr("Additional Options")
            Layout.fillWidth: true
            
            ColumnLayout {
                CheckBox {
                    text: qsTr("High Speed mode")
                    checked: true
                }
                
                CheckBox {
                    text: qsTr("Auto-commit changes")
                }
            }
        }
    }
    
    FileDialog {
        id: fileDialog
        title: qsTr("Select Boot File")
        nameFilters: ["Boot files (*.rom *.bin)"]
        onAccepted: customPathField.text = fileDialog.fileUrl
    }
}
