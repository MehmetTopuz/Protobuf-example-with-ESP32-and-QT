import QtQuick 2.0

Item {
    id: humidityItem

    property int humidityVal: 0
    property string humidityLabel: "None"

    Rectangle{
        id: humidityBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: humidityText
            text: humidityLabel
            font.pointSize: 15
            anchors.bottom: humidityImage.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: humidityImage.horizontalCenter
            color: "saddlebrown"
        }

        Image{
            id: humidityImage
            source: "qrc:/icons/humidity.png"
            width: 100
            height: 100
            anchors.centerIn: parent
        }

        Text {
            id: humidityValText
            text: qsTr("% " + parseInt(humidityVal))
            font.bold: true
            font.pointSize: 15
            anchors.top: humidityImage.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: humidityImage.horizontalCenter
        }
    }

}
