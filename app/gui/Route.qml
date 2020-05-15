import QtQuick 2.9

Item {

    id: route
    x: 0
    y: 0

    antialiasing: true


    width: parent.width
    anchors {
        top: parent.top
        bottom: parent.bottom
    }

    property var path_points: []
    property var animation_dx: 0

    function draw_path(points) {
        path_points = points
        route_canvas.requestPaint()
    }

    function updateAnimationX(x){
        animation_dx = x
        route_canvas.requestPaint()
    }

    function transformX(x){
        return 10*x - animation_dx
    }

    function transformY(y){
        return 8*y + 200;
    }

    function move(dx){
        route.x -= dx
    }

    Canvas {
        id: route_canvas
        anchors.fill: parent

        onPaint: {

            var ctx = getContext("2d");

            if(path_points.length < 3){
                return
            }

            ctx.moveTo(transformX(path_points[0]), transformY(path_points[1]))


            for(var i=2; i<path_points.length; ++i){
                ctx.lineTo(transformX(path_points[i]), transformY(path_points[++i]))
            }

            ctx.lineWidth = 5
            ctx.strokeStyle = "#545454"
            ctx.stroke()
        }
    }
}
