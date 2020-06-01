// module: GUI
// author: Lukasz Kostrzewa

import QtQuick 2.9
import QtQuick.Controls 2.2

ToolButton {
    onHoveredChanged: hovered ? opacity = 0.8 : opacity = 1
    anchors {
        leftMargin: 5
        rightMargin: 5
    }
}
