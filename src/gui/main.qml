import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtCharts 2.2
import QtQuick.Controls.Material 2.2


ApplicationWindow {

//    Material.theme: Material.Dark

    id: window
    visible: true
    width: 1400
    height: 1000
    title: qsTr("Ewolucja Pojazdow")

    header : ToolBar {

//        background: Rectangle {
//                color: "yellow"
//            }

        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: qsTr("Close")
                onClicked: Qt.quit()
            }
        }
    }

    footer: Footer {
        id: footer_
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        RowLayout {
            id: row_layout
            anchors.fill: parent
//            anchors.margins:
            spacing: 10

            Layout.fillWidth: true;

            MainMenu {
                id: main_menu
                anchors.top: parent.top
            }

            SimulationWindow {
                id: simulation_window

                Layout.fillWidth: true
                Layout.preferredHeight: 500

                anchors.top: parent.top
            }
        }

        //![1]
        ChartView {
            id: chartView
            title: "Cars position"
            legend.visible: false
            antialiasing: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            ValueAxis {
                id: axisX
                titleText: "X"
            }

            ValueAxis {
                id: axisY
                titleText: "Y"
            }

            LineSeries {
                id: s1
                axisX: axisX
                axisY: axisY
            }

            LineSeries {
                id: s2
                axisX: axisX
                axisY: axisY
            }

            function updateChart(i, x, y){
                console.log(i, x, y)
                if(i === 0){
                    s1.append(x,y)
                    chartView.axisX(s1).max = x;
                    chartView.axisY(s1).max = y;
                } else if(i === 1){
                    s2.append(x,y)
                    chartView.axisX(s2).max = x;
                    chartView.axisY(s2).max = y;
                }
            }
        }
    }

    Timer {
        id: timer_
        interval: 50
        running: false
        repeat: true
        onTriggered: simulation_window.updateCarPosition()
     }
}
