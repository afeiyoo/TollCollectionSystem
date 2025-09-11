#include "cmodeskprocess.h"

CMODeskProcess::CMODeskProcess(ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent) : QObject(parent)
{

    // m_form = aForm;
    // m_biz_env = bizEnv;
    // m_service = bizEnv->m_service;
    // m_env = env;
    // m_devices = pCtrl;

    // connect(m_form,SIGNAL(sigKeyPress(uint)),this,SLOT(onKeyPress(uint)));

    // //创建ETC交易线程
    // m_etc_proc = new CETCProcess(aForm,bizEnv,env,pCtrl,this);

    // //创建SPT交易线程
    // m_spt_proc = NULL;
    // if(m_env->laneMode == 6)
    //    m_spt_proc = new CSPTProcess(aForm,bizEnv,env,pCtrl,this);


}

void CMODeskProcess::onVDChanged(unsigned int newVD)
{

}

void CMODeskProcess::onKeyPress(unsigned int keyCode)
{

}
