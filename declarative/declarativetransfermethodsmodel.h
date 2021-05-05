#ifndef DECLARATIVETRANSFERMETHODMODEL_H
#define DECLARATIVETRANSFERMETHODMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>

class QDBusPendingCallWatcher;
class DeclarativeTransferMethodsModelPrivate;

class DeclarativeTransferMethodsModel: public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    Q_PROPERTY(bool error READ error NOTIFY errorChanged)
    Q_PROPERTY(bool filterByMultipleFileSupport READ filterByMultipleFileSupport WRITE setFilterByMultipleFileSupport NOTIFY filterByMultipleFileSupportChanged)
    Q_PROPERTY(QString mimeTypeFilter READ mimeTypeFilter WRITE setMimeTypeFilter NOTIFY mimeTypeFilterChanged)
    Q_PROPERTY(QStringList accountProviderNames READ accountProviderNames NOTIFY accountProviderNamesChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit DeclarativeTransferMethodsModel(QObject *parent = 0);
    ~DeclarativeTransferMethodsModel();

    void classBegin();
    void componentComplete();

    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    bool ready() const;
    bool error() const;
    QStringList accountProviderNames() const;

    QString mimeTypeFilter() const;
    void setMimeTypeFilter(const QString &mimeTypeFilter);

    bool filterByMultipleFileSupport() const;
    void setFilterByMultipleFileSupport(bool filterByMultipleFileSupport);

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE int findMethod(const QString &methodId) const;

Q_SIGNALS:
    void rowCountChanged();
    void readyChanged();
    void errorChanged();
    void mimeTypeFilterChanged();
    void filterByMultipleFileSupportChanged();
    void accountProviderNamesChanged();

private:
    DeclarativeTransferMethodsModelPrivate * d_ptr;
    Q_DISABLE_COPY(DeclarativeTransferMethodsModel)
    Q_DECLARE_PRIVATE(DeclarativeTransferMethodsModel)
};

#endif // DECLARATIVETRANSFERMETHODMODEL_H
