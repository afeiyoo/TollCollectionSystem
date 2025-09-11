#ifndef CDEVCTRLSERPUBDEF_H
#define CDEVCTRLSERPUBDEF_H

#include "ILaneCardCtrl.h"
#include "../include/laneweight.h"
#include "../Common/CHexBufTool.h"
#include "../Common/CommonTool.h"
//#include "../LanePublic/ccardreaderdef.h"

typedef unsigned char BYTE;
typedef unsigned int  UINT;
typedef unsigned short USHORT;


//设备类型
enum DeviceType{
    DevType_IOCtrl,         //开关量设备
    DevType_CardCtrl,       //读卡器
    DevType_WeightCtrl,      //称重设备
    DevType_InfoBoard,       //情报板
    DevType_LedCtrl,        //额显示器
    DevType_CardRoboot      //自动发卡机
};

//开关量控制的设备
#define IOCtrlDevType_Railing 0x01          //栏杆机1
#define IOCtrlDevType_PassingLamp 0x02      //通行灯
#define IOCtrlDevType_CeilingLamp 0x03      //天棚灯
#define IOCtrlDevType_Warning 0x04          //声光告警
#define IOCtrlDevType_Railing2 0x5          //栏杆机2

//读卡器控制命令类型
//enum CardCtrlOperType{
//    CardCtrlOperType_ReadPsam,          //读取PSAM卡信息
//    CardCtrlOperType_ReadCard,          //读取卡信息
//    CardCtrlOperType_WriteCard,         //写卡指令
//    CardCtrlOperType_PsamAuth           //PSAM卡授权指令
//};



enum LedCtrlCtrlType{
    LedCtrlCtrlType_ShowLine,           //  行显示指令
    LedCtrlCtrlType_ShowText,           // 文本显示指令
    LedCtrlCtrlType_Warning,            //声光报警
    LedCtrlCtrlType_PassingLamp,        //通行灯
    LedCtrlCtrlType_Clear               //清屏
};

//自动发卡机
enum CardRobootCtrlType{
    CardRobootCtrlType_RejectCard,              // 出卡指令
    CardRobootCtrlType_BadCard,                 //坏卡指令
    CardRobootCtrlType_TrigerKey,               //触发按键指令
    CardRobootCtrlType_TakeBackCardToAntenna,        //  卡回收到天线区指令
    CardRobootCtrlType_Telescopicjib,               //伸缩臂控制指
    CardRobootCtrlType_CancelKey                //按键取消
};


enum CardRobootUploadType{
    CardRobootUploadType_PowerOn,           //上电信息
    CardRobootUploadType_Status,            //状态信息
    CardRobootUploadType_OutCardInfo,          //出卡信息
    CardRobootUploadType_PickUpCardKeyPress,//按键取卡
    CardRobootUploadType_CardRemoved,       //卡被取走
    CardRobootUploadType_CardBoxInfo,        //卡夹信息
    CardRobootUploadType_TakeBackCardOk,     //卡回收完成
    CardRobootUploadType_IgnoreKeyPress     //按键忽略信息
};


//初始化指令
struct TInitData{
    UINT Seconds;
    char DateTime[15];
    char CfgVersion[13];
    BYTE HeartInterval;
    char Reserve[8];
};
//初始化指令结果
struct TInitResultData{
    UINT Seconds;
    BYTE ErrCode;
    USHORT ErrLen;
    char ErrDesc[1024];
};
//配置文件
struct TCfgData{
    char CfgVersionp[13];
    UINT DataLen;
    char Data[1024];
    char Reserve[4];
};

//心跳
typedef struct tag_DeviceheartInfo{
   UINT CurrentTime;
   char CfgVersion[13];
   char FrmVersion[20];
   USHORT IOInputStatus;
   USHORT IOOutputStatus;
   BYTE IPCStatus;
   USHORT CPU;
   USHORT Memory;
   USHORT HardDisk;
   USHORT IO;
   USHORT Temperature;
   UINT SystemTime;
   char SoftVersion[20];
   char ConfigVersion[20];
   USHORT WorkStatus;
   USHORT SubStatus;
   USHORT ErrLen;
   char ErrDesc[512];

}TDeviceHeartInfo;

//开关量设备控制指令
struct TIOCtrlData{
    BYTE DevType;                   //1-自动栏杆机,2-通行信号灯,3-天棚信号灯,4-声光报警,5-自动栏杆机2
    BYTE DeviceStatus;               //0 关闭 1 打开
    char Reserve[8];
};

//线圈状态发生变化
struct TIOCtrlUploadData{
    BYTE CoilNum;
    USHORT CoilStatus;//按车行方向顺序,依次对应相应位。如第一个和第三个线圈有信号,二进制值为0000 0000 0000 0101
    char Reserve[8];
};


//称重
struct TWeightCtrlData{
    BYTE cmd;
    BYTE idx;
    char Reserve[8];
};

//称重上报结构体参照动态库

//情报板
struct TInfoBoardCtrlData{
    BYTE OpenClose;  //情报板开关，0-关闭情报板；1-打开情报板
    BYTE FontColor;  //字体颜色，01H-绿色，02H-红色，03H-黄色
    BYTE FontSize;   //字体大小，01H~08H
    char Text[256];   //显示内容
};

//行显示指令
struct TLedCtrlShowLine{
    BYTE ScreenLight;    //屏幕亮度，00H~0FH，其中00H为自动调节，亮度依次递增
    BYTE FontSize;       //字体颜色，1-绿色；2-红色；3-黄色
    char Line1[32];
    char Line2[32];
    char Line3[32];
    char Line4[32];
    char Line5[32];
    char Line6[32];
};


//文本显示指令
struct TLedCtrlShowText{
    BYTE ScreenLight;    //屏幕亮度，00H~0FH，其中00H为自动调节，亮度依次递增
    BYTE FontSize;       //字体颜色，1-绿色；2-红色；3-黄色
    BYTE LineCount;      //显示行数，01H~06H
    char Text[128];      //显示内容
};



//费额显示器
//struct TLEDCtrlData{
//    LedCtrlCtrlType ContrlType;
//    BYTE ScreenLight;    //屏幕亮度，00H~0FH，其中00H为自动调节，亮度依次递增
//    BYTE FontSize;       //字体颜色，1-绿色；2-红色；3-黄色
//    //行显示指令
//    char Line1[32];
//    char Line2[32];
//    char Line3[32];
//    char Line4[32];
//    char Line5[32];
//    char Line6[32];
//    //文本显示指令
//    BYTE LineCount;      //显示行数，01H~06H
//    char Text[128];      //显示内容
//    //声光报警,通行灯,清屏
//    BYTE ContrlCmd;      //控制指令,0x00取消声光报警，0x01启动声光报警
//    USHORT ContrlTime;     //声光报警持续时间(秒)，0：一直持续
//    char Reserve[8];
//};


//卡机
//struct TCardRobootData{
//    CardRobootCtrlType ControlType;
//    BYTE WorkStation;
//    //卡回收到天线区指令
//    BYTE WorkStation1;       //31H：回收 1#通道卡口处的卡， 30H：不回收 1#通道卡口处的卡
//    BYTE WorkStation2;
//    BYTE WorkStation3;
//    BYTE WorkStation4;
//    //伸缩臂控制
//    BYTE DoCtrl;             //30H机械臂收回 31H机械臂伸出
//};

//状态信息
struct TCardRobootStatus{
    BYTE UpWork;              //上工位当前卡机：30H 无；31H 为 1#机；32H 为 2#机；
    BYTE DownWork;           //下工位当前卡机：30H 无；33H 为 3#机；34H 为 4#机；
    BYTE BoxStatus1;         //1#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE HasCardBag1;        //1#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT RestCards1;         //1#卡机中卡计数值：“050”标示具有 50 张卡；
    BYTE HasAnnCard1;        //1#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
    BYTE BoxStatus2;         //2#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE HasCardBag2;        //2#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT RestCards2;         //2#卡机中卡计数值：“050”标示具有 50 张卡；
    BYTE HasAnnCard2;        //2#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
    BYTE BoxStatus3;         //3#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE HasCardBag3;        //3#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT RestCards3;         //3#卡机中卡计数值：“050”标示具有 50 张卡；
    BYTE HasAnnCard3;        //3#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
    BYTE BoxStatus4;         //4#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE HasCardBag4;        //4#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT RestCards4;         //4#卡机中卡计数值：“050”标示具有 50 张卡
    BYTE HasAnnCard4;        //4#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
};

struct TCardBoxInfo{
    //卡夹信息
    UINT BoxNo1;                 //1#卡机卡夹编号（无为: 0x00,0x00,0x00,0x00）
    USHORT CardNum1;               //1#卡机卡数信息
    UINT BoxNo2;
    USHORT CardNum2;
    UINT  BoxNo3;
    USHORT CardNum3;
    UINT BoxNo4;
    USHORT CardNum4;

};

//卡机上报的数据
//struct TCardRobootUploadData{
//    CardRobootUploadType UploadType;
//    BYTE WorkStation;        //工位信息：31H 上工位；32H 为下工位； 33H 为出卡失败: 34H 为响应按键讯息超时
//    BYTE BoxNo;              //当前卡机编号： 31H 为 1#机；32H 为 2#机；33H 为 3#机；34 为 4#机；
//    //状态信息
//    BYTE UpWork;              //上工位当前卡机：30H 无；31H 为 1#机；32H 为 2#机；
//    BYTE DownWork;           //下工位当前卡机：30H 无；33H 为 3#机；34H 为 4#机；
//    BYTE BoxStatus1;         //1#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
//    BYTE HasCardBag1;        //1#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
//    UINT RestCards1;         //1#卡机中卡计数值：“050”标示具有 50 张卡；
//    BYTE HasAnnCard1;        //1#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
//    BYTE BoxStatus2;         //2#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
//    BYTE HasCardBag2;        //2#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
//    UINT RestCards2;         //2#卡机中卡计数值：“050”标示具有 50 张卡；
//    BYTE HasAnnCard2;        //2#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
//    BYTE BoxStatus3;         //3#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
//    BYTE HasCardBag3;        //3#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
//    UINT RestCards3;         //3#卡机中卡计数值：“050”标示具有 50 张卡；
//    BYTE HasAnnCard3;        //3#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
//    BYTE BoxStatus4;         //4#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
//    BYTE HasCardBag4;        //4#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
//    UINT RestCards4;         //4#卡机中卡计数值：“050”标示具有 50 张卡
//    BYTE HasAnnCard4;        //4#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
//    //卡夹信息
//    UINT BoxNo1;                 //1#卡机卡夹编号（无为: 0x00,0x00,0x00,0x00）
//    USHORT CardNum1;               //1#卡机卡数信息
//    UINT BoxNo2;
//    USHORT CardNum2;
//    UINT  BoxNo3;
//    USHORT CardNum3;
//    UINT BoxNo4;
//    USHORT CardNum4;
//    //按键忽略信息
//    BYTE Cause;              //原因 (30H:当前通道正在出卡/回收, 31H:无卡可发，32H: 卡机故障， 33H:一边故障，一边无卡. 34H 降级 模式, 不处理按键. 35H 离前一张卡取卡不到三秒，36H 连续多张坏卡，中止流程)。
//};






#endif // CDEVCTRLSERPUBDEF_H
