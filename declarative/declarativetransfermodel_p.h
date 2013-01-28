#ifndef DECLARATIVETRANSFERMODEL_P_H
#define DECLARATIVETRANSFERMODEL_P_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>
#include "transferengineinterface.h"

#include "transferdbrecord.h"

class DeclarativeTransferModel;
class  DeclarativeTransferModelPrivate: public QObject
{
    Q_OBJECT
public:
    DeclarativeTransferModelPrivate(DeclarativeTransferModel *parent);
    ~DeclarativeTransferModelPrivate();

    inline QList<TransferDBRecord>::iterator record(int key, int &row, bool &ok);
    QVariant value(int row, int role) const;
    void clearTransfers();
    void cancelTransfer(int transferId);

public Q_SLOTS:
    void refreshData();
    void refreshProgress(int key, double progress);
    void refreshStatus(int key, int status);
    void dataReceived(QDBusPendingCallWatcher *call);

public:
    DeclarativeTransferModel *q_ptr;
    Q_DISABLE_COPY(DeclarativeTransferModelPrivate)
    Q_DECLARE_PUBLIC(DeclarativeTransferModel)

    TransferEngineInterface *m_client;
    QList<TransferDBRecord> m_data;
    bool m_dataReady;
    QMutex m_mutex;
};

#endif // DECLARATIVETRANSFERMODEL_P_H
