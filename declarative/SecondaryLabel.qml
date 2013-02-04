import QtQuick 1.1
import Sailfish.Silica 1.0

Label {
    property bool interactive
    font.pixelSize: theme.fontSizeMedium
    color: interactive ? theme.highlightColor : theme.secondaryColor
}
