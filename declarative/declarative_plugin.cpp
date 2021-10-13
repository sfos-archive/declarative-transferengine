/*
 * Copyright (c) 2013 - 2018 Jolla Ltd.
 * Copyright (c) 2019 - 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary
 */

#include "declarative_plugin.h"
#include "declarativetransfer.h"
#include "declarativetransferinterface.h"
#include "declarativesharingmethodsmodel.h"
#include "transferdbrecord.h"
#include "sharingmethodinfo.h"
#include "sharingplugininfo.h"

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

    const QString path("/usr/share/translations");

    translatorEngEn->load("sailfish_transferengine_eng_en", path);
    translator->load(QLocale(), "sailfish_transferengine", "-", path);
}

void DeclarativePlugin::registerTypes(const char *uri)
{
    TransferDBRecord::registerType();

    // @uri Sailfish.TransferEngine
    qmlRegisterType<DeclarativeTransfer>(uri, 1, 0, "SailfishTransfer");
    qmlRegisterType<DeclarativeTransferInterface>(uri, 1, 0, "SailfishTransferInterface");
    qmlRegisterType<DeclarativeSharingMethodsModel>(uri, 1, 0, "SailfishSharingMethodsModel");
}
