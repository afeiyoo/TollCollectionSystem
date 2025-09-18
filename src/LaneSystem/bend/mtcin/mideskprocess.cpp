#include "mideskprocess.h"
#include "Logger.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/mgsmainwindow.h"
#include "utils/uiutils.h"

using namespace Utils;

MIDeskProcess::MIDeskProcess(MgsMainWindow *mainWindow, QObject *parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
{
    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigFuncDeal, this, &MIDeskProcess::onDealFunc);
}

MIDeskProcess::~MIDeskProcess() {}

void MIDeskProcess::onVDChanged(unsigned int newVD) {}

void MIDeskProcess::onDealFunc(uint funcIndex)
{
    LOG_INFO().noquote() << "测试成功进入" << funcIndex;
    UiUtils::showMessageBoxInfo("测试", "弹窗测试", QMessageBox::Yes);
}
