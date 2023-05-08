import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

Window {
    id: about
    title: "About RespeQt"
    width: 800
    height: 600

    Text {
        text: "RespeQt: Atari Serial Peripheral Emulator for Qt"
        font.family: "MS Shell Dlg"
        font.pointSize: 8
        font.bold: true
        renderType: Text.NativeRendering
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
    }
}
