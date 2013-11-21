#include "declarativetransfermethodsmodel.h"
#include "declarativetransfermethodsmodel_p.h"
#include "transferengineinterface.h"
#include "transfermethodinfo.h"

#include <Accounts/Manager>

DeclarativeTransferMethodsModelPrivate::DeclarativeTransferMethodsModelPrivate(DeclarativeTransferMethodsModel * parent):
    QObject(parent),
    q_ptr(parent),
    m_client(0),
    m_data(),
    m_filter(),
    m_filteredData()
{
    m_client = new TransferEngineInterface("org.nemo.transferengine",
                                               "/org/nemo/transferengine",
                                               QDBusConnection::sessionBus(),
                                               this);
    connect(m_client, SIGNAL(transferMethodListChanged()), this, SLOT(updateModel()));

    // Wake up the engine if accounts have changed and ask the new list via updateModel
    m_accountManager = new Accounts::Manager("sharing", this);
    connect(m_accountManager, SIGNAL(accountCreated(Accounts::AccountId)), this, SLOT(updateModel()));
    connect(m_accountManager, SIGNAL(accountRemoved(Accounts::AccountId)), this, SLOT(updateModel()));
    connect(m_accountManager, SIGNAL(accountUpdated(Accounts::AccountId)), this, SLOT(updateModel()));
}

DeclarativeTransferMethodsModelPrivate::~DeclarativeTransferMethodsModelPrivate()
{
    delete m_client;
    delete m_accountManager;
}


void DeclarativeTransferMethodsModelPrivate::updateModel()
{
    QDBusPendingReply<QList<TransferMethodInfo> > reply = m_client->transferMethods();
    reply.waitForFinished();

    if (reply.isError()) {
        qWarning() << "DeclarativeTransferMethodsModelPrivate::transferMethods: " << reply.error().message();
        return;
    }

    // Set new data to the model and filter it if filter has been set.
    Q_Q(DeclarativeTransferMethodsModel);
    m_data = reply.value();
    filterModel();
}

QVariant DeclarativeTransferMethodsModelPrivate::value(int row, int role) const
{
    return m_data.at(m_filteredData.at(row)).value(role);
}

QVariantMap DeclarativeTransferMethodsModelPrivate::get(int index) const
{
    Q_Q(const DeclarativeTransferMethodsModel);
    if (index < 0 || index >= m_filteredData.count()) {
        return QVariantMap();
    }
    const TransferMethodInfo &method = m_data.at(m_filteredData.at(index));
    QHash<int, QByteArray> roles = q->roleNames();
    QVariantMap map;
    for (QHash<int, QByteArray>::iterator it = roles.begin(); it != roles.end(); ++it) {
        map[it.value()] = method.value(it.key());
    }
    return map;
}

int DeclarativeTransferMethodsModelPrivate::findMethod(const QString &methodId) const
{
    for (int i=0; i<m_filteredData.count(); i++) {
        const TransferMethodInfo &method = m_data.at(m_filteredData.at(i));
        if (method.methodId == methodId) {
            return i;
        }
    }
    return -1;
}

// Filtering model is made by storing indeces of the items in m_data
// which match the filtering criteria. Stored indeces in m_filteredData
// are used for  accessing data from m_data.
void DeclarativeTransferMethodsModelPrivate::filterModel()
{
    Q_Q(DeclarativeTransferMethodsModel);
    const int oldRowCount = m_filteredData.count();
    m_filteredData.clear();

    // Accept everything if using wildcard or filter is empty
    if (m_filter.isEmpty() || m_filter == QLatin1String("*")) {
        for (int i=0; i < m_data.count(); i++) {
            m_filteredData.append(i);
        }
    } else {
        int index = 0;
        int slashIndex = m_filter.indexOf('/');
        QString subTypeWildcard = slashIndex >= 0 ? m_filter.mid(0, slashIndex) + "/*" : "";
        Q_FOREACH(TransferMethodInfo info, m_data) {
            if (info.capabilitities.contains(m_filter) ||
                info.capabilitities.contains(subTypeWildcard) ||
                info.capabilitities.contains(QLatin1String("*"))) {
                m_filteredData.append(index);
            }

            ++index;
        }
    }
    // Model is small so let's just reset it to instead of keeping
    // track which model items have changed i.e. added or removed
    // after filtering. Works fine for a relatively small amount of items.
    q->reset();


    if (oldRowCount != m_filteredData.count()) {
        emit q->rowCountChanged();
    }
}

DeclarativeTransferMethodsModel::DeclarativeTransferMethodsModel(QObject *parent):
    QAbstractListModel(parent),
    QQmlParserStatus(),
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
    roleNames[TransferMethodInfo::ShareUIPath]      = "shareUIPath";
    roleNames[TransferMethodInfo::AccountId]        = "accountId";
    setRoleNames(roleNames);

    Q_D(DeclarativeTransferMethodsModel);
    d->updateModel();
}

QVariant DeclarativeTransferMethodsModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        qWarning() << "DeclarativeTransferMethodsModel::data: invalid index!";
        return QVariant();
    }

    Q_D(const DeclarativeTransferMethodsModel);
    if (index.row() < 0 || index.row() > d->m_filteredData.count()) {
        qWarning() << "DeclarativeTransferMethodsModel::data: Index out of range!";
        return QVariant();
    }

    return d->value(index.row(), role);
}

int DeclarativeTransferMethodsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_filteredData.count();
}


QString DeclarativeTransferMethodsModel::filter() const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_filter;
}

void DeclarativeTransferMethodsModel::setFilter(const QString &filter)
{
    Q_D(DeclarativeTransferMethodsModel);
    if (d->m_filter != filter) {
        d->m_filter = filter;
        d->filterModel();
        emit filterChanged();
    }
}

QVariantMap DeclarativeTransferMethodsModel::get(int index) const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->get(index);
}

int DeclarativeTransferMethodsModel::findMethod(const QString &methodId) const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->findMethod(methodId);
}
