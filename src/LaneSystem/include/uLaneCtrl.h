/*---------------------------------------------------------------------------
Module：车道控制器单元
Author：Huanglr
Version：
0.1(2014-05-30):初始版本
0.2(2015-03-20):支持ETC车行控制灯有两个位控制
----------------------------------------------------------------------------*/
#ifndef uLaneCtrlH
#define uLaneCtrlH
#include  "windows.h"

//十位为Port,个位为Byte位置
//VD=Vehicle Detector,其它线圈主要用于ETC
#define MAX_OTHER_VD 10
typedef struct tag_ControlDef
{
   BYTE outRailDown;
   BYTE outRailUp;
   BYTE outPassingLamp;
   BYTE outPassingLampRed;
   BYTE outCeilingLamp;
   BYTE outCeilingLampRed;
   BYTE outWarning;
   BYTE inWeightVD;    //称重线圈
   BYTE inSnapVD;      //抓拍线圈
   BYTE inRailVD;      //落杆线圈
   BYTE VDSignalType;  //线圈信号触发类型(0.低电频触发 1.高电频触发)
   BYTE inOtherVD[MAX_OTHER_VD];  //其它线圈
}TControlDef;

//---------------------------------------------------------------------------
class ILaneCtrl32
{
public:
    //打开车道控制器(默认第一个设备,成功返回0,无设备返回-1，其它错误>0)
    virtual int OpenDevice(TControlDef ctlDef,int nWhich)=0;
    //关闭车道控制器
    virtual void CloseDevice()=0;
    //获取所有线圈状态
    virtual int GetVDStatus(const BYTE channel,USHORT *sta)=0;
    //检测其中一个线圈状态，需先执行GetVDStatus,有状态返回true,无状态返回false
    virtual bool CheckVD(const BYTE nWhich,USHORT vds_status)=0;
    //栏杆状态
    virtual bool IsRailingUp()=0;
    //栏杆控制
    virtual int RailingCtrl(bool isUp)=0;
    //天棚灯控制
    virtual int CeilingLampCtrl(bool isOn)=0;
    //通行指示灯控制
    virtual int PassingLampCtrl(bool isOn)=0;
    //告警控制
    virtual int WarningCtrl(bool isOn)=0;
    //获取当前配置
    virtual TControlDef GetControlDef()=0;
};

extern "C" ILaneCtrl32* __cdecl GetLaneCtrl32();
extern "C" ILaneCtrl32* __cdecl GetLaneCtrlEx(int nType,int nAddr);


#endif
