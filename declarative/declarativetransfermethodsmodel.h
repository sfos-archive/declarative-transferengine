#ifndef DECLARATIVETRANSFERMETHODMODEL_H
#define DECLARATIVETRANSFERMETHODMODEL_H

#include <QAbstractListModel>
#include <QDeclarativeParserStatus>

class DeclarativeTransferMethodsModelPrivate;
class DeclarativeTransferMethodsModel: public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(QStringList filters READ filters WRITE setFilters NOTIFY filtersChanged)

public:

    explicit  DeclarativeTransferMethodsModel(QObject *parent = 0);
    ~DeclarativeTransferMethodsModel();

    void classBegin();
    void componentComplete();

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    QStringList filters() const;
    void setFilters(const QString &filters);

Q_SIGNALS:
    void rowCountChanged();
    void filtersChanged();

private:
    DeclarativeTransferMethodsModelPrivate * d_ptr;
    Q_DISABLE_COPY(DeclarativeTransferMethodsModel)
    Q_DECLARE_PRIVATE(DeclarativeTransferMethodsModel)
};

#endif // DECLARATIVETRANSFERMETHODMODEL_H
