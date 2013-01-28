#ifndef DECLARATIVESHARE_P_H
#define DECLARATIVESHARE_P_H

#include <QObject>
#include <QUrl>
#include "declarativeshare.h"

class QDBusPendingCallWatcher;
class TransferEngineInterface;
class DeclarativeSharePrivate: public QObject
{
    Q_OBJECT
public:
    DeclarativeSharePrivate(DeclarativeShare *parent);

public Q_SLOTS:
    void progressChanged(int transferId, double progress);
    void statusChanged(int transferId, int status);
    void transferIdReceived(QDBusPendingCallWatcher *call);

public:
    QUrl m_source;
    QString m_title;
    QString m_description;
    QString m_serviceId;
    QString m_mimeType;
    bool m_metadataStripped;
    bool m_notificationsEnabled;
    int m_transferId;
    quint32 m_accountId;
    qreal m_progress;
    DeclarativeShare::Status m_status;
    DeclarativeShare::Status m_cachedStatus;
    TransferEngineInterface *m_client;
    DeclarativeShare *q_ptr;
    Q_DECLARE_PUBLIC(DeclarativeShare)
};


#endif // DECLARATIVESHARE_P_H
