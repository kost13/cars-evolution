import QtQuick 2.9

Item {    
    height: 30

    Rectangle {
        anchors.fill: parent
        color: "#3f51b5"

            Text {
                id: computations_text
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
                id: loading_icon
                height: parent.height*0.7
                width: parent.height*0.7
                fillMode: Image.PreserveAspectFit
                source: "img/icon_loading.png"
                anchors {
                    left: computations_text.right
                    leftMargin: 5
                    bottom: parent.bottom
                    bottomMargin: 5

                }

                RotationAnimator {
                       id: loading_animation
                       target: loading_icon;
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
    }

    function computationsOn(){
        computations_text.visible = true
        loading_icon.visible = true
        loading_animation.start()
    }

    function computationsOff(){
        computations_text.visible = false
        loading_icon.visible = false
        loading_animation.stop()

    }
}
