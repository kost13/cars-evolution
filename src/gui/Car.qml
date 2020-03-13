import QtQuick 2.0

Item {
    Rectangle {
         id: body
         width: 150
         height: 50
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.top: parent.top
         color: "#bb0000"
    }

    Rectangle {
         id: front_wheel
         width: 50
         height: 50
         anchors {
             left: body.right
             leftMargin: -width*0.8
             top: body.bottom
             topMargin: -width*0.5

         }
         color: "#095e7b"
         radius: width*0.5
    }

    Rectangle {
         id: rear_wheel          
         width: 50
         height: 50
         anchors {
             right: body.left
             rightMargin: -width*0.8
             top: body.bottom
             topMargin: -width*0.5

         }
         color: "#00aa00"
         radius: width*0.5
    }



}
