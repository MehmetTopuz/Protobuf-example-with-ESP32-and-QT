import QtQuick 2.0

Item {
    id: temperatureItem

    property int temperatureVal: 0
    property string temperatureLabel: "None"
    Rectangle{
        id: temperatureBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: waterLevelText
            text: temperatureLabel
            font.pointSize: 15
            anchors.bottom: temperatureImage.top
            anchors.bottomMargin: 5
            anchors.horizontalCenter: temperatureImage.horizontalCenter
            color: "saddlebrown"
        }

        Image {
            id: temperatureImage
            source: "qrc:/icons/temperature.png"
            width: 200
            height: 200
            anchors.centerIn: parent

            Rectangle{
                id:progressBarBackground
                color: "transparent"
                width: 16
                height: 112
                anchors.top: parent.top
                anchors.topMargin: 23
                anchors.left: parent.left
                anchors.leftMargin: 83

                Rectangle{
                    id: progressBarRect
                    color: "#db5669"
                    width: parent.width
                    height: temperatureToHeight(temperatureVal,parent.height)
                    radius: 5
                    anchors.bottom: parent.bottom
                }
            }
        }

        Text {
            id: temperatureText
            text: qsTr(temperatureVal+" C°")
            font.pointSize: 15
            font.bold: true
            anchors.left: temperatureImage.right
            anchors.top: temperatureImage.top
            anchors.topMargin: 55
        }
    }

    function temperatureToHeight(temperature,heightMax){
        if(temperature <= 0) return 0
        else if(temperature >= 30) return heightMax
        else{
           return temperature*heightMax/30
        }
    }
}
