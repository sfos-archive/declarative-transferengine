/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

import QtQuick 2.6
import Sailfish.Silica 1.0
import Nemo.DBus 2.0
import Sailfish.Share.AppShare 1.0

Item {
    property var shareAction
    width: parent.width
    height: busy.running ? busy.height : errorLabel.height + Theme.paddingLarge

    InfoLabel {
        id: errorLabel

        property string error
        property int fileCount

        text: {
            // User should not see <undefined>, include it just for completeness
            var name = shareAction ? shareAction.selectedTransferMethodInfo.displayName : "<undefined>"
            if (error === "") {
                return ""
            } else if (error === "org.freedesktop.DBus.Error.InvalidArgs") {
                if (fileCount == 1) {
                    //: The target application (%1) was given one file that it didn't understand
                    //% "The file can not be shared to %1"
                    return qsTrId("sailfishshare-la-error_invalid_args_single_file").arg(name)
                } else {
                    //: The target application (%1) was given multiple files and it didn't understand some of them
                    //% "The files can not be shared to %1"
                    return qsTrId("sailfishshare-la-error_invalid_args_multiple_files").arg(name)
                }
            } else {
                //: Something went wrong while sharing to the target application (%1)
                //% "Failed to share to %1"
                return qsTrId("sailfishshare-la-general_error").arg(name)
            }
        }
    }

    BusyIndicator {
        id: busy

        anchors.horizontalCenter: parent.horizontalCenter
        height: Theme.itemSizeLarge
        running: errorLabel.error === ""
    }

    ShareMethodInfo {
        id: info

        readonly property bool ready: service !== "" && path !== "" && iface !== ""

        methodId: shareAction.selectedTransferMethodInfo.methodId

        onReadyChanged: {
            if (ready) {
                var config = shareAction.toConfiguration()
                errorLabel.fileCount = config["resources"].length
                app.call("share", [config], function() {
                    shareAction.done()
                }, function(error, message) {
                    errorLabel.error = error
                    console.warn("Failed to share:", error, "with message:", message)
                })
            }
        }
    }

    DBusInterface {
        id: app

        service: info.service
        path: info.path
        iface: info.iface
    }
}
