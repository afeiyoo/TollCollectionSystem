#pragma once

#include "stdafx.h"

#include <QDateTime>
#include <QFlags>
#include <QMap>
#include <QObject>
#include <QVariant>

//==============================================================================
// 配置结构体定义
//==============================================================================
namespace ConfigDef {
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
    QString updateUrl;
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
    QString flagID;       //门架编号
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
    int nodeID;
    ST_PaymentRobotCfg paymentRobotCfg;
    ST_CardRobotCfg cardRobotCfg;
    ST_FeeScrCfg feeScrCfg;
    ST_CardReaderCfg deskReaderCfg;
    ST_PrinterCfg printerCfg;
    ST_CapCfg capCfg;
};
struct ST_DevCfg
{
    QMap<int, ST_TollNode> tollNodeCfgMap;
    QMap<int, ST_RsuCfg> rsuCfgMap;             //天线配置 index为天线编号，其中0为近天线 1为远天线
    QMap<int, ST_InfoBoardCfg> infoBoardCfgMap; //情报板 index为情报板编号

    ST_WeightCfg weightCfg;                           //称重
    ST_OverLapCfg overLapCfg;                         //字符叠加
    ST_SmartLaneControllerCfg smartLaneControllerCfg; //车道智能网关
    ST_LaneCtrlCfg laneCtrlCfg;                       //车控器
};
}//namespace config

//==============================================================================
// 交易相关对象
//==============================================================================
namespace Trade {
class OneTrade : public QObject
{
    Q_OBJECT
public:
    //通用
    QString tradeID;       //STATIONID + LANEID + TIME(YYYYMMDDhhmmss) + SEQ
    int seqNum;  //数据序列号
    QString passID; //通行ID
    QString cnLaneID; //路网中心车道号
    int vehSpeed; //车速
    int payType; //支付方式
    int dealStatus; //出口车情
    int deviceStatus; //设备状态
    int recordType; //记录类型
    int changeType; //变更类型
    double changedFee; //变更费用
    int peccType; //违章类型
    int chkPoint; //标识站
    int spendTime; //收费耗时
    QString termCode; //终端机编码
    int cardTradeNo; //用户卡交易序号
    int termTradeNo; //终端交易序号
    int isValid; //是否有效
    int userType; //车种
    int opeateSign; //操作标志
    QString reserve2; //备用字段2
    QString reserve; //备用字段
    int dataMonth; //数据月份
    int provinceID; //省份ID
    int verifycode; //校验码
    double tsVehicle; //交调口径折算车次
    QString appVersion; //车道系统版本号
    int recordType2; //记录类型
    int tollSupport; //计费支撑类型
    QString specialType; //特情类型
    int openFlag; //开放段标识


    //费用相关
    int tollRateVer;  //费率版本号
    double shouldPay; //收费总额(应收)
    int discountType; //折扣类型
    double discount; //折扣金额
    double opFactPay; //外省实收
    double factPay; //总费额(实收)
    int merRebateType; //商户折扣类型
    QString merchant; //代扣用户帐号
    double rebateRatio; //折扣率
    double merShouldPay; //商户应付金额
    double rebatedPay; //折扣后金额
    double specialPay; //特殊路段收费
    int shortFee; //最短路径交易金额
    int shortFeeMileage; //最短计费里程
    double feeRate; //交易金额占比
    int exitFeeType; //计费方式
    int transPayType; //交易支付方式
    int transFee; //卡面扣费金额，入口承载门架时填写
    uint postBalance; //扣后余额
    int cardFeeSum; //卡片交易累计金额
    int localFeeMileage; //本地计费里程
    QString splitProvince; //多条省内交易信息

    //OBU相关
    QString obuPlate; //OBU车牌
    uint restMoney; //电子钱包余额
    double tac; //ETC TAC码
    QString obuSerialNum; //OBU应用序列号
    int obuID; //电子标签鉴别码(OBUID)
    int obuContractVer; //OBU合同版本号
    int obuPayFeeSum; //OBU交易累计应收金额
    int obuDiscountFeeSum; //OBU交易累计优惠金额
    int obuMileage; //累计计费里程
    int obuTotalTradeSuccNum; //总交易成功数量
    int obuProvFeeSum; //本省交易累计金额
    int obuProvFactSum; //本省交易累计实收金额
    int obuProvTradeSuccNum; //本省交易成功数量
    QString ffObuCardNo; //EF04文件中的卡号等信息
    int noCardTimes; //标签无卡累计次数
    int obuStatus; //OBU状态
    int algorithm; //算法标识
    int obuVehClass; //OBU车型

    //卡介质相关
    int cardNetID;    //卡网络编号，识别省份
    int cardType;     //卡类型 参照EM_CardType
    int cardBizType;  //卡业务类型 参照EM_CardBizType
    QString cardID;   //卡号，"0"为无卡
    QString cardPlate; //卡车牌
    QString cpcCardID; //CPC卡号
    int cpcElec; //CPC卡电量
    int etcCardVer; //ETC卡版本号

    //入口相关
    int enNetID;      //入口路网编号
    QString enStation;  //入口收费站ID
    QString enStationName; //入口收费站名称
    QDateTime enTime;   //入口时间 YYYY-MM-DD hh:mm:ss
    QString enOperatorID; //入口操作人员ID
    QString enOperator;   //入口操作人员名称
    int enVehClass;   //入口车型
    QString enVehPlate;  //入口车牌
    QDateTime enShiftDate; //入口班次日期
    int enShiftID;   //入口班次ID
    int enVehStatus;   //入口车种
    int enLane; //入口车道号
    QString enRoadNet; //入口网络编码

    //出口相关
    int exNetID;   //出口路网编号，默认3501
    QString exStation;  //出口收费站ID
    QString exStationName; //出口收费站名称
    int exLane;   //出口车道
    QDateTime exTime; //出口时间
    QString exOperatorID;  //出口操作人员ID
    QString exOperator;    //出口操作人员名称
    int exShiftID;    //出口班次ID
    QDateTime exShiftDate;   //出口班次时间 YYYY-MM-DD
    int exVehClass; //出口车型
    int exVehStatus;  //出口车种 参考EM_VehStatus
    QString exVehPlate; //出口车牌
    int provinceNum; //通行省份个数
    QString provEnHex; //本省入口编码

    //称重相关
    int enAxisCount; //入口轴数
    int enTotalWeight; //入口总轴重
    int exAxises; //出口轴数
    int exTotalWeight; //出口总轴重

    int axisGroups; //轴组数
    int origAxisType; //原始轴型
    int axisType; //轴型
    QString axisWeight; //各轴轴重
    int origWeight; //原始总轴重
    int tollWeight; //收费总重
    int axisLimit; //总轴限
    double exceedRate; //收费超限比例
    int weightClass; //重量类型
    int tollType; //收费类型

    //票据相关
    int ticketNum; //电脑票据号
    int scrapTickets; //废票次数
    QString billCode; //账单编号
    int ticketYear; //票据年份

public:
    OneTrade()
        : // 通用字段初始化
          tradeID(""), seqNum(0), passID(""), cnLaneID(""), vehSpeed(0),
          payType(0), dealStatus(0), deviceStatus(0), recordType(0), changeType(0),
          changedFee(0.0), peccType(0), chkPoint(0), spendTime(0), termCode(""),
          cardTradeNo(0), termTradeNo(0), isValid(0), userType(0), opeateSign(0),
          reserve2(""), reserve(""), dataMonth(0), provinceID(0), verifycode(0),
          tsVehicle(0.0), appVersion(""), recordType2(0), tollSupport(0),
          specialType(""), openFlag(0),
          // 费用相关字段初始化
          tollRateVer(0), shouldPay(0.0), discountType(0), discount(0.0),
          opFactPay(0.0), factPay(0.0), merRebateType(0), merchant(""),
          rebateRatio(0.0), merShouldPay(0.0), rebatedPay(0.0), specialPay(0.0),
          shortFee(0), shortFeeMileage(0), feeRate(0.0), exitFeeType(0),
          transPayType(0), transFee(0), postBalance(0u), cardFeeSum(0),
          localFeeMileage(0), splitProvince(""),
          // OBU相关字段初始化
          obuPlate(""), restMoney(0u), tac(0.0), obuSerialNum(""), obuID(0),
          obuContractVer(0), obuPayFeeSum(0), obuDiscountFeeSum(0), obuMileage(0),
          obuTotalTradeSuccNum(0), obuProvFeeSum(0), obuProvFactSum(0),
          obuProvTradeSuccNum(0), ffObuCardNo(""), noCardTimes(0), obuStatus(0),
          algorithm(0), obuVehClass(0),
          // 卡介质相关字段初始化
          cardNetID(0), cardType(0), cardBizType(0), cardID(""), cardPlate(""),
          cpcCardID(""), cpcElec(0), etcCardVer(0),
          // 入口相关字段初始化
          enNetID(0), enStation(""), enStationName(""), enTime(QDateTime()),
          enOperatorID(""), enOperator(""), enVehClass(0), enVehPlate(""),
          enShiftDate(QDateTime()), enShiftID(0), enVehStatus(0), enLane(0),
          enRoadNet(""),
          // 出口相关字段初始化（exNetID按注释默认3501）
          exNetID(3501), exStation(""), exStationName(""), exLane(0),
          exTime(QDateTime()), exOperatorID(""), exOperator(""), exShiftID(0),
          exShiftDate(QDateTime()), exVehClass(0), exVehStatus(0), exVehPlate(""),
          provinceNum(0), provEnHex(""),
          // 称重相关字段初始化
          enAxisCount(0), enTotalWeight(0), exAxises(0), exTotalWeight(0),
          axisGroups(0), origAxisType(0), axisType(0), axisWeight(""),
          origWeight(0), tollWeight(0), axisLimit(0), exceedRate(0.0),
          weightClass(0), tollType(0),
          // 票据相关字段初始化
          ticketNum(0), scrapTickets(0), billCode(""), ticketYear(0)
    {
    }
    ~OneTrade() override{}
    void tradeInit(){
        this->~OneTrade();
        new (this) OneTrade();
    }
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
    Q_PROPERTY(int Axises MEMBER exAxises)
    Q_PROPERTY(int OrigAxisType MEMBER origAxisType)
    Q_PROPERTY(int AxisType MEMBER axisType)
    Q_PROPERTY(QString AxisWeight MEMBER axisWeight)
    Q_PROPERTY(int OrigWeight MEMBER origWeight)
    Q_PROPERTY(int TotalWeight MEMBER exTotalWeight)
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
    Q_PROPERTY(int TotalWeight MEMBER enTotalWeight)
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
// 介质相关对象
//==============================================================================
namespace Media {

//CPC过站信息
class CPCPassInfo : public QObject
{
    Q_OBJECT
public:
   int   ProvinceNum;    //通行省份个数
   int   LocalFlags;     //本省通行门架数
   uint   LocalFeeSum;    //本省累计计费金额(3B)
   uint   LocalMileAge;   //本省累计计费里程(3B)
   QString EnHexFlag;   //本省第一个入口门架编号(3B),入口站不承载门架时清0
   uint   EnFlagTime;     //本省第一个入口门架通行时间(unix时间)
   QString LastHexFlag; //最新门架编号
   uint   LastFlagTime;   //最新门架通行时间
   uint   LastFlagFee;    //最新门架计费金额(3B)
   uint   LastMileAge;    //最新门架计费里程(3B)
   QVariantList LocalHexFlags;   //最多24个门架
   int Records;    //已写入的计费信息数(不会超过22)
   QVariantList FeeCode;  //计费信息

   CPCPassInfo()
       : ProvinceNum(0), LocalFlags(0), LocalFeeSum(0u),
         LocalMileAge(0u), EnHexFlag(""), EnFlagTime(0u),
         LastHexFlag(""), LastFlagTime(0u), LastFlagFee(0u),
         LastMileAge(0u), Records(0)
   {
       LocalHexFlags.clear();
       FeeCode.clear();
   }
   ~CPCPassInfo() override{}
public:
   Q_PROPERTY(int ProvinceNum MEMBER ProvinceNum)
   Q_PROPERTY(int LocalFlags MEMBER LocalFlags)
   Q_PROPERTY(uint LocalFeeSum MEMBER LocalFeeSum)
   Q_PROPERTY(uint LocalMileAge MEMBER LocalMileAge)
   Q_PROPERTY(QString EnHexFlag MEMBER EnHexFlag)
   Q_PROPERTY(uint EnFlagTime MEMBER EnFlagTime)
   Q_PROPERTY(QString LastHexFlag MEMBER LastHexFlag)
   Q_PROPERTY(uint LastFlagTime MEMBER LastFlagTime)
   Q_PROPERTY(uint LastFlagFee MEMBER LastFlagFee)
   Q_PROPERTY(uint LastMileAge MEMBER LastMileAge)
   Q_PROPERTY(QVariantList LocalHexFlags MEMBER LocalHexFlags)
   Q_PROPERTY(int Records MEMBER Records)
   Q_PROPERTY(QVariantList FeeCode MEMBER FeeCode)
};

//计费信息(DF01-EF04)
class CPCFeeCode : public QObject
{
    Q_OBJECT
public:
    int Province;   //1 字节省份行政区域编码
    int PassFlags;  //1 字节本省ETC 门架通行个数
    uint FeeSum;     //3 字节本省累计计费金额
    uint MileAge;    //3 字节本省累计计费里程（米）
    QString EnHexFlag; //3 字节本省入口门架信息
    uint EnFlagTime;   //4 字节本省入口门架通行时间UNIX 时间）
    QString LastHexFlag;  //3 字节最新ETC 门架编码
    uint LastFlagTime;    //4 字节最新通行时间
    int FittingStatus;   //1 字节最新ETC 门架拟合成功/失败标识（00 成功 01 失败）

    CPCFeeCode()
        : Province(0), PassFlags(0), FeeSum(0u),
          MileAge(0u), EnHexFlag(""), EnFlagTime(0u),
          LastHexFlag(""), LastFlagTime(0u), FittingStatus(0)
    {
    }
    ~CPCFeeCode() override{}
public:
    Q_PROPERTY(int Province MEMBER Province)
    Q_PROPERTY(int PassFlags MEMBER PassFlags)
    Q_PROPERTY(uint FeeSum MEMBER FeeSum)
    Q_PROPERTY(uint MileAge MEMBER MileAge)
    Q_PROPERTY(QString EnHexFlag MEMBER EnHexFlag)
    Q_PROPERTY(uint EnFlagTime MEMBER EnFlagTime)
    Q_PROPERTY(QString LastHexFlag MEMBER LastHexFlag)
    Q_PROPERTY(uint LastFlagTime MEMBER LastFlagTime)
    Q_PROPERTY(int FittingStatus MEMBER FittingStatus)
};

//分省
class EF04ProvFee : public QObject
{
    Q_OBJECT
public:
    int ProCode; //省级行政区代码
    uint fee;    //金额

    EF04ProvFee()
        : ProCode(0), fee(0u)
    {
    }
    ~EF04ProvFee(){}
public:
    Q_PROPERTY(int ProCode MEMBER ProCode)
    Q_PROPERTY(uint fee MEMBER fee)
};

//OBU EF04
class OBUEF04Data : public QObject
{
    Q_OBJECT
public:
    int VehicleUserType;  //车辆用户类型
    int PassProvCount; //通行省份个数
    uint TotalFee;       //累计应收金额  3字节
    uint FactFee;        //累计实收金额  3字节
    uint SuccessCount;  //累计交易成功数  2字节
    uint TotalMileage;  //累计计费里程
    int NoCardCount;   //标签无卡累计次数
    QString EnCode;     //本省入口编码
    uint LocatFee;       //本省累计交易金额 3字节
    int LocatSuccessCount; //本省累计交易成功数
    uint LocatFactFee; //本省累计实收金额
    QString HexEncry;  //加密摘要
    uint TollFee;    //EF04 0019
    uint FlagID;    //EF04 0019内的FlagID
    QDateTime FlagPassTime;   //EF04 0019内的门架通行时间，转换为"yyyy-MM-dd hh:mm:ss"
    QVariantList ProvFee;

    OBUEF04Data()
        : VehicleUserType(0), PassProvCount(0), TotalFee(0u),
          FactFee(0u), SuccessCount(0u), TotalMileage(0u),
          NoCardCount(0), EnCode(""), LocatFee(0u),
          LocatSuccessCount(0), LocatFactFee(0u), HexEncry(""),
          TollFee(0u), FlagID(0u), FlagPassTime(QDateTime())
    {
        ProvFee.clear();
    }
    ~OBUEF04Data(){}
public:
    Q_PROPERTY(int VehicleUserType MEMBER VehicleUserType)
    Q_PROPERTY(int PassProvCount MEMBER PassProvCount)
    Q_PROPERTY(uint TotalFee MEMBER TotalFee)
    Q_PROPERTY(uint FactFee MEMBER FactFee)
    Q_PROPERTY(uint SuccessCount MEMBER SuccessCount)
    Q_PROPERTY(uint TotalMileage MEMBER TotalMileage)
    Q_PROPERTY(int NoCardCount MEMBER NoCardCount)
    Q_PROPERTY(QString EnCode MEMBER EnCode)
    Q_PROPERTY(uint LocatFee MEMBER LocatFee)
    Q_PROPERTY(int LocatSuccessCount MEMBER LocatSuccessCount)
    Q_PROPERTY(uint LocatFactFee MEMBER LocatFactFee)
    Q_PROPERTY(QString HexEncry MEMBER HexEncry)
    Q_PROPERTY(uint TollFee MEMBER TollFee)
    Q_PROPERTY(uint FlagID MEMBER FlagID)
    Q_PROPERTY(QDateTime FlagPassTime MEMBER FlagPassTime)
    Q_PROPERTY(QVariantList ProvFee MEMBER ProvFee)
};

} //namespace media
//==============================================================================
// 计费相关对象
//==============================================================================
namespace CalFee {

//兜底OBJ
class ShortFee : public QObject
{
    Q_OBJECT
public:
    int lpFeeMil;   //本省兜底里程
    int lpFee;      //本省兜底费用
    int lpEtcFee;   //本省ETC费用
    // int lpFee17;    //本省超6轴加收
    // int lpEtcFee17; //本省ETC超6轴加收
    int opFee;      //外省兜底费用
    int opEtcFee;   //外省ETC费用
    int opFeeMil;   //外省兜底里程
    int proNum;     //兜底跨省通行个数
    bool isValid;   //是否成功获取兜底费率

    ShortFee()
        : lpFeeMil(0), lpFee(0), lpEtcFee(0),
          opFee(0), opEtcFee(0), opFeeMil(0),
          proNum(0), isValid(false)
    {}
    ~ShortFee() override{}
    // QObject *CreateNew() { return new ShortFee(); }
public:
    Q_PROPERTY(int lpFee MEMBER lpFee)
    Q_PROPERTY(int lpEtcFee MEMBER lpEtcFee)
    Q_PROPERTY(int lpFeeMil MEMBER lpFeeMil)
    Q_PROPERTY(int opFee MEMBER opFee)
    Q_PROPERTY(int opEtcFee MEMBER opEtcFee)
    Q_PROPERTY(int opFeeMil MEMBER opFeeMil)
    Q_PROPERTY(int proNum MEMBER proNum)
    Q_PROPERTY(bool isValid MEMBER isValid)
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

    CloudFee()
        : shouldPay(0), factPay(0), lpShouldPay(0),
          lpFactPay(0), lpFeeMil(0), opShouldPay(0),
          opFactPay(0), opFeeMil(0), cloudFeeDetails(""),
          isDiscount(false), discountType(0), exitFeeType(0),
          proNum(0), isValid(false), errDesc("")
    {
    }
    ~CloudFee() override{}
    // QObject *CreateNew() { return new CloudFee(); }
public:
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
};

//承载门架计费OBJ
class FlagFee : public QObject
{
    Q_OBJECT
public:
    // 变量定义
    int fee;                  //门架打折前的费用
    int payFee;               //门架打折后的费用
    QString feeVersion;       //费率版本
    QString intervalDiscount; //计费单元折扣 |               1 | 2
    QString intervalFee;      //计费单元应收  |               3 | 4
    QString intervalPayFee;   //计费单元折扣后费用   |         2 | 2
    QString intervalID;       //计费单元ID      |              ...
    QString intervalName;     //计费单元名称     |
    QString agentFlags;       //
    QString agentFees;        //
    int gantryPassCount;      //门架数
    QString gantryPassInfo;   //
    int feeMil;               //门架计费里程
    int obuTotalTradeSuccNum; //交易成功数 （ETC交易时填写）
    int obuPayFeeSum;         //门架计费总额（ETC交易时填写）
    int obuDiscountFeeSum;    //折扣金额（ETC交易时填写）
    int obuProvfeeSum;        //OBU省内金额（ETC交易时填写）
    int obuProvfactSum;       //OBU省内实付（ETC交易时填写）
    int obuProvTradeSuccNum;  //OBU省内交易成功数（ETC交易时填写）
    int feeSumLocalBefore;    //CPC卡加上本省金额前（CPC交易时填写）
    int feeSumLocalAfter;     //CPC卡加上本省金额后 （CPC交易时填写）
    QString remark;           //remark
    bool isValid;             //是否有效

    FlagFee()
        : fee(0), payFee(0), feeVersion(""),
          intervalDiscount(""), intervalFee(""), intervalPayFee(""),
          intervalID(""), intervalName(""), agentFlags(""),
          agentFees(""), gantryPassCount(0), gantryPassInfo(""),
          feeMil(0), obuTotalTradeSuccNum(0), obuPayFeeSum(0),
          obuDiscountFeeSum(0), obuProvfeeSum(0), obuProvfactSum(0),
          obuProvTradeSuccNum(0), feeSumLocalBefore(0), feeSumLocalAfter(0),
          remark(""), isValid(false)
    {
    }
    ~FlagFee() override{}
    // QObject *CreateNew() { return new FlagFee(); }
public:
    Q_PROPERTY(int fee MEMBER fee)
    Q_PROPERTY(int payFee MEMBER payFee)
    Q_PROPERTY(QString feeVersion MEMBER feeVersion)
    Q_PROPERTY(QString intervalDiscount MEMBER intervalDiscount)
    Q_PROPERTY(QString intervalFee MEMBER intervalFee)
    Q_PROPERTY(QString intervalPayFee MEMBER intervalPayFee)
    Q_PROPERTY(QString intervalID MEMBER intervalID)
    Q_PROPERTY(QString intervalName MEMBER intervalName)
    Q_PROPERTY(QString agentFlags MEMBER agentFlags)
    Q_PROPERTY(QString agentFees MEMBER agentFees)
    Q_PROPERTY(int gantryPassCount MEMBER gantryPassCount)
    Q_PROPERTY(QString gantryPassInfo MEMBER gantryPassInfo)
    Q_PROPERTY(int feeMil MEMBER feeMil)
    Q_PROPERTY(int obuTotalTradeSuccNum MEMBER obuTotalTradeSuccNum)
    Q_PROPERTY(int obuPayFeeSum MEMBER obuPayFeeSum)
    Q_PROPERTY(int obuDiscountFeeSum MEMBER obuDiscountFeeSum)
    Q_PROPERTY(int obuProvfeeSum MEMBER obuProvfeeSum)
    Q_PROPERTY(int obuProvfactSum MEMBER obuProvfactSum)
    Q_PROPERTY(int obuProvTradeSuccNum MEMBER obuProvTradeSuccNum)
    Q_PROPERTY(int feeSumLocalBefore MEMBER feeSumLocalBefore)
    Q_PROPERTY(int feeSumLocalAfter MEMBER feeSumLocalAfter)
    Q_PROPERTY(QString remark MEMBER remark)
    Q_PROPERTY(bool isValid MEMBER isValid)
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
    bool isValid;          //是否有效

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
        : fee(0), lpFee(0), lpFeeMil(0),
          opFee(0), opFeeMil(0), proNum(0),
          isValid(false)
    {
        splitProvince = "";
    }
    ~CPCFee() override{}
    // QObject *CreateNew() { return new CPCFee(); }
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
    bool isValid;          //是否有效

    OBUFee()
        : fee(0), payFee(0), lpFee(0)
        , lpPayFee(0), lpFeeMil(0), opFee(0)
        , opPayFee(0), opFeeMil(0), proNum(0)
        , splitProvince(""), isValid(false)
    {
    }
    ~OBUFee() override{}
    // QObject *CreateNew() { return new OBUFee(); }
public:
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
};

//计费请求
class CalFeeRequest : public QObject
{
    Q_OBJECT
public:
    Trade::OneTrade* oneTrade;
    Media::CPCPassInfo* cpcPassInfo;
    Media::OBUEF04Data* obuEF04Data;
    bool doubleCardCal;
    bool discount;

    CalFeeRequest()
        : oneTrade(nullptr), cpcPassInfo(nullptr), obuEF04Data(nullptr),
          doubleCardCal(false), discount(false)
    {
    }
    ~CalFeeRequest() override{
        SAFE_DELETE(cpcPassInfo);
        SAFE_DELETE(obuEF04Data);
    }
public:
    Q_PROPERTY(Trade::OneTrade* oneTrade MEMBER oneTrade)
    Q_PROPERTY(Media::CPCPassInfo* cpcPassInfo MEMBER cpcPassInfo)
    Q_PROPERTY(Media::OBUEF04Data* obuEF04Data MEMBER obuEF04Data)
    Q_PROPERTY(bool doubleCardCal MEMBER doubleCardCal)
    Q_PROPERTY(bool discount MEMBER discount)
};

// //计费返回
class CalFeeResult : public QObject
{
    Q_OBJECT
public:
    Trade::OneTrade* oneTrade;
    ShortFee* shortFee;
    OBUFee* obuFee;
    CPCFee* cpcFee;
    CloudFee* cloudFee;
    FlagFee* flagFee; //承载门架
    FlagFee* gantryFee; //拟合
    int errCode;
    QString errDesc;
    int specialCode;
    QString specialDesc;
    // 构造函数：初始化所有成员变量
    CalFeeResult()
        : oneTrade(nullptr), shortFee(nullptr), obuFee(nullptr),
          cpcFee(nullptr), cloudFee(nullptr), flagFee(nullptr),
          gantryFee(nullptr), errCode(0), errDesc(""),
          specialCode(0), specialDesc("")
    {
    }
    ~CalFeeResult() override{
        SAFE_DELETE(shortFee);
        SAFE_DELETE(obuFee);
        SAFE_DELETE(cpcFee);
        SAFE_DELETE(cloudFee);
        SAFE_DELETE(flagFee);
        SAFE_DELETE(gantryFee);
    }
public:
    Q_PROPERTY(Trade::OneTrade* oneTrade MEMBER oneTrade)
    Q_PROPERTY(ShortFee* shortFee MEMBER shortFee)
    Q_PROPERTY(OBUFee* obuFee MEMBER obuFee)
    Q_PROPERTY(CPCFee* cpcFee MEMBER cpcFee)
    Q_PROPERTY(CloudFee* cloudFee MEMBER cloudFee)
    Q_PROPERTY(FlagFee* flagFee MEMBER flagFee)
    Q_PROPERTY(FlagFee* gantryFee MEMBER gantryFee)
    Q_PROPERTY(int errCode MEMBER errCode)
    Q_PROPERTY(QString errDesc MEMBER errDesc)
    Q_PROPERTY(int specialCode MEMBER specialCode)
    Q_PROPERTY(QString specialDesc MEMBER specialDesc)
};
} //namespace CalFee

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
enum ServiceMode { LOCAL = 0, ONLINE };
Q_ENUM_CREATE(ServiceMode)
Q_END_ENUM_CREATE(EM_ServiceMode)

// 车道系统类型
Q_BEGIN_ENUM_CREATE(EM_LaneMode)
enum LaneMode { ETC_IN = 1, ETC_OUT, MTC_IN, MTC_OUT, ETC_AUTO_IN, ETC_AUTO_OUT, PRE_TRADE_IN, PRE_TRADE_OUT, PAD };
Q_ENUM_CREATE(LaneMode)
Q_END_ENUM_CREATE(EM_LaneMode)

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


// 支付方式
Q_BEGIN_ENUM_CREATE(EM_PayType)
enum PayType
{
    CASH        = 0, // 现金支付
    ETC_CARD    = 1, // 电子支付
    SCAN        = 2, // 第三方支付
    UNPAID      = 3  // 闯关未付
};
Q_ENUM_CREATE(PayType)
Q_END_ENUM_CREATE(EM_PayType)


// 黑名单类型
Q_BEGIN_ENUM_CREATE(EM_BlackType)
enum BlackType
{
    VEHICLE         = 0, // 状态名单车
    CPC_CARD        = 1, // 状态名单通行卡
    ID_CARD         = 2, // 状态名单身份卡
    OBU             = 3, // 状态名单OBU
    ETC_CARD        = 4, // 状态名单ETC卡
    TERMINAL        = 5, // 状态名单终端
    LAW_VEHICLE     = 6  // 路政状态名单车
};
Q_ENUM_CREATE(BlackType)
Q_END_ENUM_CREATE(EM_BlackType)


//变更类型
Q_BEGIN_ENUM_CREATE(EM_ChangeType)
enum ChangeType
{
    NONE               = 0,  // 无
    CAR_TO_TRUCK       = 5,  // 客车变货车
    TRUCK_TO_CARD      = 6,  // 货车变客车
    HIGH_TO_LOW        = 7,  // 高变低
    LOW_TO_HIGH        = 8,  // 低变高
    CAR_TO_OPERATION   = 9,  // 客变专
    OPERATION_TO_CAR   = 10, // 专变客
    TRUCK_TO_OPERATION = 11, // 货变专
    OPERATION_TO_TRUCK = 12  // 专变货
};
Q_ENUM_CREATE(ChangeType)
Q_END_ENUM_CREATE(EM_ChangeType)

// 标识站
Q_BEGIN_ENUM_CREATE(EM_ChkPoint)
enum ChkPoint
{
    QINGZHOU_BRIDGE     = 1,  // 青州大桥
    XIAXIANG_BRIDGE     = 2,  // 厦漳跨海大桥
    PINGTAN_BRIDGE      = 4,  // 平潭大桥
    QUANZHOUWAN_BRIDGE  = 8   // 泉州湾跨海大桥
};
Q_ENUM_CREATE(ChkPoint)
Q_END_ENUM_CREATE(EM_ChkPoint)


// 出口车情
Q_BEGIN_ENUM_CREATE(EM_DealStatus)
enum DealStatus
{
    NONE                = 0,          // 无
    NO_CARD             = 1,          // 无卡
    BAD_CARD            = 2,          // 坏卡
    PLATE_DIFF          = 4,          // 车牌不符
    U_TURN              = 8,          // U转
    TIMEOUT             = 16,         // 超时
    CHANGED             = 32,         // 变更
    UNPAID              = 64,         // 未付
    NO_PLATE_DIFF       = 128,        // 非车牌不符
    U_TURN_NORMAL       = 256,        // 正常U转车
    TIMEOUT_NORMAL      = 512,        // 正常超时
    U_TURN_CAL          = 1024,       // 正常计费U转
    GUIAN_CAR           = 2048,       // 贵安专用车
    SCAN_PAY            = 4096,       // 二维码支付
    BINHAI_REBATE       = 8192,       // 滨海新城代扣
    MAWEI_REBATE        = 16384,      // 马尾大桥代扣
    WUYISHAN_REBATE     = 32768,      // 武夷山代扣
    SANMING_SHAXIAN_REBATE = 65536,      // 三明沙县代扣
    JINJIANGNAN_LONGHU_REBATE = 131072,     // 晋江南龙湖代扣
    NANPING_JIANYANG_REBATE   = 262144,     // 南平建阳代扣
    WUPINGSHIFANG_REBATE    = 1048576,    // 武平十方代扣
    SHAOWU_WUJIATANG_REBATE  = 2097152,    // 邵武吴家塘代扣
    GUANGZE_JINLING_REBATE  = 4194304,    // 光泽金岭代扣
    PUCHENG_REBATE      = 8388608,    // 浦城代扣
    YANPING_REBATE      = 16777216,   // 延平代扣
    YANPINGBEI_REBATE   = 33554432    // 延平北代扣
};
Q_ENUM_CREATE(DealStatus)
Q_END_ENUM_CREATE(EM_DealStatus)



// 计费方式
Q_BEGIN_ENUM_CREATE(EM_ExitFeeType)
enum ExitFeeType
{
    OBU_NO_DISCOUNT        = 1, // 按OBU内累计优惠后金额计费
    OBU_BEFORE_DISCOUNT    = 2, // 按OBU内累计优惠前金额计费
    CPC_CARD               = 3, // 按CPC卡内累计金额计费
    PROVINCE_CLOUD_FEE     = 4, // 省中心在线服务计费
    MOT_CLOUD_FEE          = 5, // 部中心在线服务计费
    SHORT                  = 6  // 按最小费额计费
};
Q_ENUM_CREATE(ExitFeeType)
Q_END_ENUM_CREATE(EM_ExitFeeType)


// 免费类型
Q_BEGIN_ENUM_CREATE(EM_FreeType)
enum FreeType
{
    PROVINCE_FREE  = 0, // 全省免征
    REGION_FREE    = 1  // 区域免征
};
Q_ENUM_CREATE(FreeType)
Q_END_ENUM_CREATE(EM_FreeType)



// 车道用途类型
Q_BEGIN_ENUM_CREATE(EM_LaneUseType)
enum LaneUseType
{
    NORMAL_LANE             = 0,  // 普通车道
    PORTABLE_LANE           = 1,  // 便携车道
    UNMANNED_CARD_LANE      = 2,  // 无人发卡车道
    ETC_LANE                = 3,  // ETC车道
    OPEN_LANE               = 4,  // 开放式车道
    PASSENGER_ETC_ONLY      = 5,  // 客车ETC专用道
    TRUCK_ETC_ONLY          = 6,  // 货车ETC专用道
    SELF_SERVICE_LANE       = 8,  // 自助缴费终端车道
    EMERGENCY_TERMINAL_LANE = 9,  // 应急终端车道
    PAPER_TICKET_LANE       = 10  // 自助纸券车道
};
Q_ENUM_CREATE(LaneUseType)
Q_END_ENUM_CREATE(EM_LaneUseType)

// 参数类型
Q_BEGIN_ENUM_CREATE(EM_ParamType)
enum ParamType
{
    FEE_RATE_PARAM             = 10, // 费率参数
    AXLE_TYPE_PARAM            = 11, // 轴型参数
    VEHICLE_SEAT_PARAM         = 12, // 车型坐位参数
    WEIGHT_CORRECTION_PARAM    = 13, // 计重较正参数
    SHIFT_PARAM                = 14, // 班次参数
    TRAFFIC_CALIBRATION_PARAM = 15, // 交调口径折算系数
    HOLIDAY_PARAM              = 16, // 节假日免征参数
    DISCOUNT_USER_PARAM        = 17, // 折扣用户参数
    DISCOUNT_FEE_PARAM         = 18, // DiscountFee折扣参数
    PARAM_DICT_PARAM           = 19, // T_ParamDict参数
    SEGMENT_FEE_PARAM          = 20, // 分段计费参数

    REDUCE_EXEMPT_PARAM        = 30, // 减免征车参数
    LAW_TRAILER_PARAM          = 31, // 路政拖车参数

    ETC_BLACKLIST_PARAM        = 40, // ETC状态名单参数
    VEHICLE_BLACKLIST_PARAM    = 41, // 状态名单车参数
    PASSCARD_BLACKLIST_PARAM   = 42, // 状态名单通行卡参数
    LAW_BLACKLIST_PARAM        = 43, // 路政状态名单参数
    OTHER_BLACKLIST_PARAM      = 44, // 其它状态名单

    OTHER_FEE_PARAM            = 99  // 其它收费参数
};
Q_ENUM_CREATE(ParamType)
Q_END_ENUM_CREATE(EM_ParamType)


// 代扣类型
Q_BEGIN_ENUM_CREATE(EM_RebateType)
enum RebateType
{
    GUIAN_REBATE                      = 1061,     // 贵安商学院代扣
    BINHAI_REBATE                     = 1063,     // 滨海新城代扣
    MAWEI_REBATE                      = 1065,     // 马尾大桥(福州城区)代扣
    SHAOWU_JINTANG_REBATE             = 1084,     // 邵武金塘代扣
    YANPING_LIST_REBATE                    = 1094,     // 延平代扣(名单)

    WUYISHAN_REBATE                   = 100501,   // 武夷山代扣
    SANMING_SHAXIAN_REBATE            = 100601,   // 三明沙县代扣
    // JINJIANGNAN_LONGHU__REBATE  = 100701,   // 晋江南龙湖代扣(折上折)
    JINJIANGNAN_LONGHU_REBATE         = 100702,   // 晋江南龙湖代扣
    NANPING_JIANYANG_REBATE           = 100801,   // 南平建阳代扣
    JIANYANG_NANPINGNAN_REBATE        = 100901,   // 建阳南平南代扣

    WUPING_DUANWU_2024_REBATE         = 101001,   // 武平十方代扣2024端午
    WUPING_MID_AUTUMN_2024_REBATE     = 101002,   // 武平十方代扣2024中秋
    WUPING_NATIONAL_2024_REBATE       = 101003,   // 武平十方代扣2024国庆
    WUPING_NEWYEAR_2025_REBATE        = 101011,   // 武平十方代扣2025元旦
    WUPING_SPRING_2025_REBATE         = 101012,   // 武平十方代扣2025春运
    WUPING_QINGMING_2025_REBATE       = 101013,   // 武平十方代扣2025清明
    WUPING_LABOR_2025_REBATE          = 101014,   // 武平十方代扣2025劳动
    WUPING_DUANWU_2025_REBATE         = 101015,   // 武平十方代扣2025端午
    WUPING_NATIONAL_2025_REBATE       = 101016,   // 武平十方代扣2025国庆

    GUANGZE_JINLING_REBATE            = 101201,   // 光泽金岭代扣
    PUCHENG_REBATE                    = 101301,   // 浦城代扣
    YANPING_REBATE                    = 101401,   // 延平代扣
    YANPINGBEI_REBATE                 = 101501,   // 延平北代扣

    FUZHOUDONG_JICHANG_REBATE         = 25159101, // 机场代扣(福州东-机场)
    MAWEI_JICHANG_REBATE              = 25159102  // 机场代扣(马尾-机场)
};
Q_ENUM_CREATE(RebateType)
Q_END_ENUM_CREATE(EM_RebateType)

// 记录类型2
Q_BEGIN_ENUM_CREATE(EM_RecordType)
enum RecordType
{
    WEIGHT_LOW              = 1,          //称重降级数据
    MANUAL_WEIGHT           = 2,          //手工插入称重
    CHANGE_AXIS_TYPE        = 4,          //人工改轴型
    CHANGE_AXISES           = 8,          //人工改轴数
    CHANGE_WEIGHT           = 12,         //轴型+轴数更改
    TRAILER                 = 16,         //拖车
    ACCIDENT                = 32,         //事故车
    LONG_CAR                = 64,         //超长车
    FAKE_AXIS               = 128,        //假轴车
    NO_ENOUGH_MONEY         = 512,        //ETC卡余额不足
    GREEN_TO_NORMAL         = 1024,       //绿通变普通
    ENTRY_ERROR             = 2048,       //入口信息校验出错
    SPT                     = 4096,       //自助缴费终端
    CROSS_PROVINCE          = 8192,       //跨省
    REMOTE_ASSIST           = 16384,      //远程辅助
    ENTRY_EXIT_FLAG_ERROR   = 32768,      //入出口异常
};
Q_ENUM_CREATE(RecordType)
Q_END_ENUM_CREATE(EM_RecordType)

// 记录类型2
Q_BEGIN_ENUM_CREATE(EM_RecordType2)
enum RecordType2
{
    CPC_CARD_CAL         = 1,      // 根据CPC卡内数据计费
    SHORT_CAL            = 2,      // 最短可达路径计费
    SHORT_ERROR_CAL      = 4,      // 路径不可达（省内最远站点计费）
    BLACK                = 8,      // 状态名单车或状态名单卡
    NO_DISCOUNT_ETC      = 16,     // 单ETC卡无折扣
    FORBIDDEN_PASSENGER  = 32,     // 不允许通行的两客一危车辆
    PLATE_DIFFER         = 64,     // 卡车牌与抓拍车牌不符
    ETC_CAL_ERROR        = 128,    // 出口ETC累计金额异常
    OBU_CARD_DIFF        = 256,    // 卡签入口信息不符
    TEST_VEHICLE         = 512,    // 测试车
    SHORT_80_PERCENT     = 2048    // 兜底80%
};
Q_ENUM_CREATE(RecordType2)
Q_END_ENUM_CREATE(EM_RecordType2)

// 作废类型
Q_BEGIN_ENUM_CREATE(EM_ScrapType)
enum ScrapType
{
    NORMAL_TICKET         = 0, // 正常票
    REPRINT_TICKET        = 1, // 重打票
    ADJUSTED_TICKET       = 2, // 调整票
    TRADE_SCRAP           = 3, // 交易废票
    NON_TRADE_SCRAP       = 4, // 非交易废票
    BACKEND_TRADE_SCRAP   = 5  // 后台交易废票
};
Q_ENUM_CREATE(ScrapType)
Q_END_ENUM_CREATE(EM_ScrapType)


// 出口班次日期（班次类型）
Q_BEGIN_ENUM_CREATE(EM_ShiftDateType)
enum ShiftDateType
{
    SHIFT_MORNING   = 1, // 早班
    SHIFT_AFTERNOON = 2, // 中班
    SHIFT_NIGHT     = 3  // 晚班
};
Q_ENUM_CREATE(ShiftDateType)
Q_END_ENUM_CREATE(EM_ShiftDateType)


// 特情类型
Q_BEGIN_ENUM_CREATE(EM_SpecialType)
enum SpecialType
{
    OBU_REMOVED                    = 2,   // OBU拆卸
    OBU_EXPIRED                   = 3,   // OBU过期
    OBU_NOT_ENABLED               = 4,   // OBU未启用
    OBU_NO_CARD                   = 5,   // OBU无卡
    OBU_IN_BLACKLIST             = 6,   // OBU在状态名单里
    OBU_EF04_PREFIX_ERR          = 10,  // OBU-EF04内前缀异常(第320-322字节非AA2900)
    OBU_EF04_NO_CARD_CNT_GT0     = 11,  // OBU-EF04内标签无卡次数大于0
    OBU_EF04_ENTRY_INVALID       = 12,  // OBU-EF04内入口无效
    OBU_EF04_NO_PROV             = 13,  // OBU-EF04内通行省份数量为0
    OBU_AMT_GT_MINFEE_1_5X       = 14,  // OBU累计金额大于最小费额1.5倍
    OBU_AMT_LT_MINFEE            = 15,  // OBU累计金额小于最小费额
    OBU_CROSS_PROV_NO_AMT        = 16,  // 跨省交易出口省OBU本省累计金额为0
    FEE_MILE_ABNORMAL            = 18,  // 累计计费里程异常
    ETC_CARD_EXPIRED             = 21,  // ETC卡过期
    ETC_CARD_NOT_ENABLED         = 22,  // ETC卡未启用
    ETC_CARD_IN_BLACKLIST        = 24,  // ETC卡在状态名单内
    CARD_ENTRY_INVALID           = 26,  // ETC/CPC卡入口无效
    PREPAID_INSUFFICIENT_BAL     = 27,  // 储值卡余额不足
    ETC_CARD_ZERO_BALANCE        = 28,  // ETC卡余额为0
    CARD_AMT_GT_MINFEE_1_5X      = 29,  // 卡累计金额大于最小费额1.5倍
    CARD_AMT_LT_MINFEE           = 30,  // 卡累计金额小于最小费额
    CPC_DAMAGED                  = 35,  // CPC卡损坏
    CPC_MISSING                  = 36,  // 无CPC卡
    ETC_OBU_PLATE_MISMATCH       = 41,  // ETC卡与OBU车牌（含颜色）不符
    ETC_OBU_EF04_ENTRY_MISMATCH  = 45,  // ETC卡与OBU EF04内入口信息不一致
    VEHICLE_TYPE_MISMATCH        = 51,  // 出入口车辆车型不符
    PLATE_MISMATCH               = 52,  // 出入口车牌（含颜色）不符
    FITTING_FAILED               = 68,  // 拟合失败
    PLATE_IN_PENALTY_LIST        = 118, // 车牌在追缴状态名单
    NO_TAG_VEHICLE               = 123, // 无标签车辆
    FITTING_SUCCESS              = 500, // 拟合成功
    TEMP_PASS_BY_BLACKLIST       = 501, // 状态名单临时放行
    MANUAL_AXLE_LIMIT_INPUT      = 502, // 手工录入轴限
    NON_TRANSPORT_CONTAINER      = 511, // 非运输集装箱车
    CONTAINER_20FT               = 512, // 20英尺集装箱
    CONTAINER_2x20FT             = 513, // 2×20英尺集装箱
    CONTAINER_40_OR_45FT         = 514, // 40或45英尺集装箱
    VEHICLE_TYPE_FROM_ONLINE     = 520, // 使用在线稽核车型
    VEHICLE_TYPE_FROM_INPUT      = 521, // 使用现有输入车型
    DOUBLE_PERSON_REVIEW         = 522, // 双人复核
    CPC_WRITE_FAIL_PASS          = 523  // CPC写卡失败过车
};
Q_ENUM_CREATE(SpecialType)
Q_END_ENUM_CREATE(EM_SpecialType)


// 系统标识
Q_BEGIN_ENUM_CREATE(EM_SystemID)
enum SystemID
{
    SYS_NONE     = 0, // 系统无关
    SYS_LANE     = 1, // 车道
    SYS_STATION  = 2, // 收费站
    SYS_SUBCENTER= 3, // 分中心系统
    SYS_PROV     = 4  // 省中心系统
};
Q_ENUM_CREATE(SystemID)
Q_END_ENUM_CREATE(EM_SystemID)

// 用户类型
Q_BEGIN_ENUM_CREATE(EM_UserType)
enum UserType
{
    NORMAL_VEHICLE          = 0,  // 普通车
    MILITARY_POLICE         = 8,  // 军警车
    EMERGENCY               = 10, // 紧急车
    FLEET                   = 14, // 车队
    GREEN                   = 21, // 绿通车
    HARVESTER               = 22, // 联合收割机
    RESCUE                  = 23, // 抢险救灾车
    CONTAINER_J1            = 24, // 集装箱车(J1类)
    BULK                    = 25, // 大件运输车
    EMERGENCY_RESCUE        = 26, // 应急车
    TRAILER                 = 27, // 货车列车或半挂汽车列车
    CONTAINER_J2            = 28  // 集装箱车(J2类)
};
Q_ENUM_CREATE(UserType)
Q_END_ENUM_CREATE(EM_UserType)


// 车牌颜色
Q_BEGIN_ENUM_CREATE(EM_VehColor)
enum VehColor
{
    BLUE  = 1, // 蓝
    YELLOW = 2, // 黄
    WHITE = 3, // 白
    BLACK = 4  // 黑
};
Q_ENUM_CREATE(VehColor)
Q_END_ENUM_CREATE(EM_VehColor)


// 车辆违规类型
Q_BEGIN_ENUM_CREATE(EM_VehPeccType)
enum VehPeccType
{
    NORMAL = 0, // 正常
    BREAKTHROUGH = 1, // 冲关
    UNPAID = 2, // 未付
    EVADE_FEE = 3, // 逃费
    SPECIAL_SEARCH = 4, // 特殊情况配合查找
    LAW_BLACKLIST = 5, // 路政状态名单车
    MALICIOUS_OWE = 110, // 电子收费恶意欠费车辆
    ETC_ARREARS_LIST = 111, // ETC欠费状态名单
    BANK_ADVANCE_LIST = 112 // 银行垫付状态名单
};
Q_ENUM_CREATE(VehPeccType)
Q_END_ENUM_CREATE(EM_VehPeccType)


// 车辆状态
Q_BEGIN_ENUM_CREATE(EM_VehStatus)
enum VehStatus
{
    VEH_NORMAL = 0,            // 普通车
    VEH_OFFICIAL = 1,          // 公务车
    VEH_MILITARY = 2,          // 军警车
    VEH_FRUIT = 3,             // 鲜果车
    VEH_EMERGENCY = 4,         // 紧急车
    VEH_SPECIAL = 5,           // 特殊车
    VEH_FLEET = 11,            // 车队车
    VEH_EMERGENCY_RESCUE = 26, // 应急救援车辆
    VEH_HARVESTER = 101,       // 联合收割机
    VEH_RESCUE = 102,          // 运送救灾物资车辆
    VEH_HOLIDAY_FREE = 103,    // 节假日免征车
    VEH_VACCINE = 105,         // 疫苗车
    VEH_EMERGENCY2 = 106,      // 应急车
    VEH_TEMP_FREE = 107        // 临时免征车
};
Q_ENUM_CREATE(VehStatus)
Q_END_ENUM_CREATE(EM_VehStatus)


// 告警类型
Q_BEGIN_ENUM_CREATE(EM_WarnType)
enum WarnType
{
    WARN_BALANCE_LOW = 1,                   // 余额不足
    WARN_OBU_TAMPER_FAIL = 2,              // OBU防拆失效
    WARN_OBU_DISABLED = 4,                 // OBU禁用
    WARN_IC_DISABLED = 5,                  // IC卡禁用
    WARN_OBU_EXPIRED = 6,                  // OBU有效期过期
    WARN_IC_EXPIRED = 7,                   // IC卡有效期过期
    WARN_OBU_NOT_ENABLED = 8,              // OBU尚未启用
    WARN_IC_NOT_ENABLED = 9,               // IC卡尚未启用
    WARN_TIMEOUT = 10,                     // 超时
    WARN_BREAKTHROUGH = 11,                // 闯关
    WARN_NO_ENTRY_INFO = 12,              // 无入口信息
    WARN_TAG_NO_CARD = 13,                // 标签无卡
    WARN_IC_INVALID_ISSUER = 14,          // IC卡发行方无效
    WARN_OBU_INVALID_ISSUER = 15,         // OBU发行方无效
    WARN_NO_RATE_FOUND = 16,              // 查询不到费率
    WARN_IC_REPORTED_LOST = 18,           // IC卡挂失
    WARN_OBU_REPORTED_LOST = 19,          // OBU挂失
    WARN_LANE_INTERFERENCE = 20,          // 邻道干扰
    WARN_WRITE_CARD_FAIL = 40,            // 写卡故障无法交易
    WARN_DEVICE_FAULT = 41,               // 设备故障导致无法交易
    WARN_BLACKLIST_VEHICLE = 50,          // 状态名单车
    WARN_BLACKLIST_CARD = 51,             // 状态名单卡
    WARN_BLACKLIST_TAG = 52,              // 状态名单标签
    WARN_NO_TAG = 53,                     // 无电子标签
    WARN_U_TURN = 54,                     // U转车
    WARN_ENTRY_EXIT_FLAG_ERROR = 55,      // 进出站标志错
    WARN_UNKNOWN_MODEL = 56,              // 车型不明
    WARN_TAG_CARD_MISMATCH = 57,          // 标卡信息不符
    WARN_PLATE_MISMATCH = 58,             // 车牌不符车
    WARN_FREE_VEHICLE_EXPIRED = 59,       // 免费车到期
    WARN_FREE_CARD_EXPIRED = 60,          // 免费卡到期
    WARN_PROHIBITED_VEHICLE = 61,         // 禁止通行车辆
    WARN_OBU_DEVICE_FAULT = 71,           // OBU设备故障
    WARN_OBU_ANTENNA_FAIL = 72,           // OBU与天线通讯故障
    WARN_OTHER = 99,                      // 其它
    WARN_B5_FRAME_ERROR = 100,            // B5帧错误
    WARN_ETC_CARD_INVALID = 115,          // ETC卡无效
    WARN_AXLE_MODEL_MISMATCH = 144,       // 车轴型不符
    WARN_OVERSIZED_VEHICLE = 145          // 大件运输车
};
Q_ENUM_CREATE(WarnType)
Q_END_ENUM_CREATE(EM_WarnType)
