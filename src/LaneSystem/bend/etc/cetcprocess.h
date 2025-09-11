#ifndef CETCPROCESS_H
#define CETCPROCESS_H

/*
 * ETC业务处理类
 * 由ETC桌面处理类创建与控制
*/

#include <QObject>
#include <QThread>
#include "ilaneform.h"
#include "cbizenv.h"
#include "ILaneDeviceCtrl.h"

class CETCProcess : public QObject
{
    Q_OBJECT
public:
    explicit CETCProcess(ILaneForm *aForm,CBizEnv* bizEnv,TLaneEnv* env,ILaneDeviceCtrl* pCtrl,
                         QObject *parent = 0);
    ~CETCProcess();
private:
    ILaneForm* m_form;
    CBizEnv* m_biz_env;
    TLaneEnv* m_env;
    ILaneDeviceCtrl* m_devices;
    IBizService* m_service;
    QThread m_thread;
signals:
    void sigRsuCtrl(bool isOpen);  //天线打开关闭信号
public slots:
    void onVDChanged(unsigned int newVD);
    void onRsuCtrl(bool isOpen);
};

#endif // CETCPROCESS_H
