/****************************************************************************************
** Copyright (c) 2013 - 2023 Jolla Ltd.
** Copyright (c) 2019 - 2021 Open Mobile Platform LLC.
**
** All rights reserved.
**
** This file is part of Sailfish Transfer Engine component package.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

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
