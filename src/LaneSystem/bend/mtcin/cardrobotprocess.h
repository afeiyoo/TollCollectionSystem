#pragma once

/*
 * 入口自动发卡处理类
 * 由CMIDeskProcess创建与控制
*/

#include "ILaneDeviceCtrl.h"
#include "cbizenv.h"
#include "ilaneform.h"
#include <QObject>
#include <QThread>

class CardRobotHandler : public QObject
{
    Q_OBJECT
private:
    ILaneForm *m_form;
    CBizEnv *m_biz_env;
    IBizService *m_service;
    TLaneEnv *m_env;
    ILaneDeviceCtrl *m_devices;
    QThread m_thread;

public:
    explicit CardRobotHandler(
        ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent = 0);
    ~CardRobotHandler();

signals:

public slots:
    void onVDChanged(unsigned int newVD);
};
