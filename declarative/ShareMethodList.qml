import QtQuick 1.1
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0


SilicaListView {
    id: rootList
    signal shareMethodClicked(string displayName, string userName, string methodId, int accountId, bool accountRequired)
    property alias listHeader: header.text
    spacing: 10

    model:  SailfishTransferMethodsModel {id: transferMethodsModel }
    delegate: BackgroundItem {
        width: rootList.width
        height: 100

        Label {
            id: displayNameLabel
            text: displayName
            anchors.verticalCenter: userName === "" ? parent.verticalCenter : undefined
        }

        SecondaryLabel {
            text: accountRequired ? userName : ""
            anchors {
                top: displayNameLabel.bottom
                topMargin: 10
            }
        }

        onClicked: rootList.shareMethodClicked(displayName, userName, methodId, accountId, accountRequired)
    }


    Label {
        id: header
        color: theme.highlightColor
        height: theme.standardItemHeight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: theme.fontSizeLarge

        anchors {
            top: parent.top
            topMargin: 100 - rootList.contentY
            right: parent.right
            rightMargin: 24
        }
    }
}
