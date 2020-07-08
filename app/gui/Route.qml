// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9

Item {
    id: route
    x: 0
    y: 0
    property var path_points: []
    property var animation_dx: 0

    antialiasing: true
    width: parent.width

    anchors {
        top: parent.top
        bottom: parent.bottom
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

    function draw_path(points) {
        path_points = points
        route.width = 100*(points[points.length - 2] - points[0])
        route_canvas.requestPaint()
    }

    function updateAnimationX(x){
        animation_dx = x
        route_canvas.requestPaint()
    }

    function transformX(x){
        return 100*x - animation_dx
    }

    function transformY(y){
        return -100*y + parent.height * 0.6
    }

    function move(dx){
        route.x -= dx
    }
}
