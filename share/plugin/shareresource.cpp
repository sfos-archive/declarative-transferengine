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
