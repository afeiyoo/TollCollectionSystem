#include "cetcdeskprocess.h"

CETCDeskProcess::CETCDeskProcess(ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent) : QObject(parent)
{
    m_form = aForm;
    m_biz_env = bizEnv;
    m_env = env;

    //接收主窗体按键事件
    connect(m_form,SIGNAL(sigKeyPress(uint)),this,SLOT(onKeyPress(uint)));

    //启动ETC业务处理线程
    m_etc_proc = new CETCProcess(aForm,bizEnv,env,pCtrl,this);

}


void CETCDeskProcess::onKeyPress(unsigned int keyCode)
{

}
