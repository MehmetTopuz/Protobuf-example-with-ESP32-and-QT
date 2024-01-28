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
    }
}
