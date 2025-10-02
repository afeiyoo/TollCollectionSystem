#include "mgsstatemanager.h"

#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/mgsmenudialog.h"
#include "utils/stdafx.h"

//==============================================================================
// 状态管理类
//==============================================================================
MgsStateManager::MgsStateManager(QWidget *parent)
    : QObject{parent}
{
    m_menuDialog = new MgsMenuDialog(parent);
    m_menuDialog->hide();

    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigChangeState, this, &MgsStateManager::onChangeState);
}

MgsStateManager::~MgsStateManager()
{
    SAFE_DELETE(m_curState);
}

void MgsStateManager::showMenu()
{
    if (!m_curState || !m_menuDialog)
        return;

    m_menuDialog->reset();
    m_curState->setupMenu(m_menuDialog);
    m_menuDialog->exec();
}

void MgsStateManager::onChangeState(State *newState)
{
    if (!newState)
        return;

    SAFE_DELETE(m_curState);
    m_curState = newState;
}

//==============================================================================
// 抽象状态类
//==============================================================================
void State::setWeightLow(bool weightLow)
{
    m_weightLow = weightLow;
}

//==============================================================================
// 具体状态：混合入口自动发卡模式
//==============================================================================
void MtcInAutoMode::setupMenu(MgsMenuDialog *menu)
{
    menu->addNewTab("常用", {"0. 闽通卡圈存"}, {0});
    if (!m_weightLow) {
        menu->addNewTab("设备", {"0. 启用称重降级"}, {});
    } else {
        menu->addNewTab("设备", {"0. 取消称重降级"}, {});
    }
    menu->addNewTab("系统", {"1. 关闭电脑", "2. 重启电脑", "3. 重启软件"}, {0, 1, 2});
}

//==============================================================================
// 具体状态：混合入口人工发卡模式
//==============================================================================
void MtcInManaualMode::setupMenu(MgsMenuDialog *menu)
{
    menu->addNewTab("常用", {"0. 闽通卡圈存"}, {0});
    if (!m_weightLow) {
        menu->addNewTab("设备", {"0. 启用称重降级"}, {0});
    } else {
        menu->addNewTab("设备", {"0. 取消称重降级"}, {0});
    }
    menu->addNewTab("系统", {"1. 关闭电脑", "2. 重启电脑", "3. 重启软件"}, {0, 1, 2});
}

//==============================================================================
// 具体状态：混合入口人工发纸券模式
//==============================================================================
void MtcInPaperMode::setupMenu(MgsMenuDialog *menu)
{
    menu->addNewTab("常用", {"0. 闽通卡圈存"}, {0});
    if (m_weightLow) {
        menu->addNewTab("设备", {"0. 启用称重降级"}, {});
    } else {
        menu->addNewTab("设备", {"0. 取消称重降级"}, {});
    }
    menu->addNewTab("系统", {"1. 关闭电脑", "2. 重启电脑", "3. 重启软件"}, {0, 1, 2});
}
