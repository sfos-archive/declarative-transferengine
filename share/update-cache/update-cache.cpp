/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#include <MDesktopEntry>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>

/*
 * This has a very simple cache format: each line contains an absolute path to
 * a desktop file that has some sharing methods. In any case we must read
 * localized names and descriptions for each application and that depends on
 * user locale. Moreover we can't limit files by capabilities either while
 * reading them so there is fairly little point to cache any data from the
 * files. This still allows skipping apps that don't expose any sharing methods
 * which would be majority of apps.
 */

const auto CacheFile = QStringLiteral("/var/lib/sailfish-share/desktopfiles.list");
const auto CacheFileHeader = QStringLiteral("AppShareMethodCache;1.0");
const auto ApplicationDirectory = QStringLiteral("/usr/share/applications");
const auto DesktopEntrySection = QStringLiteral("Desktop Entry");
const auto ShareMethodsKey = QStringLiteral("X-Share-Methods");

int main(int argc, char **argv)
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    bool cacheUpdated = false;
    QStringList sharingDesktopFiles = { CacheFileHeader };
    QDir applicationDirectory(ApplicationDirectory);
    for (const QString &file : applicationDirectory.entryList(
            QStringList() << QStringLiteral("*.desktop"), QDir::Files | QDir::Readable, QDir::Name)) {
        QString filePath = applicationDirectory.absoluteFilePath(file);
        MDesktopEntry entry(filePath);
        if (entry.isValid() && entry.contains(DesktopEntrySection, ShareMethodsKey)
                && entry.type() == QLatin1String("Application"))
            sharingDesktopFiles << filePath;
    }
    QFile cacheFile(CacheFile);
    if (!cacheFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Can not write to" << cacheFile.fileName();
        qWarning() << "Share method cache may not be up-to-date";
    } else {
        cacheFile.write(sharingDesktopFiles.join('\n').toUtf8());
        cacheUpdated = true;
    }

    return cacheUpdated ? 0 : 1;
}
