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
    property var animation_dx: 0
    readonly property int no_progress_max_count: 5
    property int no_progress_count: no_progress_max_count
    property var active: true

    function setActive(active_flag){
        active = active_flag
        visible = false
    }

    function initialize(color, parameters){

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

        find_boundires()

        car_rotation.origin.x = rear_x + 50
        car_rotation.origin.y = car_canvas.height-rear_y-50

        car_canvas.requestPaint()
    }

    function find_boundires(){
        var min_x = 1000
        var max_x = 0
        var min_y = 1000
        var max_y = 0
        for(var i=0; i<polygon_points.length; ++i){
            if(polygon_points[i] < min_x){
                min_x = polygon_points[i]
            } else if(polygon_points[i] > max_x){
                max_x = polygon_points[i]
            }
            ++i
            if(polygon_points[i] < min_y){
                min_y = polygon_points[i]
            } else if(polygon_points[i] > max_y){
                max_y = polygon_points[i]
            }
        }

        car_canvas.width = 2 * (max_x - min_x + 50)
        car_canvas.height = 2 * (max_y - min_y + 50)  
    }

    function transformX(x){
        return 100*x - animation_dx - car_rotation.origin.x
    }

    function transformY(y){
        return -100*y + parent.height * 0.6 - car_rotation.origin.y
    }

    function move(new_position){
        body.x = transformX(new_position[0])
        body.y = transformY(new_position[1])        
        car_rotation.angle = new_position[2]
    }

    transform: Rotation {
        id: car_rotation
        origin.x: parent.x;
        origin.y: parent.y;
        angle: 0
    }

    Canvas {
        id: car_canvas

        onPaint: {
            var ctx = getContext("2d");

//            ctx.fillStyle = Qt.rgba(1, 0, 0, 1);
//            ctx.strokeRect(0, 0, width, height);

            // wheels
            ctx.beginPath()
            ctx.arc(front_x+50, height-front_y-50, front_r, 0, 2*Math.PI, false)
            ctx.arc(rear_x+50, height-rear_y-50, rear_r, 0, 2*Math.PI, false)
            ctx.fillStyle = Qt.rgba(0.25, 0.25, 0.25, 1);
            ctx.fill()
            ctx.closePath()

            //body
            ctx.beginPath()
            ctx.moveTo(polygon_points[0]+50, height-polygon_points[1]-50)
            for(var i=2; i<polygon_points.length; i+=2){
                ctx.lineTo(polygon_points[i]+50, height-polygon_points[i+1]-50)
            }
            ctx.lineTo(polygon_points[0]+50, height-polygon_points[1]-50)

            ctx.fillStyle = car_color
            ctx.fill()
            ctx.closePath()
        }
    }
}
