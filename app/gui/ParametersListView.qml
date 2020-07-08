// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9
import QtQuick.Controls 2.2

ListView {
    interactive: false
    ScrollBar.vertical: populationScrollBar

    header: Text {
        text: "Populacja"
        height: 20
    }

    model: PopulationModel
    delegate: populationDelegate

    ScrollBar {
        id: populationScrollBar
        policy: ScrollBar.AsNeeded
    }

    Component {
        id: populationDelegate

        Row {
            spacing: 5
            width: parametersList.width
            height: 35

            Text {
                width: 12
                text: model.number
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                width: 30
                height: 30
                color: model.color
                 anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                text: ""
                width: 40
                height: 30

                background: Image {
                    id: image
                    source: "qrc:/gui/img/icon_preview.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    height: parent.height
                    width:  parent.height
                }

                onHoveredChanged: hovered ? popup.open() : popup.close()

                CarDetailsPopup {
                    id: popup
                    x: 30
                    y: -100
                    car_num: model.number - 1
                }
            }
        }
    }
}
