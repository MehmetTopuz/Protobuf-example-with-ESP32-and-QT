import QtQuick 2.0

Item {
    id:buttonItem

    property bool buttonState: true
    property string buttonText: "None"

    signal buttonClicked()

    Rectangle{
        id: buttonBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        Text {
            id: bText
            text: buttonText
            font.pointSize: 15
            anchors.bottom: buttonImage.top
            anchors.bottomMargin: 10
            anchors.horizontalCenter: buttonImage.horizontalCenter
            color: "midnightblue"
            style: Text.Raised
        }

        Image {
            id: buttonImage
            anchors.centerIn: parent
            source: {
                if(buttonState){
                    "qrc:/icons/switch-on.png"
                }
                else{
                    "qrc:/icons/switch-off.png"
                }
            }
            width: {
                if(imageMouseArea.containsMouse){
                    105
                }
                else{
                    100
                }
            }
            height: {
                if(imageMouseArea.containsMouse){
                    105
                }
                else{
                    100
                }
            }

            MouseArea{
                id: imageMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    buttonState  = !buttonState
                    buttonClicked()
                }
            }
        }
    }
}
