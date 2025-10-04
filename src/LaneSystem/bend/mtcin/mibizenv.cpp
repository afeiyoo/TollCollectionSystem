#include "mibizenv.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "global/statemanager.h"

MIBizEnv::MIBizEnv(QObject *parent)
    : QObject{parent}
{
    m_curState = new MtcInManaualMode();
    emit GM_INSTANCE->m_signalMan->sigChangeState(m_curState);
}

MIBizEnv::~MIBizEnv() {}
