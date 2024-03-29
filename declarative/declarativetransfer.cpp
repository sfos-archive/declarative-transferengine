/****************************************************************************************
** Copyright (c) 2013 - 2023 Jolla Ltd.
** Copyright (c) 2021 Open Mobile Platform LLC.
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
#include "declarativetransfer.h"
#include "transferengineinterface.h"

#include <QFile>
#include <QQmlInfo>
#include <QtDBus>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>

DeclarativeTransfer::DeclarativeTransfer(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_client = new TransferEngineInterface("org.nemo.transferengine",
                                           "/org/nemo/transferengine",
                                           QDBusConnection::sessionBus(),
                                           this);
    connect(m_client, &TransferEngineInterface::progressChanged,
            this, &DeclarativeTransfer::transferProgressChanged);
    connect(m_client, &TransferEngineInterface::statusChanged,
            this, &DeclarativeTransfer::transferStatusChanged);
}

DeclarativeTransfer::~DeclarativeTransfer()
{
}

void DeclarativeTransfer::setContent(const QVariantMap &content)
{
    if (m_content != content) {
        m_content = content;
        emit contentChanged();
    }
}

QVariantMap DeclarativeTransfer::DeclarativeTransfer::content() const
{
    return m_content;
}

void DeclarativeTransfer::setSource(const QUrl &source)
{
    // Coerce into a file:/// url if necessary.
    QUrl localFileUrl(source);
    if (!localFileUrl.isLocalFile()) {
        localFileUrl = QUrl::fromLocalFile(source.toString());
    }

    if (m_source != localFileUrl) {
        m_source = localFileUrl;
        emit sourceChanged();
    }
}

QUrl DeclarativeTransfer::source() const
{
    return m_source;
}

void DeclarativeTransfer::setMetadataStripped(bool strip)
{
    if (m_metadataStripped != strip) {
        m_metadataStripped = strip;
        emit metadataStrippedChanged();
    }
}

bool DeclarativeTransfer::metadataStripped() const
{
    return m_metadataStripped;
}

void DeclarativeTransfer::setMimeType(const QString &mimeType)
{
    if (m_mimeType != mimeType) {
        m_mimeType = mimeType;
        emit mimeTypeChanged();
    }
}

QString DeclarativeTransfer::mimeType() const
{
    return m_mimeType;
}

void DeclarativeTransfer::setUserData(const QVariantMap &userData)
{
    if (m_userData != userData) {
        m_userData = userData;
        emit userDataChanged();
    }
}

QVariantMap DeclarativeTransfer::userData() const
{
    return m_userData;
}

void DeclarativeTransfer::setNotificationsEnabled(bool enable)
{
    if (m_notificationsEnabled != enable) {
        m_notificationsEnabled = enable;
        m_client->enableNotifications(enable);
        emit notificationsEnabledChanged();
    }
}

bool DeclarativeTransfer::notificationsEnabled() const
{
    return m_notificationsEnabled;
}

void DeclarativeTransfer::setTransferMethodInfo(const QVariantMap &transferMethodInfo)
{
    if (m_transferMethodInfo != transferMethodInfo) {
        m_transferMethodInfo = transferMethodInfo;
        emit transferMethodInfoChanged();
    }
}

QVariantMap DeclarativeTransfer::transferMethodInfo() const
{
    return m_transferMethodInfo;
}

qreal DeclarativeTransfer::progress() const
{
    return m_progress;
}

DeclarativeTransfer::Status DeclarativeTransfer::status() const
{
    return m_status;
}

void DeclarativeTransfer::loadConfiguration(const QVariantMap &shareActionConfiguration)
{
    for (QVariantMap::const_iterator it = shareActionConfiguration.constBegin();
         it != shareActionConfiguration.constEnd(); ++it) {
        if (it.key() == QStringLiteral("resources")) {
            const QVariantList &resourceList = it.value().toList();
            if (resourceList.count() > 0) {
                const QVariant &value = resourceList.at(0);
                if (value.type() == QVariant::Map) {
                    setContent(value.toMap());
                } else if (value.type() == QVariant::Url) {
                    setSource(value.toUrl());
                } else if (value.type() == QVariant::String) {
                    setSource(QUrl(value.toString()));
                } else {
                    qmlInfo(this) << "Unrecognized resource type '" << value.typeName();
                }
            }
        } else if (it.key() == QStringLiteral("mimeType")) {
            setMimeType(it.value().toString());
        } else if (it.key() == QStringLiteral("title")) {
            // ignore
        } else if (it.key() == QStringLiteral("selectedTransferMethodInfo")) {
            setTransferMethodInfo(it.value().toMap());
        } else {
            qmlInfo(this) << "Unrecognized key '" << it.key() << "' with value:"  << it.value();
        }
    }
}

void DeclarativeTransfer::start()
{
    if (m_status != NotStarted) {
        qmlInfo(this) << "Transfer has already started!";
        return;
    }

    const QString methodId = m_transferMethodInfo.value("methodId").toString();
    if (methodId.isEmpty()) {
        qmlInfo(this) << "transferMethodInfo does not have methodId to specify the transfer service!";
        return;
    }

    QDBusPendingCall async = m_content.isEmpty()
            ? m_client->uploadMediaItem(m_source.toString(),
                                           methodId,
                                           m_mimeType,
                                           m_metadataStripped,
                                           m_userData)
            : m_client->uploadMediaItemContent(m_content,
                                                  methodId,
                                                  m_userData);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(async, this);
    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, &DeclarativeTransfer::transferIdReceived);
}

void DeclarativeTransfer::cancel()
{
    if (m_status == TransferStarted &&
        m_transferId != -1){
        m_client->cancelTransfer(m_transferId);
    }
}

void DeclarativeTransfer::transferProgressChanged(int transferId, double progress)
{
    if (transferId != m_transferId) {
        return;
    }

    if (m_progress != progress) {
        m_progress = progress;
        emit progressChanged(m_progress);
    }
}

void DeclarativeTransfer::transferStatusChanged(int transferId, int status)
{
    if (m_transferId == -1) {
        m_cachedStatus = static_cast<DeclarativeTransfer::Status>(status);
        return;
    }

    if (transferId != m_transferId) {
        return;
    }

    if (m_status != status) {
        m_status = static_cast<DeclarativeTransfer::Status>(status);
        emit statusChanged(m_status);
    }
}

void DeclarativeTransfer::transferIdReceived(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<int> reply = *call;
    call->deleteLater();

    if (reply.isError()) {
        qWarning() << "DeclarativeTransfer::transferIdReceived: failed to get transferId" << reply.error();
        return;
    }

    m_transferId = reply.value();

    // Make sure that we have status set right.
    if (m_cachedStatus != DeclarativeTransfer::NotStarted) {
        m_status = m_cachedStatus;
        m_cachedStatus = DeclarativeTransfer::NotStarted;
        emit statusChanged(m_status);
    }
}
