import QtQuick 1.1
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0


SilicaListView {
    id: rootList
    signal shareMethodClicked(string displayName, string userName, string methodId, string shareUIPath, int accountId)

    property alias listHeader: header.text
    property alias filter: transferMethodsModel.filter

    spacing: theme.paddingMedium
    model:  SailfishTransferMethodsModel { id: transferMethodsModel }

    delegate: BackgroundItem {
        id: backgroundItem
        width: rootList.width
        height: theme.itemSizeLarge

        Label {
            id: displayNameLabel
            text: displayName
            x:  theme.paddingLarge
            color: backgroundItem.down ? theme.highlightColor : theme.primaryColor
            anchors {
                verticalCenter: userName === "" ? parent.verticalCenter : undefined
                bottom: userName !== "" ? parent.verticalCenter: undefined
            }
        }

        SecondaryLabel {
            text: userName
            x:  theme.paddingLarge
            color: backgroundItem.down ? theme.highlightColor : theme.secondaryColor
            anchors {
                top: displayNameLabel.bottom
                topMargin: theme.paddingSmall
            }
        }

        onClicked: rootList.shareMethodClicked(displayName, userName, methodId, shareUIPath, accountId)
    }


    Label {
        id: header
        color: theme.highlightColor
        height: theme.itemSizeSmall
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: theme.fontSizeLarge

        anchors {
            top: parent.top
            topMargin: theme.itemSizeLarge - rootList.contentY
            right: parent.right
            rightMargin: theme.paddingLarge
        }
    }
}
