import QtQuick 1.1
import com.jolla.components 1.0

// TODO: Maybe this could be part of components
Item {
    property real value
    height: 30


    GlassItem {
        id: progressBar
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width * 1.1
        height: 27
        ratio: 0.0
        dimmed: false
        radius: 0.0004
        falloffRadius: 0.106

    }

    GlassItem {
        id: highlight
        width: 40
        height: 40
        anchors.verticalCenter: progressBar.verticalCenter
        transformOrigin: Item.Center
        visible: parent.value > 0
        x: parent.width * parent.value
    }
}
