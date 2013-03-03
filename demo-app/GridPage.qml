import QtQuick 1.1
import QtMobility.gallery 1.1
import org.nemomobile.thumbnailer 1.0
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0
import org.nemomobile.accounts 1.0
import com.jolla.components.accounts 1.0


Page {
    property int itemIndex

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
                        itemIndex = index
                        accountsList.showAccounts = true
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
            opacity: 0.85
            anchors.fill: parent
        }

        ShareMethodList {
            anchors.fill: parent
            onShareMethodClicked: {

                var item  = galleryModel.get(itemIndex)
                pageStack.push(shareUIPath, {
                                   methodId: methodId,
                                   accountId: accountId,
                                   displayName: displayName,
                                   accountName: userName,
                                   docItemId: item.itemId,
                                   source: item.url,
                                   mimeType: item.mimeType
                                    } )
            }
        }
    }
}
