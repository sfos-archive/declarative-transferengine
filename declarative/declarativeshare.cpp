#include "declarativeshare.h"
#include "declarativeshare_p.h"
#include "transferengineinterface.h"
#include <QtDBus>
#include <QDBusPendingCallWatcher>

DeclarativeSharePrivate::DeclarativeSharePrivate(DeclarativeShare *parent):
    QObject(),
    m_source(),
    m_serviceId(),
    m_mimeType(),
    m_metadataStripped(false),
    m_notificationsEnabled(false),
    m_transferId(-1),
    m_progress(0),
    m_status(DeclarativeShare::NotStarted),
    m_cachedStatus(DeclarativeShare::NotStarted),
    m_client(0),
    q_ptr(parent)
{
    m_client = new TransferEngineInterface("org.nemo.transferengine",
                                           "/org/nemo/transferengine",
                                            QDBusConnection::sessionBus(),
                                           this);
    connect(m_client, SIGNAL(progressChanged(int,double)),
            this,     SLOT(progressChanged(int,double)));
    connect(m_client, SIGNAL(statusChanged(int,int)),
            this,     SLOT(statusChanged(int,int)));
}

void DeclarativeSharePrivate::progressChanged(int transferId, double progress)
{
    if (transferId != m_transferId) {
        return;
    }

    if (m_progress != progress) {
        m_progress = progress;
        Q_Q(DeclarativeShare);
        emit q->progressChanged(m_progress);
    }
}

void DeclarativeSharePrivate::statusChanged(int transferId, int status)
{
    if (m_transferId == -1) {
        m_cachedStatus = static_cast<DeclarativeShare::Status>(status);
        return;
    }

    if (transferId != m_transferId) {
        return;
    }

    if (m_status != status) {
        m_status = static_cast<DeclarativeShare::Status>(status);
        Q_Q(DeclarativeShare);
        emit q->statusChanged(m_status);
    }
}

void DeclarativeSharePrivate::transferIdReceived(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<int> reply = *call;
    if (reply.isError()) {
        qWarning() << "DeclarativeSharePrivate::transferIdReceived: failed to get transferId" << reply.error();
        return;
    }

    m_transferId = reply.value();
    call->deleteLater();

    // Make sure that we have status set right.
    if (m_cachedStatus != DeclarativeShare::NotStarted) {
        m_status = m_cachedStatus;
        m_cachedStatus = DeclarativeShare::NotStarted;
        Q_Q(DeclarativeShare);
        emit q->statusChanged(m_status);
    }
}

DeclarativeShare::DeclarativeShare(QQuickItem *parent):
    QQuickItem(parent),
    d_ptr(new DeclarativeSharePrivate(this))
{

}

DeclarativeShare::~DeclarativeShare()
{
    delete d_ptr;
    d_ptr = 0;
}

void DeclarativeShare::setContent(const QVariantMap &content)
{
    Q_D(DeclarativeShare);
    if (d->m_content != content) {
        d->m_content = content;
        emit contentChanged();
    }
}

QVariantMap DeclarativeShare::DeclarativeShare::content() const
{
    Q_D(const DeclarativeShare);
    return d->m_content;
}

void DeclarativeShare::setSource(const QUrl source)
{
    Q_D(DeclarativeShare);
    if (d->m_source != source) {
        d->m_source = source;
        emit sourceChanged();
    }
}

QUrl DeclarativeShare::source() const
{
    Q_D(const DeclarativeShare);
    return d->m_source;
}

void DeclarativeShare::setMetadataStripped(bool strip)
{
    Q_D(DeclarativeShare);
    if (d->m_metadataStripped != strip) {
        d->m_metadataStripped = strip;
        emit metadataStrippedChanged();
    }
}

bool DeclarativeShare::metadataStripped() const
{
    Q_D(const DeclarativeShare);
    return d->m_metadataStripped;
}

void DeclarativeShare::setServiceId(const QString &id)
{
    Q_D(DeclarativeShare);
    if (d->m_serviceId != id) {
        d->m_serviceId = id;
        emit serviceIdChanged();
    }
}

QString DeclarativeShare::serviceId() const
{
    Q_D(const DeclarativeShare);
    return d->m_serviceId;
}

void DeclarativeShare::setMimeType(const QString &mimeType)
{
    Q_D(DeclarativeShare);
    if (d->m_mimeType != mimeType) {
        d->m_mimeType = mimeType;
        emit mimeTypeChanged();
    }
}

QString DeclarativeShare::mimeType() const
{
    Q_D(const DeclarativeShare);
    return d->m_mimeType;
}

void DeclarativeShare::setUserData(const QVariantMap &userData)
{
    Q_D(DeclarativeShare);
    if (d->m_userData != userData) {
        d->m_userData = userData;
        emit userDataChanged();
    }
}

QVariantMap DeclarativeShare::userData() const
{
    Q_D(const DeclarativeShare);
    return d->m_userData;
}




void DeclarativeShare::setNotificationsEnabled(bool enable)
{
    Q_D(DeclarativeShare);
    if (d->m_notificationsEnabled != enable) {
        d->m_notificationsEnabled = enable;
        d->m_client->enableNotifications(enable);
        emit notificationsEnabledChanged();
    }
}

bool DeclarativeShare::notificationsEnabled() const
{
    Q_D(const DeclarativeShare);
    return d->m_notificationsEnabled;
}

qreal DeclarativeShare::progress() const
{
    Q_D(const DeclarativeShare);
    return d->m_progress;
}

DeclarativeShare::Status DeclarativeShare::status() const
{
    Q_D(const DeclarativeShare);
    return d->m_status;
}


void DeclarativeShare::start()
{
    Q_D(DeclarativeShare);
    QDBusPendingCall async = d->m_content.isEmpty()
            ? d->m_client->uploadMediaItem(d->m_source.toString(),
                                           d->m_serviceId,
                                           d->m_mimeType,
                                           d->m_metadataStripped,
                                           d->m_userData)
            : d->m_client->uploadMediaItemContent(d->m_content,
                                                  d->m_serviceId,
                                                  d->m_userData);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(async, this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            d,      SLOT(transferIdReceived(QDBusPendingCallWatcher*)));
}



void DeclarativeShare::cancel()
{
    Q_D(DeclarativeShare);
    if (d->m_status == TransferStarted &&
        d->m_transferId != -1){
        d->m_client->cancelTransfer(d->m_transferId);
    }
}
