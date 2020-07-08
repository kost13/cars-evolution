// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtCharts 2.2

ChartView {
    title: "Parametery Pojazdów"

    property int generation: 0

    antialiasing: true

    Layout.fillWidth: true
    Layout.fillHeight: true

    ValueAxis {
        id: axisX
        min: 1
        titleText: "Generacja"
    }

    ValueAxis {
        id: axisY
        titleText: "Wartości parametrów"
    }

    ScatterSeries {
        id: car_rear_wheel_size
        axisX: axisX
        axisY: axisY
        name: "Promień tylnego koła"
    }

    ScatterSeries {
        id: car_front_wheel_size
        axisX: axisX
        axisY: axisY
        name: "Promień przedniego koła"
    }

    function updateChart(i){
       var parameters = PopulationModel.parameters(i)
       generation += 1
       car_rear_wheel_size.append(generation, parameters[0])
       car_front_wheel_size.append(generation, parameters[1])
       axisX.max = generation + 1
       axisY.max = Math.max(axisY.max, parameters[0]*1.2, parameters[1]*1.2)
    }
}
