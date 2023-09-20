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
