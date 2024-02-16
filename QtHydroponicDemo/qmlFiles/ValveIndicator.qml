import QtQuick 2.0

Item {
    id: valveItem

    property bool valveState: false
    property string valveLabel: "None"

    signal valveClicked()

    Rectangle{
        id: valveBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: valveText
            text: valveLabel
            font.pointSize: 15
            anchors.bottom: valveImage.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: valveImage.horizontalCenter
            color: "midnightblue"
            style: Text.Raised
        }

        Image {
            id: valveImage
            source: "qrc:/icons/valve.png"
            width: {
                if(valveMouseArea.containsMouse)
                    105
                else
                    100

            }

            height: {
                if(valveMouseArea.containsMouse)
                    105
                else
                    100

            }
            anchors.centerIn: parent

            MouseArea{
                id: valveMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    valveState = !valveState
                    valveClicked()
                }
            }
        }

        Text {
            id: stateText
            text: {
                if(valveState)
                    qsTr("ON")
                else
                    qsTr("OFF")
            }
            font.bold: true
            font.pointSize: 12
            anchors.top: valveImage.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: valveImage.horizontalCenter
        }
    }
}
