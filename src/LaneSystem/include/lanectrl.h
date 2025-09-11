#ifndef LANECTRL_H
#define LANECTRL_H

#include <QString>

#define LC_TYPE_IO      1    //io板卡(x86平台通过IO地址控制，arm平台通过网络与无限感测服务通讯)
#define LC_TYPE_WXGC    2    //无限感测一体车道控制器（串口）

class ILaneCtrl {
public:
    virtual int Open(int port) = 0;               //控制器打开 port:地址        成功返回0
    virtual int Open(QString sPortName, int rate) = 0;  //设置串口
    virtual int Close() = 0;              //控制器关闭
    virtual int GetCoil(int *v1, int *v2) = 0;     //获取线圈1  线圈2
    virtual int GetLight(int *v1, int *v2, int *v3) = 0;
    virtual int GetRailing(int *v1,int *v2) = 0;
    virtual int GetCoil2(int p, int b, int t) = 0;  //获取线圈信号 p:第几行 b：第几列 t:触发信号1或0   返回1有信号
    //设置参数
    virtual void SetBeginCoil(int p, int b, int t) = 0;  //设置抓拍线圈 p:第几行 b：第几列 t:触发信号1或0
    virtual void SetEndCoil(int p, int b, int t) = 0;   //设置落杆线圈 p:第几行 b：第几列 t:触发信号1或0
    virtual void SetRailingDown(int p, int b, int t) = 0;
    virtual void SetRailingUp(int p, int b, int t) = 0;
    virtual void SetRailingDown2(int p, int b, int t) = 0;
    virtual void SetRailingUp2(int p, int b, int t) = 0;
    virtual void SetCeilingLamp(int p, int b, int t) = 0;
    virtual void SetPassingLamp(int p, int b, int t) = 0;
    virtual void SetWarning(int p, int b, int t) = 0;
    //控制
    virtual int RailingDown(int v) = 0;   //落杠   v:1触发  0:不触发
    virtual int RailingUp(int v) = 0;     //升杠
    virtual int RailingDown2(int v) = 0;   //落杠   v:1触发  0:不触发
    virtual int RailingUp2(int v) = 0;     //升杠
    virtual int CeilingLamp(int v) = 0;   //天棚灯
    virtual int PassingLamp(int v) = 0;   //通行灯
    virtual int Warning(int v) = 0;       //告警u
    virtual void Release() = 0;

};

extern "C" ILaneCtrl *GetLaneCtrl(int type);

#endif // LANECTRL_H
