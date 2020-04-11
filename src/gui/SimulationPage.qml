import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtCharts 2.2

Item {

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        RowLayout {
            id: row_layout
            anchors.fill: parent
            spacing: 10

            Layout.fillWidth: true;

            MainMenu {
                id: main_menu
                anchors.top: parent.top
            }

            ColumnLayout {

                SimulationWindow {
                    id: simulation_window

                    Layout.fillWidth: true
                    Layout.preferredHeight: 500

                    anchors.top: parent.top
                }

                RowLayout {
                    id: summary_layout
                    width: simulation_window.width
                    spacing: 6

                    Text { text: "Generacja: 10" }
                    Text { text: "Samochody: 10" }
                    Text { text: "Dystans: 10"   }
                }
            }
        }

        RowLayout {
            id: simulationInfoLayout            
            Layout.fillWidth: true;
            Layout.fillHeight: true

            ListView {
                width: 100
                implicitHeight: 400

                model: PopulationModel

                delegate: RowLayout {
                    width: parent.width

                    Text {
                        text: model.number
                        width: 20
                    }

                    Rectangle {
                        width: 30
                        height: 30
                        color: model.color
                    }

                    Button {
                        Text { text: "Preview" }
                    }
                }
            }


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
    }

    Timer {
        id: simulationTimer
        interval: 50
        running: false
        repeat: true
        onTriggered: simulation_window.updateCarPosition()
     }
}
