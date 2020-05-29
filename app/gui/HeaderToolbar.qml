import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0

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
            id: populationButton
            text: qsTr("Populacja")
            onClicked: populationMenu.open()

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
            Layout.alignment: Qt.AlignRight
            text: qsTr("Zamknij")
            onClicked: Qt.quit()
        }

        Menu {
            id: populationMenu
            y: parent.height
            x: populationButton.x

            MenuItem {
                text: "Zapisz"
                onClicked: saveFileDialog.open()
            }
            MenuItem {
                text: "Wczytaj"
                onClicked: openFileDialog.open()
            }
        }
    }

    FileDialog {
        id: saveFileDialog
        title: "Save Population Parameters"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: false
        nameFilters: [ "JSON files (*.json)", "Text files (*.txt)" ]
        onAccepted: AppInterface.savePopulation(saveFileDialog.fileUrl)

    }

    FileDialog {
        id: openFileDialog
        title: "Open Population Parameters"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: true
        nameFilters: [ "JSON files (*.json)", "Text files (*.txt)" ]
        onAccepted: AppInterface.loadPopulation(openFileDialog.fileUrl)
    }
}
