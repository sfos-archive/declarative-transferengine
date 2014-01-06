#include <QGuiApplication>
#include <QtQuick>
#include "callbackinterface.h"
#include <QtDBus>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QApplication app(argc, argv);
    QDeclarativeView view;
#else
    QGuiApplication app(argc, argv);
    QQuickView view;
#endif

    CallbackInterface callbackIf;

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService("com.jolla.transferdemoapp")) {
        qCritical() << "DBUS service already taken";
        return -1;
    }

    if (!connection.registerObject("/com/jolla/transferdemoapp", &callbackIf)) {
        qCritical() << "Transfer Demo App: Could not register object";
    }


    // Expose our callback to QML
    view.rootContext()->setContextProperty("callbackInterface", &callbackIf);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.showFullScreen();
    return app.exec();
}
