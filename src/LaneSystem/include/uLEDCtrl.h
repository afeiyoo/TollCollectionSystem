//---------------------------------------------------------------------------
//LED控制程序 Author:huanglr Fromdate:2014-06-06
//linux 版本下LED_TYPE_NL7132 存在问题，需有设备进行调试
//---------------------------------------------------------------------------
#ifndef uLEDCtrlH
#define uLEDCtrlH
//---------------------------------------------------------------------------
#include <QtCore>

#define LED_TYPE_NONE      0
#define LED_TYPE_NL7132    1    //新大陆
#define LED_TYPE_YINGSHA   2    //英沙
#define LED_TYPE_HUAGONG   3    //华工
#define LED_TYPE_HGHFW     4    //华工恒富威
#define LED_TYPE_MTCZIKAI  5    //MTC紫凯
#define LED_TYPE_ETCZIKAI  6    //ETC紫凯
#define LED_TYPE_AiTeSi    7    //埃特斯
#define LED_TYPE_MGSKJ     8    //自定义
#define LED_TYPE_MGSKJ2    9    //自定义2
#define LED_TYPE_HONGZIHAO 10   //洪志豪
#define LED_TYPE_HUAGONG48 11   //洪志豪4X8
#define LED_TYPE_SZ        12   //
#define LED_TYPE_SZ2       13
#define LED_TYPE_SZ3       14
#define LED_TYPE_SOCKET    15   //福建一体化栏杆机
#define LED_TYPE_UDP       16   //深圳一体化费显

class ILEDCtrl:public QObject
{
    Q_OBJECT
public:
   //设置Com口 nSleep(毫秒)
   virtual int OpenLED(char* sPort,int nRate,int nSleep)=0;
   //显示文字
   virtual void PrintText(char *text)=0;
   //关闭屏
   virtual void CloseLED()=0;
   //测试状态
   virtual void TestLED()=0;
   //下微机系统复位
   virtual void ResetLED()=0;
   //回读屏
   virtual void ReadLED()=0;
   //行显示(点阵方式写屏,最多四屏)
   virtual void PrintLines(char *s1,char *s2,char *s3,char *s4)=0;
   //返回串口号
   virtual void GetComPort(char* sPort)=0;
   //费显车行灯控制
   virtual void PassLampCtrl(bool isOn)=0;
   //费显报警灯控制
   virtual void WarningLampCtrl(bool isOn)=0;
   //栏杆机控制
   virtual void LaneCtrl(bool isOn)=0;
   //语音播放
   virtual void Voice(char* msg)=0;
};

extern "C" ILEDCtrl*  GetLEDCtrl32(int nType);

#endif
