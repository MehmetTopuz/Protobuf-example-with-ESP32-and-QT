import QtQuick 2.0

Item {
    id: moistureItem

    property int moistureVal: 0

    Rectangle{
        id: moistureBackgroundRect
        width: parent.width
        height: parent.height
        color: "blue"

        Text {
            id: moistureText
            text: qsTr("Moisture Level")
            font.pointSize: 15
            anchors.bottom: moistureImage.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: moistureImage.horizontalCenter
            color: "saddlebrown"
        }

        Image{
            id: moistureImage
            source: "qrc:/icons/moisture.png"
            width: 100
            height: 100
            anchors.centerIn: parent
        }

        Text {
            id: moistureValText
            text: qsTr("% " + parseInt(moistureVal))
            font.bold: true
            font.pointSize: 15
            anchors.top: moistureImage.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: moistureImage.horizontalCenter
        }
    }

}
