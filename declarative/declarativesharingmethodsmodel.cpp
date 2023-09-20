/****************************************************************************************
** Copyright (c) 2013 - 2023 Jolla Ltd.
** Copyright (c) 2019 - 2021 Open Mobile Platform LLC.
**
** All rights reserved.
**
** This file is part of Sailfish Transfer Engine component package.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

#include "declarativesharingmethodsmodel.h"
#include "declarativesharingmethodsmodel_p.h"

#include <QDebug>

DeclarativeSharingMethodsModelPrivate::DeclarativeSharingMethodsModelPrivate(DeclarativeSharingMethodsModel * parent)
    : QObject(parent)
    , q_ptr(parent)
{
    connect(&m_pluginLoader, &SharingPluginLoader::pluginsChanged,
            this, &DeclarativeSharingMethodsModelPrivate::updateModel);
    connect(&m_pluginLoader, &SharingPluginLoader::pluginsLoaded,
            this, &DeclarativeSharingMethodsModelPrivate::modelDataReceived);
}

DeclarativeSharingMethodsModelPrivate::~DeclarativeSharingMethodsModelPrivate()
{
}

void DeclarativeSharingMethodsModelPrivate::updateModel()
{
    m_pluginLoader.reload();
}

void DeclarativeSharingMethodsModelPrivate::modelDataReceived()
{
    Q_Q(DeclarativeSharingMethodsModel);
    m_data = m_pluginLoader.plugins();
    filterModel();

    if (!m_ready) {
        m_ready = true;
        emit q->readyChanged();
    }
}

QVariant DeclarativeSharingMethodsModelPrivate::value(int row, int role) const
{
    return m_data.at(m_filteredData.at(row)).value(role);
}

QVariantMap DeclarativeSharingMethodsModelPrivate::get(int index) const
{
    Q_Q(const DeclarativeSharingMethodsModel);
    if (index < 0 || index >= m_filteredData.count()) {
        return QVariantMap();
    }
    const SharingMethodInfo &method = m_data.at(m_filteredData.at(index));
    QHash<int, QByteArray> roles = q->roleNames();
    QVariantMap map;
    for (QHash<int, QByteArray>::iterator it = roles.begin(); it != roles.end(); ++it) {
        map[it.value()] = method.value(it.key());
    }
    return map;
}

bool DeclarativeSharingMethodsModelPrivate::filterAccepts(const QStringList &capabilities, bool supportsMultipleFiles)
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
            || capabilities.contains(QStringLiteral("*"))
            || capabilities.contains(QStringLiteral("*/*"));
}

// Filtering model is made by storing indeces of the items in m_data
// which match the filtering criteria. Stored indeces in m_filteredData
// are used for  accessing data from m_data.
void DeclarativeSharingMethodsModelPrivate::filterModel()
{
    Q_Q(DeclarativeSharingMethodsModel);

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
        for (const SharingMethodInfo &info : m_data) {
            if (filterAccepts(info.capabilities(), info.supportsMultipleFiles())) {
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

DeclarativeSharingMethodsModel::DeclarativeSharingMethodsModel(QObject *parent):
    QAbstractListModel(parent),
    QQmlParserStatus(),
    d_ptr(new DeclarativeSharingMethodsModelPrivate(this))
{
}

DeclarativeSharingMethodsModel::~DeclarativeSharingMethodsModel()
{
    delete d_ptr;
    d_ptr = 0;
}

void DeclarativeSharingMethodsModel::classBegin()
{
}

void DeclarativeSharingMethodsModel::componentComplete()
{
}

QHash<int, QByteArray> DeclarativeSharingMethodsModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[SharingMethodInfo::DisplayName] = "displayName";
    roleNames[SharingMethodInfo::Subtitle] = "subtitle";
    roleNames[SharingMethodInfo::MethodId] = "methodId";
    roleNames[SharingMethodInfo::MethodIcon] = "methodIcon";
    roleNames[SharingMethodInfo::ShareUIPath] = "shareUIPath";
    roleNames[SharingMethodInfo::AccountId] = "accountId";
    roleNames[SharingMethodInfo::SupportsMultipleFiles] = "supportsMultipleFiles";

    return roleNames;
}

QVariant DeclarativeSharingMethodsModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        qWarning() << Q_FUNC_INFO << "invalid index!";
        return QVariant();
    }

    Q_D(const DeclarativeSharingMethodsModel);
    if (index.row() < 0 || index.row() > d->m_filteredData.count()) {
        qWarning() << Q_FUNC_INFO << "Index out of range!";
        return QVariant();
    }

    return d->value(index.row(), role);
}

int DeclarativeSharingMethodsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    Q_D(const DeclarativeSharingMethodsModel);
    return d->m_filteredData.count();
}

bool DeclarativeSharingMethodsModel::ready() const
{
    Q_D(const DeclarativeSharingMethodsModel);
    return d->m_ready;
}

QString DeclarativeSharingMethodsModel::mimeTypeFilter() const
{
    Q_D(const DeclarativeSharingMethodsModel);
    return d->m_mimeTypeFilter;
}

void DeclarativeSharingMethodsModel::setMimeTypeFilter(const QString &filter)
{
    Q_D(DeclarativeSharingMethodsModel);
    if (d->m_mimeTypeFilter != filter) {
        d->m_mimeTypeFilter = filter;
        d->filterModel();
        emit mimeTypeFilterChanged();
    }
}

bool DeclarativeSharingMethodsModel::filterByMultipleFileSupport() const
{
    Q_D(const DeclarativeSharingMethodsModel);
    return d->m_filterByMultipleFileSupport;
}

void DeclarativeSharingMethodsModel::setFilterByMultipleFileSupport(bool filterByMultipleFileSupport)
{
    Q_D(DeclarativeSharingMethodsModel);
    if (d->m_filterByMultipleFileSupport != filterByMultipleFileSupport) {
        d->m_filterByMultipleFileSupport = filterByMultipleFileSupport;
        d->filterModel();
        emit filterByMultipleFileSupportChanged();
    }
}

QVariantMap DeclarativeSharingMethodsModel::get(int index) const
{
    Q_D(const DeclarativeSharingMethodsModel);
    return d->get(index);
}
