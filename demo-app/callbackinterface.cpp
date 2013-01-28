#include "callbackinterface.h"
#include "demoappadaptor.h"

CallbackInterface::CallbackInterface(QObject *parent) :
    QObject(parent)
{
    new DemoAppAdaptor(this);
}


void CallbackInterface::cancelTransfer(int transferId)
{
    // Do something for real to cancel transfer.
    // We just emit signal to the QML side
    emit transferCanceled(transferId);
}

void CallbackInterface::restartTransfer(int transferId)
{
    emit transferRestarted(transferId);
}
