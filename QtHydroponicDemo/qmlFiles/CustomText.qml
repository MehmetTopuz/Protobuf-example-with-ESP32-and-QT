import QtQuick 2.0
import QtQuick.Controls 2.15

Item {

    id: textItem

    property string label: "None:"
    property string txt: "None"
    property string txtColor: "red"
    property string backgroundColor: "grey"

    Rectangle {
        id: backGroundRect
        width: parent.width
        height: parent.height
        radius: 20
        color: backgroundColor

        Label {
            id: labelItem
            text: label
            font.pointSize: 20
            color: txtColor
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: textVal
            text: txt
            font.pointSize: 20
            color: txtColor
            anchors.left: labelItem.right
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }
    }



}
