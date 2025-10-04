#include "signalmanager.h"
#include "global/statemanager.h"

SignalManager::SignalManager(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<State *>("State*");
}

SignalManager::~SignalManager() {}
