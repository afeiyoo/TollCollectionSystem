#ifndef CMODESKPROCESS_H
#define CMODESKPROCESS_H

/*
 * 混合出口桌面处理类
*/

#include "bend/etc/cetcprocess.h"
#include "csptprocess.h"
#include <QObject>

class CMODeskProcess : public QObject
{
    Q_OBJECT
private:
    ILaneForm *m_form;
    CBizEnv *m_biz_env;
    IBizService *m_service;
    TLaneEnv *m_env;
    ILaneDeviceCtrl *m_devices;
    CETCProcess *m_etc_proc;
    CSPTProcess *m_spt_proc;

public:
    explicit CMODeskProcess(
        ILaneForm *aForm, CBizEnv *bizEnv, TLaneEnv *env, ILaneDeviceCtrl *pCtrl, QObject *parent = 0);

signals:

public slots:
    void onVDChanged(unsigned int newVD);
    void onKeyPress(unsigned int keyCode);
};

#endif // CMODESKPROCESS_H
