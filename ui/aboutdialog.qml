// aboutdialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Dialog {
    id: aboutDialog
    title: qsTr("About RespeQt")
    modal: true
    standardButtons: Dialog.Ok
    
    width: 400
    height: 300
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        Image {
            Layout.alignment: Qt.AlignHCenter
            source: "qrc:/icons/main-icon/RespeQt.ico"
            width: 64
            height: 64
        }
        
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "RespeQt"
            font.bold: true
            font.pointSize: 14
        }
        
        TextArea {
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            wrapMode: TextArea.Wrap
            text: qsTr("RespeQt - Atari Serial Peripheral Emulator for Qt\n\n" +
                      "Version: X.X.X\n" +
                      "Build date: %1\n\n" +
                      "RespeQt Website: http://respeqt.github.io/\n" +
                      "RespeQt Project on GitHub: https://github.com/RespeQt/RespeQt\n\n" +
                      "Copyright © 2009-2023 by Fatih Aygün and RespeQt team\n" +
                      "This program is free software; you can redistribute it and/or modify it under the terms " +
                      "of the GNU General Public License as published by the Free Software Foundation, version 2.").arg(Qt.formatDateTime(new Date(), "yyyy-MM-dd"))
        }
    }
}
