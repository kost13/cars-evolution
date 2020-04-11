import QtQuick 2.9

Item {
    id: body
    antialiasing: true

    property var front_r: 30
    property var front_x: 60
    property var front_y: 160
    property var rear_r: 30
    property var rear_x: 200
    property var rear_y: 160
    property var polygon_points: []
    property var car_color: "#FF0000"

    function initialize(color, parameters){

        console.log('params: ' + parameters, 'color: ', color)

        car_color = color

        rear_r = parameters[0]
        front_r = parameters[1]

        rear_x = parameters[2]
        rear_y = parameters[3]

        front_x = parameters[4]
        front_y = parameters[5]

        polygon_points = []

        for(var i=2; i<parameters.length; i++){
            polygon_points.push(parameters[i])
        }

        car_canvas.requestPaint()
    }

    function move(new_position){
        body.x = new_position[0]
        body.y = new_position[1]
        car_rotation.angle = new_position[2]
    }

    transform: Rotation {
        id: car_rotation
        origin.x: parent.x + 140;
        origin.y: parent.y + 100;
        angle: 0
    }

    Canvas {
        id: car_canvas
        width: 280
        height: 140
        onPaint: {
            var ctx = getContext("2d");
            ctx.fillStyle = Qt.rgba(1, 0, 0, 1);
            ctx.strokeRect(0, 0, width, height);

            // wheels
            ctx.beginPath()
            ctx.arc(front_x+50, height-front_y, front_r, 0, 2*Math.PI, false)
            ctx.arc(rear_x+50, height-rear_y, rear_r, 0, 2*Math.PI, false)
            ctx.fillStyle = Qt.rgba(0.25, 0.25, 0.25, 1);
            ctx.fill()
            ctx.closePath()

            //body
            ctx.beginPath()
            ctx.moveTo(polygon_points[0]+50, height-polygon_points[1])
            for(var i=2; i<polygon_points.length; i+=2){
                ctx.lineTo(polygon_points[i]+50, height-polygon_points[i+1])
            }
            ctx.lineTo(polygon_points[0]+50, height-polygon_points[1])

            ctx.fillStyle = car_color
            ctx.fill()
            ctx.closePath()
        }
    }
}
