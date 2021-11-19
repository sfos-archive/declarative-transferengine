/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#ifndef APPSHAREMETHODPLUGININFO_H
#define APPSHAREMETHODPLUGININFO_H

#include <sharingplugininfo.h>

class AppShareMethodPluginInfo : public SharingPluginInfo
{
    Q_OBJECT

public:
    AppShareMethodPluginInfo();
    ~AppShareMethodPluginInfo();

    QList<SharingMethodInfo> info() const;
    void query();

private:
    static QStringList listFiles(const QString &path);
    Q_INVOKABLE void processNext();

    QStringList m_fileList;
    QList<SharingMethodInfo> m_infoList;
};

#endif // APPSHAREMETHODPLUGININFO_H
