import QtQuick 1.1
import Sailfish.Silica 1.0

Dialog {
    property url source
    property string mimeType
    property string displayName
    property string accountName
    property string shareUIPath
    property string methodId
    property int accountId
    property variant docItemId

    // TODO: Add dynamic Share UI loading here, when we have more than one Share method
    //WebShare {id: sharePage; anchors.fill: parent }

    Loader {
        id: sharePage
        source: shareUIPath
        anchors.fill: parent
    }

    onOpened: {
        sharePage.item.source = source
        sharePage.item.mimeType = mimeType
        sharePage.item.docItemId = docItemId
        sharePage.item.displayName = displayName
        sharePage.item.accountName = accountName
        sharePage.item.accountId = accountId
        sharePage.item.methodId = methodId
    }

    onDone: if (result === DialogResult.Accepted) sharePage.item.share()
}
