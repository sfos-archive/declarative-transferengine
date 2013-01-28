#ifndef CALLBACKINTERFACE_H
#define CALLBACKINTERFACE_H

#include <QObject>

class CallbackInterface : public QObject
{
    Q_OBJECT
public:
    explicit CallbackInterface(QObject *parent = 0);
    
signals:
    void transferCanceled(int transferId);
    void transferRestarted(int transferId);

public slots:
    void cancelTransfer(int transferId);
    void restartTransfer(int transferId);
};

#endif // CALLBACKINTERFACE_H
