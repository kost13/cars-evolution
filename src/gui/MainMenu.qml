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
                text: "Simulate"
                font.pixelSize: 20
                anchors.centerIn: parent
            }

            onClicked: {
                AppInterface.startSimulation()
                footer_.computationsOn()
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
                simulation_window.loadCars()
                timer_.restart()
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
                timer_.restart()
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
            onClicked: timer_.stop()
        }



    }
}
