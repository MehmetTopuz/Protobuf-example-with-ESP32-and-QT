import QtQuick 2.15
Item {
    id: pumpItem

    property bool pumpState: false
    property string pumpText: "None"

    signal pumpClicked()

    Rectangle{
        id: pumpBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: pumpTextName
            text: pumpText
            font.pointSize: 15
            anchors.bottom: pumpImage.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: pumpImage.horizontalCenter
            color: "saddlebrown"
        }

        Image {
            id: pumpImage
            source: "qrc:/icons/pump.png"
            width: {
                if(pumpMouseArea.containsMouse)
                    105
                else
                    100

            }
            height: {
                if(pumpMouseArea.containsMouse)
                    105
                else
                    100

            }
            anchors.centerIn: parent

            Image {
                id: wheelImage
                source: "qrc:/icons/gear.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: parent.width/4 + 4
                width: 45
                height: 45

                RotationAnimator{
                    target: wheelImage
                    from: 0
                    to: 360
                    loops: Animation.Infinite
                    running: pumpState
                    duration: 2000

                }
            }

            MouseArea{
                id: pumpMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    pumpState = !pumpState
                    pumpClicked()
                }

            }
        }

    }
}
