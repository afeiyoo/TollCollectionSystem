//---------------------------------------------------------------------------
//Module：自动发卡机处理模块  Author: Huanglr FromDate:2014-08-08
//Remark:
//---------------------------------------------------------------------------
#ifndef uOutCardRobotH
#define uOutCardRobotH
//---------------------------------------------------------------------------

//#include "windows.h"
#include <stdio.h>
#include <QtCore>

typedef unsigned char BYTE;
typedef unsigned int  UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned short Word;


#define MSG_CMD_TYPE_0     '0'    //正回应
#define MSG_CMD_TYPE_1     '1'    //负回应
#define MSG_CMD_TYPE_A     'A'    //上电信息
#define MSG_CMD_TYPE_B     'B'    //卡机状态信息,LPARAM为TRobotStatus*指针
#define MSG_CMD_TYPE_D     'D'    //按键取卡,LPARAM高字节为工位信息:1或2;低字节为当前卡机编号1~4
#define MSG_CMD_TYPE_C     'C'    //已出卡,  LPARAM高字节为工位信息:1或2,3出卡失败;低字节为当前卡机编号1~4
#define MSG_CMD_TYPE_E     'E'    //卡被取走,LPARAM高字节为工位信息:1或2;低字节为当前卡机编号1~4
#define MSG_CMD_TYPE_Q     'Q'    //卡夹信息,LPARAM为TCardBagInfo*指针
#define MSG_CMD_TYPE_K     'K'    //卡夹属性回应,LPARAM返回0不包含异或校验,1包含
#define MSG_CMD_TYPE_R     'R'    //自动备卡回应,LPARAM返回1~4卡机编号
#define MSG_CMD_TYPE_G     'G'    //卡回收完成,LPARAM高字节为工位信息:1或2,3;低字节为当前卡机编号1~4
#define MSG_CMD_TYPE_H     'H'    //按键忽略、延时,LPARAM高字节为工位信息:1或2;低字节为忽略或延时原因
                                  //(0:正在出卡/回收;1:无卡可发;2:卡机故障;3:一边故障，一边无卡;4:降级模式,不处理按键
                                  //5:取卡走不到3秒;6:连续多张坏卡(2张))
#define MSG_CMD_TYPE_I     'I'    // 纸券取走及状态信息 2024-09-03
#define MSG_CMD_TYPE_Z     'Z'    //LPARAM整条命令

//卡机各种运行选项(按位存储)
#define OPT_XOR_CHECK       1     //指令XOR校验

#define OPT_ARM_BACK        0     //机械臂收回
#define OPT_ARM_OUT         1     //机械臂伸出

//语音编号
#define WAV_TYPE_INIT        10
#define WAV_TYPE_NHQAJQK     11  //你好，请按键取卡
#define WAV_TYPE_QSH         12  //请稍侯
#define WAV_TYPE_QQK         13  //请取卡
#define WAV_TYPE_XXYLPA      14  //谢谢一路平安
#define WAV_TYPE_KJGZQAQZJ   21  //卡机故障，请按求助键
#define WAV_TYPE_KJWKQAQZJ   22  //卡机无卡，请按求助键
#define WAV_TYPE_XGWGZQCSGWQK   23  //下工位故障，请从上工位取卡
#define WAV_TYPE_XGWWKQCSGWQK   24  //下工位无卡，请从上工位取卡
#define WAV_TYPE_SGWGZQCXGWQK   25  //上工位故障，请从下工位取卡
#define WAV_TYPE_SGWWKQCXGWQK   26  //上工位无卡，请从下工位取卡
#define WAV_TYPE_CKYC           27  //出卡异常
#define WAV_TYPE_CXWFSBQSH      31  //车型无法识别，请稍候
#define WAV_TYPE_WCPQSH         32  //无车牌，请稍候
#define WAV_TYPE_ZXWFSBQSH      33  //轴型无法识别，请稍候
#define WAV_TYPE_QFCLQAQZJ      34  //欠费车辆，请按求助键
#define WAV_TYPE_ZHYCQAJQK      35  //ETC账户异常，请按键取卡
#define WAV_TYPE_LZHMDCQAQZJ    36  //路政黑名单车，请按求助键
#define WAV_TYPE_CXCQAQZJ       37  //超限车，请按求助键
#define WAV_TYPE_WCZSJQAQZJ     38  //无称重数据，请按求助键
#define WAV_TYPE_YCCLQAQZJ      39  //异常车辆，请按求助键
#define WAV_TYPE_TGCQSH         40  //拖挂车，请稍候

//界面类型
#define UI_TYPE_NORMAL          1  //正常界面
#define UI_TYPE_FAULT           2  //故障界面
#define UI_TYPE_WARN            3  //异常界面

#define VD_HAD    0x30  //有车
#define VD_NOHAD  0x31  //无车

//struct TMessage
//{
//    unsigned Msg;
//    union
//    {
//        struct
//        {
//            Word WParamLo;
//            Word WParamHi;
//            Word LParamLo;
//            Word LParamHi;
//            Word ResultLo;
//            Word ResultHi;

//        };
//        struct
//        {
//            int WParam;
//            int LParam;
//            int Result;

//        };

//    };
//} ;

typedef struct tag_msgInfo{
    Word LParamLo;
    Word LParamHi;
}TMsgInfo;

//卡位状态
typedef struct tag_BoxStatus
{
    BYTE BoxStatus;         //卡位状态:0正常 1故障 2有坏卡
    bool HasCardBag;        //是否装有卡夹
    int  RestCards;         //剩余卡数(包括天线区卡)
    bool HasAnnCard;        //天线区是否有卡
} TBoxStatus;

//卡机状态
typedef struct tag_RobotStatus
{
   BYTE AboveBoxNo;         //上工位当前卡机: 0无卡, 1或2号卡机
   BYTE BelowBoxNo;         //下工位当前卡机: 0无卡 3或4号卡机
   TBoxStatus BoxStatus[4];    //分别表示1~4号卡位状态
} TRobotStatus;

typedef struct tag_CardBagInfo
{
   int CardBagNo[4];       //卡夹编号(与下面的卡数一一对应)
   short Cards[4];         //卡数
} TCardBagInfo;



class IOutCardRobot:public QObject
{
    Q_OBJECT
public:
    virtual int  OpenRobot(QString sComPort,int nRate,char* logFile,int logLevel)=0;
    virtual int OpenRobotEx(char* sIP,int nPort,char* logFile,int logLevel)=0;
    virtual void SetOption(int nOption)=0;
    virtual void send_0_cmd(BYTE nRsctl)=0;         //正应答,nRsctl为接收报文中的nRsctl
    virtual void send_1_cmd(BYTE nRsctl)=0;         //负应答,nRsctl为接收报文中的nRsctl
    virtual void send_a_cmd(int nCards)=0;          //初始化指令
    virtual void send_b_cmd(BYTE boxNo)=0;          //出卡指令,boxNo正常出卡为0,取坏卡1~4
    virtual void send_c_cmd(BYTE boxNo)=0;          //坏卡回收,boxNo一般填0,需要时还可以填5上工位,6下工位坏卡出卡
    virtual void send_k_cmd()=0;                    //查询卡机属性指令
    virtual void send_r_cmd(BYTE boxNo)=0;          //卡机自动备卡设置指令,boxNo为卡机编号1~4，使用卡夹时必须使用该指令
    virtual void CloseRobot()=0;                    //关闭卡机
    virtual QString  GetComPort()=0;                //获取串口号
    virtual void TestCmd(char *hexCmd)=0;           //指令调试
    virtual void send_g_cmd()=0;                    //GDD  苏州朗为 按键取消
    virtual void send_l_cmd(BYTE boxNo)=0;          //GDD 苏州朗为 触发按键
    virtual void send_p_cmd(BYTE boxNo)=0;          //GDD  苏州朗为 将指定通道卡口处的卡回收到天线处
    virtual void send_s_cmd(BYTE boxNo,BYTE nDo)=0; //GDD 金晟安 机械臂控制
    virtual void send_pre_cmd()=0;
    virtual void send_vd_cmd(BYTE nStatus)=0;
    virtual void send_ui_cmd(BYTE nUiNo,char* uiInfo)=0;
    virtual void send_wav_cmd(BYTE nVoiceNum,BYTE nCnt)=0;
    virtual void send_url_cmd(int UpMinute,char* sUrl)=0;
    virtual void send_paper_cmd(BYTE workStation, char* data) = 0;  // 打印纸券指令
public:
    virtual void QueryRobot()=0;                    //查询卡机
    virtual void ResetRobot()=0;                    //初始化卡机
    virtual void SwitchWorkNo(BYTE boxNo,BYTE IsCanRead)=0;     //切换工位(读写)
//    virtual void setCallbackFunc(void(*func) (BYTE nCmdType,void* pFrame))=0;
signals:
    void data_from_robot(BYTE nCmdType,QByteArray pFrame);
};

extern "C"  Q_DECL_EXPORT IOutCardRobot*  GetOutCardRobot(int nType);

//typedef void (*FuncCallbackFunc)(BYTE nCmdType,void* pFrame);
//void myFuncCallbackOutRobot(BYTE nCmdType,void* pFrame);

#endif
