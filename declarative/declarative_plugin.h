#ifndef DECLARATIVE_PLUGIN_H
#define DECLARATIVE_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QTranslator>

class AppTranslator: public QTranslator
{
   Q_OBJECT
public:
   AppTranslator(QObject *parent);
   virtual ~AppTranslator();
};


class DeclarativePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.jolla.transferengine-ui")

public:
    void initializeEngine(QQmlEngine *engine, const char *uri);
    void registerTypes(const char *uri);
};

#endif // DECLARATIVE_PLUGIN_H

