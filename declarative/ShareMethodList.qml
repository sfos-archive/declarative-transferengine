import QtQuick 1.1
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0

SilicaListView {
    id: rootList

    property url source
    property variant content: ({})

    property alias listHeader: header.text
    property alias filter: transferMethodsModel.filter

    spacing: theme.paddingMedium
    model: SailfishTransferMethodsModel { id: transferMethodsModel }

    width: parent.width
    height: theme.itemSizeMedium * transferMethodsModel.count

    delegate: BackgroundItem {
        id: backgroundItem

        width: rootList.width
        height: theme.itemSizeMedium

        Column {
            width: parent.width
            anchors.centerIn: parent
            Label {
                id: displayNameLabel
                // Plugins may provide translation id and the translation for the display name
                // This module already loads plugin translations so let's make sure that also
                // display name is translated if it contains the id..
                text: qsTrId(displayName)
                x: theme.paddingLarge
                color: backgroundItem.highlighted ? theme.highlightColor : theme.primaryColor
                width: parent.width
                truncationMode: TruncationMode.Elide
            }
            SecondaryLabel {
                text: userName
                x: theme.paddingLarge
                color: backgroundItem.highlighted ? theme.highlightColor : theme.secondaryColor
                truncationMode: TruncationMode.Elide
                width: parent.width
                visible: text.length > 0
            }
        }

        onClicked: {
            pageStack.push(shareUIPath, {
                               source: rootList.source,
                               content: rootList.content,
                               methodId: methodId,
                               displayName: displayName,
                               accountId: accountId,
                               accountName: userName
                           })
        }
        Component.onCompleted: {
            if (model.index === 0) {
                header.parent = backgroundItem
            }
        }
    }
    Label {
        id: header

        color: theme.highlightColor
        height: theme.itemSizeMedium
        verticalAlignment: Text.AlignVCenter
        anchors {
            right: parent.right
            rightMargin: theme.paddingLarge
        }
    }
}
