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

/*
 * Just a helper item to read the D-Bus interface from the desktop entry file.
 */

#include <MDesktopEntry>
#include <QQmlInfo>
#include "sharemethodinfo.h"

namespace {

const auto SailjailSection = QStringLiteral("X-Sailjail");
const auto OrganizationName = QStringLiteral("OrganizationName");
const auto ApplicationName = QStringLiteral("ApplicationName");

} // namespace

ShareMethodInfo::ShareMethodInfo(QObject *parent)
    : QObject(parent)
{
}

QString ShareMethodInfo::methodId() const
{
    return m_methodId;
}

void ShareMethodInfo::setMethodId(const QString &methodId)
{
    if (m_methodId != methodId) {
        m_methodId = methodId;
        readDesktopEntryFile();
        emit methodIdChanged();
    }
}

QString ShareMethodInfo::service() const
{
    return m_service;
}

void ShareMethodInfo::setService(const QString &service)
{
    if (m_service != service) {
        m_service = service;
        emit serviceChanged();
    }
}

QString ShareMethodInfo::path() const
{
    return m_path;
}

void ShareMethodInfo::setPath(const QString &path)
{
    if (m_path != path) {
        m_path = path;
        emit pathChanged();
    }
}

QString ShareMethodInfo::iface() const
{
    return m_iface;
}

void ShareMethodInfo::setIface(const QString &iface)
{
    if (m_iface != iface) {
        m_iface = iface;
        emit ifaceChanged();
    }
}

void ShareMethodInfo::readDesktopEntryFile()
{
    QString service;
    QString path;
    QString iface;
    QString method = m_methodId.section('/', -1);
    QString file = m_methodId.section('/', 0, -2);
    if (!method.isEmpty() && !file.isEmpty()) {
        MDesktopEntry entry(file);
        QString orgName = entry.value(SailjailSection, OrganizationName);
        QString appName = entry.value(SailjailSection, ApplicationName);
        if (orgName.isEmpty() || appName.isEmpty()) {
            qmlInfo(this) << OrganizationName << " and " << ApplicationName
                          << " needs to be defined in " << SailjailSection
                          << " section of desktop entry file '" << file;
        } else {
            service = QStringLiteral("%1.%2").arg(orgName).arg(appName);
            path = QStringLiteral("/share/%1").arg(method);
            iface = QStringLiteral("org.sailfishos.share");
        }
    } else if (!m_methodId.isEmpty()) {
        qmlInfo(this) << "Invalid method id '" << m_methodId << "'";
    }
    setService(service);
    setPath(path);
    setIface(iface);
}
