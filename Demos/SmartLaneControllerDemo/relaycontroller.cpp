#include "relaycontroller.h"

#include "smartlanecontroller.h"

#include <QDateTime>

RelayController::RelayController(QObject *parent)
    : QObject{parent}
{
}

RelayController::~RelayController() {}

void RelayController::controlDev(int controlBit, int status)
{
    m_relayLevel[controlBit] = status;
    QByteArray sendData = packSendData(m_relayLevel, m_triggerMap);

    if (!m_smartLaneCtrl)
        return;
    m_smartLaneCtrl->sendCommand("A1", sendData);
}

void RelayController::setRelayForDev(int controlBit, int levelBit)
{
    m_relayLevel[controlBit] = 0; // 默认关闭
    m_triggerMap[controlBit] = levelBit;
}

void RelayController::reset()
{
    m_relayLevel.clear();
    m_triggerMap.clear();
}

void RelayController::setSmartLaneCtrl(SmartLaneController *newSmartLaneCtrl)
{
    m_smartLaneCtrl = newSmartLaneCtrl;
}

QByteArray RelayController::packSendData(const QMap<int, int> &relayLevel, const QMap<int, int> &triggerMap)
{
    QByteArray data;
    data.append(0xa1); // 固定命令 0xa1
    data.append(0x10); // 固定16路，目前只用到前8路，后8路备用
    for (int i = 1; i <= 16; ++i) {
        data.append((char) i); // 路号
        int level = 0;
        int trigger = triggerMap.value(i, 1); // 默认高电平触发
        int want = relayLevel.value(i, 0);    // 默认关闭
        if (trigger == 1) {
            level = want ? 1 : 0;
        } else {
            level = want ? 0 : 1;
        }
        data.append((char) level);
    }
    return data;
}
