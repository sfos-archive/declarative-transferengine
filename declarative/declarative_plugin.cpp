#include "declarative_plugin.h"
#include "declarativeshare.h"
#include "declarativetransferinterface.h"
#include "declarativetransfermethodsmodel.h"
#include "transferdbrecord.h"
#include "transfermethodinfo.h"

#include <QQmlEngine>
#include <QCoreApplication>
#include <QDir>

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

    // This module is reponsible of loading translations for the UIs provided by share plugins
    sharePluginsTranslatorEngEn->load("sailfish_transferengine_plugins_eng_en", path);
    sharePluginsTranslator->load(QLocale(), "sailfish_transferengine_plugins", "-", path);

    // Load 3rd party share plugin translation files
    const QDir pluginDir("/usr/share/translations/nemotransferengine");
    if (pluginDir.exists()) {
        QStringList qmFiles =  pluginDir.entryList(QStringList() << "*.qm", QDir::Files);
        foreach(const QString &qmFile, qmFiles) {
            if (qmFile.contains("_eng_en")) {
                AppTranslator *sharePlugin3rdPartyTranslatorEngEn = new AppTranslator(engine);
                sharePlugin3rdPartyTranslatorEngEn->load(qmFile, pluginDir.absolutePath());
            } else {
                AppTranslator *sharePlugin3rdPartyTranslator = new AppTranslator(engine);
                sharePlugin3rdPartyTranslator->load(QLocale(), qmFile, "-", pluginDir.absolutePath());
            }
        }
    }
}

void DeclarativePlugin::registerTypes(const char *uri)
{
    TransferMethodInfo::registerType();
    TransferDBRecord::registerType();

    // @uri Sailfish.TransferEngine
    qmlRegisterType<DeclarativeShare>(uri, 1, 0, "SailfishShare");
    qmlRegisterType<DeclarativeTransferInterface>(uri, 1, 0, "SailfishTransferInterface");
    qmlRegisterType<DeclarativeTransferMethodsModel>(uri, 1, 0, "SailfishTransferMethodsModel");
}
