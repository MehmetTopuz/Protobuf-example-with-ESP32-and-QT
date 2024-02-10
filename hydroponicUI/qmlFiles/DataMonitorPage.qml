import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.15
import QtGraphicalEffects 1.15

import com.protobuf 1.0

Item {

    id: dataMonitorItem
    width: parent.width
    height: parent.height

    ProtobufManager{
        id: protobufManager
        onMessageReceived: {    // It will be triggered when a message is received.
            // check message type

//            switch(protobufManager.getMessageType()){
//            case
//            }

            waterLevel.level = protobufManager.getWaterLevel()
            temperature.temperatureVal = protobufManager.getTemperature()
            ph.phVal = protobufManager.getPh()
            humidity.humidityVal = protobufManager.getMoisture()
            eConductivity.eConductivityVal = protobufManager.getECval()
            waterPumpOfTank.pumpState = protobufManager.getPumpState()
            valveOfTank.valveState = protobufManager.getValveState()
            ledButton.buttonState = protobufManager.getLedState()

            // do stuff

            console.log("Message received.")
        }

    }

    Rectangle{
        id: dataMonitorMainFrame
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        clip: true
        radius: 20

        Image {
            id: backgroundImage
            source: "qrc:/icons/greenhouse.jpg"
            anchors.top: topBarDataMonitor.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: {
                if(ledButton.buttonState)
                    0.7
                else
                    0.4
            }

        }

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

            waterLevelLabel: "Water Level"
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

            temperatureLabel: "Temperature"
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

            phLabel: "Ph Level"
            phVal: 7.5

        }

        HumidityIndicator{
            id: humidity
            width: 300
            height: 300
            anchors.top: waterLevel.bottom
            anchors.topMargin: 100
            anchors.horizontalCenter: waterLevel.horizontalCenter

            humidityLabel: "Humidity"
            humidityVal: 25

        }

        EConductivityIndicator{
            id: eConductivity
            width: 300
            height: 300
            anchors.top: humidity.top
            anchors.left: humidity.right
            anchors.leftMargin: 50

            eConductivityLabel: "EC Level"
            eConductivityVal: 25
        }

        PumpIndicator{
            id: waterPumpOfTank
            width: 300
            height: 300
            anchors.top: eConductivity.top
            anchors.left: eConductivity.right
            anchors.leftMargin: 50

            pumpState: true
            pumpText: "Tank Water Pump"
        }

        ValveIndicator{
            id: valveOfTank
            width: 300
            height: 300
            anchors.top: waterPumpOfTank.top
            anchors.left: waterPumpOfTank.right
            anchors.leftMargin: 50

            valveState: false
            valveLabel: "Valve State"

        }

        CustomButton{
            id: ledButton
            width: 300
            height: 300
            anchors.top: topBarDataMonitor.bottom
            anchors.topMargin: 50
            anchors.left: ph.right
            anchors.leftMargin: 50

            buttonText: "Led"
            buttonState: false

        }

//        Timer{
//            interval: 50
//            running: true
//            repeat: true
//            onTriggered: {
//                if(waterLevel.level != 100){
//                    humidity.humidityVal++
//                    waterLevel.level++
//                }
//                else{
//                    humidity.humidityVal=0
//                    waterLevel.level=0
//                    waterPumpOfTank.pumpState=false
//                }

//                if(temperature.temperatureVal != 30)
//                    temperature.temperatureVal++
//                else
//                    temperature.temperatureVal=0

//            }
//        }
    }
}
