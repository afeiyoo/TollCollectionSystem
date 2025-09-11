/**
*    @file:    tlaneconfig.h
*    @brief:   存放有关配置相关的结构体
*    @author:  SheMinglei
*    @date:    2025-08-11
*/
//===================================================

#ifndef TLANECONFIG_H
#define TLANECONFIG_H

#include <QString>
#include <QMap>

//车道运行环境（基础类）
struct TLaneEnv{
    int ServiceMode;     //后端服务类型:0.单机版 1.网络版
    QString ServiceIP;   //后端服务IP
    int ServicePort;     //后端服务端口
    int VersionCheckTime; //软件版本检测时间间隔
    QString Keyboard;  //键盘映射表

    int LaneMode;       //车道模式:1.ETC入口 2.ETC出口 3.ETC/人工入口 4.ETC/人工出口 5.ETC/自助入口 6.ETC/自助出口 7.站前预交易入口 8.站前预交易出口 9.手持机
    QString NetID;       //默认3501
    QString StationID;   //如6701
    int LaneID;          //车道号
    QString VirEnstation; //虚拟入口站
    QString LaneIP;      //车道IP
    QString FlagID;      //门架编号
    QString CNLaneID;     //注册车道编号
    int HeartBeatTime;  //心跳上传时间间隔
    QString AppVersion;  //软件版本，直接从软件读取
    bool DoubleLane;     //复式票亭
};

//串口通用
struct TSerialPortBased{
    QString ComPort; //串口号
    int BaudRate = 9600;   //波特率
};

//并口通用
struct TParallelPortBased{
    int BaseAddr;  //基地址
};

//IP化设备通用
struct TNetworkBased{
    QString IP;  //IP地址
    int Port;   //端口
};

//读卡器
struct TCardReaderCfg: TSerialPortBased,TNetworkBased {
    int Type;  //0 串口读卡器 1 卡代理(考虑以后自己部署读卡服务)
    int SlotNo;        //卡槽
    QString ApiName;    //调用厂商动态库名称
    int WorkNo;       //读卡器工位
    int index = 0;        //0 主票亭 1 另一个票亭 默认0
};

//打印机
struct TPrinterCfg: TSerialPortBased, TParallelPortBased {
    int Type;   //打印机类型 1 ds并 2 star 并  3 ds串  4 star串
};

//自助发卡机，配置为空自助入口时使用
struct TCardRobotCfg: TSerialPortBased, TNetworkBased {
    int Type;    //自助机类型 0 TCP  1 串口
    int Option;     //是否1分4
    int PalletDelayTime;     //托盘延时伸出时间,单位毫秒
    int Index;      //0 当前票亭 1 另一个票亭
};

//自助缴费终端
struct TPaymentRobotCfg: TNetworkBased {
    int PalletDelayTime;  //拖盘延时伸出时间，单位毫秒
    int Index;      //0 当前票亭 1 另一个票亭
};

//车道控制器
struct TLaneCtrlCfg{
    int BaseAddr; //基地址
    int CeilingLamp;  //顶棚灯
    int PassingLamp;   //通行灯
    int Warning;   //声光报警
    QString  RailingUp1;  //升杆1
    QString  RailingDown1; //落杆1
    QString  RailingUp2; //升杆2
    QString  RailingDown2; //落杆2
    QString  BeginCoil;  //抓拍线圈
    QString  EndCoil;    //落杆线圈
    QString  CoilNum;    //线圈数
    QString  Coil1;
    QString  Coil2;
    QString  Coil3;
    QString  Coil4;
    QString  Coil5;
    QString  Coil6;
    QString  Which;
    QString  Type;
};

//一体化栏杆机/费显
struct TFeeScrCfg: TSerialPortBased, TNetworkBased{
    int Type; //费显类型 1 新大陆 2 英沙 3 华工 4 化工恒富威 5 MTC紫凯 6 ETC紫凯 7 艾特斯 8 MGSKJ 9 MGSKJ2 10 HZH 11 HZH 4X8 15 一体化栏杆机
    QString SleepMillisecond; //发送间隔 单位毫秒
    int Index;
};

//字符叠加器
struct TOverLapCfg: TSerialPortBased{
    int Type;   //类型
};

struct TWeightCfg: TSerialPortBased{
    QString Addr; //称重地址
    QString IWP;
};

struct TInfoBoardCfg: TNetworkBased{
    int Type;
    int Index;
};

struct TSmartLaneControllerCfg: TNetworkBased{

};

struct TRsu: TNetworkBased{
    int Index; //rsu类型，混合出/入口最多2个 0 近天线 1 远天线
};

struct TDevConfig{
    // using Index = int;
    QMap<int,TCardReaderCfg> CardReaderCfgMap; //读卡器 index为前后票亭
    QMap<int, TRsu> RsuCfgMap; //天线配置 index为天线编号，其中0为近天线 1为远天线
    QMap<int, TInfoBoardCfg> InfoBoardCfgMap; //情报板 index为情报板编号
    QMap<int,TPaymentRobotCfg> PaymentRobotCfgMap; //缴费机  index为前后票亭
    QMap<int,TCardRobotCfg> CardRobotCfgMap;    //自助机   index为前后票亭
    QMap<int,TFeeScrCfg> FeeScrCfg;          //费显配置 index为前后票亭

    TPrinterCfg PrinterCfg;  //打印机
    TWeightCfg WeightCfg;   //称重
    TOverLapCfg OverLapCfg; //字符叠加
    TSmartLaneControllerCfg SmartLaneControllerCfg; //车道智能网关
    TLaneCtrlCfg LaneCtrlCfg; //车控器
};

#endif // TLANECONFIG_H
