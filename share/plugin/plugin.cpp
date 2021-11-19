/****************************************************************************************
**
** Copyright (c) 2021 Open Mobile Platform LLC.
** All rights reserved.
**
** License: Proprietary.
**
****************************************************************************************/

#include <QTranslator>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QQmlExtensionPlugin>
#include <QLocale>
#include <qqml.h>

#include "shareaction.h"
#include "shareprovider.h"
#include "shareresource.h"

class SailfishSharePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Sailfish.Share")

public:
    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(engine)
        Q_UNUSED(uri)
    }

    virtual void registerTypes(const char *uri)
    {
        Q_UNUSED(uri)
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Sailfish.Share"));
        qmlRegisterType<ShareAction>(uri, 1, 0, "ShareAction");
        qmlRegisterType<ShareProvider>(uri, 1, 0, "ShareProvider");
        qmlRegisterUncreatableType<ShareResource>(uri, 1, 0, "ShareResource", "This is not a creatable type");
    }
};

#include "plugin.moc"
