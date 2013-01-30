import QtQuick 1.1
import com.jolla.components 1.0
import Sailfish.TransferEngine 1.0
import com.jolla.components.accounts 1.0

ApplicationWindow {

    initialPage: Page {

        PageHeader { title: "Share demo" }
        SailfishTransferInterface { id: transferInterface}

        Column {
            anchors.centerIn: parent
            spacing: 40

            Button {
                text: "Share Images"
                onClicked: pageStack.push(Qt.resolvedUrl("GridPage.qml"))
            }

            Button {
                text: "Test Download/Sync"
                onClicked: pageStack.push(Qt.resolvedUrl("TransferItemTestPage.qml"))
            }

            Button {
                text: "Transfer Status"
                onClicked: pageStack.push(transfersPage)
            }

            Button {
                text: "Accounts"
                onClicked: pageStack.push( Qt.resolvedUrl("AccountsSelectionPage.qml") )
            }
        }

        Component {
            id: transfersPage
            TransfersPage {
                visible: false
            }
        }


        Row {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

            Switch {
                id: notificationSwitch
                onCheckedChanged: {
                    if (transferInterface.enableNotifications !== checked){
                        transferInterface.enableNotifications = checked
                    }
                }
            }
            Label {
                text: "Notifications on/off"
            }
        }
    }


    Component.onCompleted: notificationSwitch.checked = transferInterface.enableNotifications
}
