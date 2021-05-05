/****************************************************************************************
**
** Copyright (c) 2013 - 2021 Jolla Ltd.
** Copyright (c) 2021 Open Mobile Platform LLC.
** All rights reserved.
**
** License: Proprietary.
**
****************************************************************************************/
import QtQuick 2.6
import Sailfish.Silica 1.0

BackgroundItem {
    id: root

    height: nameLabel.height + usernameLabel.height + Theme.paddingSmall*2
    _showPress: false

    HighlightImage {
        id: icon

        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
        source: model.accountIcon
        width: Theme.iconSizeMedium
        height: Theme.iconSizeMedium
        sourceSize.width: Theme.iconSizeMedium
        sourceSize.height: Theme.iconSizeMedium
    }

    Label {
        id: nameLabel

        anchors {
            left: icon.right
            leftMargin: Theme.paddingLarge
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: icon.verticalCenter
            verticalCenterOffset: usernameLabel.text.length > 0 ? -usernameLabel.height/2 : 0
        }
        truncationMode: TruncationMode.Fade
        text: qsTrId(model.displayName)
    }

    Label {
        id: usernameLabel

        anchors {
            top: nameLabel.bottom
            left: icon.right
            leftMargin: Theme.paddingLarge
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }
        truncationMode: TruncationMode.Fade
        text: model.userName
        font.pixelSize: Theme.fontSizeExtraSmall
        color: highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
    }

}
