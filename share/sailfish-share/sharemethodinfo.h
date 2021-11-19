/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#ifndef SAILFISH_SHAREMETHODINFO_H
#define SAILFISH_SHAREMETHODINFO_H

#include <QObject>
#include <QString>

class ShareMethodInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString methodId READ methodId WRITE setMethodId NOTIFY methodIdChanged)
    Q_PROPERTY(QString service READ service NOTIFY serviceChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(QString iface READ iface NOTIFY ifaceChanged)

public:
    ShareMethodInfo(QObject *parent = 0);

    QString methodId() const;
    void setMethodId(const QString &methodId);

    QString service() const;
    QString path() const;
    QString iface() const;

Q_SIGNALS:
    void methodIdChanged();
    void serviceChanged();
    void pathChanged();
    void ifaceChanged();

private:
    void setService(const QString &service);
    void setPath(const QString &path);
    void setIface(const QString &iface);
    void readDesktopEntryFile();

    QString m_methodId;
    QString m_service;
    QString m_path;
    QString m_iface;
};

#endif
