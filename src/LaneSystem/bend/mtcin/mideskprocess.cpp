#include "mideskprocess.h"

#include "Logger.h"
#include "QSimpleUpdater.h"
#include "bend/mtcin/mibizenv.h"
#include "config/config.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/mgsmainwindow.h"
#include "tools/laneauth.h"
#include "utils/uiutils.h"

#include <QThread>

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

    // 1. 车道授权
    bool authOk = GM_INSTANCE->m_laneAuth->authCheck(GM_INSTANCE->m_config->m_businessConfig.stationID,
                                                     GM_INSTANCE->m_config->m_businessConfig.laneID,
                                                     GM_INSTANCE->m_config->m_businessConfig.laneIP,
                                                     GM_INSTANCE->m_config->m_businessConfig.CNLaneID);
    if (!authOk) {
        m_mainWindow->showFormErrorHint("车道授权失败", {"该车道未在省中心平台注册", "请及时联系运维人员处理!"});
        return;
    }
    // 2. 检查程序更新，必须是最新程序才可以登班

    // 3. 上班后，重新加载参数
}

void MIDeskProcess::onShiftOut() {}
