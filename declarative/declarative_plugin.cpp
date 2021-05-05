#include "declarative_plugin.h"
#include "declarativetransfer.h"
#include "declarativetransferinterface.h"
#include "declarativetransfermethodsmodel.h"
#include "transferdbrecord.h"
#include "transfermethodinfo.h"
#include "transferplugininfo.h"

#include <QQmlEngine>
#include <QCoreApplication>
#include <QDir>
#include <QSet>

AppTranslator::AppTranslator(QObject *parent)
   : QTranslator(parent)
{
   qApp->installTranslator(this);
}

AppTranslator::~AppTranslator()
{
   qApp->removeTranslator(this);
}

void DeclarativePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri)
    Q_ASSERT(QLatin1String(uri) == QLatin1String("Sailfish.TransferEngine"));

    // Translations
    AppTranslator *translatorEngEn = new AppTranslator(engine);
    AppTranslator *translator = new AppTranslator(engine);

    AppTranslator *sharePluginsTranslatorEngEn = new AppTranslator(engine);
    AppTranslator *sharePluginsTranslator = new AppTranslator(engine);

    const QString path("/usr/share/translations");

    translatorEngEn->load("sailfish_transferengine_eng_en", path);
    translator->load(QLocale(), "sailfish_transferengine", "-", path);

    // This module is responsible of loading translations for the UIs provided by share plugins.
    // FIXME: would be better just moving responsibility to share plugins
    sharePluginsTranslatorEngEn->load("sailfish_transferengine_plugins_eng_en", path);
    sharePluginsTranslator->load(QLocale(), "sailfish_transferengine_plugins", "-", path);

    // Load 3rd party share plugin translation files
    // TODO: to be considered if the whole third party translation loading system is worth it
    // or should it just be left for plugins themselves to handle.
    const QDir pluginDir("/usr/share/translations/nemotransferengine");
    if (pluginDir.exists()) {
        QStringList qmFiles = pluginDir.entryList(QStringList() << "*.qm", QDir::Files);
        QSet<QString> uniqueNames;

        for (const QString &qmFile : qmFiles) {
            if (qmFile.endsWith("_eng_en.qm")) {
                AppTranslator *sharePlugin3rdPartyTranslatorEngEn = new AppTranslator(engine);
                sharePlugin3rdPartyTranslatorEngEn->load(qmFile, pluginDir.absolutePath());
            } else {
                uniqueNames.insert(qmFile.left(qmFile.lastIndexOf(QLatin1Char('-'))));
            }
        }

        for (const QString &qmFile : uniqueNames) {
            AppTranslator *sharePlugin3rdPartyTranslator = new AppTranslator(engine);
            sharePlugin3rdPartyTranslator->load(QLocale(), qmFile, "-", pluginDir.absolutePath());
        }
    }
}

void DeclarativePlugin::registerTypes(const char *uri)
{
    TransferMethodInfo::registerType();
    TransferDBRecord::registerType();
    TransferPluginInfo::registerType();

    // @uri Sailfish.TransferEngine
    qmlRegisterType<DeclarativeTransfer>(uri, 1, 0, "SailfishTransfer");
    qmlRegisterType<DeclarativeTransferInterface>(uri, 1, 0, "SailfishTransferInterface");
    qmlRegisterType<DeclarativeTransferMethodsModel>(uri, 1, 0, "SailfishTransferMethodsModel");
}
