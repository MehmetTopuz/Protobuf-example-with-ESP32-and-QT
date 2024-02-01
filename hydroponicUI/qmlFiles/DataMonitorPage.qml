import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.15
import QtGraphicalEffects 1.15

Item {

    id: dataMonitorItem
    width: parent.width
    height: parent.height

    Rectangle{
        id: dataMonitorMainFrame
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        clip: true
        radius: 20

        CustomTopBar{
            id: topBarDataMonitor
            fullscreenFlag: true
        }

        WaterLevelIndicator{
            id: waterLevel
            anchors.top: topBarDataMonitor.bottom
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 50
            width: 300
            height: 300

            level: 0

        }

        TemperatureIndicator{
            id:temperature
            anchors.top: topBarDataMonitor.bottom
            anchors.topMargin: 50
            anchors.left: waterLevel.right
            anchors.leftMargin: 50
            width: 300
            height: 300

            temperatureVal: 0
        }

        PhIndicator{
            id:ph
            anchors.top: topBarDataMonitor.bottom
            anchors.topMargin: 50
            anchors.left: temperature.right
            anchors.leftMargin: 50
            width: 300
            height: 300

            phVal: 7.5

        }

        MoistureIndicator{
            id: moisture
            anchors.top: waterLevel.bottom
            anchors.topMargin: 100
            anchors.horizontalCenter: waterLevel.horizontalCenter

            moistureVal: 25

        }

        Timer{
            interval: 50
            running: true
            repeat: true
            onTriggered: {
                if(waterLevel.level != 100){
                    moisture.moistureVal++
                    waterLevel.level++
                }
                else{
                    moisture.moistureVal=0
                    waterLevel.level=0
                }

                if(temperature.temperatureVal != 30)
                    temperature.temperatureVal++
                else
                    temperature.temperatureVal=0

            }
        }
    }
}
