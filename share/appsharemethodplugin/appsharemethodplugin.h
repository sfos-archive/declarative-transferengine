/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#ifndef APPSHAREMETHODPLUGIN_H
#define APPSHAREMETHODPLUGIN_H

#include <QObject>
#include <sharingplugininterface.h>

class AppShareMethodPlugin : public QObject, public SharingPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.sailfishos.share.plugin.appsharemethod")
    Q_INTERFACES(SharingPluginInterface)

public:
    AppShareMethodPlugin();
    ~AppShareMethodPlugin();

    SharingPluginInfo *infoObject();
    QString pluginId() const;
};

#endif // APPSHAREMETHODPLUGIN_H
