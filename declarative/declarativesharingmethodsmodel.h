/****************************************************************************************
** Copyright (c) 2013 - 2013 Jolla Ltd.
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
