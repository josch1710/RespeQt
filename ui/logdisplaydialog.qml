// logdisplaydialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: logDisplayDialog
    title: qsTr("Log Display")
    modal: false
    
    width: 600
    height: 400
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 5
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            TextArea {
                id: logText
                readOnly: true
                font.family: "Courier"
                wrapMode: TextArea.NoWrap
            }
        }
        
        RowLayout {
            Layout.fillWidth: true
            
            Button {
                text: qsTr("Clear")
                icon.source: "qrc:/icons/silk-icons/icons/page_white.png"
                onClicked: logText.clear()
            }
            
            Button {
                text: qsTr("Save As...")
                icon.source: "qrc:/icons/silk-icons/icons/disk.png"
            }
            
            Item { Layout.fillWidth: true }
            
            CheckBox {
                text: qsTr("Auto-scroll")
                checked: true
            }
        }
    }
}
