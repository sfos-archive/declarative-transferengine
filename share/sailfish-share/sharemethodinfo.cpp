/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

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
