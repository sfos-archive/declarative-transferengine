import QtQuick 1.1
import com.jolla.components 1.0

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
    sourceComponent: WebShare {id: sharePage; anchors.fill: parent }

    onOpened: {
        item.source = source
        item.mimeType = mimeType
        item.docItemId = docItemId
        item.displayName = displayName
        item.accountName = accountName
        item.accountId = accountId
        item.accountRequired = accountRequired
        item.methodId = methodId
    }

    onDone: {
        if (result === DialogResult.Accepted) {
            item.share()
        }
    }
}
