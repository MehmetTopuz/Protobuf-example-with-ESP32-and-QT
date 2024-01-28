import QtQuick 2.15
import QtQuick.Window 2.15

import "qmlFiles"

Window {
    id: mainWindow
    width: 1280
    height: 960
    visible: true
    minimumHeight: 480
    minimumWidth: 640
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.Window

    title: qsTr("Smart Hydroponic")

    Loader{
        id: pageLoaderObj
        anchors.fill: parent
        sourceComponent: dataMonitorPage

        function replacePage(component){
            sourceComponent = undefined
            sourceComponent = component
        }

    }

    Component{
        id:dataMonitorPage

        DataMonitor{
            id:dataMonitor

        }
    }

}
