#include "cardrobotprocess.h"

CardRobotProcess::CardRobotProcess(ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent) : QObject(parent)
{
    m_form = aForm;
    m_biz_env = bizEnv;
    m_service = bizEnv->m_service;
    m_env = env;
    m_devices = pCtrl;

    //独立线程工作,与其它类需通过信号槽方式进行交互
    this->moveToThread(&m_thread);
    m_thread.start();
}

CardRobotProcess::~CardRobotProcess()
{
    m_thread.terminate();
    m_thread.wait();
}

void CardRobotProcess::onVDChanged(unsigned int newVD)
{

}
