#include "ilaneform.h"
#include "bend/etc/cetcdeskprocess.h"
// #include "bend/mtcin/cmideskprocess.h"
#include "bend/mtcout/cmodeskprocess.h"

ILaneForm::ILaneForm(CommLogger *pLogger, QObject *parent)
    : QObject(parent)
{
    m_logger = pLogger;
}

void ILaneForm::initBaseForm()
{
    //车道设备控制对象(待补充)
    m_dev_ctrl = NULL;

    //初始化环境变量
    m_biz_env.InitLaneEnv(&m_env);
    m_biz_env.InitBizEnv();

    //创建车道
    createLaneProc();
    emit sigTradeInfo(IT_TLOG, "车道处理对象已创建,车道模式:" + QString::number(m_env.laneMode));
}

//创建车道处理对象
void ILaneForm::createLaneProc()
{
    if (m_env.laneMode == 1 || m_env.laneMode == 2) {
        //ETC专用车道处理对象
        m_lane_proc = new CETCDeskProcess(this, &m_biz_env, &m_env, m_dev_ctrl, this);
    } else if (m_env.laneMode == 3 || m_env.laneMode == 5) {
        //入口混合车道
        // m_lane_proc = new MtcInDeskHandler(this, &m_biz_env, &m_env, m_dev_ctrl, this);
    } else if (m_env.laneMode == 4 || m_env.laneMode == 6) {
        //出口混合车道
        m_lane_proc = new CMODeskProcess(this, &m_biz_env, &m_env, m_dev_ctrl, this);
    }
}
