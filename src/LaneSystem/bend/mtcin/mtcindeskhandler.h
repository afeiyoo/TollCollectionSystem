#pragma once

/*
 * 混合入口桌面处理类
*/

#include "bend/etc/cetcprocess.h"
#include "cardrobothandler.h"
#include <QObject>

class MtcInDeskHandler : public QObject
{
    Q_OBJECT
private:
    ILaneForm *m_form;
    CBizEnv *m_biz_env;
    IBizService *m_service;
    TLaneEnv *m_env;
    ILaneDeviceCtrl *m_devices;
    CETCProcess *m_etc_proc;
    CardRobotHandler *m_robot_proc;

public:
    explicit MtcInDeskHandler(
        ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent = 0);
signals:

public slots:
    void onVDChanged(unsigned int newVD);

    // ----------------------------------------------
    bool login(const QVariantMap &params);
};
