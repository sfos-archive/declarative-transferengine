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

#ifndef SAILFISH_SHAREPROVIDER_H
#define SAILFISH_SHAREPROVIDER_H

#include <QObject>
#include <QQmlParserStatus>
#include <QString>
#include <QVariant>
#include "shareresource.h"

class ShareProvider : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(bool registerName READ registerName WRITE setRegisterName NOTIFY registerNameChanged)
    Q_PROPERTY(QStringList capabilities READ capabilities WRITE setCapabilities RESET resetCapabilities NOTIFY capabilitiesChanged)

public:
    ShareProvider(QObject *parent = 0);
    ~ShareProvider();

    void classBegin();
    void componentComplete();

    QString method() const;
    void setMethod(const QString &method);

    bool registerName() const;
    void setRegisterName(bool registerName);

    QStringList capabilities() const;
    void setCapabilities(const QStringList &capabilities);
    void resetCapabilities();

Q_SIGNALS:
    void methodChanged();
    void registerNameChanged();
    void capabilitiesChanged();

    void triggered(QVariantList resources);

private:
    class Adaptor;

    QString dbusPath() const;
    void registerObject();
    void registerService() const;

    bool m_complete = false;
    QString m_method;
    bool m_registerName = false;
    QStringList m_capabilities;
};

#endif // SAILFISH_SHAREPROVIDER_H
