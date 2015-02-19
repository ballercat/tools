import QtQuick 2.4
import QtQuick.Window 2.2
import "xqtcolors.js" as Color

Window {
    visible: true
    width: 300;
    height: 300;
    /*MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }

    }*/

    XqtButton {
        anchors.fill: parent;
        label: "Exit";
        radius: 20;
        pixelSize: 20;
        color: Color.neon_blue;
        onButtonClick : {
            Qt.quit();
        }
    }
}
