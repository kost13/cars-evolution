import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Ewolucja Pojazdow")

    Rectangle {
        anchors.fill: parent

        Button {
            anchors.left: parent.left
            anchors.top: parent.top
            id: run
            width: 100
            height: 50
            onClicked:{
                AppInterface.startSimulation()          
            }
            Text { text: "Simulate" }
        }

        Button {
            anchors.left: parent.left
            anchors.top: run.bottom
            id: simulation_button
            width: 100
            height: 50
            onClicked:{
                timer_.restart()
            }
            Text { text: "Fall" }
        }
        Button {
            anchors.left: parent.left
            anchors.top: simulation_button.bottom
            id: stop
            width: 100
            height: 50
            onClicked:{
                timer_.stop()
            }
            Text { text: "Stop" }
        }

        Timer {
            id: timer_
             interval: 50
             running: false
             repeat: true
             onTriggered: updateCarPosition()
         }

        Car {
            id:car
            x: parent.width/2 - width/2
            y: 100
            antialiasing: true
            transform: Rotation { id: car_rotation; origin.x: car.horizontalCenter; origin.y: car.verticalCenter; angle: 0}
        }
    }

    function updateCarPosition() {
        var pos = AppInterface.getPosition()
        console.log(pos)
        if(pos[0] !== -1){
            car.y = pos[1]
            car_rotation.origin.y = car.verticalCenter
            car_rotation.angle = pos[2]

        }
    }

}
