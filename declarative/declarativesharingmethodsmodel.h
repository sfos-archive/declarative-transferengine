/*
 * Copyright (c) 2013 - 2018 Jolla Ltd.
 * Copyright (c) 2019 - 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary
 */

#ifndef DECLARATIVESHARINGMETHODMODEL_H
#define DECLARATIVESHARINGMETHODMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>

class QDBusPendingCallWatcher;
class DeclarativeSharingMethodsModelPrivate;

class DeclarativeSharingMethodsModel: public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    Q_PROPERTY(bool filterByMultipleFileSupport READ filterByMultipleFileSupport WRITE setFilterByMultipleFileSupport NOTIFY filterByMultipleFileSupportChanged)
    Q_PROPERTY(QString mimeTypeFilter READ mimeTypeFilter WRITE setMimeTypeFilter NOTIFY mimeTypeFilterChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit DeclarativeSharingMethodsModel(QObject *parent = 0);
    ~DeclarativeSharingMethodsModel();

    void classBegin();
    void componentComplete();

    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    bool ready() const;

    QString mimeTypeFilter() const;
    void setMimeTypeFilter(const QString &mimeTypeFilter);

    bool filterByMultipleFileSupport() const;
    void setFilterByMultipleFileSupport(bool filterByMultipleFileSupport);

    Q_INVOKABLE QVariantMap get(int index) const;

Q_SIGNALS:
    void rowCountChanged();
    void readyChanged();
    void mimeTypeFilterChanged();
    void filterByMultipleFileSupportChanged();

private:
    DeclarativeSharingMethodsModelPrivate * d_ptr;
    Q_DISABLE_COPY(DeclarativeSharingMethodsModel)
    Q_DECLARE_PRIVATE(DeclarativeSharingMethodsModel)
};

#endif // DECLARATIVESHARINGMETHODMODEL_H
