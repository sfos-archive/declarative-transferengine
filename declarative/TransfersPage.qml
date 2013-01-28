import QtQuick 1.1
import com.jolla.components 1.0
import Sailfish.Silica.TransferEngine 1.0
import org.nemomobile.thumbnailer 1.0


Page {

    function fileName(url)
    {
        var splitString = url.split("/")
        return splitString[splitString.length - 1]
    }

    function statusText(transferType, status, date)
    {
        // TODO: Localization
        switch(status) {
        case SilicaTransferModel.NotStarted:
            //% "Waiting"
            return qsTrId("transferui_transfer-waiting")

        case SilicaTransferModel.TransferStarted:
            return ""

        case SilicaTransferModel.TransferFinished:
            return timestamp(date)

        case SilicaTransferModel.TransferInterrupted:
        {
            if (transferType === SilicaTransferModel.Sync) {
                //% "Synchronization failed"
                return qsTrId("transferui_synchronization_failed")
            } else {
                //% "Failed"
                return qsTrId("transferui_transfer-failed")
            }
        }
        case SilicaTransferModel.TransferCanceled:
            //% "Canceled"
            return qsTrId("transferui_transfer-canceled");
        }
    }
    // For some reason date object can't hand ISO8601 standard.
    // This function makes it compatible
    function dateFromISO8601(isostr) {
        var parts = isostr.match(/\d+/g);
        return new Date(parts[0], parts[1] - 1, parts[2], parts[3], parts[4], parts[5]);
    }


    function timestamp(dateTime) {
        var txt = formatter.formatDate(dateTime, Formatter.TimepointRelative)
        var elapsed = formatter.formatDate(dateTime, Formatter.DurationElapsed)
        return elapsed ? elapsed : txt
    }


    function formatFileSize(size)
    {
        if (size < 1024) {
            //% "B"
            return size + qsTrId("transferui-la_byte")
        }

        var kb = size / 1024.0
        if (kb < 1024) {
            //% "KB"
            return kb.toFixed(1) + qsTrId("transferui-la_kilobyte")
        }

        var mb = kb / 1024.0
        if (mb < 1024) {
            //% "MB"
            return mb.toFixed(1) + qsTrId("transferui-la_megabyte")
        }

        var gb = mb / 1024.0
        //% "GB"
        return gb + qsTrId("transferui-la_gigabyte")
    }

    function transferIcon(transferType)
    {
        switch (transferType) {
        case SilicaTransferModel.Upload:
            return "image://theme/icon-m-upload"
        case SilicaTransferModel.Download:
            return "image://theme/icon-m-download"
        case SilicaTransferModel.Sync:
            return "image://theme/icon-m-sync"
        default:
            console.log("TransfersPage::transferIcon: failed to get transfer type")
            return ""
        }
    }

    function mimeTypeIcon(mimeType)
    {
        var type = mimeType.split("/");

        // Handle basic media types
        if (type[0] === "image") {
            return "image://theme/icon-m-image"
        }
        if (type[0] === "video") {
            return "image://theme/icon-m-video"
        }
        if (type[0] === "audio") {
            return "image://theme/icon-m-sound"
        }

        console.log("MIME TYPE: " + mimeType + ": " +type[1])

        // Next doc types
        // TODO: CHECK the rest of document types
        if (type[1].indexOf("excel")   ||
            type[1].indexOf("pdf")     ||
            type[1].indexOf("word")    ||
            type[1].indexOf("powerpoint")) {
            return "image://theme/icon-m-document"
        }
        // Handle contacts
        if (type[1].indexOf("vcard")) {
            return "image://theme/icon-m-people"
        }

        return "image://theme/icon-m-others"
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
            height: menuOpen ? transferList.contextMenu.height + 80 + fileNameLabel.paintedHeight + 20 :
                               80 + fileNameLabel.paintedHeight + 20

            onTransferStatusChanged: if (menuOpen) transferList.contextMenu.hide()

            // TODO: We should create this dynamically if we are using url->Thumbnail, other -> Image
            Image {
                id: thumbnail
                sourceSize.width: 80
                sourceSize.height: 80
                fillMode: Image.PreserveAspectCrop
                source: transferType === SilicaTransferModel.Upload ? url : serviceIcon
                smooth: true
            }


            Label {
                id: nameLabel
                anchors {
                    top: thumbnail.top
                    left: thumbnail.right
                    leftMargin: 20
                }

                text: displayName
                elide: Text.ElideRight
                width: parent.width - thumbnail.width - 20 - mimeTypeIcon.width - transferTypeIcon.width - 40
            }

            // Home made progress bar. Components could provide something like this.
            ProgressBar {
                value: progress
                visible: status === SilicaTransferModel.TransferStarted

                height: 30
                anchors {
                    left: thumbnail.right
                    leftMargin: 0
                    right: mimeTypeIconIcon.left
                    rightMargin: 60
                    bottom: thumbnail.bottom
                }
            }

            Label {
                id: statusLabel
                text: statusText(transferType, status, dateFromISO8601(timestamp))
                visible: status !== SilicaTransferModel.TransferStarted
                anchors {
                    left: thumbnail.right
                    leftMargin: 20
                    bottom: thumbnail.bottom

                }
            }

            Image {
                id: mimeTypeIconIcon
                source: transferType !== SilicaTransferModel.Sync ? mimeTypeIcon(mimeType) : ""
                width: 40
                height: 40

                anchors {
                    right: transferTypeIcon.left
                    rightMargin: 10
                }
            }

            Image {
                id: transferTypeIcon
                source: transferIcon(transferType)
                width: 40
                height: 40

                anchors {
                    right: parent.right
                    rightMargin: 10
                }
            }

            Label {
                id: sizeLabel
                text: formatFileSize(fileSize)
                visible: transferType !== SilicaTransferModel.Sync
                anchors {
                    right: transferTypeIcon.right
                    bottom: thumbnail.bottom
                }
            }

            Label {
                id: fileNameLabel
                text: fileName(url)
                wrapMode: Text.WrapAnywhere
                width: parent.width - thumbnail.width - 20
                anchors {
                    top: thumbnail.bottom
                    topMargin: 20
                    left: statusLabel.left
                }
            }

            onClicked: {
                // No actions for properly finished transfers
                if (status === SilicaTransferModel.TransferFinished) {
                    return;
                }

                if (transferList.contextMenu === null)
                    transferList.contextMenu = contextMenuComponent.createObject(transferList)

                transferList.contextMenu.transferId = transferId
                transferList.contextMenu.status = status
                transferList.contextMenu.show(backgroundItem)
            }
        }
    }

    // Interface for e.g. canceling a transfer
    SilicaTransferInterface {
        id: transferInterface
    }

    // Actual list which displays transfers
    JollaListView {
        id: transferList
        property Item contextMenu

        header: PageHeader {
            //% "Transfers"
            title: qsTrId("transferui-he_transfers")
        }

        PullDownMenu {
            bottomMargin: 0
            MenuItem {
                //% "Clear all"
                text: qsTrId("transferui-me_clear-all")
                onClicked: transferModel.clearTransfers()
            }
        }

        anchors.fill: parent
        spacing: 15
        model: SilicaTransferModel {id: transferModel}
        delegate: transferDelegate
    }

    // Context menu for actions such as cancel and restart
    Component {
        id: contextMenuComponent

        ContextMenu {
            property int transferId
            property int status
            parent: null
            x: parent !== null ? -parent.x : 0.0

            function setText()
            {
                switch (status) {
                case SilicaTransferModel.TransferStarted:
                    //% "Cancel"
                    return qsTrId("transferui-la_cancel-transfer")
                case SilicaTransferModel.TransferCanceled:
                case SilicaTransferModel.TransferInterrupted:
                    //% "Restart"
                    return qsTrId("transferui-la_restart-transfer")
                case SilicaTransferModel.TransferFinished:
                default:
                    return ""
                }

            }

            function menuAction()
            {
                switch (status) {
                case SilicaTransferModel.TransferStarted:
                    transferInterface.cbCancelTransfer(transferId)
                    break;
                case SilicaTransferModel.TransferCanceled:
                case SilicaTransferModel.TransferInterrupted:
                    transferInterface.cbRestartTransfer(transferId)
                    break;
                case SilicaTransferModel.TransferFinished:
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
