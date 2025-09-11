#ifndef CSPTPROCESS_H
#define CSPTPROCESS_H

/*
 * 自助缴费终端处理类
 * 由混合出口桌面处理类创建和控制
*/

#include <QObject>
#include <QThread>
#include "ilaneform.h"
#include "cbizenv.h"
#include "ILaneDeviceCtrl.h"

class CSPTProcess : public QObject
{
    Q_OBJECT
public:
    explicit CSPTProcess(ILaneForm *aForm,CBizEnv* bizEnv,TLaneEnv* env,ILaneDeviceCtrl* pCtrl,
                         QObject *parent = 0);
    ~CSPTProcess();
private:
    ILaneForm* m_form;
    CBizEnv* m_biz_env;
    IBizService* m_service;
    TLaneEnv* m_env;
    ILaneDeviceCtrl* m_devices;
    QThread m_thread;

signals:

public slots:
    void onVDChanged(unsigned int newVD);

};

#endif // CSPTPROCESS_H
