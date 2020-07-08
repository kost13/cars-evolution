// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9

Rectangle {

    id: simulation_window

    property var car_objects: []    
    property var best_position: 0
    property var global_best_position: 0
    property var active_cars_num: 0

    signal newBestPosition(real xPosition)
    signal newGlobalBestPosition(real xPosition)
    signal animationFinished(int bestCarIndex)

    color: "#d3ecf6"
    border.color: "#000000"
    border.width: 2
    clip: true

    Route {
        id: route
    }

    function updateCarPosition() {
        var dx = 0
        var best_position_candidate = 0
        for(var i=0; i<car_objects.length; i++){
            if(!car_objects[i].active){
                continue
            }

            var pos = AppInterface.getPosition(i)
            if(pos[0] !== -1){
                var diff = car_objects[i].transformX(pos[0]) - simulation_window.width / 2
                if(diff > dx){
                    dx = diff
                }
                car_objects[i].move(pos)
                car_objects[i].no_progress_count = car_objects[i].no_progress_max_count

                if(pos[0] > best_position_candidate){
                    best_position_candidate = pos[0]
                }
            } else {
                car_objects[i].no_progress_count -= 1
                if(car_objects[i].no_progress_count <= 0){
                    car_objects[i].setActive(false)
                    active_cars_num -= 1

                    if(active_cars_num <= 0){
                        simulation_window.animationFinished(i)
                    }
                }
            }

        }

        if(best_position_candidate > best_position){
            best_position = best_position_candidate
            simulation_window.newBestPosition(best_position)

            if(best_position > global_best_position){
                global_best_position = best_position
                simulation_window.newGlobalBestPosition(global_best_position)
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

        active_cars_num = PopulationModel.rowCount()
        best_position = 0

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
