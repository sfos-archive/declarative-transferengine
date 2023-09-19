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

#include "shareaction.h"

#include <nemo-dbus/dbus.h>

#include <QQmlInfo>
#include <QMimeDatabase>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusUnixFileDescriptor>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTemporaryDir>

/*!
    \qmltype ShareAction
    \inqmlmodule Sailfish.Share

    Launches the Sharing UI from a predefined configuration.
*/
ShareAction::ShareAction(QObject *parent)
    : QObject(parent)
{
}

/*!
    \qmlproperty list ShareAction::resources

    A list of resources to be shared. The following resource types are allowed:

    \list
    \li url or string: path to a file on the local filesystem.
        urls are converted to string format. If the path starts with 'file:///', this
        scheme is stripped.
    \li map: holds raw content data and associated metadata
    \endlist

    Example:

    \qml
    ShareAction {
        resources: [
            "/path/to/my/file.txt",
            { "data": "<raw-bytes>", "name": "my-data-file" }
        ]
    }
    \endqml
*/
QVariantList ShareAction::ShareAction::resources() const
{
    return m_resources;
}

void ShareAction::setResources(const QVariantList &resources)
{
    if (m_resources != resources) {
        m_resources = resources;

        static const QString wildcardMatch = QStringLiteral("*");
        m_autoMimeType.clear();

        static QMimeDatabase mimeDb;

        for (QVariantList::iterator it = m_resources.begin(); it != m_resources.end(); ++it) {
            QString mimeType;
            const QVariant &value = *it;

            if (value.type() == QVariant::Map) {
                mimeType = value.toMap().value(QStringLiteral("type")).toString();
            } else if (value.type() == QVariant::Url) {
                QUrl url(value.toUrl());
                const QString filePath = url.isLocalFile() ? url.toString() : url.toLocalFile();
                mimeType = mimeDb.mimeTypeForFile(filePath).name();
                *it = filePath; // change map value from url -> string to avoid dbus serialization
            } else if (value.type() == QVariant::String) {
                QString filePath = value.toString();
                QUrl url(filePath);
                if (url.isLocalFile()) {
                    filePath = url.toLocalFile();   // strip scheme
                    *it = filePath;
                }
                mimeType = mimeDb.mimeTypeForFile(filePath).name();
            } else {
                qmlInfo(this) << "Unrecognized resource type '" << value.typeName();
            }

            // Find an overall mime type that matches all mime types in the list. E.g. if files are
            // "image/png" and "image/jpeg", use "image/*". If there are no common types, use "*".
            if (!mimeType.isEmpty() && m_autoMimeType != wildcardMatch) {
                if (m_autoMimeType.isEmpty()) {
                    m_autoMimeType = mimeType;
                } else if (m_autoMimeType != mimeType) {
                    const QString mainType = mimeType.mid(0, m_autoMimeType.indexOf('/'));
                    if (m_autoMimeType.mid(0, m_autoMimeType.indexOf('/')) == mainType) {
                        m_autoMimeType = mainType + QStringLiteral("/*");
                    } else {
                        m_autoMimeType = wildcardMatch;
                    }
                }
            }
        }

        if (m_mimeType.isEmpty() && m_mimeType != m_autoMimeType) {
            emit mimeTypeChanged();
        }
        emit resourcesChanged();
    }
}

/*!
    \qmlproperty string ShareAction::mimeType

    The mime type of the resources to be shared.

    This affects the sharing targets that are available when sharing the specified \l resources.

    The value can be a single type, or a wildcard that matches all of the resource types. For
    example, if \l resources contains a single PNG image, the \c mimeType could be "image/png".
    Any sharing target that is unable to share PNG images will not available in the sharing options.

    If the resources contains both JPEG and PNG files, the "png" part in the \c mimeType could be
    replaced by "*" to specify that the sharing target must be able to send any type of image.
    Alternatively it could specify the entire \c mimeType as "*", indicating that the sharing
    target must be able to share all types of files.

    If not set, this is auto-generated based on the \l resources value.
*/
QString ShareAction::mimeType() const
{
    return !m_mimeType.isEmpty() ? m_mimeType : m_autoMimeType;
}

void ShareAction::setMimeType(const QString &mimeType)
{
    if (m_mimeType != mimeType) {
        m_mimeType = mimeType;
        emit mimeTypeChanged();
    }
}

/*!
    \qmlproperty string ShareAction::title

    The title text to show in the sharing UI.
*/
QString ShareAction::title() const
{
    return m_title;
}

void ShareAction::setTitle(const QString &title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged();
    }
}

QVariantMap ShareAction::selectedTransferMethodInfo() const
{
    return m_selectedTransferMethodInfo;
}

void ShareAction::setSelectedTransferMethodInfo(const QVariantMap &selectedTransferMethodInfo)
{
    if (m_selectedTransferMethodInfo != selectedTransferMethodInfo) {
        m_selectedTransferMethodInfo = selectedTransferMethodInfo;
        emit selectedTransferMethodInfoChanged();
    }
}

/*!
    \qmlmethod ShareAction::trigger()

    Launches the sharing UI.
*/
void ShareAction::trigger()
{
    QDBusMessage msg = QDBusMessage::createMethodCall(
                QStringLiteral("org.sailfishos.share"),
                QStringLiteral("/"),
                QStringLiteral("org.sailfishos.share"),
                QStringLiteral("share"));

    msg.setArguments(QVariantList() << toConfiguration());

    if (!QDBusConnection::sessionBus().send(msg)) {
        qmlInfo(this) << "Unable to call org.sailfishos.share.share()"
                      << QDBusConnection::sessionBus().lastError();
    }
}

QVariantMap ShareAction::toConfiguration() const
{
    QVariantMap config;

    config.insert("resources", m_resources);
    config.insert("mimeType", mimeType());
    config.insert("title", m_title);
    config.insert("selectedTransferMethodInfo", m_selectedTransferMethodInfo);

    return config;
}

/*!
    \internal

    Loads the ShareAction using the specified \a configuration.

    Each ShareAction property that matches a key in the \a configuration will have its property
    value set to the associated configuration value for that key.
*/
void ShareAction::loadConfiguration(const QVariantMap &configuration)
{
    QVariantList resources;
    QString mimeType;
    QString title;
    QVariantMap selectedTransferMethodInfo;

    for (auto it = configuration.constBegin(); it != configuration.constEnd(); ++it) {
        const QVariant &value = NemoDBus::demarshallDBusArgument(it.value());

        if (it.key() == QStringLiteral("resources")) {
            resources = value.toList();
        } else if (it.key() == QStringLiteral("mimeType")) {
            mimeType = value.toString();
        } else if (it.key() == QStringLiteral("title")) {
            title = value.toString();
        } else if (it.key() == QStringLiteral("selectedTransferMethodInfo")) {
            selectedTransferMethodInfo = value.toMap();
        } else {
            qmlInfo(this) << "Unrecognized key '" << it.key() << "' with value:" << value;
        }
    }

    setResources(resources);
    setMimeType(mimeType);
    setTitle(title);
    setSelectedTransferMethodInfo(selectedTransferMethodInfo);
}

/*!
    \internal

    Replaces any file path resources with a QVariantMap of metadata, which includes an opened file descriptor.

    For example, if the resources are:

    \qml
    ShareAction {
        resources: ["/path/to/myfile.jpg"]
    }
    \endqml

    This function will open \c myfile.jpg and replace the resource with a QVariantMap containing
    the following attributes:

    \code
    {
        "name": "myfile.jpg",
        "type": "image/jpeg",
        "fileDescriptor": <opened-file-descriptor>
    }
    \endcode

    The file is closed when the ShareAction is destroyed.
*/
void ShareAction::replaceFileResourcesWithFileDescriptors()
{
    QVariantList resources;
    static QMimeDatabase mimeDb;

    for (int i = 0; i < m_resources.count(); ++i) {
        if (m_resources[i].type() == QVariant::String) {
            QFile *file = new QFile(m_resources[i].toString(), this);
            if (file->open(QFile::ReadOnly) && file->handle() > 0) {
                QVariantMap content;
                QFileInfo fileInfo(file->fileName());
                const QString mimeType = mimeDb.mimeTypeForFile(fileInfo).name();

                content.insert("name", fileInfo.fileName());
                content.insert("size", fileInfo.size());
                if (!mimeType.isEmpty()) {
                    content.insert("type", mimeType);
                }
                const QDBusUnixFileDescriptor fileDescriptor(file->handle());
                content.insert("fileDescriptor", QVariant::fromValue<QDBusUnixFileDescriptor>(fileDescriptor));
                resources.append(content);
                continue;
            } else {
                qmlInfo(this) << "Failed to open " << file->fileName()
                              << ", will not replace with file descriptor";
            }
        }
        resources.append(m_resources[i]);
    }

    setResources(resources);
}

QString ShareAction::writeContentToFile(const QVariantMap &contents, int maximumFileSize)
{
    const QString resourceName = contents.value("name").toString();
    if (resourceName.isEmpty()) {
        qmlInfo(this) << "writeContentToFile() failed: no name specified";
        return QString();
    }

    static const QString saveLocation(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    static const QString saveDirectory(QLatin1String("sailfish-share_XXXXXX"));

    QTemporaryDir tempDir(saveLocation + QDir::separator() + saveDirectory);
    tempDir.setAutoRemove(false);
    if (!tempDir.isValid()) {
        qmlInfo(this) << "Invalid temporary dir: " << tempDir.path();
        return QString();
    }

    QFile outputFile(tempDir.path() + QDir::separator() + resourceName);
    if (!outputFile.open(QFile::WriteOnly)) {
        qmlInfo(this) << "Unable to open file for writing: " << outputFile.fileName();
        return QString();
    }

    if (contents.contains("fileDescriptor")) {
        const QVariant fileDescriptor = contents.value("fileDescriptor");
        const int fd = fileDescriptor.value<QDBusUnixFileDescriptor>().fileDescriptor();
        if (fd <= 0) {
            qmlInfo(this) << "writeContentToFile() failed: Invalid file descriptor: " << fd;
            outputFile.remove();
            return QString();
        }

        QFile inputFile;
        if (!inputFile.open(fd, QFile::ReadOnly)) {
            qmlInfo(this) << "Unable to open file using file descriptor: " << fd;
            outputFile.remove();
            return QString();
        }

        int resourceSize = contents.value("size").toInt();
        if (resourceSize > maximumFileSize) {
            qmlInfo(this) << "writeContentToFile() failed: resource size " << resourceSize
                          << " exceeds maximumFileSize " << maximumFileSize;
            outputFile.remove();
            return QString();
        }
        if (resourceSize > INT_MAX) {
            qmlInfo(this) << "writeContentToFile() failed: resource size " << resourceSize
                          << " exceeds INT_MAX";
            outputFile.remove();
            return QString();
        }

        int maxSize = qMin(resourceSize, 1024 * 128);
        char buf[maxSize];

        qint64 bytesRead = 0;
        while (bytesRead >= 0) {
            bytesRead = inputFile.read(buf, maxSize);
            if (bytesRead > 0) {
                if (outputFile.write(buf, bytesRead) < 0) {
                    qmlInfo(this) << "Unable to write to file: " << outputFile.fileName()
                                  << " error: " << outputFile.errorString();
                    outputFile.remove();
                    return QString();
                }
            } else if (bytesRead < 0) {
                qmlInfo(this) << "Unable to read from file: " << inputFile.fileName()
                              << " error: " << inputFile.errorString();
                outputFile.remove();
                return QString();
            } else {
                break;
            }
        }

    } else if (contents.contains("data")) {
        const QByteArray rawData = contents.value("data").toByteArray();
        if (rawData.length() > maximumFileSize) {
            qmlInfo(this) << "writeContentToFile() failed: resource size " << rawData.length()
                          << " exceeds maximumFileSize " << maximumFileSize;
            outputFile.remove();
            return QString();
        }
        if (outputFile.write(rawData) < 0) {
            qmlInfo(this) << "Unable to write to file: " << outputFile.fileName()
                          << " error: " << outputFile.errorString();
            outputFile.remove();
            return QString();
        }

    } else {
        qmlInfo(this) << "writeContentToFile() failed, no fileDescriptor or data found in contents!";
        outputFile.remove();
        return QString();
    }

    return outputFile.fileName();
}

void ShareAction::removeFilesAndRmdir(const QStringList &files)
{
    for (const QString &file : files) {
        if (!QFile::remove(file)) {
            qmlInfo(this) << "Unable to delete file: " << file;
        }
        QDir dir = QFileInfo(file).dir();
        dir.rmdir(dir.absolutePath());
    }
}
