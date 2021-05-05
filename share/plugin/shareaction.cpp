/****************************************************************************************
**
** Copyright (c) 2021 Open Mobile Platform LLC.
** All rights reserved.
**
** License: Proprietary.
**
****************************************************************************************/
#include "shareaction.h"

#include <nemo-dbus/dbus.h>

#include <QQmlInfo>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QMimeDatabase>

/*!
    \qmltype ShareAction

    Launches the Sharing UI from a predefined configuration.
*/
ShareAction::ShareAction(QObject *parent)
    : QObject(parent)
{
}

/*!
    \qmlproperty QVariantList ShareAction::resources

    A list of resources to be shared. The following resource types are allowed:

    \list
    \li url or string: path to a file on the local filesystem.
        urls are converted to string format. If the path starts with 'file:///', this
        scheme is stripped.
    \li map: holds raw content data and associated metadata
    \endlist

    Example:

    \qml
    resources: [
        "/path/to/my/file.txt",
        { "data": <raw-bytes>, "name": "my-data-file" }
    ]
    \qml
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
    \qmlproperty QString ShareAction::mimeType

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
    \qmlproperty QString ShareAction::title

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
