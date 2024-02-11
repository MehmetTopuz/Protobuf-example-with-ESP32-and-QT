import QtQuick 2.0

Item {
    id: phItem

    property double phVal: 0.0
    property string phLabel: "None"

    Rectangle{
        id: phBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: phText
            text: phLabel
            font.pointSize: 15
            anchors.bottom: phImage.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: phImage.horizontalCenter
            color: "midnightblue"
            style: Text.Raised
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
            text: phVal.toFixed(1)
            font.pointSize: 15
            font.bold: true
            anchors.left: phImage.right
            anchors.leftMargin: 10
            anchors.verticalCenter: phImage.verticalCenter
        }

    }

}
