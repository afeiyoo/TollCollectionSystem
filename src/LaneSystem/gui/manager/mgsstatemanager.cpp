#include "mgsstatemanager.h"
#include "gui/mgsmenudialog.h"
#include "utils/stdafx.h"

//==============================================================================
// 状态管理类
//==============================================================================
MgsStateManager::MgsStateManager(QWidget *parent)
    : QObject{parent}
{
    m_curState = new MtcInAutoMode();
    m_menuDialog = new MgsMenuDialog(parent);
    m_menuDialog->hide();
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

void MgsStateManager::onStateChanged(State *newState)
{
    if (!newState)
        return;

    delete m_curState;
    m_curState = newState;
}

//==============================================================================
// 具体状态：混合入口自动发卡模式
//==============================================================================
void MtcInAutoMode::setupMenu(MgsMenuDialog *menu)
{
    menu->addNewTab("常用", {"1. 切换模式", "2. 闽通卡复位", "3. 节假日参数查看", "4. 打开天线"}, {0, 1, 2, 3});
    menu->addNewTab("维护", {"1. 修改软件配置", "2. 设备检测"}, {0});
    menu->addNewTab("系统", {"1. 关闭电脑", "2. 重启电脑", "3. 重启软件"}, {0, 1, 2});
}

//==============================================================================
// 具体状态：混合入口手动发卡模式
//==============================================================================
void MtcInManaualMode::setupMenu(MgsMenuDialog *menu)
{
    menu->addNewTab("常用", {"1. 切换模式", "2. 闽通卡复位", "3. 节假日参数查看", "4. 打开天线"}, {0, 1, 2});
    menu->addNewTab("维护", {"1. 修改软件配置", "2. 设备检测"}, {0, 1});
    menu->addNewTab("系统", {"1. 关闭电脑", "2. 重启电脑"}, {0});
}

//==============================================================================
// 具体状态：混合入口手动发纸券模式
//==============================================================================
void MtcInPaperMode::setupMenu(MgsMenuDialog *menu)
{
    menu->addNewTab("常用", {"1. 切换模式", "2. 闽通卡复位", "3. 节假日参数查看", "4. 打开天线"}, {0, 1});
    menu->addNewTab("维护", {"1. 修改软件配置", "2. 设备检测"}, {0, 1});
    menu->addNewTab("系统", {"1. 发纸券"}, {0});
}
