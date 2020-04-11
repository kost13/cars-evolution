import QtQuick 2.9

Rectangle {

    property var car_objects: []

    color: "#d3ecf6"
    border.color: "#000000"
    border.width: 2

    function updateCarPosition() {
        for(var i=0; i<car_objects.length; i++){
            var pos = AppInterface.getPosition(i)
            if(pos[0] !== -1){
                car_objects[i].move(pos)
                chartView.updateChart(i, pos[0], pos[1])
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
        var car_component = Qt.createComponent("Car.qml");
        var cars = AppInterface.getCars()
        for(var i=0; i<cars.length; i++){
            var car = car_component.createObject(simulation_window);
            car.initialize(cars[i])
            car_objects.push(car)
        }
    }
}
