import QtQuick 2.0
import QtDocGallery 5.0
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0

Page {

    DocumentGalleryModel {
        id: galleryModel
        rootType: DocumentGallery.Image
        properties: [ "url", "title", "dateTaken", "mimeType" ]
        autoUpdate: true
        sortProperties: ["dateTaken"]
    }

    SilicaGridView {
        id: grid
        anchors.fill: parent
        cellWidth: width / 2
        cellHeight: width / 2
        model: galleryModel
        onMovingChanged: if (moving) accountsList.showAccounts = false

        delegate:
            Image {
                asynchronous: true
                source:  "image://nemoThumbnail/" + url
                sourceSize.width: grid.cellWidth
                sourceSize.height: grid.cellHeight

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        accountsList.showAccounts = true
                        shareMethodList.source = url
                    }
                }
            }

        ScrollDecorator {}
    }

    // Slide menu from bottom to up which displays accounts
    Item {
        id: accountsList
        property bool showAccounts

        width: parent.width
        height: parent.height / 2
        y: showAccounts ? parent.height / 2 : parent.height
        Behavior on y { NumberAnimation { duration: 400 }}

        // Background
        Rectangle {
            color: "black"
            opacity: Theme.opacityOverlay
            anchors.fill: parent
        }

        ShareMethodList {
            id: shareMethodList
            anchors.fill: parent
        }
    }
}
