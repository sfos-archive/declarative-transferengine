#include "declarativetransfermethodsmodel.h"
#include "declarativetransfermethodsmodel_p.h"
#include "transferengineinterface.h"
#include "transfermethodinfo.h"
#include "transferplugininfo.h"

#include <Accounts/Manager>
#include <QDebug>

DeclarativeTransferMethodsModelPrivate::DeclarativeTransferMethodsModelPrivate(DeclarativeTransferMethodsModel * parent)
    : QObject(parent)
    , q_ptr(parent)
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
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_client->transferMethods2(), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(modelDataReceived(QDBusPendingCallWatcher*)));

    QDBusPendingCallWatcher *pluginMetaDataWatcher = new QDBusPendingCallWatcher(m_client->pluginMetaData(), this);
    connect(pluginMetaDataWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(pluginMetaDataReceived(QDBusPendingCallWatcher*)));
}

void DeclarativeTransferMethodsModelPrivate::modelDataReceived(QDBusPendingCallWatcher *call)
{
    Q_Q(DeclarativeTransferMethodsModel);
    QDBusPendingReply<QList<TransferMethodInfo> > reply = *call;

    bool isError = reply.isError();

    if (isError) {
        qWarning() << Q_FUNC_INFO << reply.error().message();
    } else {
        m_data = reply.value();
        filterModel();
    }

    if (m_error != isError) {
        m_error = isError;
        emit q->errorChanged();
    }

    if (!m_ready) {
        m_ready = true;
        emit q->readyChanged();
    }

    call->deleteLater();
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

bool DeclarativeTransferMethodsModelPrivate::filterAccepts(const QStringList &capabilities, bool supportsMultipleFiles)
{
    if (m_filterByMultipleFileSupport && !supportsMultipleFiles) {
        return false;
    }

    if (m_mimeTypeFilter.isEmpty() || m_mimeTypeFilter == QLatin1String("*")) {
        return true;
    }

    const int slashIndex = m_mimeTypeFilter.indexOf('/');
    const QString subTypeWildcard = slashIndex >= 0 ? m_mimeTypeFilter.mid(0, slashIndex) + "/*" : QString();
    return capabilities.contains(m_mimeTypeFilter)
            || capabilities.contains(subTypeWildcard)
            || capabilities.contains(QStringLiteral("*"));
}

void DeclarativeTransferMethodsModelPrivate::pluginMetaDataReceived(QDBusPendingCallWatcher *call)
{
    Q_Q(DeclarativeTransferMethodsModel);
    QDBusPendingReply<QList<QVariantMap> > reply = *call;
    call->deleteLater();

    if (reply.isError()) {
        qWarning() << Q_FUNC_INFO << reply.error().message();
        return;
    }

    QList<QVariantMap> metaDataList = reply.value();
    if (m_pluginsMetaData == metaDataList) {
        return;
    }

    m_pluginsMetaData = metaDataList;

    QStringList accountProviderNames;
    for (const QVariantMap &map : m_pluginsMetaData) {
        const QString providerName = map.value(QStringLiteral("accountProviderName")).toString();
        const QStringList capabilities = map.value(QStringLiteral("capabilities")).toStringList();

        if (!providerName.isEmpty() && filterAccepts(capabilities, false)) {
            accountProviderNames << providerName;
        }
    }

    if (m_accountProviderNames != accountProviderNames) {
        m_accountProviderNames = accountProviderNames;
        emit q->accountProviderNamesChanged();
    }
}

// Filtering model is made by storing indeces of the items in m_data
// which match the filtering criteria. Stored indeces in m_filteredData
// are used for  accessing data from m_data.
void DeclarativeTransferMethodsModelPrivate::filterModel()
{
    Q_Q(DeclarativeTransferMethodsModel);

    // Model is small so let's just reset it to instead of keeping
    // track which model items have changed i.e. added or removed
    // after filtering. Works fine for a relatively small amount of items.
    q->beginResetModel();

    const int oldRowCount = m_filteredData.count();
    m_filteredData.clear();

    // Accept everything if multi-file support is not required, and capabilities filter accepts all
    // file types.
    if (!m_filterByMultipleFileSupport
            && (m_mimeTypeFilter.isEmpty() || m_mimeTypeFilter == QLatin1String("*"))) {
        for (int i=0; i < m_data.count(); i++) {
            m_filteredData.append(i);
        }
    } else {
        int index = 0;
        Q_FOREACH(const TransferMethodInfo &info, m_data) {
            if (filterAccepts(info.capabilitities, info.supportsMultipleFiles())) {
                m_filteredData.append(index);
            }
            ++index;
        }
    }

    q->endResetModel();

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
    Q_D(DeclarativeTransferMethodsModel);
    d->updateModel();
}

QHash<int, QByteArray> DeclarativeTransferMethodsModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[TransferMethodInfo::DisplayName] = "displayName";
    roleNames[TransferMethodInfo::UserName] = "userName";
    roleNames[TransferMethodInfo::MethodId] = "methodId";
    roleNames[TransferMethodInfo::ShareUIPath] = "shareUIPath";
    roleNames[TransferMethodInfo::AccountId] = "accountId";
    roleNames[TransferMethodInfo::AccountIcon] = "accountIcon";
    roleNames[TransferMethodInfo::SupportsMultipleFiles] = "supportsMultipleFiles";

    return roleNames;
}

QVariant DeclarativeTransferMethodsModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        qWarning() << Q_FUNC_INFO << "invalid index!";
        return QVariant();
    }

    Q_D(const DeclarativeTransferMethodsModel);
    if (index.row() < 0 || index.row() > d->m_filteredData.count()) {
        qWarning() << Q_FUNC_INFO << "Index out of range!";
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

bool DeclarativeTransferMethodsModel::ready() const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_ready;
}

bool DeclarativeTransferMethodsModel::error() const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_error;
}

QStringList DeclarativeTransferMethodsModel::accountProviderNames() const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_accountProviderNames;
}

QString DeclarativeTransferMethodsModel::mimeTypeFilter() const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_mimeTypeFilter;
}

void DeclarativeTransferMethodsModel::setMimeTypeFilter(const QString &filter)
{
    Q_D(DeclarativeTransferMethodsModel);
    if (d->m_mimeTypeFilter != filter) {
        d->m_mimeTypeFilter = filter;
        d->filterModel();
        emit mimeTypeFilterChanged();
    }
}

bool DeclarativeTransferMethodsModel::filterByMultipleFileSupport() const
{
    Q_D(const DeclarativeTransferMethodsModel);
    return d->m_filterByMultipleFileSupport;
}

void DeclarativeTransferMethodsModel::setFilterByMultipleFileSupport(bool filterByMultipleFileSupport)
{
    Q_D(DeclarativeTransferMethodsModel);
    if (d->m_filterByMultipleFileSupport != filterByMultipleFileSupport) {
        d->m_filterByMultipleFileSupport = filterByMultipleFileSupport;
        d->filterModel();
        emit filterByMultipleFileSupportChanged();
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
