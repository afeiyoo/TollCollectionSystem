#include "mideskprocess.h"

#include "Logger.h"
#include "bend/mtcin/mibizenv.h"
#include "config/config.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/mgsmainwindow.h"
#include "tools/laneauth.h"
#include "utils/uiutils.h"

using namespace Utils;

MIDeskProcess::MIDeskProcess(MgsMainWindow *mainWindow, QObject *parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
{
    // 业务环境变量初始化
    m_bizEnv = new MIBizEnv(this);

    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigMenuRequest, this, &MIDeskProcess::onMenuRequest);
    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigCloseLane, this, &MIDeskProcess::onCloseLane);
    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigOpenLane, this, &MIDeskProcess::onOpenLane);
    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigShiftIn, this, &MIDeskProcess::onShiftIn);
    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigShiftOut, this, &MIDeskProcess::onShiftOut);
}

MIDeskProcess::~MIDeskProcess() {}

void MIDeskProcess::onVDChanged(unsigned int newVD) {}

void MIDeskProcess::onMenuRequest(uint funcIndex)
{
    LOG_INFO().noquote() << "测试成功进入" << funcIndex;
    UiUtils::showMessageBoxInfo("测试", "弹窗测试", QMessageBox::Yes);
}

void MIDeskProcess::onCloseLane() {}

void MIDeskProcess::onOpenLane() {}

void MIDeskProcess::onShiftIn()
{
    if (m_bizEnv->m_isShifted) {
        LOG_INFO().noquote() << "车道已上班，不响应上班请求";
        return;
    }

    // 1. 重新加载参数

    // 2. 车道授权
    bool authOk = GM_INSTANCE->m_laneAuth->authCheck(GM_INSTANCE->m_config->m_businessConfig.stationID,
                                                     GM_INSTANCE->m_config->m_businessConfig.laneID,
                                                     GM_INSTANCE->m_config->m_businessConfig.laneIP,
                                                     GM_INSTANCE->m_config->m_businessConfig.CNLaneID);
    m_mainWindow->showFormErrorHint("车道授权失败",
                                    {"请通过省中心配置平台注册",
                                     "请联系运维人员处理",
                                     "情况大家激发大家",
                                     "的骄傲法律角度是发的开机卡了"});
}

void MIDeskProcess::onShiftOut() {}
