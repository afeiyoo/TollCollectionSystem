#include "signalmanager.h"
#include "gui/manager/mgsstatemanager.h"

SignalManager::SignalManager(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<State *>("State*");
}

SignalManager::~SignalManager() {}
