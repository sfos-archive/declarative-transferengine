import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0
import com.jolla.settings.accounts 1.0
import com.jolla.signonuiservice 1.0

SilicaListView {
    id: rootList

    property url source
    property variant content: ({})

    property alias filter: transferMethodsModel.filter

    property alias containerPage: accountCreator.endDestination
    property alias serviceFilter: accountCreator.serviceFilter
    property bool showAddAccount: true

    model: SailfishTransferMethodsModel { id: transferMethodsModel }

    width: parent.width
    height: Theme.itemSizeSmall * transferMethodsModel.count

    delegate: BackgroundItem {
        width: rootList.width

        Image {
            id: icon
            x: Theme.horizontalPageMargin
            anchors.verticalCenter: parent.verticalCenter
            source: model.accountIcon
        }

        Label {
            id: displayNameLabel
            // Plugins may provide translation id and the translation for the display name
            // This module already loads plugin translations so let's make sure that also
            // display name is translated if it contains the id..
            text: qsTrId(displayName)
            color: highlighted ? Theme.highlightColor : Theme.primaryColor
            truncationMode: TruncationMode.Fade
            anchors {
                left: icon.right
                leftMargin: Theme.paddingMedium
                verticalCenter: parent.verticalCenter
            }
            width: Math.min(implicitWidth, parent.width - x - Theme.horizontalPageMargin)
        }
        SecondaryLabel {
            text: userName
            color: highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
            truncationMode: TruncationMode.Fade
            anchors {
                left: displayNameLabel.right
                leftMargin: Theme.paddingSmall
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
                verticalCenter: parent.verticalCenter
            }
            visible: text.length > 0
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
    }
    footer: BackgroundItem {
        Image {
            id: addIcon
            x: Theme.horizontalPageMargin
            visible: showAddAccount
            anchors.verticalCenter: parent.verticalCenter
            source: "image://theme/icon-m-add" + (highlighted ? "?" + Theme.highlightColor : "")
        }

        Label {
            id: addAccountLabel
            //% "Add account"
            text: qsTrId("transferui-la-add_account")
            visible: showAddAccount
            anchors {
                left: addIcon.right
                leftMargin: Theme.paddingMedium
                verticalCenter: parent.verticalCenter
            }
            width: parent.width - x - Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            color: highlighted ? Theme.highlightColor : Theme.primaryColor
        }

        onClicked: {
            jolla_signon_ui_service.inProcessParent = containerPage
            accountCreator.startAccountCreation()
        }
    }

    SignonUiService {
        id: jolla_signon_ui_service
    }

    AccountCreationManager {
       id: accountCreator
    }
}
