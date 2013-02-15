import QtQuick 1.1
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0
import org.nemomobile.thumbnailer 1.0


Page {
    id: transfersPage
    property Item _remorsePopup

    function fileName(url)
    {
        var splitString = url.split("/")
        return splitString[splitString.length - 1]
    }

    function statusText(transferType, status)
    {
        switch(status) {
        case SailfishTransferModel.NotStarted:
            //% "Waiting"
            return qsTrId("transferui-la_transfer_waiting")

        case SailfishTransferModel.TransferStarted:
            // Just break and handle started transfers after this block
            break

        case SailfishTransferModel.TransferFinished:
            //% "Finished"
            return qsTrId("transferui-la_transfer_finished")


        case SailfishTransferModel.TransferInterrupted:
            //% "Failed"
            return qsTrId("transferui-la_transfer_failed")


        case SailfishTransferModel.TransferCanceled:
            //% "Canceled"
            return qsTrId("transferui-la-transfer_canceled");

        }

        switch (transferType) {
        case SailfishTransferModel.Sync:
            //% "Syncing"
            return qsTrId("transferui-la_transfer_syncing")
        case SailfishTransferModel.Download:
            //% "Downloading"
            return qsTrId("transferui-la_transfer_downloading")
        case SailfishTransferModel.Upload:
            //% "Uploading"
            return qsTrId("transferui-la_transfer_uploading")
        }

        //% "Unknown"
        return qsTrId("transferui-la-transfer_unknown")
    }

    // For some reason date object can't hand ISO8601 standard.
    // This function makes it compatible
    function dateFromISO8601(isostr) {
        var parts = isostr.match(/\d+/g);
        return new Date(parts[0], parts[1] - 1, parts[2], parts[3], parts[4], parts[5]);
    }


    function formattedTimestamp(dateTime) {
        var today = new Date;

        // return time, if it's today
        if (dateTime.getFullYear() === today.getFullYear() &&
            dateTime.getMonth() === today.getMonth() &&
            dateTime.getDay() === today.getDay()) {
            return formatter.formatDate(dateTime, Formatter.TimepointRelative)
        }

        return formatter.formatDate(dateTime, Formatter.DurationElapsed)
    }


    // Format the size using max three digits
    // e.g. 0.1, 1.1, 11.1
    function formatFileSize(size)
    {
        if (size < 100) {
            //% "B"
            return size + qsTrId("transferui_la-byte")
        }

        var kb = size / 1024.0
        if (kb < 100) {
            //% "kB"
            return kb.toFixed(1) + qsTrId("transferui-la_kilobyte")
        }

        var mb = kb / 1024.0
        if (mb < 100) {
            //% "MB"
            return mb.toFixed(1) + qsTrId("transferui-la_megabyte")
        }

        var gb = mb / 1024.0
        if (gb < 100) {
            //% "GB"
            return gb.toFixed(1) + qsTrId("transferui-la_gigabyte")
        }

        var tb = gb / 1024.0
        //% "TB"
        return tb.toFixed(2) + qsTrId("transferui-la_terabyte")
    }

    function transferIcon(transferType)
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

    function mimeTypeIcon(mimeType)
    {
        if (mimeType.length === 0)
            return ""

        var type = mimeType.split("/");


        // Handle basic media types
        if (type[0] === "image") {
            return "image://theme/icon-l-image"
        }
        if (type[0] === "video") {
            return "image://theme/icon-l-video"
        }
        if (type[0] === "audio") {
            return "image://theme/icon-l-sound"
        }


        // Next doc types
        // TODO: CHECK the rest of document types
        if (type[1].indexOf("excel")   ||
            type[1].indexOf("pdf")     ||
            type[1].indexOf("word")    ||
            type[1].indexOf("powerpoint")) {
            return "image://theme/icon-l-document"
        }
        // Handle contacts
        if (type[1].indexOf("vcard")) {
            return "image://theme/icon-l-people"
        }

        return "image://theme/icon-l-others"
    }

    Formatter {
        id: formatter
    }

    Label {
        anchors.centerIn: parent
        //% "No Transfers"
        text: transferModel.count === 0 ? qsTrId("transferui-la-no_transfers") : ""
    }


    // Delegate for a transfer entry in a list
    Component {
        id: transferDelegate
        BackgroundItem {
            id: backgroundItem
            property bool menuOpen: transferList.contextMenu != null && transferList.contextMenu.parent === backgroundItem
            property int transferStatus: status
            property url thumbnailUrl: url
            property url appIconUrl: applicationIcon
            property Item thumbnail

            height: menuOpen ? transferList.contextMenu.height + theme.itemSizeExtraLarge:
                               theme.itemSizeExtraLarge

            // Load thumbs on demand and only once. Note that share thumbnail is used only for local images/thumbs
            onThumbnailUrlChanged: thumbnail = shareThumbnail.createObject(backgroundItem)
            onAppIconUrlChanged: thumbnail = appThumbnail.createObject(backgroundItem)

            // Close open context menu, if the status changes
            onTransferStatusChanged: if (menuOpen) transferList.contextMenu.hide()

            // Component for local thumbnails. Only used by sharing entry.
            Component {
                id: shareThumbnail
                Thumbnail {
                    width: theme.itemSizeExtraLarge
                    height: theme.itemSizeExtraLarge
                    sourceSize.width: width
                    sourceSize.height: height
                    source: url
                    opacity: 0.5
                    priority: index >= transferList.firstVisible && index < transferList.firstVisible + 10
                              ? Thumbnail.NormalPriority
                              : Thumbnail.LowPriority
                }
            }
            // Component for application thumbnail. Only used by Sync or Download entry.
            Component {
                id: appThumbnail
                Item {
                    width: theme.itemSizeExtraLarge
                    height: theme.itemSizeExtraLarge

                    Rectangle {
                        color: theme.primaryColor
                        opacity: 0.1
                        anchors.fill: parent
                    }

                    Image {
                        source: applicationIcon
                        opacity: 0.5
                        asynchronous: true
                        anchors.centerIn: parent
                        sourceSize.width: theme.itemSizeSmall
                        sourceSize.height: theme.itemSizeSmall
                    }
                }
            }

            Image {
                id: mimeTypeIconIcon
                source: mimeTypeIcon(mimeType)
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: thumbnail
                asynchronous: true
                visible: mimeType.length > 0 && thumbnail.status === Image.Ready
            }

            Image {
                id: transferTypeIcon
                source: transferIcon(transferType)
                width: theme.itemSizeSmall / 2
                height: theme.itemSizeSmall / 2
                asynchronous: true
                smooth: !transferList.moving
                anchors {
                    top: thumbnail.top
                    topMargin: theme.paddingMedium
                    left: thumbnail.right
                    leftMargin: theme.paddingLarge
                }
            }

            Label {
                id: sizeLabel
                text: formatFileSize(fileSize)
                visible: transferType !== SailfishTransferModel.Sync && status == SailfishTransferModel.TransferFinished
                font.pixelSize: theme.fontSizeMedium
                color: backgroundItem.down || menuOpen ? theme.highlightColor : theme.primaryColor
                anchors {
                    bottom: transferTypeIcon.bottom
                    left: transferTypeIcon.right
                    leftMargin: theme.paddingMedium
                }
            }


            Label {
                id: statusLabel
                text: statusText(transferType, status)
                visible: status === SailfishTransferModel.TransferStarted ||
                         status === SailfishTransferModel.TransferInterrupted ||
                         status === SailfishTransferModel.TransferCanceled
                // Failed color comes from Jaakko.
                color: status == SailfishTransferModel.TransferInterrupted ? "#ff4c4c" : backgroundItem.down || menuOpen ? theme.highlightColor : theme.primaryColor
                font.pixelSize: theme.fontSizeMedium
                anchors {
                    bottom: transferTypeIcon.bottom
                    left: transferTypeIcon.right
                    leftMargin: theme.paddingMedium
                }
            }

            // Home made progress bar. Components could provide something like this.
            ProgressBar {
                value: visible ? progress : 0
                visible: status === SailfishTransferModel.TransferStarted
                color: backgroundItem.down || menuOpen ? theme.highlightColor : theme.primaryColor
                anchors {
                    left: thumbnail.right
                    leftMargin: theme.paddingLarge
                    right: parent.right
                    rightMargin: theme.paddingLarge * 2
                    bottom: fileNameLabel.top
                }
            }

            Label {
                id: timeLabel
                text:  formattedTimestamp(dateFromISO8601(timestamp))
                font.pixelSize: theme.fontSizeExtraSmall
                opacity: 0.5
                visible: status !== SailfishTransferModel.TransferStarted
                color: backgroundItem.down || menuOpen ? theme.highlightColor : theme.primaryColor
                anchors {
                    top: transferTypeIcon.bottom
                    topMargin: theme.paddingSmall
                    left: thumbnail.right
                    leftMargin: theme.paddingLarge
                }
            }

            Label {
                id: fileNameLabel
                text: fileName(url)
                truncationMode: TruncationMode.Fade
                width: parent.width - thumbnail.width - 2 * theme.paddingLarge
                font.pixelSize: theme.fontSizeExtraSmall
                opacity: 0.5
                color: backgroundItem.down || menuOpen ? theme.highlightColor : theme.primaryColor
                anchors {
                    left: thumbnail.right
                    leftMargin: theme.paddingLarge
                    right: parent.right
                    rightMargin: theme.paddingLarge
                    bottom: thumbnail.bottom
                }
            }


            // Placeholder for a service icon
            Image {
                source: serviceIcon
                width: theme.itemSizeSmall / 2
                height: theme.itemSizeSmall / 2

                anchors {
                    right: parent.right
                    rightMargin: theme.paddingLarge
                    top: parent.top
                    topMargin: theme.paddingMedium
                }
            }

            onClicked: {
                // No actions for properly finished transfers
                if (status === SailfishTransferModel.TransferFinished) {
                    return;
                }

                // There must be cancel or restart enabled in order to show context menu
                if (cancelEnabled || restartEnabled) {
                    if (transferList.contextMenu === null)
                        transferList.contextMenu = contextMenuComponent.createObject(transferList)

                    transferList.contextMenu.transferId     = transferId
                    transferList.contextMenu.status         = status
                    transferList.contextMenu.cancelEnabled  = cancelEnabled
                    transferList.contextMenu.restartEnabled = restartEnabled
                    transferList.contextMenu.show(backgroundItem)
                }
            }
        }
    }

    // Interface for e.g. canceling a transfer
    SailfishTransferInterface {
        id: transferInterface
    }

    // Actual list which displays transfers
    SilicaListView {
        id: transferList
        property Item contextMenu
        property int firstVisible: Math.max(0, indexAt(0, contentY))

        header: PageHeader {
            //% "Transfers"
            title: qsTrId("transferui-he_transfers")
        }

        PullDownMenu {
            bottomMargin: 0
            MenuItem {
                //% "Clear all"
                text: qsTrId("transferui-me_clear-all")
                enabled: transferModel.count > 0
                onClicked: {
                    if (_remorsePopup === null) {
                        _remorsePopup = remorsePopupComponent.createObject(transfersPage)
                    }

                    //: Clearing transfers in 5 seconds
                    //% "Clearing transfers"
                    _remorsePopup.execute(qsTrId("transferui-me-clear-transfers"),
                                              function() {
                                                  transferModel.clearTransfers()
                                              }
                                          )
                }

                Component {
                    id: remorsePopupComponent
                    RemorsePopup {}
                }
            }
        }

        anchors.fill: parent
        model: SailfishTransferModel {id: transferModel}
        delegate: transferDelegate
        cacheBuffer: transferList.height
    }

    // Context menu for actions such as cancel and restart
    Component {
        id: contextMenuComponent

        ContextMenu {
            property int transferId
            property int status
            property bool cancelEnabled
            property bool restartEnabled

            parent: null
            x: parent !== null ? -parent.x : 0.0

            function setText()
            {
                switch (status) {
                case SailfishTransferModel.TransferStarted:
                    //% "Cancel"
                    return qsTrId("transferui-la_cancel-transfer")
                case SailfishTransferModel.TransferCanceled:
                case SailfishTransferModel.TransferInterrupted:
                    //% "Restart"
                    return qsTrId("transferui-la_restart-transfer")
                case SailfishTransferModel.TransferFinished:
                default:
                    return ""
                }

            }

            function menuAction()
            {
                switch (status) {
                case SailfishTransferModel.TransferStarted:
                    transferInterface.cbCancelTransfer(transferId)
                    break;
                case SailfishTransferModel.TransferCanceled:
                case SailfishTransferModel.TransferInterrupted:
                    transferInterface.cbRestartTransfer(transferId)
                    break;
                case SailfishTransferModel.TransferFinished:
                    console.log("Not implemented")
                    break
                }
            }

            MenuItem {
                text: setText()
                onClicked: menuAction()
            }
        }
    }
}
