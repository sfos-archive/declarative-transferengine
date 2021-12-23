/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#ifndef SAILFISH_SHAREPROVIDER_H
#define SAILFISH_SHAREPROVIDER_H

#include <QObject>
#include <QQmlParserStatus>
#include <QString>
#include <QVariant>
#include "shareresource.h"

class ShareProvider : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(bool registerName READ registerName WRITE setRegisterName NOTIFY registerNameChanged)
    Q_PROPERTY(QStringList capabilities READ capabilities WRITE setCapabilities RESET resetCapabilities NOTIFY capabilitiesChanged)

public:
    ShareProvider(QObject *parent = 0);
    ~ShareProvider();

    void classBegin();
    void componentComplete();

    QString method() const;
    void setMethod(const QString &method);

    bool registerName() const;
    void setRegisterName(bool registerName);

    QStringList capabilities() const;
    void setCapabilities(const QStringList &capabilities);
    void resetCapabilities();

Q_SIGNALS:
    void methodChanged();
    void registerNameChanged();
    void capabilitiesChanged();

    void triggered(QVariantList resources);

private:
    class Adaptor;

    QString dbusPath() const;
    void registerObject();
    void registerService() const;

    bool m_complete = false;
    QString m_method;
    bool m_registerName = false;
    QStringList m_capabilities;
};

#endif // SAILFISH_SHAREPROVIDER_H
