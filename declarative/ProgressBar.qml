import QtQuick 1.1
import Sailfish.Silica 1.0

// TODO: Maybe this could be part of components
Item {
    property real value
    property color color: theme.primaryColor
    height: theme.paddingLarge


    GlassItem {
        id: progressBar
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width * 1.1
        height: parent.height
        ratio: 0.0
        dimmed: false
        radius: 0.0004
        falloffRadius: 0.106
        color: parent.color
    }

    GlassItem {
        id: highlight
        width: theme.itemSizeSmall / 2
        height: theme.itemSizeSmall
        anchors.verticalCenter: progressBar.verticalCenter
        transformOrigin: Item.Center
        visible: parent.value > 0
        x: parent.width * parent.value
    }
}
