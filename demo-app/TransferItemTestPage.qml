import QtQuick 1.1
import com.jolla.components 1.0
import Sailfish.TransferEngine 1.0

Page  {

    property int transferId
    property real transferProgress: 0
    property url sourceIcon
    property string sourceName
    property bool downloadSelected
    property string mimeType
    property url filePath
    property variant callback: ['com.jolla.transferdemoapp', '/com/jolla/transferdemoapp', 'com.jolla.transferdemoapp']

    Row {
        id: serviceSelection
        anchors.horizontalCenter: parent.horizontalCenter
        y: 100

        Switch {
            id: sw1
            iconSource: "image://theme/icon-l-facebook"
            onCheckedChanged: {
                if (checked) {
                    sourceIcon = iconSource
                    sourceName = "Facebook"
                    sw2.checked = false
                    sw3.checked = false
                }
            }
            Component.onCompleted: checked = true
        }

        Switch {
            id: sw2
            iconSource: "image://theme/icon-l-exchange"
            onCheckedChanged: {
                if (checked) {
                    sourceIcon = iconSource
                    sourceName = "Exchange"
                    sw1.checked = false
                    sw3.checked = false
                }
            }
        }

        Switch {
            id: sw3
            iconSource: "image://theme/icon-l-twitter"
            onCheckedChanged: {
                if (checked) {
                    sourceIcon = iconSource
                    sourceName = "Twitter"
                    sw1.checked = false
                    sw2.checked = false
                }
            }
        }
    }

    Row {
        id: actionSelection
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: serviceSelection.bottom
            topMargin: 20
        }

        Switch {
            id: downloadSwitch
            iconSource: "image://theme/icon-m-download"
            //checked: true
            onCheckedChanged: {
                if (checked) {
                    syncSwitch.checked = false
                    downloadSelected = true
                }
            }
            Component.onCompleted: checked = true
        }

        Switch {
            id: syncSwitch
            iconSource: "image://theme/icon-m-sync"
            onCheckedChanged: {
                if (checked) {
                    downloadSwitch.checked = false
                    downloadSelected = false
                }
            }
        }
    }

    Row {
        id: mimeTypeSelection
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: actionSelection.bottom
            topMargin: 20
        }

        Switch {
            id: iSwitch
            iconSource: "image://theme/icon-m-image"

            onCheckedChanged: {
                if (checked) {
                    dSwitch.checked = false
                    vSwitch.checked = false
                    mimeType = "image/jpeg"
                    filePath ="file://home/nemo/Pictures/VeryLongFileNameWhichSeemsToNeverEndAndShouldBeWrappedToSeveralDifferentLines.jpg"
                }
            }
            Component.onCompleted: checked = true
        }

        Switch {
            id: dSwitch
            iconSource: "image://theme/icon-m-document"
            onCheckedChanged: {
                if (checked) {
                    iSwitch.checked = false
                    vSwitch.checked = false
                    mimeType = "application/msword"
                    filePath ="file://home/nemo/Documents/diary.doc"
                }
            }
        }

        Switch {
            id: vSwitch
            iconSource: "image://theme/icon-m-video"
            onCheckedChanged: {
                if (checked) {
                    iSwitch.checked = false
                    dSwitch.checked = false
                    mimeType = "video/avi"
                    filePath = "file://home/nemo/Videos/homevideo.ogg"
                }
            }
        }
    }

    Button {
        text: "Create Transfer Event"
        onClicked: createEvent()
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: mimeTypeSelection.bottom
            topMargin: 40
        }
    }


    Label {
        text: "Progress: " +  (transferProgress * 100).toFixed(1)  + "%"
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }

    function createEvent()
    {
        if (downloadSelected) {
            console.log("Download selected")
            transferId = transferInterface.createDownloadEvent(sourceName,
                                                               sourceIcon,
                                                               filePath ,
                                                               mimeType,
                                                               callback)
        } else {
            console.log("Sync selected")
            transferId = transferInterface.createSyncEvent(sourceName,
                                                           sourceIcon,
                                                           callback)
        }

        console.log("Created transfer with id: " + transferId)
        if (transferId > 0) {
            transferInterface.startTransfer(transferId)
            transferTimer.start();
        }
    }

    function updateProgress()
    {
         transferProgress = transferProgress + 0.025

        if (transferProgress < 1.0 ) {
            transferInterface.updateTransferProgress(transferId, transferProgress)
        } else {
            transferInterface.finishTransfer(transferId, SailfishTransferInterface.TransferFinished, "");
            transferTimer.stop()
            transferProgress = 0
        }
    }

    Timer {
        id: transferTimer
        interval: 200
        onTriggered: updateProgress()
        repeat: true
    }

    Connections {
        target: callbackInterface

        onTransferCanceled: {
            console.log("Transfer Canceled")
            transferTimer.stop()
            transferProgress = 0
            transferInterface.finishTransfer(transferId, SailfishTransferInterface.TransferCanceled, "Canceled by user")
        }

        onTransferRestarted: {
            console.log("Transfer restarted")
            transferInterface.startTransfer(transferId)
            transferProgress = 0
            transferTimer.start()
        }
    }

    SailfishTransferInterface { id: transferInterface }
}
