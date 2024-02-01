import QtQuick 2.0

Item {
    id: phItem

    property double phVal: 0.0

    Rectangle{
        id: phBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: phText
            text: qsTr("Ph Level")
            font.pointSize: 15
            anchors.bottom: phImage.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: phImage.horizontalCenter
            color: "saddlebrown"
        }

        Image{
            id: phImage
            source: "qrc:/icons/ph.png"
            width: 100
            height: 100
            anchors.centerIn: parent
        }

        Text {
            id: phValText
            text: phVal
            font.pointSize: 15
            font.bold: true
            anchors.left: phImage.right
            anchors.leftMargin: 10
            anchors.verticalCenter: phImage.verticalCenter
        }

    }

}
