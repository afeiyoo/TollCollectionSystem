#pragma once

#include "stdafx.h"

#include <QDateTime>
#include <QFlags>
#include <QMap>
#include <QObject>

//==============================================================================
// 配置结构体定义
//==============================================================================

// 数据库配置
struct ST_DatabaseCfg
{
    uint type; // 数据库类型
    QString ip;
    uint port;
    QString dbName;
    QString user;
    QString password;
};

// 后端服务配置
struct ST_RpcServerCfg
{
    uint mode;
    uint socketType; // 连接类型 0-tcp, 1-websocket
    uint port;
};

// 日志配置
struct ST_LogCfg
{
    QString format;
    uint filesLimit;
};

// 车道系统基础配置
struct ST_SystemCfg
{
    int serviceMode;
    int serviceSocketType;
    QString serviceIP;
    int servicePort;
    int versionCheckTime;
    int keyboardType;
};

// 车道系统业务配置
struct ST_BusinessCfg
{
    // 车道模式:1.ETC入口 2.ETC出口 3.ETC/人工入口 4.ETC/人工出口 5.ETC/自助入口 6.ETC/自助出口 7.站前预交易入口 8.站前预交易出口 9.手持机
    int laneMode;
    QString netID;
    QString stationID;
    QString stationName;
    int laneID;
    QString virEnstation; //虚拟入口站
    QString laneIP;       //车道IP
    QString gantryID;     //门架编号
    QString CNLaneID;     //注册车道编号
    int heartBeatTime;    //心跳上传时间间隔
    bool doubleLane;      //复式票亭
};

// 读卡器配置
struct ST_CardReaderCfg
{
    int type;        //0 串口读卡器 1 卡代理(考虑以后自己部署读卡服务)
    int slotNo;      //卡槽
    QString apiName; //调用厂商动态库名称
    int workNo;      //读卡器工位
    int index;       //0 主票亭 1 另一个票亭 默认0

    QString comPort; //串口号
    int baudRate;    //波特率

    QString ip; //IP地址
    int port;   //端口
};

// 打印机配置
struct ST_PrinterCfg
{
    int type; //打印机类型 1 ds并 2 star 并  3 ds串  4 star串

    QString comPort; //串口号
    int baudRate;    //波特率

    int baseAddr; //基地址
};

// 自助发卡机配置，配置为空自助入口时使用
struct ST_CardRobotCfg
{
    ST_CardReaderCfg reader[4]; //读卡器
    int type;                   //自助机类型 0 TCP  1 串口
    int option;                 //是否1分4
    int palletDelayTime;        //托盘延时伸出时间,单位毫秒
    int index;                  //0 当前票亭 1 另一个票亭

    QString comPort; //串口号
    int baudRate;    //波特率

    QString ip; //IP地址
    int port;   //端口
};

// 自助缴费终端配置
struct ST_PaymentRobotCfg
{
    ST_CardReaderCfg reader[4]; //读卡器
    int palletDelayTime;        //拖盘延时伸出时间，单位毫秒
    int index;                  //0 当前票亭 1 另一个票亭

    QString ip; //IP地址
    int port;   //端口
};

// 抓拍配置
struct ST_CapCfg
{
    int type; //抓拍类型

    QString ip; //IP地址
    int port;   //端口
};

//车道控制器
struct ST_LaneCtrlCfg
{
    int baseAddr;         //基地址
    int ceilingLamp;      //顶棚灯
    int passingLamp;      //通行灯
    int warning;          //声光报警
    QString railingUp1;   //升杆1
    QString railingDown1; //落杆1
    QString railingUp2;   //升杆2
    QString railingDown2; //落杆2
    QString beginCoil;    //抓拍线圈
    QString endCoil;      //落杆线圈
    QString coilNum;      //线圈数
    QString coil1;
    QString coil2;
    QString coil3;
    QString coil4;
    QString coil5;
    QString coil6;
    QString which;
    QString type;
};

//一体化栏杆机/费显
struct ST_FeeScrCfg
{
    int type; //费显类型 1 新大陆 2 英沙 3 华工 4 化工恒富威 5 MTC紫凯 6 ETC紫凯 7 艾特斯 8 MGSKJ 9 MGSKJ2 10 HZH 11 HZH 4X8 15 一体化栏杆机
    QString sleepMillisecond; //发送间隔 单位毫秒
    int index;

    QString comPort; //串口号
    int baudRate;    //波特率

    QString ip; //IP地址
    int port;   //端口
};

//字符叠加器
struct ST_OverLapCfg
{
    int type; //类型

    QString comPort; //串口号
    int baudRate;    //波特率
};

// 称重配置
struct ST_WeightCfg
{
    QString addr; //称重地址
    QString IWP;

    QString comPort; //串口号
    int baudRate;    //波特率
};

// 情报板配置
struct ST_InfoBoardCfg
{
    int type;
    int index;

    QString ip; //IP地址
    int port;   //端口
};

// 智能车控器配置
struct ST_SmartLaneControllerCfg
{
    QString ip; //IP地址
    int port;   //端口
};

// RSU配置
struct ST_RsuCfg
{
    int Index;  //rsu类型，混合出/入口最多2个 0 近天线 1 远天线
    QString ip; //IP地址
    int port;   //端口
};

// 收费节点
struct ST_TollNode
{
    int NodeID;
    ST_PaymentRobotCfg PaymentRobot;
    ST_CardRobotCfg CardRobot;
    ST_FeeScrCfg FeeScr;
    ST_CardReaderCfg DeskReader;
    ST_PrinterCfg PrinterCfg;
    ST_CapCfg CapCfg;
};
struct ST_DevConfig
{
    QMap<int, ST_TollNode> TollNodeCfgMap;
    QMap<int, ST_RsuCfg> RsuCfgMap;             //天线配置 index为天线编号，其中0为近天线 1为远天线
    QMap<int, ST_InfoBoardCfg> InfoBoardCfgMap; //情报板 index为情报板编号

    ST_WeightCfg WeightCfg;                           //称重
    ST_OverLapCfg OverLapCfg;                         //字符叠加
    ST_SmartLaneControllerCfg SmartLaneControllerCfg; //车道智能网关
    ST_LaneCtrlCfg LaneCtrlCfg;                       //车控器
};

//==============================================================================
// 交易相关对象
//==============================================================================
namespace Trade {
class OneTrade : public QObject
{
    Q_OBJECT
public:
    QString tradeID;       //STATIONID + LANEID + TIME(YYYYMMDDhhmmss) + SEQ
    int seqNum;            //数据序列号
    QString passID;        //通行ID
    int tollRateVer;       //费率版本号
    int cardNetID;         //卡网络编号，识别省份
    int cardType;          //卡类型 参照EM_CardType
    int cardBizType;       //卡业务类型 参照EM_CardBizType
    QString cardID;        //卡号，"0"为无卡
    int enNetID;           //入口路网编号
    QString enStation;     //入口收费站ID
    QString enStationName; //入口收费站名称
    QDateTime enTime;      //入口时间 YYYY-MM-DD hh:mm:ss
    QString enOperatorID;  //入口操作人员ID
    QString enOperator;    //入口操作人员名字
    int enVehClass;        //入口车型
    QString enVehPlate;    //入口车牌
    int exNetID;           //出口路网编号，默认3501
    QString exStation;     //出口收费站ID
    QString exStationName; //出口收费站名称
    int exLane;            //出口车道
    QDateTime exTime;      //出口时间
    QString exOperatorID;  //出口操作人员ID
    QString exOperator;    //出口操作人员名称
    int exShiftID;         //出口班次ID
    QDateTime exShiftDate; //出口班次时间 YYYY-MM-DD
    int exVehClass;        //出口车型
    int exVehStatus;       //出口车种 参考EM_VehStatus
    QString exVehPlate;    //出口车牌
    QString obuPlate;      //OBU车牌
    QString cardPlate;
    int vehSpeed;
    int axisGroups;
    int axises;
    int origAxisType;
    int axisType;
    QString axisWeight;
    int origWeight;
    int totalWeight;
    int tollWeight;
    int axisLimit;
    double exceedRate;
    int weightClass;
    int tollType;
    double shouldPay;
    int discountType;
    double discount;
    double factPay;
    int merRebateType;
    double rebateRatio;
    double merShouldPay;
    double rebatedPay;
    double specialPay;
    int ticketNum;
    int payType;
    int dealStatus;
    int deviceStatus;
    int recordType;
    int changeType;
    double changedFee;
    int peccType;
    int chkPoint;
    int spendTime;
    int isValid;
    int scrapTickets;
    int obuID;
    QString termCode;
    int cardTradeNo;
    int termTradeNo;
    uint restMoney;
    double tac;
    QString obuSerialNum;
    int obuStatus;
    int provinceID;
    int verifycode;
    double tsVehicle;
    QString reserve;
    int dataMonth;
    int enLane;
    uint postBalance;
    int algorithm;
    QString cpcCardID;
    int cpcElec;
    double opFactPay;
    int etcCardVer;
    int obuContractVer;
    QString reserve2;
    QString appVersion;
    int recordType2;
    int tollSupport;
    int enAxisCount;
    int enTotalWeight;
    int userType;
    int obuTotalTradeSuccNum;
    int provinceNum;
    int noCardTimes;
    int obuPayFeeSum;
    int obuDiscountFeeSum;
    int obuMileage;
    QString provEnHex;
    int obuProvFeeSum;
    int obuProvFactSum;
    int obuProvTradeSuccNum;
    QString ffObuCardNo;
    int cardFeeSum;
    int shortFee;
    int shortFeeMileage;
    double feeRate;
    int exitFeeType;
    int transPayType;
    QString splitProvince;
    QString specialType;
    QString merchant;
    QString cnLaneID;
    QString enRoadNet;
    int openFlag;
    int localFeeMileage;
    int obuVehClass;
    QString billCode;
    int ticketYear;
    int opeateSign;
    int transFee; //卡面扣费金额，入口承载门架时填写
    QDateTime enShiftDate;
    int enShiftID;
    int enVehStatus;

    OneTrade()
        : tradeID("")
        , seqNum(0)
        , passID("")
        , tollRateVer(0)
        , cardNetID(0)
        , cardType(0)
        , cardBizType(0)
        , cardID("")
        , enNetID(0)
        , enStation("")
        , enStationName("")
        , enTime(QDateTime())
        , enOperatorID("")
        , enVehClass(0)
        , enVehPlate("")
        , exNetID(3501)
        , exStation("")
        , exStationName("")
        , exLane(0)
        , exTime(QDateTime())
        , exOperatorID("")
        , exOperator("")
        , exShiftID(0)
        , exShiftDate(QDateTime())
        , exVehClass(0)
        , exVehStatus(0)
        , exVehPlate("")
        , obuPlate("")
        , cardPlate("")
        , vehSpeed(0)
        , axisGroups(0)
        , axises(0)
        , origAxisType(0)
        , axisType(0)
        , axisWeight("")
        , origWeight(0)
        , totalWeight(0)
        , tollWeight(0)
        , axisLimit(0)
        , exceedRate(0.0)
        , weightClass(0)
        , tollType(0)
        , shouldPay(0.0)
        , discountType(0)
        , discount(0.0)
        , factPay(0.0)
        , merRebateType(0)
        , rebateRatio(0.0)
        , merShouldPay(0.0)
        , rebatedPay(0.0)
        , specialPay(0.0)
        , ticketNum(0)
        , payType(0)
        , dealStatus(0)
        , deviceStatus(0)
        , recordType(0)
        , changeType(0)
        , changedFee(0.0)
        , peccType(0)
        , chkPoint(0)
        , spendTime(0)
        , isValid(0)
        , scrapTickets(0)
        , obuID(0)
        , termCode("")
        , cardTradeNo(0)
        , termTradeNo(0)
        , restMoney(0u)
        , tac(0.0)
        , obuSerialNum("")
        , obuStatus(0)
        , provinceID(0)
        , verifycode(0)
        , tsVehicle(0.0)
        , reserve("")
        , dataMonth(0)
        , enLane(0)
        , postBalance(0u)
        , algorithm(0)
        , cpcCardID("")
        , cpcElec(0)
        , opFactPay(0.0)
        , etcCardVer(0)
        , obuContractVer(0)
        , reserve2("")
        , appVersion("")
        , recordType2(0)
        , tollSupport(0)
        , enAxisCount(0)
        , enTotalWeight(0)
        , userType(0)
        , obuTotalTradeSuccNum(0)
        , provinceNum(0)
        , noCardTimes(0)
        , obuPayFeeSum(0)
        , obuDiscountFeeSum(0)
        , obuMileage(0)
        , provEnHex("")
        , obuProvFeeSum(0)
        , obuProvFactSum(0)
        , obuProvTradeSuccNum(0)
        , ffObuCardNo("")
        , cardFeeSum(0)
        , shortFee(0)
        , shortFeeMileage(0)
        , feeRate(0.0)
        , exitFeeType(0)
        , transPayType(0)
        , splitProvince("")
        , specialType("")
        , merchant("")
        , cnLaneID("")
        , enRoadNet("")
        , openFlag(0)
        , localFeeMileage(0)
        , obuVehClass(0)
        , billCode("")
        , ticketYear(0)
        , opeateSign(0)
        , transFee(0)
    {}
};

class OutTrade : public OneTrade
{
    Q_OBJECT
public:
    // 统一编写的Q_PROPERTY
    Q_PROPERTY(QString TradeID MEMBER tradeID)             //STATIONID + LANEID + TIME(YYYYMMDDhhmmss) + SEQ
    Q_PROPERTY(int SeqNum MEMBER seqNum)                   //数据序列号
    Q_PROPERTY(QString PassID MEMBER passID)               //通行ID
    Q_PROPERTY(int TollRateVer MEMBER tollRateVer)         //费率版本号
    Q_PROPERTY(int CardNetID MEMBER cardNetID)             //卡网络编号，识别省份
    Q_PROPERTY(int CardType MEMBER cardType)               //卡类型 参照EM_CardType
    Q_PROPERTY(int CardBizType MEMBER cardBizType)         //卡业务类型 参照EM_CardBizType
    Q_PROPERTY(QString CardID MEMBER cardID)               //卡号，"0"为无卡
    Q_PROPERTY(int EnNetID MEMBER enNetID)                 //入口路网编号
    Q_PROPERTY(QString EnStation MEMBER enStation)         //入口收费站ID
    Q_PROPERTY(QString EnStationName MEMBER enStationName) //入口收费站名称
    Q_PROPERTY(QDateTime EnTime MEMBER enTime)             //入口时间 YYYY-MM-DD hh:mm:ss
    Q_PROPERTY(QString EnOperatorID MEMBER enOperatorID)   //入口操作人员ID
    Q_PROPERTY(int EnVehClass MEMBER enVehClass)           //入口车型
    Q_PROPERTY(QString EnVehPlate MEMBER enVehPlate)       //入口车牌
    Q_PROPERTY(int ExNetID MEMBER exNetID)                 //出口路网编号，默认3501
    Q_PROPERTY(QString ExStation MEMBER exStation)         //出口收费站ID
    Q_PROPERTY(QString ExStationName MEMBER exStationName) //出口收费站名称
    Q_PROPERTY(int ExLane MEMBER exLane)                   //出口车道
    Q_PROPERTY(QDateTime ExTime MEMBER exTime)             //出口时间
    Q_PROPERTY(QString ExOperatorID MEMBER exOperatorID)   //出口操作人员ID
    Q_PROPERTY(QString ExOperator MEMBER exOperator)       //出口操作人员名称
    Q_PROPERTY(int ExShiftID MEMBER exShiftID)             //出口班次ID
    Q_PROPERTY(QDateTime ExShiftDate MEMBER exShiftDate)   //出口班次时间 YYYY-MM-DD
    Q_PROPERTY(int ExVehClass MEMBER exVehClass)           //出口车型
    Q_PROPERTY(int ExVehStatus MEMBER exVehStatus)         //出口车种 参考EM_VehStatus
    Q_PROPERTY(QString ExVehPlate MEMBER exVehPlate)       //出口车牌
    Q_PROPERTY(QString OBUPlate MEMBER obuPlate)           //OBU车牌
    Q_PROPERTY(QString CardPlate MEMBER cardPlate)
    Q_PROPERTY(int VehSpeed MEMBER vehSpeed)
    Q_PROPERTY(int AxisGroups MEMBER axisGroups)
    Q_PROPERTY(int Axises MEMBER axises)
    Q_PROPERTY(int OrigAxisType MEMBER origAxisType)
    Q_PROPERTY(int AxisType MEMBER axisType)
    Q_PROPERTY(QString AxisWeight MEMBER axisWeight)
    Q_PROPERTY(int OrigWeight MEMBER origWeight)
    Q_PROPERTY(int TotalWeight MEMBER totalWeight)
    Q_PROPERTY(int TollWeight MEMBER tollWeight)
    Q_PROPERTY(int AxisLimit MEMBER axisLimit)
    Q_PROPERTY(double ExceedRate MEMBER exceedRate)
    Q_PROPERTY(int WeightClass MEMBER weightClass)
    Q_PROPERTY(int TollType MEMBER tollType)
    Q_PROPERTY(double ShouldPay MEMBER shouldPay)
    Q_PROPERTY(int DiscountType MEMBER discountType)
    Q_PROPERTY(double Discount MEMBER discount)
    Q_PROPERTY(double FactPay MEMBER factPay)
    Q_PROPERTY(int MerRebateType MEMBER merRebateType)
    Q_PROPERTY(double RebateRatio MEMBER rebateRatio)
    Q_PROPERTY(double MerShouldPay MEMBER merShouldPay)
    Q_PROPERTY(double RrebatedPay MEMBER rebatedPay)
    Q_PROPERTY(double SpecialPay MEMBER specialPay)
    Q_PROPERTY(int TicketNum MEMBER ticketNum)
    Q_PROPERTY(int PayType MEMBER payType)
    Q_PROPERTY(int DealStatus MEMBER dealStatus)
    Q_PROPERTY(int DeviceStatus MEMBER deviceStatus)
    Q_PROPERTY(int RecordType MEMBER recordType)
    Q_PROPERTY(int ChangeType MEMBER changeType)
    Q_PROPERTY(double ChangedFee MEMBER changedFee)
    Q_PROPERTY(int PeccType MEMBER peccType)
    Q_PROPERTY(int ChkPoint MEMBER chkPoint)
    Q_PROPERTY(int SpendTime MEMBER spendTime)
    Q_PROPERTY(int IsValid MEMBER isValid)
    Q_PROPERTY(int ScrapTickets MEMBER scrapTickets)
    Q_PROPERTY(int OBUID MEMBER obuID)
    Q_PROPERTY(QString TermCode MEMBER termCode)
    Q_PROPERTY(int CardTradeNo MEMBER cardTradeNo)
    Q_PROPERTY(int TermTradeNo MEMBER termTradeNo)
    Q_PROPERTY(uint RestMoney MEMBER restMoney)
    Q_PROPERTY(double TAC MEMBER tac)
    Q_PROPERTY(QString OBUSerialNum MEMBER obuSerialNum)
    Q_PROPERTY(int OBUStatus MEMBER obuStatus)
    Q_PROPERTY(int ProvinceID MEMBER provinceID)
    Q_PROPERTY(int Verifycode MEMBER verifycode)
    Q_PROPERTY(double TSVehicle MEMBER tsVehicle)
    Q_PROPERTY(QString Reserve MEMBER reserve)
    Q_PROPERTY(int DataMonth MEMBER dataMonth)
    Q_PROPERTY(int EnLane MEMBER enLane)
    Q_PROPERTY(uint PostBalance MEMBER postBalance)
    Q_PROPERTY(int Algorithm MEMBER algorithm)
    Q_PROPERTY(QString CPCCardID MEMBER cpcCardID)
    Q_PROPERTY(int CPCElec MEMBER cpcElec)
    Q_PROPERTY(double OpFactPay MEMBER opFactPay)
    Q_PROPERTY(int ETCCardVer MEMBER etcCardVer)
    Q_PROPERTY(int OBUContractVer MEMBER obuContractVer)
    Q_PROPERTY(QString Reserve2 MEMBER reserve2)
    Q_PROPERTY(QString AppVersion MEMBER appVersion)
    Q_PROPERTY(int RecordType2 MEMBER recordType2)
    Q_PROPERTY(int TollSupport MEMBER tollSupport)
    Q_PROPERTY(int EnAxisCount MEMBER enAxisCount)
    Q_PROPERTY(int EnTotalWeight MEMBER enTotalWeight)
    Q_PROPERTY(int UserType MEMBER userType)
    Q_PROPERTY(int OBUTotalTradeSuccNum MEMBER obuTotalTradeSuccNum)
    Q_PROPERTY(int ProvinceNum MEMBER provinceNum)
    Q_PROPERTY(int NoCardTimes MEMBER noCardTimes)
    Q_PROPERTY(int OBUPayFeeSum MEMBER obuPayFeeSum)
    Q_PROPERTY(int OBUDiscountFeeSum MEMBER obuDiscountFeeSum)
    Q_PROPERTY(int OBUMileage MEMBER obuMileage)
    Q_PROPERTY(QString ProvEnHex MEMBER provEnHex)
    Q_PROPERTY(int OBUProvFeeSum MEMBER obuProvFeeSum)
    Q_PROPERTY(int OBUProvFactSum MEMBER obuProvFactSum)
    Q_PROPERTY(int OBUProvTradeSuccNum MEMBER obuProvTradeSuccNum)
    Q_PROPERTY(QString FFObuCardNo MEMBER ffObuCardNo)
    Q_PROPERTY(int CardFeeSum MEMBER cardFeeSum)
    Q_PROPERTY(int ShortFee MEMBER shortFee)
    Q_PROPERTY(int ShortFeeMileage MEMBER shortFeeMileage)
    Q_PROPERTY(double FeeRate MEMBER feeRate)
    Q_PROPERTY(int ExitFeeType MEMBER exitFeeType)
    Q_PROPERTY(int TransPayType MEMBER transPayType)
    Q_PROPERTY(QString SplitProvince MEMBER splitProvince)
    Q_PROPERTY(QString SpecialType MEMBER specialType)
    Q_PROPERTY(QString Merchant MEMBER merchant)
    Q_PROPERTY(QString CNLaneID MEMBER cnLaneID)
    Q_PROPERTY(QString EnRoadNet MEMBER enRoadNet)
    Q_PROPERTY(int OpenFlag MEMBER openFlag)
    Q_PROPERTY(int LocalFeeMileage MEMBER localFeeMileage)
    Q_PROPERTY(int OBUVehClass MEMBER obuVehClass)
    Q_PROPERTY(QString BillCode MEMBER billCode)
    Q_PROPERTY(int TicketYear MEMBER ticketYear)
    Q_PROPERTY(int OpeateSign MEMBER opeateSign)
};

class InTrade : public OneTrade
{
    Q_OBJECT
public:
    // 对应的Q_PROPERTY（与变量名严格匹配）
    Q_PROPERTY(QString TradeID MEMBER tradeID)
    Q_PROPERTY(int SeqNum MEMBER seqNum)
    Q_PROPERTY(int CardNetID MEMBER cardNetID)
    Q_PROPERTY(int CardType MEMBER cardType)
    Q_PROPERTY(int CardBizType MEMBER cardBizType)
    Q_PROPERTY(QString CardID MEMBER cardID)
    Q_PROPERTY(int EnNetID MEMBER enNetID)
    Q_PROPERTY(QString EnStation MEMBER enStation)
    Q_PROPERTY(QString StationName MEMBER enStationName)
    Q_PROPERTY(int EnLane MEMBER enLane)
    Q_PROPERTY(QDateTime EnTime MEMBER enTime)
    Q_PROPERTY(QString EnOperatorID MEMBER enOperatorID)
    Q_PROPERTY(QString EnOperator MEMBER enOperator)
    Q_PROPERTY(int EnShiftID MEMBER enShiftID)
    Q_PROPERTY(QDateTime EnShiftDate MEMBER enShiftDate)
    Q_PROPERTY(int EnVehClass MEMBER enVehClass)
    Q_PROPERTY(int EnVehStatus MEMBER enVehStatus)
    Q_PROPERTY(QString EnVehPlate MEMBER enVehPlate)
    Q_PROPERTY(QString OBUPlate MEMBER obuPlate)
    Q_PROPERTY(QString CardPlate MEMBER cardPlate)
    Q_PROPERTY(int PeccType MEMBER peccType)
    Q_PROPERTY(int DealStatus MEMBER dealStatus)
    Q_PROPERTY(int DeviceStatus MEMBER deviceStatus)
    Q_PROPERTY(int RecordType MEMBER recordType)
    Q_PROPERTY(int IsValid MEMBER isValid)
    Q_PROPERTY(int ScrapTickets MEMBER scrapTickets)
    Q_PROPERTY(int OBUID MEMBER obuID)
    Q_PROPERTY(QString TermCode MEMBER termCode)
    Q_PROPERTY(int CardTradeNo MEMBER cardTradeNo)
    Q_PROPERTY(int TermTradeNo MEMBER termTradeNo)
    Q_PROPERTY(double RestMoney MEMBER restMoney)
    Q_PROPERTY(double TAC MEMBER tac)
    Q_PROPERTY(QString OBUSerialNum MEMBER obuSerialNum)
    Q_PROPERTY(int OBUStatus MEMBER obuStatus)
    Q_PROPERTY(int SpendTime MEMBER spendTime)
    Q_PROPERTY(int VehSpeed MEMBER vehSpeed)
    Q_PROPERTY(int ProvinceID MEMBER provinceID)
    Q_PROPERTY(int Verifycode MEMBER verifycode)
    Q_PROPERTY(int DataMonth MEMBER dataMonth)
    Q_PROPERTY(QString Reserve MEMBER reserve)
    Q_PROPERTY(int Algorithm MEMBER algorithm)
    Q_PROPERTY(int EtcCardVer MEMBER etcCardVer)
    Q_PROPERTY(int OBUContractVer MEMBER obuContractVer)
    Q_PROPERTY(QString AppVersion MEMBER appVersion)
    Q_PROPERTY(int AxisCount MEMBER enAxisCount)
    Q_PROPERTY(int TotalWeight MEMBER totalWeight)
    Q_PROPERTY(int UserType MEMBER userType)
    Q_PROPERTY(int TransFee MEMBER transFee)
    Q_PROPERTY(QString PassID MEMBER passID)
    Q_PROPERTY(QString SpecialType MEMBER specialType)
    Q_PROPERTY(QString CNLaneID MEMBER cnLaneID)
    Q_PROPERTY(int AxisLimit MEMBER axisLimit)
    Q_PROPERTY(int OBUVehClass MEMBER obuVehClass)
};
} // namespace Trade

//==============================================================================
// 计费相关对象
//==============================================================================
namespace CalObj {
//兜底OBJ
class ShortFee : public QObject
{
    Q_OBJECT
public:
    int lpFeeMil;   //本省兜底里程
    int lpFee;      //本省兜底费用
    int lpEtcFee;   //本省ETC费用
    int lpFee17;    //本省超6轴加收
    int lpEtcFee17; //本省ETC超6轴加收
    int opFee;      //外省兜底费用
    int opEtcFee;   //外省ETC费用
    int opFeeMil;   //外省兜底里程
    int proNum;     //兜底跨省通行个数
    bool isValid;   //是否成功获取兜底费率

    Q_PROPERTY(int lpFee MEMBER lpFee)
    Q_PROPERTY(int lpEtcFee MEMBER lpEtcFee)
    Q_PROPERTY(int lpFee17 MEMBER lpFee17)
    Q_PROPERTY(int lpEtcFee17 MEMBER lpEtcFee17)
    Q_PROPERTY(int lpFeeMil MEMBER lpFeeMil)
    Q_PROPERTY(int opFee MEMBER opFee)
    Q_PROPERTY(int opEtcFee MEMBER opEtcFee)
    Q_PROPERTY(int opFeeMil MEMBER opFeeMil)
    Q_PROPERTY(int proNum MEMBER proNum)
    Q_PROPERTY(bool isValid MEMBER isValid)

    ShortFee()
        : lpFeeMil(0)
        , opFeeMil(0)
        , lpFee(0)
        , opFee(0)
        , lpEtcFee(0)
        , opEtcFee(0)
        , lpFee17(0)
        , lpEtcFee17(0)
        , proNum(0)
        , isValid(false)
    {}
    QObject *CreateNew() { return new ShortFee(); }
};

//远程计费OBJ
class CloudFee : public QObject
{
    Q_OBJECT
public:
    // 变量定义
    int shouldPay;           //应收总金额，对应payFee
    int factPay;             //实收总金额，注意，对应splitAmount
    int lpShouldPay;         //本省应收
    int lpFactPay;           //本省实收
    int lpFeeMil;            //本省里程数
    int opShouldPay;         //外省应收
    int opFactPay;           //外省实收
    int opFeeMil;            //外省里程数
    QString cloudFeeDetails; //分省信息，用 | 分割
    bool isDiscount;         //调远程计费时有没有打折
    int discountType;        //特殊折扣类型，specialDiscountType
    int exitFeeType;         //计费方式 4,5 分别为省在线和部在线
    int proNum;              //通行省份个数
    bool isValid;            //是否成功在线计费
    QString errDesc;         //在线计费失败原因

    // 统一编写的Q_PROPERTY
    Q_PROPERTY(int shouldPay MEMBER shouldPay)                 //应收总金额，对应payFee
    Q_PROPERTY(int factPay MEMBER factPay)                     //实收总金额，对应splitAmount
    Q_PROPERTY(int lpShouldPay MEMBER lpShouldPay)             //本省应收
    Q_PROPERTY(int lpFactPay MEMBER lpFactPay)                 //本省实收
    Q_PROPERTY(int lpFeeMil MEMBER lpFeeMil)                   //本省里程数
    Q_PROPERTY(int opShouldPay MEMBER opShouldPay)             //外省应收
    Q_PROPERTY(int opFactPay MEMBER opFactPay)                 //外省实收
    Q_PROPERTY(int opFeeMil MEMBER opFeeMil)                   //外省里程数
    Q_PROPERTY(QString cloudFeeDetails MEMBER cloudFeeDetails) //分省信息，用 | 分割
    Q_PROPERTY(bool isDiscount MEMBER isDiscount)              //调远程计费时有没有打折
    Q_PROPERTY(int discountType MEMBER discountType)           //特殊折扣类型，specialDiscountType
    Q_PROPERTY(int exitFeeType MEMBER exitFeeType)             //计费方式 4,5 分别为省在线和部在线
    Q_PROPERTY(int proNum MEMBER proNum)                       //通行省份个数
    Q_PROPERTY(bool isValid MEMBER isValid)                    //是否成功在线计费
    Q_PROPERTY(QString errDesc MEMBER errDesc)                 //在线计费失败原因

    CloudFee()
        : shouldPay(0)
        , factPay(0)
        , lpShouldPay(0)
        , opShouldPay(0)
        , opFactPay(0)
        , lpFactPay(0)
        , lpFeeMil(0)
        , opFeeMil(0)
        , isDiscount(false)
        , discountType(0)
        , exitFeeType(0)
        , proNum(0)
        , isValid(false)
    {
        cloudFeeDetails = "";
        errDesc = "";
    }
    QObject *CreateNew() { return new CloudFee(); }
};

//承载门架计费OBJ
class FlagFee : public QObject
{
    Q_OBJECT
public:
    // 变量定义
    int fee;                  //承载门架打折前的费用
    int payFee;               //承载门架打折后的费用
    QString intervalDiscount; //计费单元折扣 |               1 | 2
    QString intervalFee;      //计费单元应收  |               3 | 4
    QString intervalPayFee;   //计费单元折扣后费用   |         2 | 2
    QString intervalID;       //计费单元ID      |              ...
    QString intervalName;     //计费单元名称     |
    int feeMil;               //承载门架计费里程
    bool isValid;             //是否有效（根据上下文推断的注释）

    // 统一编写的Q_PROPERTY
    Q_PROPERTY(int fee MEMBER fee)                               //承载门架打折前的费用
    Q_PROPERTY(int payFee MEMBER payFee)                         //承载门架打折后的费用
    Q_PROPERTY(QString intervalDiscount MEMBER intervalDiscount) //计费单元折扣 | 1 | 2
    Q_PROPERTY(QString intervalFee MEMBER intervalFee)           //计费单元应收 | 3 | 4
    Q_PROPERTY(QString intervalPayFee MEMBER intervalPayFee)     //计费单元折扣后费用 | 2 | 2
    Q_PROPERTY(QString intervalID MEMBER intervalID)             //计费单元ID | ...
    Q_PROPERTY(QString intervalName MEMBER intervalName)         //计费单元名称 |
    Q_PROPERTY(int feeMil MEMBER feeMil)                         //承载门架计费里程
    Q_PROPERTY(bool isValid MEMBER isValid)                      //是否有效

    FlagFee()
        : fee(0)
        , payFee(0)
        , feeMil(0)
        , isValid(false)
    {
        intervalDiscount = "";
        intervalFee = "";
        intervalPayFee = "";
        intervalID = "";
        intervalName = "";
    }
    QObject *CreateNew() { return new FlagFee(); }
};

//CPC卡内介质计费OBJ
class CPCFee : public QObject
{
    Q_OBJECT
public:
    // 变量定义
    int fee;               //卡面金额 (总金额)
    int lpFee;             //本省卡面应收
    int lpFeeMil;          //本省里程数
    int opFee;             //外省卡面应收
    int opFeeMil;          //外省里程数
    int proNum;            //通行省份个数
    QString splitProvince; //卡内分省信息，用 |分割
    bool isValid;          //是否有效（根据上下文推断）

    // 统一编写的Q_PROPERTY
    Q_PROPERTY(int fee MEMBER fee)                         //卡面金额 (总金额)
    Q_PROPERTY(int lpFee MEMBER lpFee)                     //本省卡面应收
    Q_PROPERTY(int lpFeeMil MEMBER lpFeeMil)               //本省里程数
    Q_PROPERTY(int opFee MEMBER opFee)                     //外省卡面应收
    Q_PROPERTY(int opFeeMil MEMBER opFeeMil)               //外省里程数
    Q_PROPERTY(int proNum MEMBER proNum)                   //通行省份个数
    Q_PROPERTY(QString splitProvince MEMBER splitProvince) //卡内分省信息，用 |分割
    Q_PROPERTY(bool isValid MEMBER isValid)                //是否有效

    CPCFee()
        : fee(0)
        , lpFee(0)
        , lpFeeMil(0)
        , opFee(0)
        , opFeeMil(0)
        , proNum(0)
        , isValid(false)
    {
        splitProvince = "";
    }
    QObject *CreateNew() { return new CPCFee(); }
};

//OBU介质计费Obj
class OBUFee : public QObject
{
    Q_OBJECT
public:
    // 变量定义
    int fee;               //OBU金额
    int payFee;            //OBU打折后金额
    int lpFee;             //本省OBU打折前
    int lpPayFee;          //本省OBU打折后
    int lpFeeMil;          //本省计费里程数
    int opFee;             //外省OBU应收
    int opPayFee;          //外省OBU实收
    int opFeeMil;          //外省计费里程数
    int proNum;            //通行省份个数
    QString splitProvince; //OBU分省信息，用 |分割
    bool isValid;          //是否有效（根据上下文推断）

    // 统一编写的Q_PROPERTY
    Q_PROPERTY(int fee MEMBER fee)                         //OBU金额
    Q_PROPERTY(int payFee MEMBER payFee)                   //OBU打折后金额
    Q_PROPERTY(int lpFee MEMBER lpFee)                     //本省OBU打折前
    Q_PROPERTY(int lpPayFee MEMBER lpPayFee)               //本省OBU打折后
    Q_PROPERTY(int lpFeeMil MEMBER lpFeeMil)               //本省计费里程数
    Q_PROPERTY(int opFee MEMBER opFee)                     //外省OBU应收
    Q_PROPERTY(int opPayFee MEMBER opPayFee)               //外省OBU实收
    Q_PROPERTY(int opFeeMil MEMBER opFeeMil)               //外省计费里程数
    Q_PROPERTY(int proNum MEMBER proNum)                   //通行省份个数
    Q_PROPERTY(QString splitProvince MEMBER splitProvince) //OBU分省信息，用 |分割
    Q_PROPERTY(bool isValid MEMBER isValid)                //是否有效
    OBUFee()
        : fee(0)
        , payFee(0)
        , lpFee(0)
        , lpPayFee(0)
        , lpFeeMil(0)
        , opFee(0)
        , opPayFee(0)
        , opFeeMil(0)
        , proNum(0)
        , isValid(false)
    {
        splitProvince = "";
    }
    QObject *CreateNew() { return new OBUFee(); }
};

//计费请求
class CalFeeRequest : public QObject
{
    Q_OBJECT
public:
    CalFeeRequest() {}
    virtual ~CalFeeRequest() {}
};
} //namespace CalObj

//==============================================================================
// 通用枚举定义
//==============================================================================

// 数据库类型枚举
Q_BEGIN_ENUM_CREATE(EM_DataBaseType)
enum DataBaseType { MYSQL = 1, DAMENG };
Q_ENUM_CREATE(DataBaseType)
Q_END_ENUM_CREATE(EM_DataBaseType)

// 服务的socket类型
Q_BEGIN_ENUM_CREATE(EM_SocketType)
enum SocketType { TCP = 0, WEBSOCKET };
Q_ENUM_CREATE(SocketType)
Q_END_ENUM_CREATE(EM_SocketType)

// 后端服务调用方式
Q_BEGIN_ENUM_CREATE(EM_ServiceMode)
enum SocketType { LOCAL = 0, ONLINE };
Q_END_ENUM_CREATE(EM_ServiceMode)

// 卡类型
Q_BEGIN_ENUM_CREATE(EM_CardType)
enum CardType {
    NONE = 0,     //空白卡或未知
    PAPER = 12,   //纸性券
    CPC = 15,     //CPC
    OBU = 21,     //单片式OBU（暂未用过）
    PREPAID = 22, //储值卡
    CREDIT = 23   //记账卡
};
Q_ENUM_CREATE(CardType)
Q_END_ENUM_CREATE(EM_CardType)

// 卡业务类型
Q_BEGIN_ENUM_CREATE(EM_CardBizType)
enum CardBizType {
    MANUAL = 10,         //人工发卡模式
    ROBOT = 13,          //自助发卡机
    OPENING = 14,        //开放式车道
    HOLIDAY = 16,        //节假日免征直行
    ECNY = 30,           //数字人民币
    WECHAT = 31,         //微信支付
    ALIPAY = 32,         //支付宝支付
    CPC_ETC_PAY = 33,    //CPC卡使用ETC卡支付
    PAPER_ETC_PAY = 34,  //纸券使用ETC卡支付
    NO_CARD_ETC_PAY = 35 //无卡使用ETC卡支付
};
Q_ENUM_CREATE(CardBizType)
Q_END_ENUM_CREATE(EM_CardBizType)

// Vehstatus
Q_BEGIN_ENUM_CREATE(EM_VehStatus)
enum VehStatus {
    NORMAL = 0,   //普通车
    OFFICIAL = 1, //公务车
    POLICE = 2,   //警哥
    FRUIT = 3     //鲜果车
};
Q_ENUM_CREATE(VehStatus)
Q_END_ENUM_CREATE(EM_VehStatus)

// 车牌颜色
Q_BEGIN_ENUM_CREATE(EM_PlateColor)
enum PlateColor {
    Blue = 0x0000,
    Yellow = 0x0001,
    Black = 0x0002,
    White = 0x0003,
    Green = 0x0004,
    Plain = 0x0005,    // 拼
    Gradient = 0x0006, // 渐
    Temporty = 0x0007, // 临
    Unknown = 0x0009
};
Q_ENUM_CREATE(PlateColor)
Q_END_ENUM_CREATE(EM_PlateColor)
