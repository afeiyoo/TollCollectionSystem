//---------------------------------------------------------------------------
//Module:    RSU控制模块
//Author:    Huanglr
//FromDate:  2014-08-19
//Remark:    v1.2(2016-05-14 1.使用buf方式处理数据 2.重新抽象类)
//--------------------------------------------------------------------------

#ifndef uRampRsuCtrl32H
#define uRampRsuCtrl32H

#include "../../include/CXCardCtrlDef.h"

//---------------------------------------------------------------------------
#include <stdio.h>
#include <QtCore>

typedef unsigned char BYTE;
typedef unsigned int  UINT;
typedef unsigned short USHORT;

//RSU天线消息
#define WM_RSU_MSG    WM_USER+215
#define SEL_NEAR_RSU  0
#define SEL_FAR_RSU   1

//EF04偏移地址和长度
#define EF04_OFFSET   314
#define EF04_LEN      95
#define EF04_PROVOFFSET   405
#define EF04_PROVLEN      80
#define EF04_B3LEN    91

//C2停止类型
#define STOPTYPE_RESTART  0x01   //重新开始交易
#define STOPTYPE_RESEND   0x02   //重新发送当前帧数据
#define STOPTYPE_BEEP    0x03  //蜂鸣器响三声
#define STOPTYPE_SOUND   0x04 //播放特情语音标识

//C1播报语音类型
#define CONTINUE_TYPE_NONE 0x00 //不发送语音
#define CONTINUE_TYPE_SOUND 0x01 //发送交易成功语音标识

//RSU指令类型
typedef struct tagRampRsuCmd{
   BYTE SelRsu;
   BYTE NewRsctl;
   BYTE CmdType;
   BYTE DealType;   //1:串口 2：网口
   BYTE Version;
   UINT DataLen;
} TRampRsuCmd;

//天线类型
#define RSU_TYPE_GEV   1           //金溢
#define RSU_TYPE_ARTC  2           //埃特斯

//业务处理接口
#define BIZ_RET_CONTINUE  0        //发送C1继续交易
#define BIZ_RET_STOP      1        //发送C2中止交易
#define BIZ_RET_C3        0xC3     //入口发送C3指令
#define BIZ_RET_C6        0xC6     //出口发送C6指令
#define BIZ_RET_C8        0xC8     //异常交易C8指令(重新取回B5)
#define BIZ_RET_C9        0xC9     //

//车道模式
#define LANE_MODE_IN      3        //ETC入口
#define LANE_MODE_OUT     4        //ETC出口

//0019版本
#define VERSION_0019_00   0x00
#define VERSION_0019_4F   0x4F
#define VERSION_0019_5F   0x5F

//是否操作 OBU-ESAM
#define EF04_NO  0x00
#define EF04_DO  0x01

//C0帧RSU初始化数据
typedef struct tag_RampC0Data{
  BYTE LaneMode;                   //车道模式,3入口4出口
  BYTE WaitTime;                   //最小重读时间 路侧单元自动发送 BST 的间隔，单位 ms
  BYTE TxPower;                    //功率级数
  BYTE PLLChannelID;               //信道号,0或1
  BYTE TransMode;                  //复合交易09H
  BYTE EF04Opt;                    //是否操作OBU-ESAM  DF01/EF04文件00H不操作，01H操作
  UINT EF04Offset;                 //读EF04偏移量，如314字节为0x01 0x3A
  UINT EF04Len;                    //EF04读长度，如91字节为0x00 0x5B
  char Reserve[3];                 //保留字节
} TRampC0Data;

//B0帧RSU状态
typedef struct tag_RampB0Data{
   BYTE  RSUStatus;                //rsu主状态参数；0x00表示正常，否则表示异常；
   BYTE  PSAMNum;                  //Psam卡个数
   char  hexRSUTerminalId[10][13];    //PSAM卡终端机编号，省标PSAM卡号 (12+1)
//   char  hexRSUTerminalId2[13];    //PSAM卡终端机编号，国标PSAM卡号 (12+1)
   BYTE  RSUAlgId;                 //算法标识
   BYTE  RSUManuID;                //RSU厂商代码，16进制表示
   char  hexRSUID[7];              //RSU编号，16进制表示(6+1)
   USHORT  RSUVersion;             //RSU软件版本号，16进制表示
   char  Reserved[5];              //保留字节。
} TRampB0Data;

//B2帧OBU自身信息
typedef struct tag_RampB2Data{
   UINT OBUID;                    //OBU编号
   BYTE ErrorCode;                //执行状态代码，此处取值0x00
   BYTE OBUType;                  //OBU类型01H 为双片式 OBU，02H 为单片式 OBU，03H 为 CPC 卡，其他值保留
   char hexIssuerIdentifier[17];  //发行商代码 16+1
   BYTE ContractType;             //合同类型(V2.5)
   BYTE ContractVersion;          //合同版本号(V2.5)
   char hexSerialNumber[17];      //应用序列号16+1
   char DateofIssue[9];           //启用日期(yyyymmdd)
   char DateofExpire[9];          //过期日期(yyyymmdd)
   BYTE EquitmentStatus;          //设备类型
   BYTE OBUStatus1;               //OBu存在状态
   BYTE OBUStatus2;               //OBu存在状态
} TRampB2Data;


//B5帧,交易结束
typedef struct tag_RampB5Data{
   UINT  OBUID;                           //OBU号
   BYTE  ErrorCode;                       //执行状态代码
   char  hexGBtermid[13];                 //国标PSAM卡终端号(6*2+1)
   char  hexTransTime[15];                //交易时间(yyyymmddhhnnss,7*2+1)
   int   PSAMTransSerial;                 //PSAM卡交易序号
   short ETCTradNo;                       //联机交易序号 CPU卡
   BYTE  TransType;                       //交易类型9为复合消费,1为普通消费,新流程为9
   UINT  CardRestMoney;                   //卡余额，高位在前，低位在后；(扣款后卡片余额)
   UINT  TAC;                             //TAC码
   BYTE  AlgorithmFlag;                   //算法标识(只有V2.5版本才有)
   BYTE  KeyVersion;                      //消费密钥版本号
} TRampB5Data;

//B6帧,CPC
typedef struct tag_RampB6Data{
   UINT CPCMac;
   BYTE  ErrorCode;                      //执行状态代码
   char  SysInfo[26];                    //CPC卡(EF01)系统信息文件)
   USHORT  Status;                       //CPC卡状态
   char   EnExInfo[43];                  //CPC卡(DF01/EF01)入/出口信息文件
   char   PassInfo[101];                 //CPC卡过站信息(DF01/EF02)
   USHORT  FeeLen;                       //CPC卡(DF01/EF04)计费信息文件长度
   char   FeeInfo[512];                  //CPC卡(DF01/EF04)计费信息文件
   TCPCSysInfo CPCSysInfo;
   TCPCEnExInfo CPCEnExInfo;
   TCPCPassInfo CPCPassInfo;
   TCPCFeeInfo CPCFeeInfo;
} TRampB6Data;

//0015卡基本信息文件(43字节)
typedef struct tag_Ramp0015FileData{
   char hexPubOrg[17];                    //福建2301001(8B)
   BYTE CardType;                         //卡类型
   BYTE CardVer;                          //卡版本号
   BYTE hexCardNet[5];                    //卡网络编号(福建为0x2301)
   char hexCardNo[17];                    //卡内部编号(8B)
   char hexStartDate[9];                  //启用日期(4B)
   char hexStopDate[9];                   //停用日期(4B)
   char VehPlate[13];                     //车牌号(12B)
   BYTE UserType;                         //用户类型00-普通用户
   short PlateColor;                      //车牌色
   BYTE VehClass;                         //新格式卡车型
} TRamp0015FileData;

//0019复合文件消费记录(43字节)
typedef struct tag_Ramp0019FileData{
   BYTE CappType;        //复合应用类型，标准取值0xAA
   BYTE RecLen;          //记录长度，标准取值0x29
   BYTE LockFlag;        //应用锁定标记 0x00
   char EnStation_h[5];     //入口站
   BYTE EnLane;          //出入口车道[**共用字段]
   UINT EnTime;          //入口时间(**新版本使用unix时间)
   BYTE VehClass;        //车型
   BYTE Status;      //入出口状态3-入口 4出口
   UINT EnOperator;      //出入口收费员工号(3B)[**共用字段]
   char VehPlate[15];    //出入口抓拍车牌14+1[**共用字段]
   BYTE PlateColor;      //车牌颜色(**新增字段 )
   char ExStation_h[5];     //出口站
   UINT ExTime;          //出口时间(**新版本使用unix时间)
   char ChkPoint[6];     //出口标识站(6B,**新版无用字段)
   USHORT Reserve;       //备用(**新版无用字段)
   UINT FlagID;            //ETC门架编号(3字节**新增)
   UINT FlagPassTime;      //通行门架时间(Unix时间**新增)
   char RoadNetNo[5];     //收费路网编号[**新增]
   BYTE   ShiftID;       //入出口班次[**新增]
   BYTE   Version;       //新旧版本号[**新增,0.旧版本 0x4F表示新版本]
   char   Remark[3];     //收费公路ETC预留[**新增]
   BYTE  AxisCount;
   UINT  TollWeight;
   BYTE  VehicleStatus;
   UINT  TollFee;
} TRamp0019FileData;



//C3帧(只对入口有效)
typedef struct tag_RampC3Data{
  UINT OBUID;
  TRamp0019FileData File0019;     //实际只需要后40字节
  char hexEnTime[15];         //(7B)yyyymmddhhnnss
} TRampC3Data;

//
typedef struct tag_RampEF04ProvFee{
   BYTE ProCode; //省级行政区代码
   UINT Fee;     //金额 3字节
}TRampEF04ProvFee;

//OBU-ESAM EF04
typedef struct tag_RampOBUEF04Data{
   TRamp0019FileData File0019;    //实际1-39字节
   TRamp0015FileData File0015;    //卡基本信息0015文件数据1-20字节
   BYTE VehicleUserType;  //车辆用户类型
   BYTE PassProvCount; //通行省份个数
   UINT TotalFee;       //累计应收金额  3字节
   UINT FactFee;        //累计实收金额  3字节
   UINT SuccessCount;  //累计交易成功数  2字节
   UINT TotalMileage;  //累计计费里程
   BYTE NoCardCount;   //标签无卡累计次数
   char EnCode[7];     //本省入口编码
   UINT LocatFee;       //本省累计交易金额 3字节
   BYTE LocatSuccessCount; //本省累计交易成功数
   UINT LocatFactFee; //本省累计实收金额
   char HexEncry[17];  //加密摘要
   TRampEF04ProvFee provFee[20];
} TRampOBUEF04Data;

//单片式 OBU 入出口信息(DF01/EF02)写入内容
typedef struct tag_RampEnExInfo{
   char RoadNetNo[5];     //收费路网编号[**新增]
   char EnStation_h[5];     //入口站
   BYTE EnLane;          //入口车道[**共用字段]
   UINT EnTime;          //入口时间(**新版本使用unix时间)
   char ExStation_h[5];     //出口站
   BYTE ExLane;          //出口车道[**共用字段]
   UINT ExTime;          //出口时间(**新版本使用unix时间)
   BYTE VehClass;        //车型
   BYTE Status;      //入出口状态3-入口 4出口
   char VehPlate[13];    //出入口抓拍车牌12+1[**共用字段]
   BYTE PlateColor;      //车牌颜色(**新增字段 )
   BYTE  AxisCount;
   int   VehicleLengh;                     //货车长2字节
   int   VehicleWidth;                     //货车宽1字节
   int   VehicleHeight;                    //货车高1字节
   UINT  TollWeight;
   char  Remark[32];    //31预留
} TRampEnExInfo;


//C6帧(只对出口有效)
typedef struct tag_RampC6Data{
  UINT OBUID;
  int ConsumeMoney;          //单位:分
  char hexExTime[15];        //(7B)yyyymmddhhnnss
  TRamp0019FileData File0019;    //43字节
  BYTE OBUTradeType;         //交易类型
  char hexOBUDivFactor[17];  //OBU一级分散因子 16+1  B2帧的issuer
  UINT EF04Offset;           //读EF04偏移量，如314字节为0x01 0x3A
  UINT EF04Len;              //EF04读长度，如91字节为0x00 0x5B
  TRampOBUEF04Data obuEfo4;          //OBU-ESAM EF04 内容
  TRampEnExInfo enExInfo;        //单片式 OBU 入出口信息(DF01/EF02)写入内容
} TRampC6Data;

//C8异常交易指令
typedef struct tag_RampC8Data{
  UINT OBUID;
  UINT UserSerial;  //用户卡交易序号
  BYTE OpMode;      //00H：仅仅获取 TAC；01H：获取 TAC 后并写入 EF04。
  UINT EF04Offset;  //
  UINT EF04Len;     //
  TRampOBUEF04Data obuEfo4;
} TRampC8Data;

typedef struct tag_RampB3Data{
   UINT  OBUID;                            //OBU号
   BYTE  ErrorCode;                        //执行状态代码，此处取值0x00
   char  VehicleLicencePlateNumber[13];    //OBU记载的车牌号(12+1)*
   short VehicleLicencePlateColor;         //车牌颜色*
   BYTE  VehicleClass;                     //车辆类型*
   BYTE  VehicleUserType;                  //车辆用户类型*
   int   VehicleDimensions;                //车辆尺寸，包括长(2个字节)、宽(1个字节)、高(1个字节)
   BYTE  VehicleWheels;                    //车轮数
   BYTE  VehicleAxles;                     //车轴数
   short VehicleWheelBases;                //轴距，二进制表示，长度为2个字节，单位为分米；如0x28，表示轴距为4米。
   UINT  VehicleWeightLimits;              //车辆载重(货车)或座位数(客车)，二进制表示，单位为公斤/座 *
   char  VehicleSpecificInformation[17];   //车辆特征描述*，字符用ASCII编码表示，汉字用GB2312码表示，如"奔驰307"
   char  VehicleEngineNumber[17];          //车辆发动机号
   int   VehicleLengh;                     //货车长
   int   VehicleWidth;                     //货车宽
   int   VehicleHeight;                    //货车高
   UINT  VehicleWeightApproved;            //货车核载质量/准牵引总质量kg
   UINT  VehicleWeightCurb;                //货车整备质量kg
   UINT  VehicleWeightToll;                //货车车辆总质量kg
   BYTE  VehiclePeopleNum;                 //货车核定载人数
   char  VehicleIdentificationNumber[18];  //车辆识别代码
   char  Reserve[21];                      //预留 客车预留20字节，货车预留14字节
   int   VehicleSeat;                      //客车座位数,货车该字段值为0
   TRampOBUEF04Data obuEfo4;                   //OBU-ESAM EF04 内容
   char HexOBUVehInfo[79]; //OBU车辆信息 HEX未解析的数据
   char HexObuEF04[91]; //EF04信息 HEX未解析的数据
} TRampB3Data;

//B4帧,卡和过车信息
typedef struct tag_RampB4Data{
   UINT  OBUID;               //OBU号
   BYTE  ErrorCode;           //执行状态代码，此处取值0x00
   UINT CardRestMoney;        //卡余额，高位在前，低位在后；
   TRamp0015FileData File0015;    //卡基本信息0015文件数据
   TRamp0019FileData File0019;    //复合消费文件0019文件数据
   //short  File000C;         //福建省私有文件000C标志位,分解为下面两个值
   BYTE VehFlag;              //车辆属性(第0位1表示车卡绑定;第1位1表示收费卡;第2位1表示货车)
   BYTE Seats;                //座位数
   TRampEnExInfo enExInfo;        //单片式 OBU 入出口信息(DF01/EF02)写入内容
   char HexFile0015[50];
   char HexFile0019[43];
} TRampB4Data;

//B8帧,返回TAC信息
typedef struct tag_RampB8Data{
   UINT  OBUID;               //OBU号
   BYTE  ErrorCode;           //执行状态代码，此处取值0x00
   UINT  TAC;                 //交易 TAC 码
   UINT  TradeMac;            //交易 MAC 码
   UINT  CardRestMoney;       //卡余额，高位在前，低位在后；(扣款后卡片余额)
} TRampB8Data;

//C9获取EF04拆分指令
typedef struct tag_RampC9Data{
  UINT OBUID;
  BYTE ReadMode;      //00H：仅仅获取 TAC；01H：获取 TAC 后并写入 EF04。
  UINT ReadOffset;  //读 EF04 偏移
  UINT ReadLen;     //读 EF04 长度
  BYTE WriteMode;  //00H：不更新 01H：更新(按 WriteInfo 指定的内容更新) 02H：固定清除(将指定偏移量和长度的 EF04 内容 写 0x00)。
  UINT WriteOffset; //写 Ef04 偏移量
  UINT WriteLen;   //写 Ef04 文件长度
  TRampOBUEF04Data obuEfo4;                   //OBU-ESAM EF04 内容
} TRampC9Data;

//B9帧,返回EF04拆分信息
typedef struct tag_RampB9Data{
   UINT  OBUID;               //OBU号
   BYTE  ErrorCode;           //执行状态代码，此处取值0x00
   UINT  ReadStatus;          //读取拆分信息状态：0 为正常，非 0 异常，不操作填 0
   UINT  WriteStatus;         //更新信息状态：0 为正常，非 0 异常，不操作填 0
   UINT  ReadLen;             //Ef04 文件读取长度
   TRampEF04ProvFee provFee[20];  //OBU-ESAM EF04 中各省内容
   BYTE  ProvCount;           //拆分省份数
} TRampB9Data;


//PSAM卡授权初始化信息
typedef struct tag_RampPsamInfo
{
   BYTE PSAMChannel;
   BYTE HexTerminalId[13];
   BYTE HexSerialNo[21];
   BYTE PSAMVersion;
   BYTE HexDivFactor[9];
   BYTE HexRandom[17];
} TRampPsamInfo;

//PSAM卡授权初始化返回(BD),目前只用PsamInfo1
typedef struct tag_RampBAData
{
   BYTE ErrorCode;
   BYTE PsamNum;
   TRampPsamInfo  PsamInfo[10];
} TRampBAData;

//PSAM卡授权结果返回
typedef struct tag_RampBBData
{
   BYTE ErrorCode;
   BYTE PsamNum;
   BYTE AuthResult[10][4]; //1字节通道号+1字节状态+2字节sw1sw2
//   BYTE AuthResult2[4];
} TRampBBData;

//地感信息
typedef struct tag_RampD0Data{
  BYTE FrameType;
  UINT OBUID;
  UINT X;
  UINT Y;
} TRampD0Data;

//PSAM卡授权初始化指令(CA,不需要定义)
//PSAM卡授权指令(CB,目前只用单卡，所以只有APDU1,其它为0)

typedef struct tag_RampAPDUInfo
{
    BYTE Channel;
    BYTE AuthFlag;
    char APDU[13];
}TRampAPDUInfo;

typedef struct tag_RampCBData
{
   BYTE PsamNum;
   TRampAPDUInfo APDUInfo[10];
} TRampCBData;



//心跳帧
typedef struct tag_RampB1Data
{
   BYTE PsamNum;
   BYTE PSAMChannel[10];
   BYTE PSAMLock[10];
   BYTE PSAMAuth[10];
   BYTE AntenaNum;
   BYTE AntennaStatus[20];
} TRampB1Data;

//设置 BeaconID 信息帧结果返回
typedef struct tag_RampBEData
{
   BYTE Status;
   UINT BeaconID;
} TRampBEData;


//RSU控制接口,由本模块实现
class IRampRsuCtrl32:public QObject
{
    Q_OBJECT
public:
   virtual int  OpenRsuCtrlEx(BYTE nSelRsu,char* sIp,int nPort)=0;
   virtual void CloseRsuCtrl()=0;                //关闭RSU控制
   virtual void RsuConfig(TRampC0Data data)=0;       //天线初始化设置
   virtual int  VDStatusNotify(UINT nStatus)=0;  //地感状态变化指令(相控阵天线专用)
   virtual int  StartTrade()=0;                  //打开天线(开始交易)
   virtual int  StopTrade()=0;                   //关闭天线(结束交易)
   virtual int  SendC1Cmd(UINT obuid,BYTE rsctl, BYTE type)=0;
   virtual int  SendC2Cmd(UINT obuid,BYTE rsctl,BYTE stopType)=0;
   virtual int  SendC3Cmd(TRampC3Data &data,BYTE rsctl)=0;
   virtual int  SendC6Cmd(TRampC6Data &data,BYTE rsctl)=0;
   virtual int  SendC8Cmd(TRampC8Data &data,BYTE rsctl)=0;
   virtual int  SendC9Cmd(TRampC9Data &data,BYTE rsctl)=0;
   virtual int  SendCACmd(BYTE rsctl)=0;
   virtual int  SendCBCmd(TRampCBData &data,BYTE rsctl)=0;
   virtual int  SendCECmd(BYTE SetMode,UINT Interval)=0; //发送设置BeaconID   00. 停止持续改变 BeaconID 01. 间隔一定时间连续改变 BeaconID 02. 临时改变一次 BeaconID ; Interval 改变 BeaconID 的间隔时间，
   virtual int  SendCFCmd(BYTE rsctl,BYTE status)=0;    //发送心跳应答指令
   virtual void SetLogger(const char *logFile, int logLevel)=0;   //输出日志配置
public:
Q_SIGNALS:
   void data_from_rsu(QByteArray pHead,QByteArray pFrame);
};

extern "C" Q_DECL_EXPORT IRampRsuCtrl32* GetRampRsuCtrl32();        //根据RSU类型*

#endif
