#ifndef ETCRAMPMIXINC_H
#define ETCRAMPMIXINC_H
#include <QtCore>
#include "lanepubdefine.h"
#include "uRampRsuCtrl32.h"


// ETC全国联网定义
#define EXCEPT_TRADE_LESSMONEY 1 //余额不足
#define EXCEPT_OBU_STATUS 2      // OBU状态错
#define EXCEPT_BLACK_VEH 50      //状态名单车
#define EXCEPT_CARD_BLACK 51     //状态名单卡*
#define EXCEPT_OBU_BLACK 52      // OBU状态名单
#define EXCEPT_OBU_EXPIRE 6      // OBU超期
#define EXCEPT_CARD_EXPIRE 7     //卡的过期时间
#define EXCEPT_OBU_INACTIVE 8    // OBU未启用*
#define EXCEPT_CARD_INACTIVE 9   //卡未启用*
#define EXCEPT_DRIVE_TIMEOUT 10  //超时车
#define EXCEPT_TRADE_PECC 11     //冲关车
#define EXCEPT_INOUT_FLAG 55     //进出站标志错
#define EXCEPT_OBU_NOCARD 13     //标签无卡*
#define EXCEPT_CARD_ISSUER 14    // IC卡发行方无效
#define EXCEPT_OBU_ISSUER 15     // OBU发行方无效
#define EXCEPT_TRADE_ENERR 16 //查询不到费率(入口不明,费率计算结果为0或为负数)*
#define EXCEPT_CALFEE_ERR 17  //费率计算错误(2015.08.31新增)
#define EXCEPT_CARD_LOST 18   // IC卡挂失*(福建无此情况)
#define EXCEPT_OBU_LOST 19    // OBU挂失*(福建无此情况)
#define EXCEPT_RSU_DISTURB 20 //邻道干绕*
#define EXCEPT_WRITE_CARD 40  // B4帧写卡错误
#define EXCEPT_LOGICAL_OTHER  99 //其它逻辑错误*(出现以下福建省自定义错语时,上报交通部使用这个错误)
#define EXCEPT_PHY_ERROR 100 //物理出错(写卡成功但交易失败B5帧)*
//福建省自定义
#define EXCEPT_LIMIT 61             //车辆禁止通行
#define EXCEPT_LOCATION_ILLEGAL 111 //卡标归属地不一致
#define EXCEPT_VEHPLATE_NOTSAME 57  //卡标车牌不一致
#define EXCEPT_DRIVE_U 54           // U转车
#define EXCEPT_CARD_TYPE 115        //卡类型错
#define EXCEPT_TRADE_ERROR 116      //交易失败(B5帧返回错误)
//#define EXCEPT_OBU_VEHCLASS        117   //OBU车型不明
#define EXCEPT_OBU_VEHCLASS 56    // OBU车型不明
#define EXCEPT_FREE_TIMEOUT 118   //超出免征期限
#define EXCEPT_FREE_SCOPE 119     //超出免费区域
#define EXCEPT_NO_OBU 53          //无OBU
#define EXCEPT_CMD_ERROR 121      // RSU指令帧错
#define EXCEPT_SYS_ERROR 122      //系统错(未知)
#define EXCEPT_OBU_ERROR 71       // OBU设备故障
#define EXCEPT_NETID_EREROR 123   //入网编号不明
#define EXCEPT_LAW_BALCK 124      //路政状态名单
#define EXCEPT_LAW_TRAILER 125    //拖挂车
#define EXCEPT_LAW_BULK 126       //大件运输车
#define EXCEPT_LAW_WEIGHT 127     //无称重数据
#define EXCEPT_STOP_WEIGHT 128    //超限称重数据
#define EXCEPT_USERTYPE_LTC 129   //出口绿通车
#define EXCEPT_USERTYPE_LHSGJ 130 //出口联合收割机
#define EXCEPT_LAW_JZX 131        //集装箱J1
#define EXCEPT_LAW_JZX2 132       //集装箱J2
#define EXCEPT_OBU_FEE 133        // OBU计费信息异常
#define EXCEPT_OBU_EN 134         //入口信息异常
#define EXCEPT_OBU_PROV 135       // OBU通行省份个数不对
#define EXCEPT_OBU_FEE0 136       // OBU计费信息异常(累计金额为0)
#define EXCEPT_ERR_FEE 137        //实收金额超大
#define EXCEPT_ERR_WEIGHT 138     //入口称重信息异常
#define EXCEPT_ERR_AXIS 139       //超6轴
#define EXCEPT_ERR_KSJZX 140       //跨省集装箱
#define EXCEPT_ERR_RETRADE 141       //重复交易
#define EXCEPT_ERR_NOAXIS 142       //罕见轴型

typedef struct tagMixTradeInfo{
    TRampB2Data B2Data;
    TRampB3Data B3Data;
    TRampB4Data B4Data;
    TRampB5Data B5Data;
    int VehClass;//计费车型
    int VehStatus;//车种
    int UserType;//用户类型
    int ShouldPay;//应收
    int FactPay;//实收
    int OpFactPay;//外省实收
    bool bHolidayFree;//是否节假日免征车
    bool bFreeVeh;//
    int MerRebateType;
    int RebatedPay;
    int FFFactPay; //门架实收
    char LedHint[256];
    char PassID[50];
    char SpecialType[256];
    int DealStatus;
    char sLine1[50];
    char sLine2[50];
    char sLine3[50];
    char sLine4[50];
}TMixTradeInfo;

//ETC业务回调接口,供ETC业务处理主模块调用
class IETCCallBack
{
public:
    //获取当前车辆重量  返回0允许继续交易，返回小于0不允许继续交易; =-99 罕见轴型; =-2 超限; =-1无称重;=-3车牌与抓拍不符 nFlag:(1:大件运输车)
    virtual int getCurVehWeight(int &axisType,int &tollWeight,int &limitWeight,QString &axisWeight,bool &isWeightLow, int &nFlag,QString vehPlate="",QString sEnStation="",BYTE VehicleStatus=0x00)=0;
    //检查相同车牌是否重复交易 0：允许交易，不等于0不允许交易
    virtual int checkDuplicateTrade(QString vehPlate="")=0;
    //状态名单检查(isCard=false表示sID为obusn)//暂不做，都由ETC自己判断
    virtual int checkBlackList(QString sID,bool isCard=true)=0;
    //拖挂车车型确认(返回-1即关闭天线,0则交易继续),无颜色车牌
    virtual int checkTrailerClass(QString vehPlate="")=0;
    //集装箱车辆类型确认(返回-1即关闭天线,0则交易继续),无颜色车牌
    virtual int checkContainerType(QString vehPlate="")=0;
    //状态名单、状态名单临时放行(返回-1即关闭天线,0则临时放行),无颜色车牌
    virtual int tempPassing(QString vehPlate="")=0;
    //交易结果通知(errCode=0表示交易成功,tradeShowInfo用于界面显示,用map表示),errCode值是异常定义EXCEPT_
    virtual void tradeNotice(TMixTradeInfo mixTradeInfo,int errCode,QString errDesc="")=0;

    //交易状态通知  status=0 交易开始;status=1 交易已发送C6; status=2 废票成功
	virtual int tradeStatusNotice(int status,UINT obuid)=0;
	//天线状态通知(天线连接是否正常，PSAM卡授权是否正常) 0:天线正常,PSAM卡已授权  1:天线正常,PSAM卡未授权 2:天线异常
    virtual void rsuStatus(int status)=0;
    //车牌入口复位允许交易标记 0:允许交易 -1不允许交易
    virtual int CheckEnExFlag(QString noColorPlate) = 0;
};

//混合车道ETC业务(由ETC模块实现)
class IMixETCBiz
{
public:
    //初始化(参数需要修改,传入baseenv,bizenv,当前车道号，是否开放段，承载门架编号等等)
    //注意要能与现有ETC车道的配置整合
  //  virtual bool initETCLane(TCommonEnv *env,IETCCallBack* etcCallBack,int laneType,int laneID,QString virEnStation="")=0;
    //开天线
    virtual int openMixRSU(bool changeBeaconID=false)=0;
    //关天线
    virtual int closeMixRsu()=0;
    //交班接
    virtual int manueMixShift(QString operatorID,QDateTime shiftDate,int shiftID)=0;
    //获取OBU信息(===obuinfo和cardinfo参数类型待修改===) 0获取到该车牌OBU信息，小于0未获取到OBU信息  flag>=1:删除队列里数据 其他获取队列数据
    virtual int getETCInfo(TMixTradeInfo &mixTradeInfo,QString vehPlate="", int flag=0)=0;
    //保存当前交易
    virtual int saveMixCurTrade(QVariantMap tradeMap)=0;
	// 废票操作
    virtual int cancelMixOBUTrade(QString obuPlate) = 0;
	//获取PSAM卡信息  查看PSAM卡信息界面显示
    virtual void getPsamInfo(QString &hexTermNo,QString &psamNum,bool &isAuth)=0;
};


#endif // ETCRAMPMIXINC_H
