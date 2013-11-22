#ifndef DECLARATIVETRANSFERMETHODSMODEL_P_H
#define DECLARATIVETRANSFERMETHODSMODEL_P_H

#include "transfermethodinfo.h"
#include "declarativetransfermethodsmodel.h"
#include <QObject>

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

    void filterModel();

public Q_SLOTS:
    void updateModel();

public:
// Member vars
    DeclarativeTransferMethodsModel *q_ptr;
    Q_DECLARE_PUBLIC(DeclarativeTransferMethodsModel)

    TransferEngineInterface *m_client;
    QList<TransferMethodInfo> m_data;
    QString m_filter;
    QList<int> m_filteredData;
    Accounts::Manager *m_accountManager;
};

#endif // DECLARATIVETRANSFERMETHODSMODEL_P_H
