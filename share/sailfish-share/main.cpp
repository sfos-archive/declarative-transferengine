#include <QObject>
#include <QLocale>
#include <QQuickView>
#include <QQmlEngine>
#include <QTranslator>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QGuiApplication>
#include <QDebug>

#ifdef HAS_BOOSTER
#include <MDeclarativeCache>
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef HAS_BOOSTER
    QScopedPointer<QGuiApplication> app(MDeclarativeCache::qApplication(argc, argv));
    QScopedPointer<QQuickView> view(MDeclarativeCache::qQuickView());
#else
    QScopedPointer<QGuiApplication> app(new QGuiApplication(argc, argv));
    QScopedPointer<QQuickView> view(new QQuickView);
#endif

    QScopedPointer<QTranslator> engineeringEnglish(new QTranslator);
    engineeringEnglish->load("sailfishshare_eng_en", "/usr/share/translations");
    QScopedPointer<QTranslator> translator(new QTranslator);
    translator->load(QLocale(), "sailfishshare", "-", "/usr/share/translations");

    app->installTranslator(engineeringEnglish.data());
    app->installTranslator(translator.data());

    QCoreApplication::setOrganizationName(QStringLiteral("org.sailfishos"));
    QCoreApplication::setApplicationName(QStringLiteral("Sailfish Share"));

    QQmlEngine engine;
    QObject::connect(&engine, &QQmlEngine::quit, app.data(), &QGuiApplication::quit);
    QQmlComponent comp(&engine, QString(DEPLOYMENT_PATH) + QLatin1String("main.qml"));
    if (!comp.isReady()) {
        qWarning() << comp.errorString();
        return -1;
    }
    comp.create();

    int result = app->exec();
    app->removeTranslator(translator.data());
    app->removeTranslator(engineeringEnglish.data());
    return result;
}
