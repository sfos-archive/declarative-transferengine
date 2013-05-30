import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.theme 1.0
import Sailfish.TransferEngine 1.0

Item {
    id: root

    property int displayedTransferId: -1

    anchors.fill: parent

    function getTransferIcon(transferType)
    {
        // TODO: How we figure out if upload/download is from device2device e.g. BT.
        switch (transferType) {
        case SailfishTransferModel.Upload:
            return "image://theme/icon-s-cloud-upload"
        case SailfishTransferModel.Download:
            return "image://theme/icon-s-cloud-download"
        case SailfishTransferModel.Sync:
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

    SailfishTransferModel {
        id: transferModel

        function _reloadDisplayedItem() {
            if (count == 0) {
                root.displayedTransferId = -1
                return
            }
            for (var i=0; i<count; i++) {
                var transfer = get(i)
                if (transfer.status === SailfishTransferModel.NotStarted || transfer.status === SailfishTransferModel.TransferStarted) {
                    root.displayedTransferId = transfer.transferId
                    return
                }
            }
            root.displayedTransferId = -1
        }

        onCountChanged: _reloadDisplayedItem()
    }

    // This is a big hack at the moment. We need to add a way to specifically fetch the data for
    // the newest running/starting transfer to avoid having to load the whole model just to get
    // this data. E.g. maybe modify DeclarativeShare to allow its transferId to be set, and then
    // it could be used as an object for receiving all the data and progress updates for that transfer?
    Column {
        width: parent.width

        Repeater {
            model: transferModel

            Item {
                property int status: model.status

                width: root.width; height: root.height
                visible: model.transferId === root.displayedTransferId

                onStatusChanged: {
                    if (model.transferId === root.displayedTransferId
                            && (status !== SailfishTransferModel.NotStarted || status !== SailfishTransferModel.TransferStarted)) {
                        transferModel._reloadDisplayedItem()
                    }
                }

                Item {
                    id: icon
                    y: Theme.paddingLarge
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: Theme.itemSizeLarge; height: Theme.itemSizeLarge

                    ProgressCircle {
                        anchors.fill: parent
                        value: model.progress
                    }

                    Image {
                        anchors.centerIn: parent
                        source: root.getTransferIcon(model.transferType)
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
                        text: transferModel.transfersInProgress == 1 ? root.getFileName(model.url) : transferModel.transfersInProgress
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.highlightColor
                        horizontalAlignment: transferModel.transfersInProgress == 1 ? Text.AlignLeft : Text.AlignHCenter
                    }
                }
            }
        }
    }
}
