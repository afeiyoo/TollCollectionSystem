#include "cetcprocess.h"

CETCProcess::CETCProcess(ILaneForm *aForm,CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent) : QObject(parent)
{
    m_form = aForm;
    m_biz_env = bizEnv;
    m_service = bizEnv->m_service;  //车道后端服务
    m_env = env;
    m_devices = pCtrl;


    connect(this,SIGNAL(sigRsuCtrl(bool)),this,SLOT(onRsuCtrl(bool)));

    //独立线程工作,与其它类需通过信号槽方式进行交互
    this->moveToThread(&m_thread);
    m_thread.start();
}

CETCProcess::~CETCProcess()
{
    m_thread.terminate();
    m_thread.wait();
}

void CETCProcess::onVDChanged(unsigned int newVD)
{

}

void CETCProcess::onRsuCtrl(bool isOpen)
{

}
