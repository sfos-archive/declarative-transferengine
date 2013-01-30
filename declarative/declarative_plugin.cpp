#include "declarative_plugin.h"
#include "declarativeshare.h"
#include "declarativetransfermodel.h"
#include "declarativetransferinterface.h"
#include "declarativetransfermethodsmodel.h"
#include "transferdbrecord.h"
#include "transfermethodinfo.h"

#include <QDeclarativeEngine>
#include <qdeclarative.h>
#include <QApplication>

AppTranslator::AppTranslator(QObject *parent)
   : QTranslator(parent)
{
   qApp->installTranslator(this);
}

AppTranslator::~AppTranslator()
{
   qApp->removeTranslator(this);
}


void DeclarativePlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(uri)
    Q_ASSERT(QLatin1String(uri) == QLatin1String("Sailfish.TransferEngine"));

    // Translations
    AppTranslator *engineeringEnglish = new AppTranslator(engine);
    AppTranslator *translator = new AppTranslator(engine);
    engineeringEnglish->load("sailfish_transferengine_eng_en", "/usr/share/translations");
    translator->load(QLocale(), "sailfish_transferengine", "-", "/usr/share/translations");
}

void DeclarativePlugin::registerTypes(const char *uri)
{
    TransferMethodInfo::registerType();
    TransferDBRecord::registerType();

    // @uri Sailfish.TransferEngine
    qmlRegisterType<DeclarativeShare>(uri, 1, 0, "SailfishShare");
    qmlRegisterType<DeclarativeTransferModel>(uri, 1, 0, "SailfishTransferModel");
    qmlRegisterType<DeclarativeTransferInterface>(uri, 1, 0, "SailfishTransferInterface");
    qmlRegisterType<DeclarativeTransferMethodsModel>(uri, 1, 0, "SailfishTransferMethodsModel");
}

Q_EXPORT_PLUGIN2(Jollashare, DeclarativePlugin)

