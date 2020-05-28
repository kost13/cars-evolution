import QtQuick 2.9
import QtQuick.Controls 2.2

Component {

    Rectangle {
        width: 600
        height: 600      

        ListView {

            anchors.fill: parent
            anchors.margins: 10

            header: Text {
                text: "Ustawienia algorytmu ewolucyjnego"
                font.pointSize: 20
                anchors.margins: 10
                anchors.bottomMargin: 30
            }

            model: EvolutionSettingsModel
            delegate: settingsDelegate
            interactive: false

            Component {
                   id: settingsDelegate
                   Row {
                       id: rowWrapper
                       spacing: 10
                       anchors.margins: 10
                       height: 30



                       Text {
                           width: 260
                           height: parent.height
                           text: model.name
                           font.pointSize: 12
                       }

                       TextInput {
                           width: 50
                           validator: DoubleValidator {
                               bottom: 0.0
                               top: 10.0
                               notation: DoubleValidator.StandardNotation
                               locale: "us"
                           }
                           height: parent.height
                           text: model.value
                           font.pointSize: 12
                           onAccepted: model.value = text
                       }

                       Text {
                           height: parent.height
                           text: model.description
                           font.pointSize: 10
                       }
                   }
               }
        }
    }
}
