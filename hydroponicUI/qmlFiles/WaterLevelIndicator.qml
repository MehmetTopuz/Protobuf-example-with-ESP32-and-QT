import QtQuick 2.0

Item {
    id: waterLevelIndicatorItem

    property int level: 0
    property string waterLevelLabel: "None"

    Rectangle{
        id: waterLevelBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: waterLevelText
            text: waterLevelLabel
            font.pointSize: 15
            anchors.bottom: waterTankImg.top
            anchors.bottomMargin: 5
            anchors.horizontalCenter: waterTankImg.horizontalCenter
            color: "saddlebrown"
        }
        Image {
            id: waterTankImg
            source: "qrc:/icons/reservoir.png"
            width: 200
            height: 200
            anchors.centerIn: parent

            Rectangle{
                id: progressBarRectBackground
                anchors.top: parent.top
                anchors.topMargin: 37
                anchors.left: parent.left
                anchors.leftMargin: 43
                width: 112
                height: 145
                color: "transparent"

                Rectangle {
                    id: progressBarRect
                    anchors.bottom: parent.bottom
                    radius: 5
                    width: parent.width
                    height: levelToHeigth(level,parent.height)
                    color: levelToColor(level)
                }

                Text {
                    id: levelText
                    text: qsTr("%"+parseInt(level))
                    font.pointSize: 15
                    font.bold: true
                    anchors.centerIn: parent
                }
            }

        }

    }

    function levelToHeigth(levelVal, heightVal){
        return (levelVal*heightVal/100)
    }
    function levelToColor(level){

        if(level < 25) return "red"
        else if(level >= 25 && level < 50) return "yellow"
        else if(level >= 50 && level < 75) return "aqua"
        else if(level >= 75 && level <= 100) return "dodgerblue"
        else return "dark"

    }
}
