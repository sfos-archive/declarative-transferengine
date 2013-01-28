#ifndef DECLARATIVE_PLUGIN_H
#define DECLARATIVE_PLUGIN_H

#include <QDeclarativeExtensionPlugin>
#include <QTranslator>

class AppTranslator: public QTranslator
{
   Q_OBJECT
public:
   AppTranslator(QObject *parent);
   virtual ~AppTranslator();
};


class DeclarativePlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
    
public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri);
    void registerTypes(const char *uri);
};

#endif // DECLARATIVE_PLUGIN_H

