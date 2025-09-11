//---------------------------------------------------------------------------

#ifndef cCaptureCtrlH
#define cCaptureCtrlH
#include <QtCore>
#if defined(Q_OS_LINUX)
#else
#include <QWidget>
#endif
#include "ccapturedef.h"
//---------------------------------------------------------------------------


//
//signals:
//void data_from_cap(BYTE* pHead,BYTE* pFrame); //抓拍返回的数据
//void connect_from_cap(bool bConnect);//连接抓拍设备情况
//
#if defined(Q_OS_LINUX)
class IQXCaptureCtrl:public QObject
#else
class IQXCaptureCtrl:public QWidget//QObject
#endif
{
    Q_OBJECT
signals:
    void data_from_cap(int nIndex);
public:
   //打开设备
   virtual int OpenCapture(char *ServIP,int nPort,int nIndex=0)=0;
   //关闭设备
   virtual void CloseCapture()=0;
   //状态检查
   virtual int CheckStatus()=0;
   //获取车辆信息
   virtual int GetVehicleInfo(TCapVehInfo &veh)=0;
   //异步触发
   virtual int Trigger()=0;
   //同步触发
   virtual int SyncTrigger(TCapVehInfo &veh)=0;
   //校时功能(yyyymmddhhnnss)
   virtual int AdjustTime(char *sTime)=0;
   //播放视频
   virtual int PlayVideo(int nLeft,int nTop,int nRight,int nBottom,int nChannel)=0;
   //字符叠加
   virtual int SendCharOverlap(char* Overlap,int Len)=0;
   //输出日志配置
   virtual void SetLogger(const char *logFile, int logLevel)=0;
   //初始化
   virtual int CapConfig(BYTE TriggerMode,char *StationHex,char *StationName,BYTE LaneNum)=0;    //0.视频触发 1.地感线圈触发
   //二进制数据转图片数据
   virtual int GetBit2Bmp(const unsigned char* bitBuf,unsigned char* bmpBuf)=0;
};

//nType: 1.汉王标清 2.高德威标清 3.高德威板卡标清
extern "C" Q_DECL_EXPORT IQXCaptureCtrl* GetQtCaptureCtrl32(int nType);
#endif


