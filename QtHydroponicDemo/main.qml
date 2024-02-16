import QtQuick 2.15
import QtQuick.Window 2.15

import "qmlFiles"

Window {
    id: mainWindow
    width: 1600
    height: 960
    visible: true
    minimumHeight: 480
    minimumWidth: 640
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.Window

    title: qsTr("Smart Hydroponic Demo")

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

        DataMonitorPage{
            id:dataMonitor

        }
    }

//    Component{
//            id:animationPage

//            AnimationPage{
//                id:animation

//            }
//        }

//        Component{
//            id:loginPage

//            LoginPage{
//                id:login

//            }
//        }
//        Component{
//            id:mainPage

//            MainPage{
//                id:mainPage

//            }
//        }

}
