// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9

Item {    
    height: 30

    Rectangle {
        anchors.fill: parent
        color: "#3f51b5"

        Text {
            id: computationsText
            text: "Running computations"
            color: "white"
            font.pixelSize: 16
            anchors {
                left: parent.left
                bottom: parent.bottom
                bottomMargin: 5
                leftMargin: 5
            }
            visible: false
        }

        Image {
            id: loadingIcon
            height: parent.height*0.7
            width: parent.height*0.7
            fillMode: Image.PreserveAspectFit
            source: "img/icon_loading.png"
            anchors {
                left: computationsText.right
                leftMargin: 5
                bottom: parent.bottom
                bottomMargin: 5

            }

            RotationAnimator {
               id: loadingAnimation
               target: loadingIcon;
               from: 0;
               to: 360;
               duration: 1000
               running: false
               loops: Animation.Infinite
           }

           visible: false
        }
    }

    Connections {
        target: AppInterface
        onSimulationEnded: {
            computationsOff()
        }
        onSimulationStarted: {
            computationsOn()
        }
    }

    function computationsOn(){
        computationsText.visible = true
        loadingIcon.visible = true
        loadingAnimation.start()
    }

    function computationsOff(){
        computationsText.visible = false
        loadingIcon.visible = false
        loadingAnimation.stop()
    }
}
