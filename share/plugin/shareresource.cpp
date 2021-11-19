/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#include <QtQml>
#include "shareaction.h"
#include "shareresource.h"

namespace {

const auto TypeKey = QStringLiteral("type");
const auto NameKey = QStringLiteral("name");
const auto DataKey = QStringLiteral("data");
const auto PathKey = QStringLiteral("path");

} // namespace

/*!
    \qmltype ShareResource
    \inqmlmodule Sailfish.Share

    Type for resources from \l ShareProvider::triggered. This type can not be
    created directly from QML.

    Resources can have two different types: \c ShareResource.StringDataType and
    \c ShareResource.FilePathType. Depending on which type the resource is,
    different properties can be accessed. \l ShareResource::type can be always
    accessed.
*/

ShareResource::ShareResource(ResourceType type, QVariantMap data)
    : m_type(type)
    , m_data(data)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

ShareResource::~ShareResource()
{
}

/*!
    Constructs StringDataType resource.

    \internal
*/
QObject *ShareResource::stringResource(const QString &name, const QString &data)
{
    QVariantMap values;
    values[NameKey] = name;
    values[DataKey] = data;
    return new ShareResource(StringDataType, values);
}

/*!
    Constructs FilePathType resource.

    \internal
*/
QObject *ShareResource::filePathResource(const QString &filePath)
{
    QVariantMap values;
    values[PathKey] = filePath;
    return new ShareResource(FilePathType, values);
}

/*!
    \qmlproperty int ShareResource::type

    ShareResource's type. Either \c ShareResource.StringDataType or \c
    ShareResource.FilePathType.

    You may alter your application's behaviour based on this value:
    \qml
    function handleResource(resource) {
        if (resource.type === ShareResource.FilePathType) {
        handleSharedFilePath(resource.filePath)
        } else if (resource.type === ShareResource.StringDataType) {
            handleSharedData(resource.name, resource.data)
        } else {
            console.warn("Unknown resource type:", resource.type)
            showSharingError()
        }
    }
    \endqml
*/
ShareResource::ResourceType ShareResource::type() const
{
    return m_type;
}

/*!
    \qmlproperty string ShareResource::name

    This is the name of the shared resource if resource's type is \c
    ShareResource.StringDataType.
*/
QString ShareResource::name() const
{
    if (m_data.contains(NameKey))
        return m_data[NameKey].toString();
    return QString();
}

/*!
    \qmlproperty string ShareResource::data

    This is the content of the shared resource if resource's type is \c
    ShareResource.StringDataType.
*/
QString ShareResource::data() const
{
    if (m_data.contains(DataKey))
        return m_data[DataKey].toString();
    return QString();
}

/*!
    \qmlproperty string ShareResource::filePath

    This is a path to the shared file if resource's type is \c
    ShareResource.FilePathType.
*/
QString ShareResource::filePath() const
{
    if (m_data.contains(PathKey))
        return m_data[PathKey].toString();
    return QString();
}
