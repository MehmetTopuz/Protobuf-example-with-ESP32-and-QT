import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {

    property bool fullscreenFlag: false
    property string backgroundColor: "darkgray"

    id: topBarBackgroundRect
    width: parent.width
    height: 40
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    radius: {
        if(mainWindow.visibility === Window.Maximized)
            0
        else
            20
    }
    color: backgroundColor

    DragHandler {
        onActiveChanged: if(active){
                             mainWindow.startSystemMove()
                         }
    }

    MouseArea {
        id: topBarMouseArea
        anchors.fill: parent

        onDoubleClicked: {
            if(fullscreenFlag === true){
                if(mainWindow.visibility === Window.Maximized)
                {
                    showNormal()
                    console.log(mainWindow.width)
                    console.log(topBarBackgroundRect.width)
                }
                else
                {
                    showMaximized()
                    console.log(mainWindow.width)
                    console.log(topBarBackgroundRect.width)
                }
            }
        }
    }

    Rectangle {
        id: topBarSmallRectangleRight
        height: 15
        width: 15
        color: backgroundColor
        anchors.right: topBarBackgroundRect.right
        anchors.bottom: topBarBackgroundRect.bottom
    }

    Rectangle {
        id: topBarSmallRectangleleft
        height: 15
        width: 15
        color: backgroundColor
        anchors.left: topBarBackgroundRect.left
        anchors.bottom: topBarBackgroundRect.bottom
    }

    Image {
        id: topBarLogo
        source: "qrc:/icons/hydroponic_icon.png"
        height: parent.height - 20
        width: parent.height - 20
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: topBarAppName
        width: parent.height
        height: parent.height
        text: "Smart Hydroponic Demo"
        color: "black"
        font.pointSize: 10
        anchors.left: topBarLogo.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    RoundButton {
        id: closeButton
        width: 20
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        hoverEnabled: true

        onClicked: {
            visibility: close()
        }

        background: Rectangle {
            radius: 20
            color: closeButton.hovered ? "red" : "darkgray"

            Image {
                id: closeImage
                source: "qrc:/icons/close.png"
                width: 15
                height: 15
                anchors.centerIn: parent
            }
        }
    }

    RoundButton {
        id: maximizeButton
        width: 20
        height: 20
        anchors.right: closeButton.left
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        hoverEnabled: fullscreenFlag
        enabled: fullscreenFlag

        onClicked: {
            visibility: {
                if((mainWindow.visibility === Window.Maximized) && (mainWindow.width === Screen.width))
                    showNormal()
                else
                {
                    mainWindow.visibility = Window.Windowed
                    showMaximized()
                }
            }
        }

        background: Rectangle {
            radius: 20

            color: {
                if(fullscreenFlag === true)
                {
                    if(maximizeButton.hovered)
                        "gray"
                    else
                        "darkgray"
                }
                else
                    "darkgray"
            }
            Image {
                id: maximizeImage
                source: {
                    if((mainWindow.visibility === Window.Maximized) && (mainWindow.width === Screen.width))
                        "qrc:/icons/restore-down.png"
                    else
                        "qrc:/icons/maximize.png"

                }
                width: 15
                height: 15
                anchors.centerIn: parent
            }
        }
    }

    RoundButton {
        id: minimizeButton
        width: 20
        height: 20
        anchors.right: maximizeButton.left
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        hoverEnabled: true

        onClicked: {
            visibility: showMinimized()
        }
        background: Rectangle {
            radius: 20
            color: minimizeButton.hovered ? "gray" : "darkgray"

            Image {
                id: minimizeImage
                source: "qrc:/icons/minus.png"
                width: 15
                height: 15
                anchors.centerIn: parent
            }
        }
    }

}
