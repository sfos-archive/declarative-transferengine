import QtQuick 1.1
import Sailfish.Silica 1.0

Dialog {
    property url source
    property string mimeType
    property variant docItemId
    property string displayName
    property string accountName
    property int accountId
    property bool accountRequired
    property string methodId

    // TODO: Based on accountRequired property, load different page
    WebShare {id: sharePage; anchors.fill: parent }

    onOpened: {
        sharePage.source = source
        sharePage.mimeType = mimeType
        sharePage.docItemId = docItemId
        sharePage.displayName = displayName
        sharePage.accountName = accountName
        sharePage.accountId = accountId
        sharePage.accountRequired = accountRequired
        sharePage.methodId = methodId
    }

    onDone: {
        if (result === DialogResult.Accepted) {
            sharePage.share()
        }
    }
}
