import QtQuick 1.1
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0
import org.nemomobile.thumbnailer 1.0

Page {
    // Just forward the proprerties
    property alias source: webShare.source
    property alias mimeType: webShare.mimeType
    property alias docItemId: webShare.docItemId
    property alias displayName: webShare.displayName
    property alias accountName: webShare.accountName
    property alias accountId: webShare.accountId
    property alias accountRequired: webShare.accountRequired
    property alias methodId: webShare.methodId

    WebShare {
        id: webShare
        anchors.fill: parent

        Button {
            text: "Share"
            opacity: enabled ? 1.0 : 0.5
            anchors {
                bottom: parent.bottom
                bottomMargin: 30
                horizontalCenter: parent.horizontalCenter
            }

            onClicked: webShare.share()
        }

    }
}

