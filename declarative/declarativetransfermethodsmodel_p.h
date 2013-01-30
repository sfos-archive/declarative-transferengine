#ifndef DECLARATIVETRANSFERMETHODSMODEL_P_H
#define DECLARATIVETRANSFERMETHODSMODEL_P_H

#include "transfermethodinfo.h"
#include "declarativetransfermethodsmodel.h"
#include <QObject>

class TransferEngineInterface;
//class DeclarativeTransferMethodsModel;
class DeclarativeTransferMethodsModelPrivate: public QObject
{
    Q_OBJECT
public:

    DeclarativeTransferMethodsModelPrivate(DeclarativeTransferMethodsModel * parent);
    ~DeclarativeTransferMethodsModelPrivate();
    QVariant value(int row, int role) const;

public Q_SLOTS:
    void transferMethods();

public:
// Member vars
    DeclarativeTransferMethodsModel *q_ptr;
    Q_DECLARE_PUBLIC(DeclarativeTransferMethodsModel)

    TransferEngineInterface *m_client;
    QList<TransferMethodInfo> m_data;
};

#endif // DECLARATIVETRANSFERMETHODSMODEL_P_H
