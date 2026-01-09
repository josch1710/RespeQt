import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: driveWidget
    objectName: "driveWidget"
    width: 336
    height: 52
    color: "transparent"

    ColumnLayout {
        anchors.fill: parent
        spacing: 3
        anchors.margins: 3

        Rectangle {
            id: driveFrame
            objectName: "driveFrame"
            Layout.fillWidth: true
            Layout.minimumWidth: 330
            Layout.preferredHeight: 46
            border.width: 1
            border.color: "gray"
            radius: 2

            RowLayout {
                anchors.fill: parent
                anchors.margins: 3
                spacing: 3

                Label {
                    id: driveLabel
                    objectName: "driveLabel"
                    text: "1:"
                    Layout.preferredWidth: 26
                    Layout.preferredHeight: 26
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: enabled ? "#686868" : "#787878"
                }

                // Reihe von ToolButtons
                ToolButton {
                    id: buttonMountDisk
                    objectName: "buttonMountDisk"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/silk-icons/icons/disk.png"
                    ToolTip.text: qsTr("Mount a disk image")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonMountFolder
                    objectName: "buttonMountFolder"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/silk-icons/icons/folder.png"
                    ToolTip.text: qsTr("Mount a folder image")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonSave
                    objectName: "buttonSave"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/silk-icons/icons/drive_save.png"
                    checkable: true
                    ToolTip.text: qsTr("Save image")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: autoSave
                    objectName: "autoSave"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/silk-icons/icons/drive_go.png"
                    checkable: true
                    ToolTip.text: qsTr("Auto save")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonEditDisk
                    objectName: "buttonEditDisk"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/silk-icons/icons/folder_edit.png"
                    ToolTip.text: qsTr("Show properties")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonToggleChip
                    objectName: "buttonToggleChip"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: checked ? 
                        "qrc:/icons/silk-icons/icons/package_go.png" :
                        "qrc:/icons/silk-icons/icons/package_delete.png"
                    checkable: true
                    ToolTip.text: qsTr("Toggle Chip/Super Archiver compatibility")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonToggleHappy
                    objectName: "buttonToggleHappy"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: checked ?
                        "qrc:/icons/silk-icons/icons/emoticon_happy.png" :
                        "qrc:/icons/silk-icons/icons/emoticon_unhappy.png"
                    checkable: true
                    ToolTip.text: qsTr("Toggle Happy compatibility")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonNextSide
                    objectName: "buttonNextSide"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/oxygen-icons/16x16/actions/2rightarrow.png"
                    checkable: true
                    ToolTip.text: qsTr("Load next software disk or side")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonToolDisk
                    objectName: "buttonToolDisk"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: checked ?
                        "qrc:/icons/silk-icons/icons/shield_go.png" :
                        "qrc:/icons/silk-icons/icons/shield.png"
                    checkable: true
                    ToolTip.text: qsTr("Boot tool disk image")
                    ToolTip.visible: hovered
                }

                ToolButton {
                    id: buttonToggleOSB
                    objectName: "buttonToggleOSB"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/other-icons/toggleosb.png"
                    checkable: true
                    ToolTip.text: qsTr("Boot translator disk image")
                    ToolTip.visible: hovered
                }

                ColumnLayout {
                    id: imageInfo
                    spacing: 0
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
                    Layout.fillWidth: true

                    Label {
                        id: labelFileName
                        objectName: "labelFileName"
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        font.family: "MS Shell Dlg 2"
                        font.pointSize: 8
                    }

                    Label {
                        id: labelImageProperties
                        objectName: "labelImageProperties"
                        Layout.fillWidth: true
                        color: "#808080"
                    }
                }

                ToolButton {
                    id: buttonEject
                    objectName: "buttonEject"
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    icon.source: "qrc:/icons/silk-icons/icons/control_eject.png"
                    ToolTip.text: qsTr("Unmount")
                    ToolTip.visible: hovered
                }
            }
        }
    }
}
