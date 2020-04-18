import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.0

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
                text: "Simulate"
                font.pixelSize: 20
                anchors.centerIn: parent
            }

            onClicked: {
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
                text: "Resume"
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

        Button {
            id: savePopulationButton
            width: parent.width

            Text {
                text: "Save\npopulation"
                font.pixelSize: 14
                anchors.centerIn: parent
            }
            onClicked: saveFileDialog.open()
        }

        Button {
            id: loadPopulationButton
            width: parent.width

            Text {
                text: "Load\npopulation"
                font.pixelSize: 14
                anchors.centerIn: parent
            }
            onClicked: openFileDialog.open()
        }
    }

    Connections {
        target: PopulationModel
        onModelReset: {
            simulation_window.loadCars()
        }

    }

    FileDialog {
        id: saveFileDialog
        title: "Save Population Parameters"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: false
        nameFilters: [ "JSON files (*.json)", "Text files (*.txt)" ]
        onAccepted: {
            var status = AppInterface.savePopulation(saveFileDialog.fileUrl)
        console.log("save file dialog", status)
        }
    }

    FileDialog {
        id: openFileDialog
        title: "Open Population Parameters"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: true
        nameFilters: [ "JSON files (*.json)", "Text files (*.txt)" ]
        onAccepted: AppInterface.loadPopulation(openFileDialog.fileUrl)
//        Component.onCompleted: visible = true
    }


}
