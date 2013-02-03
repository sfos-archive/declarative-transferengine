import QtQuick 1.1
import Sailfish.Silica 1.0
import org.nemomobile.thumbnailer 1.0
import QtMobility.gallery 1.1
import Sailfish.TransferEngine 1.0

Item {
    id: sharePage
    property url source
    property string mimeType
    property alias docItemId: galleryItem.item
    property alias displayName: displayNameLabel.text
    property alias accountName: accountNameLabel.text
    property int accountId
    property bool accountRequired
    property string methodId

    function share()
    {
        shareItem.start()
    }

    // This is copied from the gallery
    DocumentGalleryItem {
        id: galleryItem
        autoUpdate: false
        properties: [ 'fileName', 'fileSize', 'dateTaken', 'duration' ]
        onStatusChanged: {
            if (status == DocumentGalleryItem.Finished) {
                fileNameLabel.text  = galleryItem.metaData.fileName
                fileSizeLabel.text  = sizeDescription(galleryItem.metaData.fileSize)
                var dateTime        = new Date(galleryItem.metaData.dateTaken)
                dateTakenLabel.text = formatter.formatDate(dateTime, Formatter.Timepoint)

                if (itemType == DocumentGallery.Video) {
                    //% "Duration"
                    //durationHeading.text = qsTrId("gallery-la-duration")
                    //durationLabel.text = durationDescription(galleryItem.metaData.duration)
                }
            }
        }

        function sizeDescription(size) {
            // FIXME: in french these should be mega/kilo octets
            var bytes = parseInt(size)
            if (bytes > (1024 * 1024)) {
                //% "MB"
                return Math.floor(bytes / (1024 * 1024)) + qsTrId("webshareui-la_megabyte")
            }
            if (bytes > 1024) {
                //% "KB"
                return Math.floor(bytes / 1024) + qsTrId("webshareui-la_kilobyte")
            }
            //% "B"
            return size + qsTrId("webshareui-la_byte")
        }

        function durationDescription(duration) {
            var seconds = parseInt(duration)
            var minutes = Math.floor(seconds / 60) % 60
            var hours = Math.floor(seconds / 3600)
            seconds %= 60

            return ((hours < 10) ? '0' : '') + hours + ':' + ((minutes < 10) ? '0' : '') + minutes + ':' + ((seconds < 10) ? '0' : '') + seconds
        }
    }

    Formatter { id: formatter }

    // Visual items
    PageHeader {
        id: header;
        //% "Share"
        title: qsTrId("webshareui-he_share")
    }

    Thumbnail {
        id: thumbnail
        source: parent.source
        //mimeType: parent.mimeType
        width: 160
        height: 160
        sourceSize.width: 160
        sourceSize.height: 160
        anchors.top: header.bottom
        onSourceChanged: console.log("source: " + parent.source)
    }

    // File information
    Column {
        anchors {
            left: thumbnail.right
            leftMargin: 20
            top: thumbnail.top
        }
        PrimaryLabel { id: fileNameLabel; truncationMode: TruncationMode.Fade; width: sharePage.width - thumbnail.width - 30 }
        PrimaryLabel { id: fileSizeLabel }
        SecondaryLabel { id: dateTakenLabel }
    }

    // Account information
    Column {
        id: accountInfo
        anchors {
            top: thumbnail.bottom
            topMargin: 20
            leftMargin: 20
        }
        PrimaryLabel {id: displayNameLabel }
        SecondaryLabel { id: accountNameLabel }
    }

    // Text fields
    Column {
        id: textInputFields
        anchors {
            top: accountInfo.bottom
            topMargin: 50
            left: parent.left
            right: parent.right
        }

        TextField {
            id: captionTextField
            width: parent.width
            inputMethodHints: Qt.ImhNoPredictiveText
            //% "Caption"
            placeholderText: qsTrId("webshareui-ph_caption_placeholder")
            Keys.onReturnPressed:  descriptionTextField.focus = true
        }
        TextField {
            id: descriptionTextField
            width: parent.width
            inputMethodHints: Qt.ImhNoPredictiveText
            //% "Description"
            placeholderText: qsTrId("webshareui-ph_description_placeholder")
            Keys.onReturnPressed:  parent.focus = true
        }
    }

    // Currently only placeholders
    Column {
        id: actions
        spacing: 40
        anchors {
            top: textInputFields.bottom
            topMargin: 50
        }
        // TODO: add controls here
        //% "Remove metadata"
        PrimaryLabel { text: qsTrId("shareui_remove-metadata") }
        //% "Scale image"
        PrimaryLabel { text: qsTrId("shareui_scale-image") }
    }

    SailfishShare {
        id: shareItem
        source: parent.source
        title: captionTextField.text
        description: descriptionTextField.text
        metadataStripped: false // TODO: get the real selection here
        mimeType: parent.mimeType
        accountId: parent.accountId
        serviceId: parent.methodId
    }
}
