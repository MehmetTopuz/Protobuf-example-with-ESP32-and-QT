import QtQuick 2.0

Item {
    id: eConductivityItem

    property double eConductivityVal: 0
    property string eConductivityLabel: "None"

    Rectangle{
        id: eConductivityBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: eConductivityText
            text: eConductivityLabel
            font.pointSize: 15
            anchors.bottom: ecImage.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: ecImage.horizontalCenter
            color: "saddlebrown"
        }

        Image {
            id: ecImage
            source: "qrc:/icons/flash.png"
            width: 100
            height: 100
            anchors.centerIn: parent

        }

        Text{
            id: ecValText
            text: qsTr(eConductivityVal.toFixed(2) + " ms/cm")
            font.bold: true
            font.pointSize: 12
            anchors.top: ecImage.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: ecImage.horizontalCenter
        }

    }
}
