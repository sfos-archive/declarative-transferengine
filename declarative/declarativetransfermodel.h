#ifndef TRANSFERMODEL_H
#define TRANSFERMODEL_H

#include <QAbstractListModel>
#include <QDeclarativeParserStatus>

#include "transfertypes.h"


class DeclarativeTransferModelPrivate;
class DeclarativeTransferModel: public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_ENUMS(TransferStatus)
    Q_ENUMS(TransferType)
    Q_PROPERTY(int count READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int transfersInProgress READ transfersInProgress NOTIFY transfersInProgressChanged)

public:

    enum TransferStatus {        
        NotStarted          = TransferEngineData::NotStarted,
        TransferStarted     = TransferEngineData::TransferStarted,
        TransferCanceled    = TransferEngineData::TransferCanceled,
        TransferFinished    = TransferEngineData::TransferFinished,
        TransferInterrupted = TransferEngineData::TransferInterrupted
    };

    enum TransferType {
        Upload      = TransferEngineData::Upload,
        Download    = TransferEngineData::Download,
        Sync        = TransferEngineData::Sync
    };

    explicit  DeclarativeTransferModel(QObject *parent = 0);
    ~DeclarativeTransferModel();

    void classBegin();
    void componentComplete();

    int transfersInProgress() const;

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    Q_INVOKABLE void clearTransfers();
    Q_INVOKABLE QVariantMap get(int index) const;

Q_SIGNALS:
    void rowCountChanged();
    void transferStatusChanged(int index, int status);
    void transfersInProgressChanged();

private:
    DeclarativeTransferModelPrivate *d_ptr;
    Q_DISABLE_COPY(DeclarativeTransferModel)
    Q_DECLARE_PRIVATE(DeclarativeTransferModel)
};


#endif // TRANSFERMODEL_H
