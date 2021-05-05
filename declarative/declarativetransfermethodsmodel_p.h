#ifndef DECLARATIVETRANSFERMETHODSMODEL_P_H
#define DECLARATIVETRANSFERMETHODSMODEL_P_H

#include "transfermethodinfo.h"
#include "declarativetransfermethodsmodel.h"
#include <QObject>
#include <QVariantMap>
#include <QStringList>

namespace Accounts {
class Manager;
}

class TransferEngineInterface;
class DeclarativeTransferMethodsModelPrivate: public QObject
{
    Q_OBJECT
public:

    DeclarativeTransferMethodsModelPrivate(DeclarativeTransferMethodsModel * parent);
    ~DeclarativeTransferMethodsModelPrivate();

    QVariant value(int row, int role) const;
    QVariantMap get(int row) const;
    int findMethod(const QString &methodId) const;
    bool filterAccepts(const QStringList &capabilities, bool supportsMultipleFiles);

    void filterModel();

public Q_SLOTS:
    void updateModel();
    void modelDataReceived(QDBusPendingCallWatcher *call);
    void pluginMetaDataReceived(QDBusPendingCallWatcher *call);

public:
// Member vars
    DeclarativeTransferMethodsModel *q_ptr;
    Q_DECLARE_PUBLIC(DeclarativeTransferMethodsModel)

    TransferEngineInterface *m_client = nullptr;
    QList<TransferMethodInfo> m_data;
    QString m_mimeTypeFilter;
    QList<int> m_filteredData;
    QStringList m_accountProviderNames;
    Accounts::Manager *m_accountManager = nullptr;
    QList<QVariantMap> m_pluginsMetaData;
    bool m_ready = false;
    bool m_error = false;
    bool m_filterByMultipleFileSupport = false;
};

#endif // DECLARATIVETRANSFERMETHODSMODEL_P_H
