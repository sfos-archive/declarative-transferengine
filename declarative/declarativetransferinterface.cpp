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

#include "declarativetransferinterface.h"
#include "transferengineclient.h"

class DeclarativeTransferInterfacePrivate
{
public:
    TransferEngineClient *m_client;
};

DeclarativeTransferInterface::DeclarativeTransferInterface(QQuickItem *parent) :
    QQuickItem(parent),
    d_ptr(new DeclarativeTransferInterfacePrivate)
{
    Q_D(DeclarativeTransferInterface);
    d->m_client = new TransferEngineClient(this);
}

DeclarativeTransferInterface::~DeclarativeTransferInterface()
{
    Q_D(DeclarativeTransferInterface);
    delete d->m_client;

    delete d_ptr;
    d_ptr = 0;
}

void DeclarativeTransferInterface::cbCancelTransfer(int transferId)
{
    Q_D(DeclarativeTransferInterface);
    d->m_client->cbCancelTransfer(transferId);
}

void DeclarativeTransferInterface::cbRestartTransfer(int transferId)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->cbRestartTransfer(transferId);
}

void DeclarativeTransferInterface::clearTransfer(int transferId)
{
    Q_D(DeclarativeTransferInterface);
    d->m_client->clearTransfer(transferId);
}

void DeclarativeTransferInterface::clearTransfers()
{
    Q_D(DeclarativeTransferInterface);
    d->m_client->clearTransfers();
}

void DeclarativeTransferInterface::startTransfer(int transferId)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->startTransfer(transferId);
}

void DeclarativeTransferInterface::setNotifications(bool enable)
{
    Q_D(DeclarativeTransferInterface);
    if (notificationsEnabled() != enable) {
        d->m_client->enableNotifications(enable);
        emit notificationsChanged();
    }
}

bool DeclarativeTransferInterface::notificationsEnabled() const
{
    Q_D(const DeclarativeTransferInterface);
    return d->m_client->notificationsEnabled();
}

int DeclarativeTransferInterface::createDownloadEvent(const QString &displayName,
                                                      const QUrl &applicationIcon,
                                                      const QUrl &serviceIcon,
                                                      const QUrl &url,
                                                      const QString &mimeType,
                                                      quint64 expectedFileSize,
                                                      const QStringList &callback,
                                                      const QString &cancelMethod,
                                                      const QString &restartMethod)
{
    Q_D(DeclarativeTransferInterface);
    CallbackInterface cbIf;
    if (callback.size() == 3) {
        cbIf = CallbackInterface(callback.at(0),
                                 callback.at(1),
                                 callback.at(2),
                                 cancelMethod,
                                 restartMethod);
    }
    return d->m_client->createDownloadEvent(displayName,
                                            applicationIcon,
                                            serviceIcon,
                                            url,
                                            mimeType,
                                            expectedFileSize,
                                            cbIf);
}

int DeclarativeTransferInterface::createSyncEvent(const QString &displayName,
                                                  const QUrl &applicationIcon,
                                                  const QUrl &serviceIcon,
                                                  const QStringList &callback,
                                                  const QString &cancelMethod,
                                                  const QString &restartMethod)
{
    Q_D(DeclarativeTransferInterface);
    CallbackInterface cbIf;
    if (callback.size() == 3) {
        cbIf = CallbackInterface(callback.at(0),
                                 callback.at(1),
                                 callback.at(2),
                                 cancelMethod,
                                 restartMethod);
    }
    return d->m_client->createSyncEvent(displayName,
                                        applicationIcon,
                                        serviceIcon,
                                        cbIf);
}

void DeclarativeTransferInterface::updateTransferProgress(int transferId, qreal progress)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->updateTransferProgress(transferId, progress);
}

void DeclarativeTransferInterface::finishTransfer(int transferId, Status status, const QString &reason)
{
    Q_D(const DeclarativeTransferInterface);
    d->m_client->finishTransfer(transferId, static_cast<TransferEngineClient::Status>(status), reason);
}
