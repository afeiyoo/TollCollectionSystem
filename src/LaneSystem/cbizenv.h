#ifndef CBIZENV_H
#define CBIZENV_H

/*
 * 环境类型：TLaneEnv和CBizEnv
 * 此处的环境类型与后端服务的环境类型分开
*/

#include <QObject>
#include <QMutex>
#include <QGlobalStatic>

#include "ILaneDeviceCtrl.h"
#include "ibizservice.h"
#include "global/tlaneconfig.h"
#include "Logger.h"
#include "ConsoleAppender.h"
#include "RollingFileAppender.h"

#define GENV CBizEnv::GetInstance()


//业务环境类
class CBizEnv : public QObject
{
    Q_OBJECT
public:
    explicit CBizEnv(QObject *parent = 0);
    ~CBizEnv();
    static CBizEnv *GetInstance();
    void InitLaneEnv(); //运行环境初始化
    void InitBizEnv();  //业务环境初始化
    void RegistLogs();  //注册日志文件
    TLaneEnv GetLaneEnv() const;
    TDevConfig GetDevConfig() const;
    QString GetKeyName(int keyCode) const;
    int GetKeyCode(QString keyName) const;
public:
    QMutex m_mutex;    //更改或读取数据共享锁
    IBizService *m_service; //车道后端业务服务对象

private:
    CBizEnv(const CBizEnv &) = delete;
    CBizEnv &operator=(const CBizEnv &) = delete;

    bool loadLaneEnv();
    bool loadKeyboard();
    bool loadDevCfg();
    bool createDefaultCfg();
    bool createDefaultKeyboardCfg(int type);
private:
    QList<TCapVehInfo> m_cap_list;  //车牌识别队列
    QList<TVehicleWeightInfo> m_weight_list; //称重数据队列
    QMap<QString,RollingFileAppender*> m_loggerAppenders; //日志等级
    TLaneEnv m_lane_env;
    TDevConfig m_dev_cfg;
    QMap<QString,QString> m_keyboardMap;
signals:

public slots:
    void onWeightInfo(TVehicleWeightInfo weightInfo);  //称重检测事件
    void onCapInfo(TCapVehInfo capInfo);  //车牌识别事件
    void onVehRecognize();   //车型识别事件
};



#endif // CBIZENV_H
