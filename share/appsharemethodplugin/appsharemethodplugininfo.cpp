/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#include <MDesktopEntry>
#include <QDir>
#include <QLoggingCategory>
#include <QStandardPaths>
#include <QUrl>
#include "mimetypefilter.h"
#include "appsharemethodplugininfo.h"

Q_LOGGING_CATEGORY(lcPlugin, "org.sailfishos.share.appsharemethod", QtWarningMsg)

namespace {

const auto CacheFile = QStringLiteral("/var/lib/sailfish-share/desktopfiles.list");
const auto CacheFileHeader = QStringLiteral("AppShareMethodCache;1.0");
const auto ApplicationDirectory = QStringLiteral("/usr/share/applications");
const auto UserApplicationDirectoryTemplate = QStringLiteral("%1/applications");
const auto DesktopEntrySection = QStringLiteral("Desktop Entry");
const auto ShareMethodsKey = QStringLiteral("X-Share-Methods");
const auto SailjailSection = QStringLiteral("X-Sailjail");
const auto OrganizationName = QStringLiteral("OrganizationName");
const auto ApplicationName = QStringLiteral("ApplicationName");
const auto ShareMethodSection = QStringLiteral("X-Share Method %1");
const auto SubtitleKey = QStringLiteral("Description");
const auto CapabilitiesKey = QStringLiteral("Capabilities");
const auto SupportsMultipleFilesKey = QStringLiteral("SupportsMultipleFiles");
const auto ShareUIPath = QStringLiteral("/usr/share/sailfish-share/plugin/AppShareMethodPlugin.qml");

bool applicationHasSharing(const MDesktopEntry &entry)
{
    /* If application specifies sharing methods,
     * then we make some sanity checks for developer convenience.
     * If any of the checks fail, then return false, otherwise return true.
     */
    if (!entry.isValid() || !entry.contains(DesktopEntrySection, ShareMethodsKey)
            || entry.type() != QLatin1String("Application"))
        return false;

    if (entry.icon().isEmpty()) {
        qCWarning(lcPlugin) << entry.name() << "is missing icon and can not be used for sharing";
        return false;
    }

    if (!entry.contains(SailjailSection, OrganizationName)) {
        qCWarning(lcPlugin) << entry.name() << "is missing" << SailjailSection << "/" << OrganizationName
                            << "and can not be used for sharing";
        return false;
    }

    if (!entry.contains(SailjailSection, ApplicationName)) {
        qCWarning(lcPlugin) << entry.name() << "is missing" << SailjailSection << "/" << ApplicationName
                            << "and can not be used for sharing";
        return false;
    }

    return true;
}

QString iconId(const QString &icon)
{
    // The same logic for determining icon as in lipstick, except that empty name is not possible
    if (icon.contains(QLatin1String(":/")) || icon.startsWith(QLatin1String("data:image/png;base64")))
        return icon;
    if (icon.startsWith('/'))
        return QUrl::fromLocalFile(icon).toString();
    return QStringLiteral("image://theme/") + icon;
}

QStringList filterMimeTypes(const QStringList &mimeTypes)
{
    QStringList filtered;
    for (const QString &mimeType : mimeTypes) {
        if (MimeTypeFilter::validMimeTypeFilter(mimeType))
            filtered.append(mimeType);
    }
    return filtered;
}

bool isTrue(const QString &value)
{
    return value == QLatin1String("yes") || value == QLatin1String("true");
}

} // namespace

AppShareMethodPluginInfo::AppShareMethodPluginInfo()
{
}

AppShareMethodPluginInfo::~AppShareMethodPluginInfo()
{
}

QList<SharingMethodInfo> AppShareMethodPluginInfo::info() const
{
    return m_infoList;
}

void AppShareMethodPluginInfo::query()
{
    qCDebug(lcPlugin) << "Querying for applications with sharing methods";
    m_fileList.clear();
    m_infoList.clear();

    bool success = false;
    QFile cacheFile(CacheFile);
    if (!cacheFile.open(QIODevice::ReadOnly)) {
        qCWarning(lcPlugin) << "Could not read desktop entry cache from" << cacheFile.fileName();
    } else {
        qCInfo(lcPlugin) << "Reading cache from" << CacheFile;
        QStringList content = QString::fromUtf8(cacheFile.readAll()).split('\n', QString::SkipEmptyParts);
        if (!content.isEmpty() && content[0] == CacheFileHeader) {
            m_fileList = content.mid(1);
            success = true;
        } else {
            qCWarning(lcPlugin) << "Wrong version of cache file" << cacheFile.fileName();
        }
    }
    if (!success) {
        qCInfo(lcPlugin) << "Falling back to reading all files from" << ApplicationDirectory;
        m_fileList = listFiles(ApplicationDirectory);
    }

    QString userApplicationDirectory = UserApplicationDirectoryTemplate.arg(
            QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    qCInfo(lcPlugin) << "Reading sharing methods from" << userApplicationDirectory;
    m_fileList.append(listFiles(userApplicationDirectory));

    QMetaObject::invokeMethod(this, "processNext", Qt::QueuedConnection);
}

QStringList AppShareMethodPluginInfo::listFiles(const QString &path)
{
    QDir directory(path);
    QStringList files;
    for (const QString &file : directory.entryList(QStringList() << QStringLiteral("*.desktop"),
                                                   QDir::Files | QDir::Readable, QDir::Name)) {
        files << directory.absoluteFilePath(file);
    }
    return files;
}

void AppShareMethodPluginInfo::processNext()
{
    if (m_fileList.isEmpty()) {
        qCDebug(lcPlugin) << "Done querying for applications with sharing methods";
        emit infoReady();
        return;
    }

    QString filePath = m_fileList.takeFirst();
    MDesktopEntry entry(filePath);
    qCDebug(lcPlugin) << "Checking" << entry.fileName() << "for sharing plugins";
    if (applicationHasSharing(entry)) {
        QString name = entry.name();
        QString icon = iconId(entry.icon());
        QStringList methods = entry.stringListValue(DesktopEntrySection, ShareMethodsKey);
        for (const QString &method : methods) {
            QString section(ShareMethodSection.arg(method));

            qCInfo(lcPlugin) << "Found" << section << "in" << entry.fileName();

            SharingMethodInfo info;
            info.setDisplayName(name);
            info.setSubtitle(entry.localizedValue(section, SubtitleKey));
            info.setMethodIcon(icon);
            info.setMethodId(QStringLiteral("%1/%2").arg(entry.fileName()).arg(method));
            info.setShareUIPath(ShareUIPath);
            if (entry.contains(section, CapabilitiesKey))
                info.setCapabilities(filterMimeTypes(entry.stringListValue(section, CapabilitiesKey)));
            else
                info.setCapabilities(QStringList() << QStringLiteral("*"));
            info.setSupportsMultipleFiles(isTrue(entry.value(section, SupportsMultipleFilesKey)));
            m_infoList << info;
        }
    }

    QMetaObject::invokeMethod(this, "processNext", Qt::QueuedConnection);
}
