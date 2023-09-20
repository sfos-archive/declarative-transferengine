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
