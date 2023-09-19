/****************************************************************************************
** Copyright (c) 2021 Open Mobile Platform LLC.
** Copyright (c) 2023 Jolla Ltd.
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

#ifndef SAILFISH_SHAREACTION_H
#define SAILFISH_SHAREACTION_H

#include <QObject>
#include <QVariantMap>
#include <QList>

#include <limits.h>

class ShareAction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList resources READ resources WRITE setResources NOTIFY resourcesChanged)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QVariantMap selectedTransferMethodInfo READ selectedTransferMethodInfo WRITE setSelectedTransferMethodInfo NOTIFY selectedTransferMethodInfoChanged)

public:
    ShareAction(QObject *parent = 0);

    QVariantList resources() const;
    void setResources(const QVariantList &resources);

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    QString title() const;
    void setTitle(const QString &title);

    QVariantMap selectedTransferMethodInfo() const;
    void setSelectedTransferMethodInfo(const QVariantMap &selectedTransferMethodInfo);

public slots:
    void trigger();
    void loadConfiguration(const QVariantMap &configuration);
    QVariantMap toConfiguration() const;
    void replaceFileResourcesWithFileDescriptors();
    QString writeContentToFile(const QVariantMap &contents, int maximumFileSize = INT_MAX);
    void removeFilesAndRmdir(const QStringList &files);

Q_SIGNALS:
    void done();
    void resourcesChanged();
    void mimeTypeChanged();
    void titleChanged();
    void selectedTransferMethodInfoChanged();

private:
    QVariantList m_resources;
    QVariantMap m_selectedTransferMethodInfo;
    QString m_mimeType;
    QString m_autoMimeType;
    QString m_title;
};

#endif
