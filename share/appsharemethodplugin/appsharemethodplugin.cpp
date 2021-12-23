/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#include "appsharemethodplugin.h"
#include "appsharemethodplugininfo.h"

AppShareMethodPlugin::AppShareMethodPlugin()
{
}

AppShareMethodPlugin::~AppShareMethodPlugin()
{
}

SharingPluginInfo *AppShareMethodPlugin::infoObject()
{
    return new AppShareMethodPluginInfo;
}

QString AppShareMethodPlugin::pluginId() const
{
    return QLatin1String("AppShareMethod");
}
