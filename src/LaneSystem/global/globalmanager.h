#pragma once

#include "QJson/include/parser.h"
#include "QJson/include/serializer.h"
#include <QObject>

#define GM_INSTANCE GlobalManager::instance()

class QSimpleUpdater;
class SignalManager;
class ModeManager;
class LaneService;
class Config;
class MgsMainWindow;
class LaneAuth;
class GlobalManager : public QObject
{
    Q_OBJECT
public:
    explicit GlobalManager(QObject *parent = nullptr);
    ~GlobalManager() override;

    static GlobalManager *instance();

    void init();

public:
    SignalManager *m_signalMan = nullptr;
    ModeManager *m_modeMan = nullptr;
    QSimpleUpdater *m_updater = nullptr;
    // Json解析对象
    QJson::Parser *m_jsonParser = nullptr;
    QJson::Serializer *m_jsonSerializer = nullptr;
    // rpc服务端-车道服务
    LaneService *m_laneService = nullptr;
    // 软件配置对象
    Config *m_config = nullptr;
    // 车道授权工具对象
    LaneAuth *m_laneAuth = nullptr;
};
