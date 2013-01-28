import QtQuick 1.1
import com.jolla.components 1.0
import Sailfish.Silica.TransferEngine 1.0

JollaListView {
    id: rootList
    signal shareMethodClicked(string displayName, string userName, string methodId, int accountId, bool accountRequired)
    property alias listHeader: header.text


    model:  SilicaTransferMethodsModel {id: transferMethodsModel }
    delegate: BackgroundItem {
        width: parent.width
        height: Math.max(container.height * 1.1, theme.pageHeaderHeight)

        Column {
            id: container
            x: 5
            spacing: 10

            Label {
                text: displayName
                color: theme.highlightColor
                font {
                    family: theme.fontFamilyHeading
                    pixelSize: theme.fontSizeLarge
                }
            }

            Label {
                text: accountRequired ? userName : ""
            }
        }

        onClicked: rootList.shareMethodClicked(displayName, userName, methodId, accountId, accountRequired)
    }

    Label {
        id: header
        color: theme.highlightColor
        height: theme.standardItemHeight
        verticalAlignment: Text.AlignVCenter
        anchors {
            top: parent.top
            topMargin: theme.pageHeaderHeight - rootList.contentY
            right: parent.right
            rightMargin: 24
        }
    }
}
