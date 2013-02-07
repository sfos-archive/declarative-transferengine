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
    property string accountName
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
                //dateTakenLabel.text = formatter.formatDate(dateTime, Formatter.Timepoint)

                if (itemType == DocumentGallery.Video) {
                    //% "Duration"
                    //durationHeading.text = qsTrId("gallery-la-duration")
                    //durationLabel.text = durationDescription(galleryItem.metaData.duration)
                }
            }
        }

        function sizeDescription(size) {
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

    Image {
        id: thumbnail
        source: parent.source
        //mimeType: parent.mimeType        
        sourceSize.width: parent.width
        sourceSize.height: 540
        asynchronous: true
        fillMode: Image.PreserveAspectCrop
    }

    SilicaListView {
        anchors {
            top: parent.top
            bottom: parent.verticalCenter
            left: parent.left
            right: parent.right
        }

        PullDownMenu {

            MenuItem {
                //% "Remove metadata
                text: qsTrId("shareui_remove-metadata")
            }

            BackgroundItem {
                Label {
                    anchors.centerIn: parent
                    id: fileNameLabel
                }
            }
        }
    }


    SilicaListView {
        y: 540 - theme.itemSizeLarge
        height: sharePage.height - y
        width: parent.width
        model: actionModel
    }


    VisualItemModel {
        id: actionModel

        // Description
        TextField {
            id: descriptionTextField
            x: theme.paddingLarge
            width: sharePage.width - (theme.paddingLarge*2)
            height: theme.itemSizeLarge
            inputMethodHints: Qt.ImhNoPredictiveText
            //% "Add a description"
            placeholderText: qsTrId("webshareui-ph_description_placeholder")
            Keys.onReturnPressed:  parent.focus = true
        }

        Row {
            // Scale image combobox
            ComboBox {
                width: sharePage.width / 2
                //% "Scale image"
                label: qsTrId("webshareui-la_scale_image")
                wrap: true
                currentIndex: 3

                menu: ContextMenu {
                    //% "25%"
                    MenuItem { text: qsTrId("wbshareui-va_25_percent") }
                    //% "50%"
                    MenuItem { text: qsTrId("webshareui-va_50_percent") }
                    //% "75%"
                    MenuItem { text: qsTrId("webshareui-va_75_percent") }
                    //% "Original"
                    MenuItem { text: qsTrId("webshareui-va_original") }
                }
            }

            // File size
            SecondaryLabel {
                id: fileSizeLabel
                width: (sharePage.width / 2) - theme.paddingLarge
                height: theme.itemSizeLarge
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                color: theme.secondaryHighlightColor
                font.pixelSize: theme.fontSizeExtraLarge
            }
        }

        // Account info
        Item {
            width: sharePage.width - theme.paddingLarge
            height: theme.itemSizeLarge
            x: theme.paddingLarge

            // Service name
            PrimaryLabel {
                id: displayNameLabel
                anchors.bottom: parent.verticalCenter
            }

            // Account name
            SecondaryLabel {
                id: accountNameLabel
                //% "No account name"
                text: sharePage.accountName == "" ? qsTrId("webshareui_la-no-account-name") : sharePage.accountName
                anchors.top: displayNameLabel.bottom
            }
        }

        // Destination folder info
        Item {
            width: sharePage.width - theme.paddingLarge
            height: theme.itemSizeLarge
            x: theme.paddingLarge

            PrimaryLabel {
                id: destinationFolderHeader
                //% "Destination folder"
                text: qsTrId("webshareui_la-destination-folder")
                anchors.bottom: parent.verticalCenter
            }

            // TODO: This should come from the share plugin
            SecondaryLabel {
                id: destinationFolderNameLabel
                //% "Jolla mobile uploads"
                text: qsTrId("webshareui_la-upload-folder-name")
                anchors.top: destinationFolderHeader.bottom
            }
        }
    }

    SailfishShare {
        id: shareItem
        source: parent.source
        //title: captionTextField.text
        description: descriptionTextField.text
        metadataStripped: false // TODO: get the real selection here
        mimeType: parent.mimeType
        accountId: parent.accountId
        serviceId: parent.methodId
    }
}
