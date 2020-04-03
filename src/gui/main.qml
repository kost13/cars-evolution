import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
//import QtQuick.Controls.Material 2.2


ApplicationWindow {

//    Material.theme: Material.Dark

    id: window
    visible: true
    width: 1400
    height: 1000
    title: qsTr("Ewolucja Pojazdow")

    header : ToolBar {

//        background: Rectangle {
//                color: "yellow"
//            }


        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: qsTr("Close")
                onClicked: Qt.quit()
            }
        }
    }

    footer: Footer {
        id: footer_
    }


    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        MainMenu {
            id: main_menu
            anchors.top: parent.top
        }

        SimulationWindow {
            id: simulation_window

            Layout.fillWidth: true
            Layout.preferredHeight: 500

            anchors.top: parent.top
        }
    }

    Timer {
        id: timer_
        interval: 50
        running: false
        repeat: true
        onTriggered: simulation_window.updateCarPosition()
     }
}
