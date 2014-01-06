#ifndef DECLARATIVETRANSFERMETHODMODEL_H
#define DECLARATIVETRANSFERMETHODMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>

class DeclarativeTransferMethodsModelPrivate;
class DeclarativeTransferMethodsModel: public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

public:

    explicit  DeclarativeTransferMethodsModel(QObject *parent = 0);
    ~DeclarativeTransferMethodsModel();

    void classBegin();
    void componentComplete();

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    QString filter() const;
    void setFilter(const QString &filter);

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE int findMethod(const QString &methodId) const;

Q_SIGNALS:
    void rowCountChanged();
    void filterChanged();

private:
    DeclarativeTransferMethodsModelPrivate * d_ptr;
    Q_DISABLE_COPY(DeclarativeTransferMethodsModel)
    Q_DECLARE_PRIVATE(DeclarativeTransferMethodsModel)
};

#endif // DECLARATIVETRANSFERMETHODMODEL_H
