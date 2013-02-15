#include "declarativetransferinterface.h"
#include "transferengineinterface.h"

class DeclarativeTransferInterfacePrivate
{
public:
    TransferEngineInterface *m_client;
};

DeclarativeTransferInterface::DeclarativeTransferInterface(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    d_ptr(new DeclarativeTransferInterfacePrivate)
{
    Q_D(DeclarativeTransferInterface);
    d->m_client = new TransferEngineInterface("org.nemo.transferengine",
                                              "/org/nemo/transferengine",
                                              QDBusConnection::sessionBus(),
                                              this);
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
    d->m_client->cancelTransfer(transferId);
}

void DeclarativeTransferInterface::cbRestartTransfer(int transferId)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->restartTransfer(transferId);
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
    QDBusPendingReply<bool> reply = d->m_client->notificationsEnabled();
    reply.waitForFinished();

    if (reply.isError()) {
        qWarning() << "DeclarativeTransferInterface::notifications: failed to get notifications!";
        return false;
    }

    return reply.value();
}

int DeclarativeTransferInterface::createDownloadEvent(const QString &displayName,
                                                      const QUrl &applicationIcon,
                                                      const QUrl &serviceIcon,
                                                      const QUrl &url,
                                                      const QString &mimeType,
                                                      const QStringList &callback,
                                                      const QString &cancelMethod,
                                                      const QString &restartMethod)
{
    Q_D(const DeclarativeTransferInterface);
    QDBusPendingReply<int> reply = d->m_client->createDownload(displayName,
                                                               applicationIcon.toString(),
                                                               serviceIcon.toString(),
                                                               url.toString(),
                                                               mimeType,
                                                               callback,
                                                               cancelMethod,
                                                               restartMethod);
    reply.waitForFinished();

    if (reply.isError()) {
        qWarning() << "DeclarativeTransferInterface::createDownloadEvent: failed to get transfer ID!";
        return false;
    }

    return reply.value();
}

int DeclarativeTransferInterface::createSyncEvent(const QString &displayName,
                                                  const QUrl &applicationIcon,
                                                  const QUrl &serviceIcon,
                                                  const QStringList &callback,
                                                  const QString &cancelMethod,
                                                  const QString &restartMethod)
{
    Q_D(const DeclarativeTransferInterface);
    QDBusPendingReply<int> reply = d->m_client->createSync(displayName,
                                                           applicationIcon.toString(),
                                                           serviceIcon.toString(),
                                                           callback,
                                                           cancelMethod,
                                                           restartMethod);
    reply.waitForFinished();

    if (reply.isError()) {
        qWarning() << "DeclarativeTransferInterface::createSyncEvent: failed to get transfer ID!";
        return false;
    }

    return reply.value();
}

void DeclarativeTransferInterface::updateTransferProgress(int transferId, qreal progress)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->updateTransferProgress(transferId, progress);
}

void DeclarativeTransferInterface::finishTransfer(int transferId, Status status, const QString &reason)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->finishTransfer(transferId, static_cast<int>(status), reason);
}
