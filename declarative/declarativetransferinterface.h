/****************************************************************************************
** Copyright (c) 2013 - 2023 Jolla Ltd.
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

#ifndef DECLARATIVETRANSFERINTERFACE_H
#define DECLARATIVETRANSFERINTERFACE_H

#include <QQuickItem>
#include "transfertypes.h"

class DeclarativeTransferInterfacePrivate;
class DeclarativeTransferInterface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool enableNotifications READ notificationsEnabled WRITE setNotifications NOTIFY notificationsChanged)
    Q_ENUMS(Status)

public:
    enum Status {
        TransferFinished = TransferEngineData::TransferFinished,
        TransferCanceled = TransferEngineData::TransferCanceled,
        TransferInterrupted = TransferEngineData::TransferInterrupted
    };

    explicit DeclarativeTransferInterface(QQuickItem *parent = 0);
    ~DeclarativeTransferInterface();

    // TODO: MOVE cbCance/RestartMethods to private API
    // Cancel an existing transfer
    // NOTE: This makes transfer engine to call client's callback, which does the actual cancelation.
    Q_INVOKABLE void cbCancelTransfer(int transferId);

    // Restart failed or canceled transfer
    Q_INVOKABLE void cbRestartTransfer(int transferId);

    // Clear a finished, failed or canceled transfer
    Q_INVOKABLE void clearTransfer(int transferId);

    // Clear all finished, failed or canceled transfers
    Q_INVOKABLE void clearTransfers();

    // Start a new transfer
    Q_INVOKABLE void startTransfer(int transferId);

    // Create a new download event. Returns transfer id
    // callback - Dbus service, path, interface
    Q_INVOKABLE int createDownloadEvent(const QString &displayName,
                                        const QUrl &applicationIcon,
                                        const QUrl &serviceIcon,
                                        const QUrl &url,
                                        const QString &mimeType,
                                        quint64 expectedFileSize,
                                        const QStringList &callback,
                                        const QString &cancelMethod,
                                        const QString &restartMethod);

    // Create a new sync event. Returns transfer id
    Q_INVOKABLE int createSyncEvent(const QString &displayName,
                                    const QUrl &applicationIcon,
                                    const QUrl &serviceIcon,
                                    const QStringList &callback,
                                    const QString &cancelMethod,
                                    const QString &restartMethod);

    // Update the progress of the existing transfer
    Q_INVOKABLE void updateTransferProgress(int transferId, qreal progress);

    // Make transfer finished.
    Q_INVOKABLE void finishTransfer(int transferId, Status status, const QString &reason);

    // Enable notifications
    void setNotifications(bool enable);

    // return true/false if notifications are enabled
    bool notificationsEnabled() const;

Q_SIGNALS:
    void notificationsChanged();

private:
    DeclarativeTransferInterfacePrivate *d_ptr;
    Q_DECLARE_PRIVATE(DeclarativeTransferInterface)
};

#endif // DECLARATIVETRANSFERINTERFACE_H
