#include "declarativetransfermodel.h"
#include "declarativetransfermodel_p.h"
#include "transferdbrecord.h"

#include <QtDebug>
#include <QDBusPendingReply>



DeclarativeTransferModelPrivate::DeclarativeTransferModelPrivate(DeclarativeTransferModel *parent):
    q_ptr(parent)
{
    m_client = new TransferEngineInterface("org.nemo.transferengine",
                                           "/org/nemo/transferengine",
                                           QDBusConnection::sessionBus(),
                                           this);

    connect(m_client,   SIGNAL(progressChanged(int,double)),
            this,       SLOT(refreshProgress(int,double)));
    connect(m_client,   SIGNAL(transfersChanged()),
            this,       SLOT(refreshData()));
    connect(m_client,   SIGNAL(statusChanged(int,int)),
            this,       SLOT(refreshStatus(int,int)));
}

DeclarativeTransferModelPrivate::~DeclarativeTransferModelPrivate()
{
    delete m_client;
}

QList<TransferDBRecord>::iterator DeclarativeTransferModelPrivate::record(int key, int &row, bool &ok)
{
    ok = false;
    row = -1;
    QList<TransferDBRecord>::iterator i;
    for (i = m_data.begin(); i != m_data.end(); ++i) {
        ++row;

        if ((*i).transfer_id == key) {
            ok = true;
            return i;
        }
    }
    row = -1;
    if (!ok) {
        qWarning() << "DeclarativeTransferModelPrivate::record: Failed to find the correct row";
        return i;
    }

    return i;
}

void DeclarativeTransferModelPrivate::refreshProgress(int key, double progress)
{
    QMutexLocker locker(&m_mutex);
    bool ok;
    int row;
    QList<TransferDBRecord>::iterator i = record(key, row, ok);

    if (!ok) {
        qWarning() << "DeclarativeTransferModelPrivate::refreshProgress: failed to fetch a record!";
        return;
    }

    (*i).progress = progress;
    Q_Q(DeclarativeTransferModel);
    emit q->dataChanged(q->createIndex(row, 0), q->createIndex(row, 0));
}

void DeclarativeTransferModelPrivate::refreshStatus(int key, int status)
{
    QMutexLocker locker(&m_mutex);
    bool ok;
    int row;
    QList<TransferDBRecord>::iterator i = record(key, row, ok);

    if (!ok) {
        qWarning() << "DeclarativeTransferModelPrivate::refreshStatus: failed to fetch a record!";
        return;
    }

    (*i).status = status;
    Q_Q(DeclarativeTransferModel);
    emit q->dataChanged(q->createIndex(row, 0), q->createIndex(row, 0));
}


QVariant DeclarativeTransferModelPrivate::value(int row, int role) const
{
    return m_data.at(row).value(role);
}


void DeclarativeTransferModelPrivate::clearTransfers()
{
    m_client->clearTransfers();
}

void DeclarativeTransferModelPrivate::cancelTransfer(int transferId)
{
    m_client->cancelTransfer(transferId);
}


void DeclarativeTransferModelPrivate::refreshData()
{
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_client->transfers(), this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(dataReceived(QDBusPendingCallWatcher*)));

}

void DeclarativeTransferModelPrivate::dataReceived(QDBusPendingCallWatcher *call)
{
    QMutexLocker locker(&m_mutex);
    QDBusPendingReply<QList<TransferDBRecord> > reply = *call;

    if (reply.isError()) {
        qWarning() << " DeclarativeTransferModelPrivate::loadData: " << reply.error().message();
        return;
    }

    Q_Q(DeclarativeTransferModel);
    QList<TransferDBRecord> newData = reply.value();
    const int oldRowCount = m_data.count();
    const int newRowCount = newData.count();

    // Handle all the rows which are removed (We can't have only one row removed)
    if (newData.isEmpty() && !m_data.isEmpty()) {
        q->beginRemoveRows(QModelIndex(), 0, oldRowCount);
        m_data.clear();
        q->endRemoveRows();
    }
    else
    // There are still rows left (active transfers)
    if (newRowCount > 0 && newRowCount < oldRowCount ) {
        q->beginRemoveRows(QModelIndex(),newRowCount, oldRowCount - 1);
        m_data = newData;
        q->endRemoveRows();
    }
    else
    // Handle row added i.e. a new transfer. We can "trust" that new items are always the first
    // items, because db sorts the query like that.
    if (newRowCount > oldRowCount) {
        q->beginInsertRows(QModelIndex(), 0, (newRowCount - oldRowCount)-1);
        m_data = newData;
        q->endInsertRows();
    }

    if (newRowCount != oldRowCount) {
        emit q->rowCountChanged();
    }

    call->deleteLater();
    qDebug() << "DeclarativeTransferModelPrivate::dataReceived: new row count: " << newRowCount;
}



DeclarativeTransferModel::DeclarativeTransferModel(QObject *parent):
    QAbstractListModel(parent),
    QDeclarativeParserStatus(),
    d_ptr(new DeclarativeTransferModelPrivate(this))
{

}


DeclarativeTransferModel::~DeclarativeTransferModel()
{
    delete d_ptr;
    d_ptr = 0;
}

void DeclarativeTransferModel::classBegin()
{

}

void DeclarativeTransferModel::componentComplete()
{

    // Role names could come from the "server" if
    // they change too often e.g. during development.
    QHash<int, QByteArray> roleNames;
    roleNames[TransferDBRecord::TransferID] = "transferId";
    roleNames[TransferDBRecord::TransferType] = "transferType";
    roleNames[TransferDBRecord::Progress]   = "progress";
    roleNames[TransferDBRecord::URL]        = "url";
    roleNames[TransferDBRecord::Status]     = "status";
    roleNames[TransferDBRecord::PluginID]   = "pluginId";
    roleNames[TransferDBRecord::Timestamp]  = "timestamp";
    roleNames[TransferDBRecord::DisplayName]= "displayName";
    roleNames[TransferDBRecord::MimeType]   = "mimeType";
    roleNames[TransferDBRecord::FileSize]   = "fileSize";
    roleNames[TransferDBRecord::ServiceIcon]= "serviceIcon";
    setRoleNames(roleNames);

    Q_D(DeclarativeTransferModel);
    d->refreshData();
}


QVariant DeclarativeTransferModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qWarning() << "DeclarativeTransferModel::data: invalid index!";
        return QVariant();
    }

    Q_D(const DeclarativeTransferModel);
    if (index.row() < 0 || index.row() > d->m_data.count()) {
        qWarning() << "DeclarativeTransferModel::data: Index out of range!";
        return QVariant();
    }

    return d->value(index.row(), role);
}

int DeclarativeTransferModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);
    Q_D(const DeclarativeTransferModel);
    return d->m_data.count();
}


void DeclarativeTransferModel::clearTransfers()
{
    Q_D(DeclarativeTransferModel);
    d->clearTransfers();
}




