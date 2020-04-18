import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ToolBar {

//        background: Rectangle {
//                color: "yellow"
//            }

    signal pageSelected(string page)

    RowLayout {
        anchors.fill: parent

        ToolbarButton {
            text: qsTr("Symulacja")
            onClicked: pageSelected("simulation")
        }

        ToolbarButton {
            text: qsTr("Ustawienia")
            onClicked: pageSelected("settings")
        }

        ToolbarButton {
            text: qsTr("Algorytm")
            onClicked: pageSelected("algorithm")
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ToolbarButton {
            anchors.right: parent.right
            text: qsTr("Zamknij")
            onClicked: Qt.quit()
        }
    }
}
