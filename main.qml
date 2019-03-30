import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtMultimedia 5.8

Window {
    visible: true
    width: 860
    height: 600
    title: qsTr("Gomoku")

    property int customPlayer: 1
    property int movePlayer: 1
    property int moveCount: 1
    property var stack: []
    property bool isEnd: false
    property bool humanMode: false

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
                            stack[i].visible = false;
                            stack[i].destroy();
//                            stack.splice(i, 1);
                        }
                    }
                } else {
                    var sourceImg = val == 1 ? '"imgs/white.png"}' : '"imgs/black.png"}';
                    var lastElement = Qt.createQmlObject('import QtQuick 2.9; Image {width: 20; height: 20; x: ' + newX + '; y: ' + newY + '; source: ' + sourceImg, board);
                    stack.push(lastElement);

                    if (val == -1) {
                        moveCount++;
                    }

                    movePlayer = movePlayer * -1;
                    playerText.text = movePlayer == 1 ? qsTr("white") : qsTr("black");
                }

            }

            onReactionChanged: {
                moveTimestamp.text = qsTr("AI reaction time: " + time);
            }

            onWinnerChecked: {
                popupWin.contentItem.text = "Congratulations! Win player - " + (player === 1 ? "white" : "black");
                popupWin.open();
                isEnd = true;
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

                var lastElement = Qt.createQmlObject('import QtQuick 2.9; Image {width: 20; height: 20; x: ' + newX + '; y: ' + newY + '; source: ' + sourceImg, board);
                stack.push(lastElement);

                playerText.text = movePlayer === 1 ? qsTr("white") : qsTr("black");

                if (!humanMode) {
                    if (!game.moveAI(movePlayer)) {
                        return ;
                    }
                }
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

    Text {
        id: moveTimestamp
        x: 606
        y: 500
        width: 58
        height: 22
        text: qsTr("AI reaction time: ")
        font.pixelSize: 18
    }

    Button {
        id: aiVsAi
        x: 628
        y: 198
        width: 192
        height: 40
        text: qsTr("AI vs AI mode")
        font.pointSize: 22

        SoundEffect {
            id: playSound
            source: "imgs/AIvoice.wav"
        }

        Timer {
            id: timer
            interval: timeSlider.value * 1000
            running: false
            repeat: true
            onTriggered: {
                if (stack.length === 0) {
                    game.setMove(9,9,movePlayer);
                } else if (!game.moveAI(movePlayer)) {
                    return ;
                }

                if (isEnd) running = false;
            }
        }

        onClicked: {
            playSound.play();
            timer.running = true;
        }
    }

    Button {
        id: reverse
        x: 628
        y: 308
        width: 192
        height: 47
        text: qsTr("Reverse game")
        font.pointSize: 22

        onClicked: {
            customPlayer *= -1;
            if (stack.length === 0) {
                game.setMove(9,9,movePlayer);
            } else if (!game.moveAI(movePlayer)) {
                return ;
            }
        }
    }

    Button {
        id: moveAIone
        x: 628
        y: 368
        width: 192
        height: 43
        text: qsTr("Help by AI")
        font.pointSize: 22
        onClicked: {
            if (timer.running === true) {
                return ;
            }

            if (stack.length === 0) {
                game.setMove(9,9,movePlayer);
            } else if (!game.moveAI(movePlayer)) {
                return ;
            }

            if (!game.moveAI(movePlayer)) {
                return ;
            }
        }
    }

    Slider {
        id: timeSlider
        x: 628
        y: 267
        width: 192
        height: 40
        value: 1.5
        stepSize: 0.25
        from: 0.0
        to: 3.0
    }

    Text {
        id: element
        x: 628
        y: 250
        text: qsTr("Sleep time for AI (now " + timeSlider.value + " sec)")
        font.pixelSize: 15
    }

    Button {
        id: humanVsHuman
        x: 628
        y: 95
        width: 192
        height: 40
        text: qsTr("Human vs Human mode")
        font.pointSize: 15
        onClicked: {
            timer.running = false;
            humanMode = true;
        }
    }

    Button {
        id: humanVsAi
        x: 628
        y: 147
        width: 192
        height: 40
        text: qsTr("Human vs AI mode")
        font.pointSize: 19
        onClicked: {
            timer.running = false;
            humanMode = false;

            if (movePlayer != customPlayer) {
                if (!game.moveAI(movePlayer)) {
                    return ;
                }
            }
        }
    }

    Text {
        id: moveNumber
        x: 606
        y: 58
        width: 49
        height: 21
        text: qsTr("Move #" + moveCount)
        font.pixelSize: 21
    }

    Switch {
        id: alphaBeta
        x: 628
        y: 431
        text: qsTr("Alpha-beta prooning")
        checked: true
        onClicked: game.alphaBetaProoning = checked
    }
}
