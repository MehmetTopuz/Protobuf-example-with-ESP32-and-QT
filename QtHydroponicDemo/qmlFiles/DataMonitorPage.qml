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

        property  int xVal: 0
        onMessageReceived: {    // It will be triggered when a message is received.
            // check message type

            switch(protobufManager.getMessageType()){
            case ProtobufManager.DATA:
                // get data
                sectorText.txt = protobufManager.getSectorName()
                deviceIdText.txt = protobufManager.getDeviceId()
                waterLevel.level = protobufManager.getWaterLevel()
                temperature.temperatureVal = protobufManager.getTemperature()
                ph.phVal = protobufManager.getPh()
                humidity.humidityVal = protobufManager.getMoisture()
                //eConductivity.eConductivityVal = protobufManager.getECval()
                eConductivity.appendData(xVal++,protobufManager.getECval())
                waterPumpOfTank.pumpState = protobufManager.getPumpState()
                valveOfTank.valveState = protobufManager.getValveState()
                ledButton.buttonState = protobufManager.getLedState()
                break;

            case ProtobufManager.HEART_BEAT:

                // do stuff
                break;

            case ProtobufManager.MESSAGE_OK:

                // do stuff
                break;

            case ProtobufManager.MESSAGE_ERROR:

                // do stuff
                break;

            case ProtobufManager.MESSAGE_TIMEOUT:

                // do stuff
                break;

            default:
                console.log("Invalid Message Type.")
                break;
            }
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

        CustomText {
            id: sectorText
            anchors.top: topBarDataMonitor.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 50
            width: 250
            height: 50

            label: "Zone:"
            txtColor: "cornsilk"
            backgroundColor: "cadetblue"
        }

        CustomText {
            id: deviceIdText
            anchors.top: topBarDataMonitor.bottom
            anchors.topMargin: 20
            anchors.left: sectorText.right
            anchors.leftMargin: 50
            width: 250
            height: 50

            label: "Device ID:"
            txtColor: "cornsilk"
            backgroundColor: "cadetblue"
        }

        WaterLevelIndicator{
            id: waterLevel
            anchors.top: sectorText.bottom
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
            anchors.top: sectorText.bottom
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
            anchors.top: sectorText.bottom
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

//        EConductivityIndicator{
//            id: eConductivity
//            width: 300
//            height: 300
//            anchors.top: humidity.top
//            anchors.left: humidity.right
//            anchors.leftMargin: 50

//            eConductivityLabel: "EC Level"
//            eConductivityVal: 25
//        }

        CustomChart {
            id: eConductivity
            width: 300
            height: 300
            anchors.top: humidity.top
            anchors.left: humidity.right
            anchors.leftMargin: 50

            chartName: "EC Level"

            //change the axes dynamically later.
        }


        PumpIndicator{
            id: waterPumpOfTank
            width: 300
            height: 300
            anchors.top: eConductivity.top
            anchors.left: eConductivity.right
            anchors.leftMargin: 50

            pumpState: false
            pumpText: "Tank Water Pump"

            onPumpClicked: {
                if(pumpState)
                    protobufManager.sendCommand(ProtobufManager.CMD_PUMP_ON)
                else
                    protobufManager.sendCommand(ProtobufManager.CMD_PUMP_OFF)
            }

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

            onValveClicked: {
                if(valveOfTank.valveState)
                    protobufManager.sendCommand(ProtobufManager.CMD_VALVE_ON)
                else
                    protobufManager.sendCommand(ProtobufManager.CMD_VALVE_OFF)
            }

        }

        CustomButton{
            id: ledButton
            width: 300
            height: 300
            anchors.top: sectorText.bottom
            anchors.topMargin: 50
            anchors.left: ph.right
            anchors.leftMargin: 50

            buttonText: "Led"
            buttonState: false

            onButtonClicked: {
                if(ledButton.buttonState)
                    protobufManager.sendCommand(ProtobufManager.CMD_LED_ON)
                else
                    protobufManager.sendCommand(ProtobufManager.CMD_LED_OFF)
            }

        }

    }
}