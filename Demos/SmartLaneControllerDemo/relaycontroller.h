#pragma once

#include <QMap>
#include <QObject>

class SmartLaneController;
class RelayController : public QObject
{
    Q_OBJECT

public:
    explicit RelayController(QObject *parent = nullptr);
    ~RelayController() override;

    void controlDev(int controlBit, int status);
    void setRelayForDev(int controlBit, int levelBit);
    void reset();

    void setSmartLaneCtrl(SmartLaneController *newSmartLaneCtrl);

private:
    QByteArray packSendData(const QMap<int, int> &relayLevel, const QMap<int, int> &triggerMap);

    QMap<int, int> m_relayLevel;                    // 继电器路数-打开/关闭
    QMap<int, int> m_triggerMap;                    // 继电器路数-高电平/低电平
    SmartLaneController *m_smartLaneCtrl = nullptr; // 智能车道控制器
};
