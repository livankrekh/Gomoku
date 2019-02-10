import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 900
    height: 600
    title: qsTr("Gomoku")

    property int movePlayer: 1
    property var stack: []
    property bool isEnd: false

    Image {
        id: board
        x: 16
        y: 15
        width: 570
        height: 570
        fillMode: Image.PreserveAspectFit
        source: "imgs/board2.png"

        Connections {
            target: game
            onMatrixChanged: {
                var newX = x * 30 + 5 - Math.floor(x * 0.15)
                var newY = y * 30 + 5 - Math.floor(y * 0.15)

                if (val == 0) {
                    for (var i = 0; i < stack.length; i++) {
                        if (Math.abs(stack[i].x - newX) < 10 && Math.abs(stack[i].y - newY) < 10) {
                            stack[i].destroy();
                            stack.splice(i, 1);
                            return ;
                        }
                    }
                } else {
                    var sourceImg = val == 1 ? '"imgs/white.png"}' : '"imgs/black.png"}';
                    var lastElement = Qt.createQmlObject('import QtQuick 2.9; Image {width: 20; height: 20; x: ' + newX + '; y: ' + newY + '; source: ' + sourceImg, board);
                    stack.push(lastElement);

                    movePlayer = movePlayer * -1;
                    playerText.text = movePlayer == 1 ? qsTr("white") : qsTr("black");
                }

            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (isEnd) {
                    return ;
                }

                var iX = Math.round((mouseX - 20) / 30);
                var iY = Math.round((mouseY - 20) / 30);
                var newX = iX * 30 + 5 - Math.floor(iX * 0.15)
                var newY = iY * 30 + 5 - Math.floor(iY * 0.15)
                var sourceImg = movePlayer == 1 ? '"imgs/white.png"}' : '"imgs/black.png"}';

                if (iX < 0 || iY < 0) {
                    return ;
                }

                if (!game.setMove(iX, iY, movePlayer)) {
                    return ;
                }

                if (game.checkPairRule(iX, iY, movePlayer)) {
                    console.log("Pair rule works!");
                }

                if (game.checkWin(iX, iY, movePlayer)) {
                    popupWin.contentItem.text += movePlayer === 1 ? "white" : "black";
                    popupWin.open();
                    isEnd = true;
                }

                var lastElement = Qt.createQmlObject('import QtQuick 2.9; Image {width: 20; height: 20; x: ' + newX + '; y: ' + newY + '; source: ' + sourceImg, board);
                stack.push(lastElement);

                movePlayer = movePlayer * -1;
                playerText.text = movePlayer === 1 ? qsTr("white") : qsTr("black");
            }
        }
    }

    Popup {
        id: popupWin
        x: 250
        y: 250
        width: 250
        height: 40
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        contentItem: Text {
            text: "Congratulations! Win player - "
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
        x: 740
        y: 25
        width: 58
        height: 22
        text: qsTr("white")
        font.pixelSize: 21
    }
}
