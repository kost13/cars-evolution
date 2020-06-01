// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    property var best_car: []
    property var best_distance: []

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
                        if(xPosition > cars_global_distance_summary.cars_distance){
                            cars_global_distance_summary.cars_distance = xPosition
                        }
                    }

                    onAnimationFinished: {
                        flush_best_car()
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

            ParametersListView {
                id: parametersList
                width: 120
                Layout.fillHeight: true
            }

            Chart {
                id: chartView
            }
        }
    }

    Connections {
        target: AppInterface
        onNewBestCar: {
            best_car.push(car_index)
            best_distance.push(distance)
        }
    }

    Timer {
        id: simulationTimer
        interval: 30
        running: false
        repeat: true
        onTriggered: simulation_window.updateCarPosition()
     }

    Timer {
        id: newSimulationTimer
        interval: 800
        running: false
        repeat: false
        onTriggered: {
            flush_best_car()
            AppInterface.startSimulation()
        }
     }


    function flush_best_car(){
        for(var i=0; i<best_car.length; ++i){
            chartView.updateChart(best_car[i])

            if(best_distance[i] > cars_global_distance_summary.cars_distance){
                cars_global_distance_summary.cars_distance = best_distance[i]
            }
        }
        best_car = []
        best_distance = []
    }

}
