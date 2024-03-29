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
#ifndef DECLARATIVETRANSFER_H
#define DECLARATIVETRANSFER_H

#include <QQuickItem>
#include "transfertypes.h"

class QDBusPendingCallWatcher;
class TransferEngineInterface;

/**
 * QML Share element is a non-visual item which can be used for starting a share operation from
 * the QML code. Basic usage:
 *
 * import Sailfish.TransferEngine 1.0
 *
 * SailfishTransfer {
 *      source: url
 *      mimeType: "image/jpeg"
 *      serviceId: "Facebook" // This must be retrieved from ShareMethodList
 *      metadataStripped: true // Get rid of metadata
 *      userData: {"accountId": 6, "description": "Lorem ipsum, etc" } // User data
 *  }
 *
 *  ...
 *  // Start sharing
 *  onClicked: shareItem.start()
 *
 * Alternatively, if the required plugin supports non-file sharing, you may specify the content to
 * be shared instead of the file url. This is done by specifying a "content" object map with a
 * "data" value, and optionally, a "name" value:
 *
 * SailfishTransfer {
 *      content: {"data": "BEGIN:VCARD\nVERSION:4.0\nN:Smith;John;;;\nEND:VCARD", "name": "JohnSmith.vcf"}
 * }
 *
 *
 *  The following userData keys are handled by the Transfer Engine:
 *
 *  "accountId" - The id of the account to be used for sharing
 *  "description" - The description, which is stored by transfer engine. Depending
 *                  on the service, this might be used.
 *  "title" - The title for the media, which is stored by transfer engine. Depending
 *            on the service, this might be used.
 *  "scalePercent" - The scale as in value between 0-1, e.g. 0.5 indicates 50% downscale.
 *
 *  All the other keys are not stored by transfer engine, but the user data is passed
 *  to the C++ share plugin and it's in plugin reponsible to use user data.
 */
class DeclarativeTransfer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source WRITE setSource READ source NOTIFY sourceChanged)
    Q_PROPERTY(QVariantMap content WRITE setContent READ content NOTIFY contentChanged)
    Q_PROPERTY(QString mimeType WRITE setMimeType READ mimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(QVariantMap userData WRITE setUserData READ userData NOTIFY userDataChanged)
    Q_PROPERTY(bool metadataStripped WRITE setMetadataStripped READ metadataStripped NOTIFY metadataStrippedChanged)
    Q_PROPERTY(bool notificationsEnabled WRITE setNotificationsEnabled READ notificationsEnabled NOTIFY notificationsEnabledChanged)
    Q_PROPERTY(QVariantMap transferMethodInfo WRITE setTransferMethodInfo READ transferMethodInfo NOTIFY transferMethodInfoChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:

    enum Status {
        NotStarted          = TransferEngineData::NotStarted,
        TransferStarted     = TransferEngineData::TransferStarted,
        TransferCanceled    = TransferEngineData::TransferCanceled,
        TransferFinished    = TransferEngineData::TransferFinished,
        TransferInterrupted = TransferEngineData::TransferInterrupted
    };
    Q_ENUM(Status)

    DeclarativeTransfer(QQuickItem *parent = 0);
    ~DeclarativeTransfer();

    void setContent(const QVariantMap &content);
    QVariantMap content() const;

    void setSource(const QUrl &source);
    QUrl source() const;

    void setMimeType(const QString &mimeType);
    QString mimeType() const;

    void setUserData(const QVariantMap &userData);
    QVariantMap userData() const;

    void setMetadataStripped(bool strip);
    bool metadataStripped() const;

    void setNotificationsEnabled(bool enable);
    bool notificationsEnabled() const;

    void setTransferMethodInfo(const QVariantMap &transferMethodInfo);
    QVariantMap transferMethodInfo() const;

    qreal progress() const;

    DeclarativeTransfer::Status status() const;

    Q_INVOKABLE void loadConfiguration(const QVariantMap &shareActionConfiguration);
    Q_INVOKABLE void start();
    Q_INVOKABLE void cancel();

Q_SIGNALS:
    void sourceChanged();
    void contentChanged();
    void serviceIdChanged();
    void mimeTypeChanged();
    void userDataChanged();
    void metadataStrippedChanged();
    void notificationsEnabledChanged();
    void transferMethodInfoChanged();
    void progressChanged(qreal progress);
    void statusChanged(Status status);

private:
    void init();
    void transferProgressChanged(int transferId, double progress);
    void transferStatusChanged(int transferId, int status);
    void transferIdReceived(QDBusPendingCallWatcher *call);

    QUrl m_source;
    QVariantMap m_content;
    QString m_mimeType;
    QVariantMap m_userData;
    QVariantMap m_transferMethodInfo;
    bool m_metadataStripped = false;
    bool m_notificationsEnabled = false;
    int m_transferId = -1;
    qreal m_progress = 0;
    DeclarativeTransfer::Status m_status = DeclarativeTransfer::NotStarted;
    DeclarativeTransfer::Status m_cachedStatus = DeclarativeTransfer::NotStarted;
    TransferEngineInterface *m_client = nullptr;
};

#endif

