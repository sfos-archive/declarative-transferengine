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
    property bool comboBoxOpen

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
                fileSizeLabel.text  = sizeDescription(galleryItem.metaData.fileSize)
                var dateTime        = new Date(galleryItem.metaData.dateTaken)
                fileInfoLabel.text  = galleryItem.metaData.fileName + ", " + formatter.formatDate(dateTime, Formatter.Timepoint)

                // TODO: Info for videos
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
                //% "kB"
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

    // Header, which is hidden on landscape and if the comboBox is open
    DialogHeader {
        id: header;
        //% "Share"
        acceptText: qsTrId("webshareui-he_share")
        visible: isPortrait || !comboBoxOpen
    }

    // Top menu
    SilicaListView {
        id: topMenu
        anchors {
            top: parent.top
            bottom: parent.verticalCenter
            left: parent.left
            right: isPortrait ? parent.right : parent.horizontalCenter
        }

        PullDownMenu {
            id: pulldownMenu
            MenuItem {
                // Helper method to get label text and to get engineer eng/untranslated string correctly
                function menuItemText()
                {
                    if (shareItem.metadataStripped) {
                        //% "Keep metadata"
                        return qsTrId("webshareui-keep-metadata")
                    } else {
                        //% "Remove metadata"
                        return qsTrId("webshareui-remove-metadata")
                    }
                }

                text: menuItemText()
                onClicked: shareItem.metadataStripped = !shareItem.metadataStripped
            }

            // Placeholder for file info
            BackgroundItem {
                Label {
                    anchors.centerIn: parent
                    id: fileInfoLabel
                    width: topMenu.width
                    truncationMode: TruncationMode.Fade
                }
            }
        }

        header: Thumbnail {
            id: thumbnail
            source: sharePage.source
            mimeType: sharePage.mimeType
            sourceSize.width: isPortrait ? sharePage.width : sharePage.width / 2
            sourceSize.height: isPortrait ? 540 : sharePage.height
            width: sourceSize.width
            height: sourceSize.height
            opacity: 0.5

            SecondaryLabel {
                // Helper method to get label text and to get engineer eng/untranslated string correctly
                function labelText()
                {
                    if (shareItem.metadataStripped) {
                        //% "Metadata removed"
                        return qsTrId("webshareui-la-metadata-removed")
                    } else {
                        //% "Metadata included"
                        return qsTrId("webshareui-la-metadata-included")
                    }
                }

                font.pixelSize: theme.fontSizeExtraLarge
                text: labelText()
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                width: parent.width - (2 * theme.paddingLarge)
                horizontalAlignment: Text.AlignHCenter
                smooth: true
                y: theme.itemSizeExtraLarge
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    // List for the options at the bottom of the view
    SilicaListView {
        id: optionList
        anchors {
            top: isPortrait ? parent.verticalCenter : parent.top
            topMargin: isPortrait ? 40 : 80
            left: isPortrait? parent.left : parent.horizontalCenter
            right: parent.right
            bottom: parent.bottom

        }

        model: optionModel
    }

    VisualItemModel {
        id: optionModel

        // TODO: when writing a long decription, this TextField should
        //       extend towards the op and add there opacity ramp effect.
        //       at this point, I'm not aware that TextField would support
        //       extending to the top.
        // Description
        TextField {
            id: descriptionTextField
            width: optionList.width
            inputMethodHints: Qt.ImhNoPredictiveText
            //% "Add a description"
            placeholderText: qsTrId("webshareui-ph_description_placeholder")

            //: Description label of the image
            //% "Description"
            label: qsTrId("webshareui-la_description")
            Keys.onReturnPressed:  sharePage.focus = true
        }

        Row {
            // Scale image combobox
            ComboBox {
                width: optionList.width / 2
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

                    // Tell the page that we are open. On landscape we need to
                    // hide the share title
                    onActiveChanged: sharePage.comboBoxOpen = active
                }
            }

            // File size
            SecondaryLabel {
                id: fileSizeLabel
                width: (optionList.width / 2) - theme.paddingLarge
                height: theme.itemSizeLarge
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                color: theme.secondaryHighlightColor
                font.pixelSize: theme.fontSizeExtraLarge
            }
        }

        // Account info
        Item {
            width: optionList.width - theme.paddingLarge
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
            width: optionList.width - theme.paddingLarge
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
        description: descriptionTextField.text
        metadataStripped: true
        mimeType: parent.mimeType
        accountId: parent.accountId
        serviceId: parent.methodId
    }
}
