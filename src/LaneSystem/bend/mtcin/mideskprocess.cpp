#include "mtcindeskhandler.h"

MtcInDeskHandler::MtcInDeskHandler(
    ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent)
    : QObject(parent)
{
    // m_form = aForm;
    // m_biz_env = bizEnv;
    // m_service = bizEnv->m_service;
    // m_env = env;
    // m_devices = pCtrl;

    // connect(m_form, SIGNAL(sigKeyPress(uint)), this, SLOT(onKeyPress(uint)));

    // //创建自动发卡机业务处理线程
    // m_robot_proc = NULL;
    // if (m_env->laneMode == 5)
    //     m_robot_proc = new CardRobotHandler(aForm, bizEnv, env, pCtrl, this);

    // //创建ETC业务处理线程
    // m_etc_proc = new CETCProcess(aForm, bizEnv, env, pCtrl, this);
}

void MtcInDeskHandler::onVDChanged(unsigned int newVD) {}

bool MtcInDeskHandler::login(const QVariantMap &params)
{
    QString userID = params["userID"].toString();
    QString password = params["password"].toString();

    // 调用远程服务，查询参数是否正确
}
