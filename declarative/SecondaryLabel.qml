import QtQuick 1.1
import com.jolla.components 1.0

Label {
    property bool interactive

    font {
        pixelSize: theme.fontSizeMedium
        //family: theme.fontFamilyHeading
    }
    color: interactive ? theme.highlightColor : theme.secondaryColor
}
