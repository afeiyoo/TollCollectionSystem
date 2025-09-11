#ifndef CETCDESKPROCESS_H
#define CETCDESKPROCESS_H

/*
 * ETC的桌面处理类
*/

#include <QObject>
#include "cetcprocess.h"

class CETCDeskProcess : public QObject
{
    Q_OBJECT
public:
    explicit CETCDeskProcess(ILaneForm *aForm,CBizEnv* bizEnv,TLaneEnv* env,ILaneDeviceCtrl* pCtrl,
                             QObject *parent = 0);
private:
    ILaneForm* m_form;
    CBizEnv* m_biz_env;
    TLaneEnv* m_env;
    CETCProcess* m_etc_proc;
signals:

public slots:
    void onKeyPress(unsigned int keyCode);
};

#endif // CETCDESKPROCESS_H
