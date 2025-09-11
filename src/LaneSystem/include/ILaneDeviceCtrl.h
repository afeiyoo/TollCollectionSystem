#ifndef ILANEDEVICECTRL_H
#define ILANEDEVICECTRL_H

#include <QObject>
#include "../include/laneweight.h"
#include "../include/ccapturedef.h"

typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef unsigned int  UINT;

//设备类型
#define DEVICE_TYPE_IO           1   //开关量设备
#define DEVICE_TYPE_CARDREADER   2   //读卡器
#define DEVICE_TYPE_WEIGHT       3   //称重设备
#define DEVICE_TYPE_INFOBOARD    4   //情报板
#define DEVICE_TYPE_LED          5   //额显示器
#define DEVICE_TYPE_CARDROBOT    6   //自动发卡机

//开关量控制的设备
#define IO_RAILING       0x01      //栏杆机1
#define IO_PASSINGLAMP   0x02      //通行灯
#define IO_CEILINGLAMP   0x03      //天棚灯
#define IO_WARNING       0x04      //声光告警
#define IO_RAILING2      0x05      //栏杆机2

//设备初始化
typedef struct tagInitDeviceReq
{
   UINT seconds;
   char datetime[7];
   char cfgVersion[13];
   char reserve[8];
} TInitDeviceReq;

typedef struct tagInitDeviceResp
{
    UINT seconds;
    USHORT errCode;
    UINT errLen;
    char errDesc[64];
} TInitDeviceResp;

//发送设备配置
typedef struct tagDeviceConfig
{
   UINT seconds;
   char cfgVersion[13];
   UINT cfgDataLen;
   char cfgData[20480];
} TDeviceConfig;

//简单应答
typedef struct tagDeviceResp
{
    char operateID[20];
    UINT dataLen;
    char data[128];
} TDeviceResp;

//心跳
typedef struct tagDeviceheartInfo{
   UINT currentTime;
   char cfgVersion[13];
   char frmVersion[20];
   USHORT ioInputStatus;
   USHORT ioOutputStatus;
   BYTE ipcStatus;
   USHORT cpu;
   USHORT memory;
   USHORT hardDisk;
   USHORT io;
   USHORT temperature;
   UINT systemTime;
   char softVersion[20];
   char configVersion[20];
   USHORT workStatus;
   USHORT subStatus;
   USHORT errLen;
   char errDesc[512];

}TDeviceHeartInfo;


//开关量设备控制指令
typedef struct tagIODevCtrl{
    BYTE devType;                   //1-自动栏杆机,2-通行信号灯,3-天棚信号灯,4-声光报警,5-自动栏杆机2
    BYTE deviceStatus;               //0 关闭 1 打开
    char reserve[8];
}TIODevCtrl;

//线圈状态发生变化
typedef struct tagIOVdStatus{
    BYTE coilNum;
    USHORT coilStatus;//按车行方向顺序,依次对应相应位。如第一个和第三个线圈有信号,二进制值为0000 0000 0000 0101
    char reserve[8];
}TIOVdStatus;


//称重
typedef struct tagWeightCmd{
    BYTE cmd;
    BYTE idx;
    char reserve[8];
}TWeightCmd;

//行显示指令
typedef struct tagLedCtrlShowLine{
    BYTE screenLight;    //屏幕亮度，00H~0FH，其中00H为自动调节，亮度依次递增
    BYTE fontSize;       //字体颜色，1-绿色；2-红色；3-黄色
    char line1[32];
    char line2[32];
    char line3[32];
    char line4[32];
    char line5[32];
    char line6[32];
}TLedCtrlShowLine;


//文本显示指令
typedef struct tagLedCtrlShowText{
    BYTE screenLight;    //屏幕亮度，00H~0FH，其中00H为自动调节，亮度依次递增
    BYTE fontSize;       //字体颜色，1-绿色；2-红色；3-黄色
    BYTE lineCount;      //显示行数，01H~06H
    char text[128];      //显示内容
}TLedCtrlShowText;

//情报板
typedef struct tagInfoBoardInfo{
    BYTE openClose;  //情报板开关，0-关闭情报板；1-打开情报板
    BYTE fontColor;  //字体颜色，01H-绿色，02H-红色，03H-黄色
    BYTE fontSize;   //字体大小，01H~08H
    char text[256];   //显示内容
}TInfoBoardInfo;

//状态信息
typedef struct tagCardRobotStatus{
    BYTE upWork;              //上工位当前卡机：30H 无；31H 为 1#机；32H 为 2#机；
    BYTE downWork;           //下工位当前卡机：30H 无；33H 为 3#机；34H 为 4#机；
    BYTE boxStatus1;         //1#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE hasCardBag1;        //1#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT restCards1;         //1#卡机中卡计数值：“050”标示具有 50 张卡；
    BYTE hasAnnCard1;        //1#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
    BYTE boxStatus2;         //2#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE hasCardBag2;        //2#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT restCards2;         //2#卡机中卡计数值：“050”标示具有 50 张卡；
    BYTE hasAnnCard2;        //2#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
    BYTE boxStatus3;         //3#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE hasCardBag3;        //3#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT restCards3;         //3#卡机中卡计数值：“050”标示具有 50 张卡；
    BYTE hasAnnCard3;        //3#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
    BYTE boxStatus4;         //4#卡机状态：30H 卡机正常；31H 卡机故障；32H 卡机有坏卡；33H 卡机离线
    BYTE hasCardBag4;        //4#卡夹状态：30H 卡夹已装上；31H 卡夹已卸下；
    UINT restCards4;         //4#卡机中卡计数值：“050”标示具有 50 张卡
    BYTE hasAnnCard4;        //4#卡机轨道有卡状态:30H 无卡,31H 天线有卡,32H 卡口有卡
}TCardRobotStatus;

typedef struct tagCardBoxInfo{
    //卡夹信息
    UINT boxNo1;                 //1#卡机卡夹编号（无为: 0x00,0x00,0x00,0x00）
    USHORT cardNum1;               //1#卡机卡数信息
    UINT boxNo2;
    USHORT cardNum2;
    UINT  boxNo3;
    USHORT cardNum3;
    UINT boxNo4;
    USHORT cardNum4;

}TCardBoxInfo;

// 纸券取走及状态信息    2024-09-03
typedef struct tagPaperStatus{
    BYTE WorkStation; // 工位，0：默认工位 1：上工位 2：下工位
    BYTE Cause; // 原因，0：正常取走纸券 1：设备状态
    char PaperNum[20];  // 纸券号
    BYTE StatusUp;  // 上工位打印机状态
    BYTE StatusDown;    // 下工位打印机状态
} TPaperStatus;


class ILaneDeviceCtrl:public QObject
{
    Q_OBJECT
public:
    explicit ILaneDeviceCtrl(){
        qRegisterMetaType<BYTE>("BYTE");
        qRegisterMetaType<USHORT>("USHORT");
        qRegisterMetaType<UINT>("UINT");
        qRegisterMetaType<TInitDeviceResp>("TInitDeviceResp");
        qRegisterMetaType<TDeviceResp>("TDeviceResp");
        qRegisterMetaType<TIOVdStatus>("TIOVdStatus");
        qRegisterMetaType<TDeviceHeartInfo>("TDeviceHeartInfo");
        qRegisterMetaType<TVehicleWeightInfo>("TVehicleWeightInfo");
        qRegisterMetaType<TCardRobotStatus>("TCardRobotStatus");
        qRegisterMetaType<TCardBoxInfo>("TCardBoxInfo");
        qRegisterMetaType<TInitDeviceReq>("TInitDeviceReq");
        qRegisterMetaType<TDeviceConfig>("TDeviceConfig");
        qRegisterMetaType<TCapVehInfo>("TCapVehInfo");
        qRegisterMetaType<TPaperStatus>("TPaperStatus");
    }
signals:
    //设备服务指令
    void initDeviceResp(TInitDeviceResp resp);
    void deviceResp(TDeviceResp resp);
    void deviceRestart();
    void vdStatus(TIOVdStatus ioVdStatus);
    void heartInfo(TDeviceHeartInfo deviceHeartInfo);
    void weightInfo(TVehicleWeightInfo vehicleWeightInfo);
    void cardRobotInfo(BYTE nCmdType,BYTE nWorkStation,BYTE BoxNo,TCardRobotStatus cardRobootStatus,TCardBoxInfo cardBoxInfo, TPaperStatus paperStatus);
    void captureInfo(QByteArray capVehInfo);
    void simulate_capture(void* capInfo);
    int simulate_rsu(QByteArray aHead,QByteArray aFrame);
    void initLaneNotice();
    //车道主服务指令
    int initDevice(TInitDeviceReq req);
    int configDevice(TDeviceConfig cfg);
   //车道控制器
    int ceilingLampCtrl(bool isOn);
    int railingCtrl(bool isUp);
    int passingLampCtrl(bool isOn);
    int warningCtrl(bool isOn);
    int railingCtrl2(bool isUp);
    //情报板
    int showWelcomeInfo(BYTE color, QString sMsg);
    int showCloseInfo();
    int showRepairInfo();
    /** 2025-07-16 昊星情报板新增 */
    void sigPlayFile(int num);
     //自动发卡机
    void send_a_cmd(int nCards);//初始化指令
    void send_b_cmd(BYTE boxNo);//出卡指令,boxNo正常出卡为0,取坏卡1~4
    void send_c_cmd(BYTE boxNo); //坏卡回收,boxNo一般填0,需要时还可以填5上工位,6下工位坏卡出卡
    void send_k_cmd(); //查询卡机属性指令
    void send_r_cmd(BYTE boxNo);//卡机自动备卡设置指令,boxNo为卡机编号1~4，使用卡夹时必须使用该指令
    void send_g_cmd();//  苏州朗为 按键取消
    void send_l_cmd(BYTE boxNo);// 苏州朗为 触发按键
    void send_p_cmd(BYTE boxNo);//  苏州朗为 将指定通道卡口处的卡回收到天线处
    void send_s_cmd(BYTE boxNo, BYTE nDo); // 金晟安 机械臂控制
    void send_vd_cmd(BYTE nStatus);  //地感
    void send_ui_cmd(BYTE nUiNo,QString uiInfo); //界面
    void send_wav_cmd(BYTE nVoiceNum,BYTE nCnt); //语音
    void send_url_cmd(int UpMinute,QString sUrl);//设置上传URL
    void send_paper_cmd(BYTE workstation, QString data);      // 打印纸券指令
    //费显
    void printText(QString text);
    void printLines(QString s1, QString s2, QString s3, QString s4,QString s5 = "",QString s6 = "");
    void ledCtrlWarning(BYTE ctrlCmd,USHORT ctrLTime);
    void ledCtrlPassingLamp(BYTE ctrlCmd);
    void ledVoice(QString text);
      //称重
    int sendWeightCmd(int cmd, int idx);
    //抓拍
    void capTrigger();
    void sendCharOverlap(int nRow,int nCol,QString text);
};



#endif // ILANEDEVICECTRL_H
