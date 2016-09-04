#ifndef DECLARATIVETRANSFERINTERFACE_H
#define DECLARATIVETRANSFERINTERFACE_H

#include <QQuickItem>
#include "transfertypes.h"

class DeclarativeTransferInterfacePrivate;
class DeclarativeTransferInterface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool enableNotifications READ notificationsEnabled WRITE setNotifications NOTIFY notificationsChanged)
    Q_ENUMS(Status)

public:
    enum Status {
        TransferFinished = TransferEngineData::TransferFinished,
        TransferCanceled = TransferEngineData::TransferCanceled,
        TransferInterrupted = TransferEngineData::TransferInterrupted
    };

    explicit DeclarativeTransferInterface(QQuickItem *parent = 0);
    ~DeclarativeTransferInterface();

    // TODO: MOVE cbCance/RestartMethods to private API
    // Cancel an existing transfer
    // NOTE: This makes transfer engine to call client's callback, which does the actual cancelation.
    Q_INVOKABLE void cbCancelTransfer(int transferId);

    // Restart failed or canceled transfer
    Q_INVOKABLE void cbRestartTransfer(int transferId);

    // Clear a finished, failed or canceled transfer
    Q_INVOKABLE void clearTransfer(int transferId);

    // Clear all finished, failed or canceled transfers
    Q_INVOKABLE void clearTransfers();

    // Start a new transfer
    Q_INVOKABLE void startTransfer(int transferId);

    // Create a new download event. Returns transfer id
    // callback - Dbus service, path, interface
    Q_INVOKABLE int createDownloadEvent(const QString &displayName,
                                        const QUrl &applicationIcon,
                                        const QUrl &serviceIcon,
                                        const QUrl &url,
                                        const QString &mimeType,
                                        quint64 expectedFileSize,
                                        const QStringList &callback,
                                        const QString &cancelMethod,
                                        const QString &restartMethod);

    // Create a new sync event. Returns transfer id
    Q_INVOKABLE int createSyncEvent(const QString &displayName,
                                    const QUrl &applicationIcon,
                                    const QUrl &serviceIcon,
                                    const QStringList &callback,
                                    const QString &cancelMethod,
                                    const QString &restartMethod);

    // Update the progress of the existing transfer
    Q_INVOKABLE void updateTransferProgress(int transferId, qreal progress);

    // Make transfer finished.
    Q_INVOKABLE void finishTransfer(int transferId, Status status, const QString &reason);

    // Enable notifications
    void setNotifications(bool enable);

    // return true/false if notifications are enabled
    bool notificationsEnabled() const;

Q_SIGNALS:
    void notificationsChanged();

private:
    DeclarativeTransferInterfacePrivate *d_ptr;
    Q_DECLARE_PRIVATE(DeclarativeTransferInterface)
};

#endif // DECLARATIVETRANSFERINTERFACE_H
