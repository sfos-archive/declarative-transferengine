/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#ifndef SAILFISH_SHARERESOURCE_H
#define SAILFISH_SHARERESOURCE_H

#include <QObject>
#include <QString>
#include <QVariant>

class ShareResource : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ResourceType type READ type CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString data READ data CONSTANT)
    Q_PROPERTY(QString filePath READ filePath CONSTANT)

public:
    enum ResourceType {
        // 0 is reserved for future use
        StringDataType = 1,  // { name: string, data: string }
        FilePathType,        // { filePath: string }
    };
    Q_ENUM(ResourceType)

    ShareResource(ResourceType type, QVariantMap values);
    ~ShareResource();

    static QObject *stringResource(const QString &name, const QString &data);
    static QObject *filePathResource(const QString &filePath);

    ResourceType type() const;
    QString name() const;
    QString data() const;
    QString filePath() const;

private:
    ResourceType m_type;
    QVariantMap m_data;
};

#endif // SAILFISH_SHARERESOURCE_H
