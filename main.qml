import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 900
    height: 600
    title: qsTr("Gomoku")

    property int movePlayer: 1

    Image {
        id: board
        x: 16
        y: 15
        width: 570
        height: 570
        fillMode: Image.PreserveAspectFit
        source: "imgs/board2.png"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                var iX = Math.round((mouseX - 20) / 30);
                var iY = Math.round((mouseY - 20) / 30);
                var newX = iX * 30 + 5 - Math.floor(iX * 0.15)
                var newY = iY * 30 + 5 - Math.floor(iY * 0.15)
                var sourceImg = movePlayer == 1 ? '"imgs/white.png"}' : '"imgs/black.png"}';

                Qt.createQmlObject('import QtQuick 2.9; Image {width: 20; height: 20; x: ' + newX + '; y: ' + newY + '; source: ' + sourceImg, board);

                movePlayer -= movePlayer * 2
                playerText.text = movePlayer == 1 ? qsTr("White") : qsTr("Black");
            }
        }
    }

    Text {
        id: simpleText
        x: 606
        y: 25
        width: 117
        height: 22
        text: qsTr("Move player:")
        font.pixelSize: 21
    }

    Text {
        id: playerText
        x: 723
        y: 25
        width: 58
        height: 22
        text: qsTr("White")
        font.pixelSize: 21
    }
}
