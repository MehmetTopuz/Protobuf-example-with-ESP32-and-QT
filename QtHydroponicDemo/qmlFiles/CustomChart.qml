import QtQuick 2.15
import QtCharts 2.6

Item {
    id: chartItem

    property  string chartName: "None"

    Rectangle {
        id: chartBackgroundRect
        width: parent.width
        height: parent.height
        color: "transparent"

        ChartView{
            id: chartView
            title: chartName
            anchors.fill: parent
            antialiasing: true
            backgroundColor: "beige"

            ValueAxis {
                id: axisY
                gridVisible: true
                tickCount: 1
                min: 0
                max: 20
            }

            ValueAxis {
                id: axisX
                min: 0
                max: 20
                gridVisible: true
                tickCount: 1
            }

            LineSeries {
                id: lineSeries
                name: chartName
                axisX: axisX
                axisY: axisY

                XYPoint{x:0;y:0}


            }

        }


    }

    function appendData(x,y) {
        lineSeries.append(x,y)
    }
}
