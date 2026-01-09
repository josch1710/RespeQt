// docdisplaywindow.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: docDisplayWindow
    title: qsTr("Documentation")
    
    width: 700
    height: 500
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
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
                    text: qsTr("Home")
                    icon.source: "qrc:/icons/silk-icons/icons/house.png"
                }
            }
        }
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            TextArea {
                readOnly: true
                textFormat: TextArea.RichText
                wrapMode: TextArea.Wrap
            }
        }
    }
}
