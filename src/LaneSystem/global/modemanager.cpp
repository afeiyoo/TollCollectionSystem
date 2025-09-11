#include "modemanager.h"
#include "gui/component/mgsmenu.h"

// -------------------- ModeManager --------------------------
ModeManager::ModeManager(QObject *parent)
    : QObject{parent}
{
    m_curMode = new MtcInAutoMode();
    m_menu = new MgsMenu();
    m_menu->hide();
}

ModeManager::~ModeManager()
{
    delete m_curMode;
    delete m_menu;
}

void ModeManager::showMenu()
{
    if (!m_curMode || !m_menu)
        return;

    m_menu->reset();
    m_curMode->setupMenu(m_menu);
    m_menu->show();
}

void ModeManager::onModeChanged(Mode *newMode)
{
    if (!newMode)
        return;

    delete m_curMode;
    m_curMode = newMode;
}

// -------------------- MtcInAutoMode --------------------------
void MtcInAutoMode::setupMenu(MgsMenu *menu)
{
    menu->addNewTab("常用", {"1. 切换模式", "2. 闽通卡复位", "3. 节假日参数查看", "4. 打开天线"}, {0, 1, 2, 3});
    menu->addNewTab("维护", {"1. 修改软件配置", "2. 设备检测"}, {0});
    menu->addNewTab("系统", {"1. 关闭电脑", "2. 重启电脑", "3. 重启软件"}, {0, 1, 2});
}

// -------------------- MtcInManaualMode --------------------------
void MtcInManaualMode::setupMenu(MgsMenu *menu)
{
    menu->addNewTab("常用", {"1. 切换模式", "2. 闽通卡复位", "3. 节假日参数查看", "4. 打开天线"}, {0, 1, 2});
    menu->addNewTab("维护", {"1. 修改软件配置", "2. 设备检测"}, {0, 1});
    menu->addNewTab("系统", {"1. 关闭电脑", "2. 重启电脑"}, {0});
}

// -------------------- MtcInPaperMode --------------------------
void MtcInPaperMode::setupMenu(MgsMenu *menu)
{
    menu->addNewTab("常用", {"1. 切换模式", "2. 闽通卡复位", "3. 节假日参数查看", "4. 打开天线"}, {0, 1});
    menu->addNewTab("维护", {"1. 修改软件配置", "2. 设备检测"}, {0, 1});
    menu->addNewTab("系统", {"1. 发纸券"}, {0});
}
