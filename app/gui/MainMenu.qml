// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    width: 100

    Column {
        anchors {
            fill: parent
        }

        spacing: 2

        Button {
            id: run_button
            width: parent.width

            Text {
                text: "Symulacja"
                font.pixelSize: 20
                anchors.centerIn: parent
            }

            onClicked: {
                flush_best_car()
                AppInterface.startSimulation()                
            }
        }

        Button {
            id: simulation_button
            width: parent.width

            Text {
                text: "Start"
                font.pixelSize: 20
                anchors.centerIn: parent
            }
            onClicked: {                
                simulationTimer.restart()
                simulation_button.enabled = false
            }
        }

        Button {
            id: resume_button
            width: parent.width

            Text {
                text: "Wznów"
                font.pixelSize: 20
                anchors.centerIn: parent
            }
            onClicked: {
                simulationTimer.restart()
            }
        }

        Button {
            id: stop_button
            width: parent.width

            Text {
                text: "Stop"
                font.pixelSize: 20
                anchors.centerIn: parent
            }
            onClicked: simulationTimer.stop()
        }
    }

    Connections {
        target: PopulationModel
        onModelReset: {
            simulation_window.loadCars()
            cars_number_summmary.cars_number = PopulationModel.rowCount()
            simulation_window.loadRoute()
        }

    }
}
