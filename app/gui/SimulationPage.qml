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
                    Layout.preferredHeight: 300

                    anchors.top: parent.top

                    onNewBestPosition: {
                        cars_distance_summary.cars_distance = xPosition
                    }
                }

                RowLayout {
                    id: summary_layout
                    width: simulation_window.width
                    spacing: 6

                    Text {
                        id: generation_summary
                        property var generation_number: 0
                        text: "Generacja: " + generation_number
                    }
                    Text {
                        id: cars_number_summmary
                        property var cars_number: 0
                        text: "Samochody: " + cars_number
                    }
                    Text {
                        id: cars_distance_summary
                        property var cars_distance: 0
                        text: "Dystans: " + cars_distance.toFixed(2)
                    }
                }

                Connections {
                    target: AppInterface
                    onSimulationEnded: {
                        generation_summary.generation_number += 1
                    }
                }
            }
        }

        RowLayout {
            id: simulationInfoLayout            
            Layout.fillWidth: true;
            Layout.fillHeight: true

            clip: true

            ListView {                
                id: parametersList

                width: 120
                Layout.fillHeight: true

                interactive: false
                ScrollBar.vertical: populationScrollBar

                header: Text {
                    text: "Populacja"
                    height: 20
                }

                model: PopulationModel
                delegate: populatioDelegate

                ScrollBar {
                    id: populationScrollBar
                    policy: ScrollBar.AsNeeded
                }

                Component {
                    id: populatioDelegate

                    Row {
                        spacing: 5
                        width: parent.width
                        height: 35

                        Text {
                            width: 12
                            text: model.number
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Rectangle {
                            width: 30
                            height: 30
                            color: model.color
                             anchors.verticalCenter: parent.verticalCenter
                        }

                        Button {
                            text: ""
                            width: 40
                            height: 30

                            background: Image {
                                id: image
                                source: "qrc:/gui/img/icon_preview.png"
                                fillMode: Image.PreserveAspectFit
                                anchors.centerIn: parent
                                height: parent.height
                                width:  parent.height
                            }

                            onHoveredChanged: hovered ? popup.open() : popup.close()

                            CarDetailsPopup {
                                id: popup
                                x: 30
                                y: 0
                                car_num: model.number - 1
                            }
                        }
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
