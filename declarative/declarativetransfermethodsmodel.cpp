#include "declarativetransfermethodsmodel.h"
#include "declarativetransfermethodsmodel_p.h"
#include "transferengineinterface.h"
#include "transfermethodinfo.h"


DeclarativeTransferMethodsModelPrivate::DeclarativeTransferMethodsModelPrivate(DeclarativeTransferMethodsModel * parent):
    QObject(parent),
    q_ptr(parent)
{
    m_client = new TransferEngineInterface("org.nemo.transferengine",
                                               "/org/nemo/transferengine",
                                               QDBusConnection::sessionBus(),
                                               this);
    connect(m_client, SIGNAL(transferMethodListChanged()), this, SLOT(transferMethods()));
}

DeclarativeTransferMethodsModelPrivate::~DeclarativeTransferMethodsModelPrivate()
{
    delete m_client;
}


void DeclarativeTransferMethodsModelPrivate::transferMethods()
{
    QDBusPendingReply<QList<TransferMethodInfo> > reply = m_client->transferMethods();
    reply.waitForFinished();

    if (reply.isError()) {
        qWarning() << "DeclarativeTransferMethodsModelPrivate::transferMethods: " << reply.error().message();
        return;
    }

    Q_Q(DeclarativeTransferMethodsModel);
    QList<TransferMethodInfo> newData = reply.value();
    bool rowCountChanged = m_data.count() != newData.count();

    // Not so nice, but we are dealing here relatively small amount of items,
    // let's just reset the model on changes.
    m_data = newData;
    q->reset();
    if (rowCountChanged) {
        emit q->rowCountChanged();
    }
}

QVariant DeclarativeTransferMethodsModelPrivate::value(int row, int role) const
{
    return m_data.at(row).value(role);
}


DeclarativeTransferMethodsModel::DeclarativeTransferMethodsModel(QObject *parent):
    QAbstractListModel(parent),
    QDeclarativeParserStatus(),
    d_ptr(new DeclarativeTransferMethodsModelPrivate(this))
{

}

DeclarativeTransferMethodsModel::~DeclarativeTransferMethodsModel()
{
    delete d_ptr;
    d_ptr = 0;
}

void DeclarativeTransferMethodsModel::classBegin()
{

}

void DeclarativeTransferMethodsModel::componentComplete()
{
    QHash<int, QByteArray> roleNames;
    roleNames[TransferMethodInfo::DisplayName]      = "displayName";
    roleNames[TransferMethodInfo::UserName]         = "userName";
    roleNames[TransferMethodInfo::MethodId]         = "methodId";
    roleNames[TransferMethodInfo::AccountId]        = "accountId";
    roleNames[TransferMethodInfo::AccountRequired]  = "accountRequired";
    setRoleNames(roleNames);

    Q_D(DeclarativeTransferMethodsModel);
    d->transferMethods();
}

QVariant DeclarativeTransferMethodsModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        qWarning() << "DeclarativeTransferMethodsModel::data: invalid index!";
        return QVariant();
    }

    Q_D(const DeclarativeTransferMethodsModel);
    if (index.row() < 0 || index.row() > d->m_data.count()) {
        qWarning() << "DeclarativeTransferMethodsModel::data: Index out of range!";
        return QVariant();
    }

    return d->value(index.row(), role);
}

int DeclarativeTransferMethodsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_data.count();
}


QStringList DeclarativeTransferMethodsModel::filters() const
{

}
void DeclarativeTransferMethodsModel::setFilters(const QString &filters)
{

}
