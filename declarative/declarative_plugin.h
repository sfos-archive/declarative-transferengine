#ifndef DECLARATIVE_PLUGIN_H
#define DECLARATIVE_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QTranslator>
#include <QQmlEngine>

class AppTranslator: public QTranslator
{
   Q_OBJECT
public:
   AppTranslator(QObject *parent);
   virtual ~AppTranslator();
};

class DeclarativeContentAction: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Error error READ error NOTIFY errorChanged)
    Q_ENUMS(Error)

public:
    enum Error {
        NoError,
        FileTypeNotSupported,
        FileDoesNotExist,
        InvalidUrl
    };

    explicit DeclarativeContentAction(QObject *parent = 0);
    Q_INVOKABLE bool trigger(const QUrl &url);

    Error error() const;

signals:
    void errorChanged();
private:

    Error m_error;
};

static QObject *content_action(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new DeclarativeContentAction;
}

class DeclarativePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.jolla.transferengine-ui")

public:
    void initializeEngine(QQmlEngine *engine, const char *uri);
    void registerTypes(const char *uri);
};

#endif // DECLARATIVE_PLUGIN_H

