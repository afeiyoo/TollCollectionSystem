#ifndef ILANEFORMCTRL_H
#define ILANEFORMCTRL_H

#include <QObject>
#include "ILaneCardCtrl.h"
typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef unsigned int  UINT;

//界面编号定义
#define UI_MAIN         10	//主界面
#define UI_LOGIN        20	//登录界面
#define UI_EDIT_PLATE   21	//修改车牌界面
#define UI_CASH_CHANGE  22	//找零界面
#define UI_LT_QRCODE    23	//绿通二维码
#define UI_AXIS_CHANGE  24	//车型轴更改
#define UI_FLEET        25	//车队过车界面
#define UI_STATION      26	//输入站代码
#define UI_HOLIDAY      27	//节假日过车
#define UI_TICKET       28  //票据查看界面
#define UI_TRAILER      29  //拖车界面
#define UI_SCAN_PAY     30	//扫码支付界面
#define UI_ASSIST       31  //远程辅助界面（车型）
#define UI_AXIS_ASSIST  32  //轴型辅助界面
#define UI_CHECK_UPDATE 33  //检测更新
#define UI_WEB_VIEW     34  //展示网页的界面
#define UI_CHECK_AUTH   35  //检查车道签权
/** 2024-04-26 start 集装箱预约信息界面 */
#define UI_JZX_DETAIL   36
#define UI_JZX_CLEAN_PIC 37
#define UI_JZX_DETAIL_AGAIN 38
/** 2024-04-26 end */
/** 2024-06-04 start 嫌疑车辆证据界面 */
#define UI_EVIDENCE             39
#define UI_EVIDENCE_CLEAN_PIC   40
/** 2024-06-04 end */
#define UI_WAIT                 41  //等待界面
#define UI_WAIT_CLOSE           42  //关闭等待界面
#define UI_VEH_PATH_MAP         43  //显示计费路径
#define UI_VEH_PATH_MAP_CLEAN_PIC 44 //清空计费路径地图
#define UI_BULK_CONFIRM         45   //大件运输车确认
#define UI_BULK_MANUAL_INPUT    46   //大件运输车手工确认
#define UI_SHIFT          47   //工班信息输入
#define UI_SPECIAL_CARDS   48   //特请卡录入

#define UI_HINT_SHIFTVEH 89 // 提示班次车辆不符
#define UI_HINT         90	//提示界面
#define UI_MENU         91	//菜单选择界面
#define UI_HINT_MENU    92	//提示+菜单选择界面
#define UI_HINT_INPUT   93	//提示+输入框
#define UI_HINT_GRANT   94	//提示+授权
#define UI_HINT_TABLE   95	//提示+列表
#define UI_CONFIG       96  //配置界面

//数据类型
#define  DT_SHIFT    1   //班次
#define  DT_TRADE    2  //交易数据
#define  DT_HINT1    3   //提示
#define  DT_HINT2    4   //提示2
#define  DT_TITLE    5  //头部显示
#define  DT_BLACK    6  //状态名单显示
#define  DT_WEIGHTINFO   7  //称重信息显示
#define  DT_LOWHINT  8  //底部提示
#define  DT_PIC      9  //图片
#define  DT_DEVICE   10  //设备
#define  DT_LOG      11  //日志
#define  DT_SHIFTPRE 12  //上一班次信息
#define  DT_WEIGHTDOWN 13  //降级信息
#define  DT_WEIGHTCOUNT 14  //称重缓冲区数
#define  DT_FLAGINFO 15  //承载门架信息
#define  DT_FLLOWINFO 16  //当前流程信息
#define  DT_TICKETINFO 17  //票据信息
#define  DT_LASTTRADE 18  //上一交易
#define  DT_USETYPE 19  //车种
#define  DT_VEHTYPE 20  //车辆类型 客车模式、货车模式
#define  DT_LANEMODE 21  //车道模式 自动发卡模式，人工发卡模式
#define  DT_CLEAR 22  //清除界面
#define  DT_VEHCLASS 23  //车型
#define  DT_CARDROBOT 24  //自动发卡机
#define  DT_SETDEVICEORDER 25  //设置设备图标顺序
#define  DT_SETVDSTATUS 26  //设置地感状态
#define  DT_SETFLEET 27  //车队显示
#define  DT_SETHOLIDAY 28  //节假日显示
#define  DT_CLOUDPAY 29   //云支付 刷新提示
#define  DT_QueCodeList 30 //绿通二维码列表
#define  DT_CPU  31 //身份卡刷新
#define  DT_STOP_ASSIST 32//停止辅助
#define  DT_APP_VERSION 33  //软件版本
#define  DT_FEE_VERSION 34 //费率版本
#define  DT_SAVE_END  99 //保存数据库完成
#define  DT_STOP_EXE  90 //关闭程序
#define  DT_SHOW_HELP 91 // 车道求助

//字体颜色
#define UIFONTCOLOR_BLACK 0X00
#define UIFONTCOLOR_RED 0X01
#define UIFONTCOLOR_BLUE 0X02
#define UIFONTCOLOR_GREEN 0X03
#define UIFONTCOLOR_GRAY 0X04

//界面初始化
typedef struct tagInitFormReq
{
   UINT seconds;
   char datetime[7];
   char cfgVersion[13];
   char reserve[8];
} TInitFormReq;

typedef struct tagInitFormResp
{
    UINT seconds;
    BYTE errCode;
    UINT errLen;
    char errDesc[64];
} TInitFormResp;

//发送界面配置
typedef struct tagFormConfig
{
   UINT seconds;
   char cfgVersion[13];
   UINT cfgDataLen;
   char cfgData[20480];
} TFormConfig;

//简单应答
typedef struct tagFrontResp
{
    int operateID;
    UINT dataLen;
    char data[128];
} TFrontResp;

//主界面显示
typedef struct tagMainShowData
{
   BYTE dataColor;
   BYTE dataType;
   BYTE dataFormat;
   UINT dataLen;
   QByteArray data;
} TMainShowData;

typedef struct tagInterActiveData
{
    int operateID;
    int UI;
    int infoLen;
    QByteArray info;
} TInterActiveData;


//class CSigFormToService : public QObject
//{
//   Q_OBJECT
//signals:

//};

class ILaneFormCtrl: public ILaneCardCtrl
{
    Q_OBJECT
public:
    explicit ILaneFormCtrl()
    {
        qRegisterMetaType<BYTE>("BYTE");
        qRegisterMetaType<USHORT>("USHORT");
        qRegisterMetaType<UINT>("UINT");
        qRegisterMetaType<TInitFormReq>("TInitFormReq");
        qRegisterMetaType<TFormConfig>("TFormConfig");
        qRegisterMetaType<TMainShowData>("TMainShowData");
        qRegisterMetaType<TInitFormResp>("TInitFormResp");
        qRegisterMetaType<TFrontResp>("TFrontResp");
    }
signals:
    //主服务指令消息
    int initForm(TInitFormReq req);
    int configForm(TFormConfig cfg);
    int mainShow(TMainShowData data);
    int clearShow(BYTE dataType);
    int interactive(int operateid,int nUI,QString info);
    int printTicket(QString printData);
    int fetchPrintStatus();
    int playVoiceFile(QString fileName);
    int playSpecialVoice(BYTE soundType,BYTE msgType,QString reserve);
    //前端界面指令消息
    void initFormResp(TInitFormResp resp);
    void frontResp(TFrontResp resp);
    void formRestart();
    void keyPress(UINT keyCode);
    void printResp(BYTE errCode,QString errDesc);
    void printStatus(BYTE errCode);
    //socket连接信号
    void tcpsocket(bool bConnect);
};



#endif // ILANEFORMCTRL_H
