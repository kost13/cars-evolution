import QtQuick 2.9

Rectangle {

    id: simulation_window

    property var car_objects: []    

    color: "#d3ecf6"
    border.color: "#000000"
    border.width: 2
    clip: true

    Route {
        id: route
    }

    function updateCarPosition() {
        var dx = 0
        for(var i=0; i<car_objects.length; i++){
            var pos = AppInterface.getPosition(i)
            if(pos[0] !== -1){
                var diff = car_objects[i].transformX(pos[0]) - simulation_window.width / 2
                if(diff > dx){
                    dx = diff
                }
                car_objects[i].move(pos)
//                chartView.updateChart(i, pos[0], pos[1])
            }
        }

        if(dx > 0){
            for(var i=0; i<car_objects.length; i++){
                    car_objects[i].animation_dx += dx
            }
            route.move(dx)
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
        for(var i=0; i<PopulationModel.rowCount(); i++){
            var car = car_component.createObject(simulation_window);            
            var parameters = PopulationModel.parameters(i)
            var color = PopulationModel.color(i)
            car.initialize(color, parameters)
            car_objects.push(car)
        }
    }

    function loadRoute(){
        var points = AppInterface.getRoute()
        route.x = 0
        route.draw_path(points)

    }
}
