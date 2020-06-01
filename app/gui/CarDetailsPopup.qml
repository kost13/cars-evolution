import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Popup {

    width: 280
    height: 140
    padding: 0
    property var car_num: 0

    Car {
        id: carModel
        anchors.fill: parent
    }

    Component.onCompleted: {
        var parameters = PopulationModel.parameters(car_num)
        var color = PopulationModel.color(car_num)
        carModel.initialize(color, parameters)
        var size = carModel.shrinkToMinimumSize()
        width = size[0]
        height = size[1]
    }
}
