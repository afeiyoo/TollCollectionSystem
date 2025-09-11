#ifndef ILANEFORM_H
#define ILANEFORM_H

/*
 * 车道窗体信号类，主要用来控制显示和接收按键事件
*/

#include <QtCore>
#include <QObject>
#include <QByteArray>
// #include "CommLogger.h"
#include "cbizenv.h"
#include "ILaneDeviceCtrl.h"

//显示消息类型(IT = Information Type)
#define IT_EN      10  //入口信息
#define IT_EX      11  //出口信息
#define IT_SPECIAL 12  //特情信息
#define IT_SPLIT   13  //分省收费信息
#define IT_TLOG    14  //日志信息
#define IT_WEIGHT  15  //称重信息
#define IT_ROBOT   16  //自助终端信息
#define IT_HISTRADE  17  //历史交易信息

class ILaneForm : public QObject
{
    Q_OBJECT
private:
    // CommLogger* m_logger;
    QObject* m_lane_proc;
    CBizEnv m_biz_env;
    TLaneEnv m_env;
    ILaneDeviceCtrl *m_dev_ctrl;
    void createLaneProc();
public:
    explicit ILaneForm(QObject *parent = 0);
    void initBaseForm();

signals:
    void sigTradeInfo(int infoType,QString infoData);
    void sigDeviceStatus(int nStatus);
    void sigCapImage(QByteArray imgBuf);
    void sigKeyPress(unsigned int keyCode);  //按键响应
public slots:
};

#endif // ILANEFORM_H
