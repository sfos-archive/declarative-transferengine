/****************************************************************************************
**
** Copyright (c) 2021 Open Mobile Platform LLC.
** All rights reserved.
**
** License: Proprietary.
**
****************************************************************************************/
#ifndef SAILFISH_SHAREACTION_H
#define SAILFISH_SHAREACTION_H

#include <QObject>
#include <QVariantMap>
#include <QList>

#include <limits.h>

class ShareAction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList resources READ resources WRITE setResources NOTIFY resourcesChanged)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QVariantMap selectedTransferMethodInfo READ selectedTransferMethodInfo WRITE setSelectedTransferMethodInfo NOTIFY selectedTransferMethodInfoChanged)

public:
    ShareAction(QObject *parent = 0);

    QVariantList resources() const;
    void setResources(const QVariantList &resources);

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    QString title() const;
    void setTitle(const QString &title);

    QVariantMap selectedTransferMethodInfo() const;
    void setSelectedTransferMethodInfo(const QVariantMap &selectedTransferMethodInfo);

public slots:
    void trigger();
    void loadConfiguration(const QVariantMap &configuration);
    QVariantMap toConfiguration() const;
    void replaceFileResourcesWithFileDescriptors();
    QString writeContentToFile(const QVariantMap &contents, int maximumFileSize = INT_MAX);
    void removeFilesAndRmdir(const QStringList &files);

Q_SIGNALS:
    void done();
    void resourcesChanged();
    void mimeTypeChanged();
    void titleChanged();
    void selectedTransferMethodInfoChanged();

private:
    QVariantList m_resources;
    QVariantMap m_selectedTransferMethodInfo;
    QString m_mimeType;
    QString m_autoMimeType;
    QString m_title;
};

#endif
