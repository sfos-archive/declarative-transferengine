import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0
import org.nemomobile.transferengine 1.0

CoverBackground {
    id: root

    property int displayedTransferId: -1
    property int transfersCount: transferModel.count
    property variant transferObject: transferModel.transfersInProgress > 0
                                     ? transferModel.get(0)
                                     : undefined

    onTransfersCountChanged: reloadDisplayedItem()

    function reloadDisplayedItem() {
        if (transfersCount == 0) {
            root.displayedTransferId = -1
            return
        }
        for (var i=0; i<transfersCount; i++) {
            var transfer = transferModel.get(i)
            if (transfer.status === TransferModel.NotStarted || transfer.status === TransferModel.TransferStarted) {
                root.displayedTransferId = transfer.transferId
                return
            }
        }
        root.displayedTransferId = -1
    }

    function getTransferIcon(transferType)
    {
        // TODO: How we figure out if upload/download is from device2device e.g. BT.
        switch (transferType) {
        case TransferModel.Upload:
            return "image://theme/icon-s-cloud-upload"
        case TransferModel.Download:
            return "image://theme/icon-s-cloud-download"
        case TransferModel.Sync:
            return "image://theme/icon-s-sync"
        default:
            console.log("TransfersPage::transferIcon: failed to get transfer type")
            return ""
        }
    }

    function getFileName(url)
    {
        var splitString = url.split("/")
        return splitString[splitString.length - 1]
    }

    CoverActionList {
        id: multipleActionsList
        enabled: root.displayedTransferId >= 0 && transferModel.transfersInProgress == 1

        CoverAction {
            iconSource: "image://theme/icon-cover-transfers"
            onTriggered: {
                window.showSettingsPage("transferui/mainpage.qml")
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-cancel"
            onTriggered: {
                transferInterface.cbCancelTransfer(root.displayedTransferId)
            }
        }
    }

    CoverActionList {
        enabled: transferModel.transfersInProgress > 1

        CoverAction {
            iconSource: "image://theme/icon-cover-transfers"
            onTriggered: {
                window.showSettingsPage("transferui/mainpage.qml")
            }
        }
    }

    SailfishTransferInterface {
        id: transferInterface
    }


    Item {
        property int status: transferObject ? transferObject.status : -1

        width: root.width; height: root.height
        visible: transferObject.transferId === root.displayedTransferId

        onStatusChanged: {
            if (transferObject && transferObject.transferId === root.displayedTransferId
                    && (status !== TransferModel.NotStarted || status !== TransferModel.TransferStarted)) {
                    root.reloadDisplayedItem()
            }
        }

        Item {
            id: icon
            y: Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            width: Theme.itemSizeLarge; height: Theme.itemSizeLarge

            ProgressCircle {
                anchors.fill: parent
                value: transferObject.progress
            }

            Image {
                anchors.centerIn: parent
                source: root.getTransferIcon(transferObject.transferType)
            }
        }

        Column {
            anchors {
                top: icon.bottom
                topMargin: Theme.paddingLarge
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width - Theme.paddingLarge*2

            Label {
                width: parent.width
                truncationMode: TruncationMode.Fade
                text: transferModel.transfersInProgress == 1 ? root.getFileName(transferObject.url) : transferModel.transfersInProgress
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.highlightColor
                horizontalAlignment: transferModel.transfersInProgress == 1 ? Text.AlignLeft : Text.AlignHCenter
            }
        }
    }
}
