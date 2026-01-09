// cassettedialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: cassetteDialog
    title: qsTr("Cassette Image")
    modal: true
    standardButtons: Dialog.Close
    
    width: 450
    height: 300
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        GroupBox {
            title: qsTr("Cassette Image File")
            Layout.fillWidth: true
            
            ColumnLayout {
                spacing: 5
                
                RowLayout {
                    TextField {
                        id: cassettePath
                        Layout.fillWidth: true
                        readOnly: true
                    }
                    
                    Button {
                        text: qsTr("Browse...")
                        onClicked: fileDialog.open()
                    }
                }
                
                Label {
                    text: qsTr("Current position: 0/0 blocks")
                }
            }
        }
        
        GroupBox {
            title: qsTr("Playback Controls")
            Layout.fillWidth: true
            
            RowLayout {
                Button {
                    text: qsTr("Play")
                    icon.source: "qrc:/icons/silk-icons/icons/control_play.png"
                }
                
                Button {
                    text: qsTr("Pause")
                    icon.source: "qrc:/icons/silk-icons/icons/control_pause.png"
                }
                
                Button {
                    text: qsTr("Stop")
                    icon.source: "qrc:/icons/silk-icons/icons/control_stop.png"
                }
            }
        }
        
        TextArea {
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            placeholderText: qsTr("Cassette Status Log")
        }
    }
    
    FileDialog {
        id: fileDialog
        title: qsTr("Select Cassette Image")
        nameFilters: ["Cassette images (*.cas)"]
        onAccepted: cassettePath.text = fileDialog.fileUrl
    }
}
