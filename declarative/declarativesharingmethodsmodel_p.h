/*
 * Copyright (c) 2013 - 2016 Jolla Ltd.
 * Copyright (c) 2019 - 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary
 */

#ifndef DECLARATIVESHARINGMETHODSMODEL_P_H
#define DECLARATIVESHARINGMETHODSMODEL_P_H

#include "sharingmethodinfo.h"
#include "sharingpluginloader.h"
#include "declarativesharingmethodsmodel.h"
#include <QObject>
#include <QVariantMap>
#include <QStringList>

class DeclarativeSharingMethodsModelPrivate: public QObject
{
    Q_OBJECT
public:

    DeclarativeSharingMethodsModelPrivate(DeclarativeSharingMethodsModel * parent);
    ~DeclarativeSharingMethodsModelPrivate();

    QVariant value(int row, int role) const;
    QVariantMap get(int row) const;
    bool filterAccepts(const QStringList &capabilities, bool supportsMultipleFiles);

    void filterModel();

public Q_SLOTS:
    void updateModel();
    void modelDataReceived();

public:
// Member vars
    DeclarativeSharingMethodsModel *q_ptr;
    Q_DECLARE_PUBLIC(DeclarativeSharingMethodsModel)

    SharingPluginLoader m_pluginLoader;
    QList<SharingMethodInfo> m_data;
    QString m_mimeTypeFilter;
    QList<int> m_filteredData;
    QList<QVariantMap> m_pluginsMetaData;
    bool m_ready = false;
    bool m_filterByMultipleFileSupport = false;
};

#endif // DECLARATIVESHARINGMETHODSMODEL_P_H
