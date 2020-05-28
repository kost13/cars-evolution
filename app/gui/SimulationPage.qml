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
            spacing: 10
            Layout.preferredHeight: parent.height * 0.5
            Layout.alignment: Qt.AlignTop

            Layout.fillWidth: true

            MainMenu {
                id: main_menu
                Layout.alignment: Qt.AlignTop
            }

            ColumnLayout {

                SimulationWindow {
                    id: simulation_window

                    Layout.fillWidth: true
                    Layout.preferredHeight: row_layout.height * 0.9
                    Layout.alignment: Qt.AlignTop

                    onNewBestPosition: {
                        cars_distance_summary.cars_distance = xPosition
                    }

                    onNewGlobalBestPosition: {
                        cars_global_distance_summary.cars_distance = xPosition
                    }

                    onAnimationFinished: {
                        chartView.updateChart(bestCarIndex)
                        newSimulationTimer.start()
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
                    Text {
                        id: cars_global_distance_summary
                        property var cars_distance: 0
                        text: "Najlepszy dystans: " + cars_distance.toFixed(2)
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
                delegate: populationDelegate

                ScrollBar {
                    id: populationScrollBar
                    policy: ScrollBar.AsNeeded
                }

                Component {
                    id: populationDelegate

                    Row {
                        spacing: 5
                        width: parametersList.width
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
                                y: -100
                                car_num: model.number - 1
                            }
                        }
                    }
                }
            }

            ChartView {
                id: chartView
                title: "Parameters"

                antialiasing: true

                Layout.fillWidth: true
                Layout.fillHeight: true

                ValueAxis {
                    id: axisX
                    min: 1
                    titleText: "Simulation number"
                }

                ValueAxis {
                    id: axisY
                    titleText: "Parameter Values"
                }

                ScatterSeries {
                    id: car_rear_wheel_size
                    axisX: axisX
                    axisY: axisY                    
                    name: "Rear wheel"
                }

                ScatterSeries {
                    id: car_front_wheel_size
                    axisX: axisX
                    axisY: axisY
                    name: "Front wheel"
                }

                function updateChart(i){
                   var parameters = PopulationModel.parameters(i)
                   car_rear_wheel_size.append(generation_summary.generation_number, parameters[0])
                   car_front_wheel_size.append(generation_summary.generation_number, parameters[1])
                   axisX.max = generation_summary.generation_number + 1
                   axisY.max = Math.max(axisY.max, parameters[0]*1.2, parameters[1]*1.2)
                }
            }
        }
    }

    Timer {
        id: simulationTimer
        interval: 100
        running: false
        repeat: true
        onTriggered: simulation_window.updateCarPosition()
     }

    Timer {
        id: newSimulationTimer
        interval: 1000
        running: false
        repeat: false
        onTriggered: AppInterface.startSimulation()
     }
}
