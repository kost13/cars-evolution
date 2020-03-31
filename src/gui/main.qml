import QtQuick 2.9
import QtQuick.Controls 1.2

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Ewolucja Pojazdow")

    property var car_objects: []

    SimulationWindow {
        id: simulation_window
        anchors {
            right: parent.right
            top: parent.top
            topMargin: 5
            rightMargin: 5
        }
    }

    Column {
        anchors {
            top: parent.top
            left: parent.left
            topMargin: 5
            leftMargin: 5
        }

        spacing: 10

        Button {
            id: run_button
            width: 100
            height: 50
            Text {
                text: "Simulate"
                font.pointSize: 24
                anchors.centerIn: parent
            }
            onClicked: AppInterface.startSimulation()
        }
        Button {
            id: simulation_button
            width: 100
            height: 50
            Text {
                text: "Fall"
                font.pointSize: 24
                anchors.centerIn: parent
            }
            onClicked: timer_.restart()
        }
        Button {
            id: stop_button
            width: 100
            height: 50
            Text {
                text: "Stop"
                font.pointSize: 24
                anchors.centerIn: parent
            }
            onClicked: timer_.stop()
        }
        Button {
            id: populate_button
            width: 100
            height: 50
            Text {
                text: "Populate"
                font.pointSize: 24
                anchors.centerIn: parent
            }
            onClicked: loadCars()
        }

    }


    Timer {
        id: timer_
         interval: 50
         running: false
         repeat: true
         onTriggered: updateCarPosition()
     }

    function updateCarPosition() {
        for(var i=0; i<car_objects.length; i++){
            var pos = AppInterface.getPosition(i)
            if(pos[0] !== -1){
                car_objects[i].move(pos)
            }
        }
    }

    function clearCars(){
        for(var i=0; i<car_objects.length; i++){
            car_objects[i].destroy()
        }
        car_objects = []
    }

    function loadCars(){
        clearCars()
        var cars = AppInterface.getCars()
        var car_component = Qt.createComponent("Car.qml");
        for(var i=0; i<cars.length; i++){
            var car = car_component.createObject(simulation_window);
            car.initialize(cars[i])
            car_objects.push(car)
        }
    }
}
