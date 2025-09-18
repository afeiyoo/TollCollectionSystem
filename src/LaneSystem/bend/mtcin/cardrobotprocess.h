#pragma once

#include "ILaneDeviceCtrl.h"
#include <QObject>
#include <QThread>

class CardRobotProcess : public QObject
{
    Q_OBJECT

public:
    explicit CardRobotProcess(
        ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent = 0);
    ~CardRobotProcess();

signals:

public slots:
    void onVDChanged(unsigned int newVD);

private:
    ILaneForm *m_form;
    CBizEnv *m_biz_env;
    IBizService *m_service;
    TLaneEnv *m_env;
    ILaneDeviceCtrl *m_devices;
    QThread m_thread;
};
