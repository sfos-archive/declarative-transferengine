#include "declarativetransferinterface.h"
#include "transferengineclient.h"

class DeclarativeTransferInterfacePrivate
{
public:
    TransferEngineClient *m_client;
};

DeclarativeTransferInterface::DeclarativeTransferInterface(QQuickItem *parent) :
    QQuickItem(parent),
    d_ptr(new DeclarativeTransferInterfacePrivate)
{
    Q_D(DeclarativeTransferInterface);
    d->m_client = new TransferEngineClient(this);
}


DeclarativeTransferInterface::~DeclarativeTransferInterface()
{
    Q_D(DeclarativeTransferInterface);
    delete d->m_client;

    delete d_ptr;
    d_ptr = 0;
}

void DeclarativeTransferInterface::cbCancelTransfer(int transferId)
{
    Q_D(DeclarativeTransferInterface);
    d->m_client->cbCancelTransfer(transferId);
}

void DeclarativeTransferInterface::cbRestartTransfer(int transferId)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->cbRestartTransfer(transferId);
}


void DeclarativeTransferInterface::clearTransfers()
{
    Q_D(DeclarativeTransferInterface);
    d->m_client->clearTransfers();
}

void DeclarativeTransferInterface::startTransfer(int transferId)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->startTransfer(transferId);
}



void DeclarativeTransferInterface::setNotifications(bool enable)
{
    Q_D(DeclarativeTransferInterface);
    if (notificationsEnabled() != enable) {
        d->m_client->enableNotifications(enable);
        emit notificationsChanged();
    }
}

bool DeclarativeTransferInterface::notificationsEnabled() const
{
    Q_D(const DeclarativeTransferInterface);
    return d->m_client->notificationsEnabled();
}

int DeclarativeTransferInterface::createDownloadEvent(const QString &displayName,
                                                      const QUrl &applicationIcon,
                                                      const QUrl &serviceIcon,
                                                      const QUrl &url,
                                                      const QString &mimeType,
                                                      quint64 expectedFileSize,
                                                      const QStringList &callback,
                                                      const QString &cancelMethod,
                                                      const QString &restartMethod)
{
    Q_D(DeclarativeTransferInterface);
    CallbackInterface cbIf;
    if (callback.size() == 3) {
        cbIf = CallbackInterface(callback.at(0),
                                 callback.at(1),
                                 callback.at(2),
                                 cancelMethod,
                                 restartMethod);
    }
    return d->m_client->createDownloadEvent(displayName,
                                            applicationIcon,
                                            serviceIcon,
                                            url,
                                            mimeType,
                                            expectedFileSize,
                                            cbIf);
}

int DeclarativeTransferInterface::createSyncEvent(const QString &displayName,
                                                  const QUrl &applicationIcon,
                                                  const QUrl &serviceIcon,
                                                  const QStringList &callback,
                                                  const QString &cancelMethod,
                                                  const QString &restartMethod)
{
    Q_D(DeclarativeTransferInterface);
    CallbackInterface cbIf;
    if (callback.size() == 3) {
        cbIf = CallbackInterface(callback.at(0),
                                 callback.at(1),
                                 callback.at(2),
                                 cancelMethod,
                                 restartMethod);
    }
    return d->m_client->createSyncEvent(displayName,
                                        applicationIcon,
                                        serviceIcon,
                                        cbIf);
}

void DeclarativeTransferInterface::updateTransferProgress(int transferId, qreal progress)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->updateTransferProgress(transferId, progress);
}

void DeclarativeTransferInterface::finishTransfer(int transferId, Status status, const QString &reason)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->finishTransfer(transferId, static_cast<TransferEngineClient::Status>(status), reason);
}
