import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    id: window
    visible: true
    width: 1400
    height: 1000
    title: qsTr("Ewolucja Pojazdow")

    header : HeaderToolbar {
        id: headerToolbar
        Component.onCompleted: {
            headerToolbar.pageSelected.connect(setPage)
        }
    }

    footer: Footer {
        id: footerBar
    }

    StackView {
        id: stack
        initialItem: simulationView
        anchors.fill: parent
    }

    SimulationPage {
        id: simulationView
    }

    SettingsPage {
        id: settingsView
    }

    AlgorithmPage {
        id: algorithmView
    }   

    function setPage(page){
        if(stack.depth > 1){
            if(page === "simulation"){
                stack.pop()
            } else if(page === "settings"){
                stack.replace(settingsView)
            } else if(page === "algorithm"){
                stack.replace(algorithmView)
            }
        } else {
            if(page === "settings"){
                stack.push(settingsView)
            } else if(page === "algorithm"){
                stack.push(algorithmView)
            }
        }
    }
}
