//---------------------------------------------------------------------------

#ifndef uVehRecognizerCtrlH
#define uVehRecognizerCtrlH
//#include <QtCore>
#include <QObject>
//---------------------------------------------------------------------------


//车型识别器返回的视频最大长度
#define IMAGE_INFO_MAX_LEN  2*1024*1024
//#define BIZ_RET_F1  0        //发送F1正应答
//#define BIZ_RET_F2  1        //发送F2负应答


typedef unsigned char BYTE;
typedef unsigned int  UINT;
typedef unsigned short USHORT;


//车型信息(
typedef struct tag_VehRecoTypeInfo
{
  char VehPlate[20];	//车牌
  int  VehColor;   		//车牌颜色： 0蓝色; 1黄色; 2黑色; 3白色; 4渐变绿色; 5黄绿双拼色; 6蓝白渐变色; 9未确定
  char VehTime[15]; 		//过车时间，BCD码， YYYYMMDDhhmmss
  int  VehClass;     	//车型： 客车1-4   货车11-16   专项作业车21-26
  char AxleType[20];  	//轴型，字符串格式，并以\0结尾
  int  AxleCount;     	//轴数
  int TotalLength;  	//车货总长度，单位厘米 BCD码       0~999999，无法识别填0x000000
  int TotalWidth;      	//总宽度，单位厘米 BCD码，          0~999999，无法识别填0x000000
  int TotalHeight;   	//总高度，单位厘米 BCD码，     0~999999，无法识别填0x000000
  int ExtFlag;          /*扩展标识。默认0无扩展标识  从低位开始按位存放扩展信息，最多32个扩展信息，可叠加。
                            1    拖挂车标识
                            2    两客一危标识     */
  char Reserve[9];   	//备用
} TVehRecoTypeInfo;

//图片信息
typedef struct tag_VehRecoImageInfo
{
  char VehPlate[20];    //车牌，gb2312字符串格式
  int  VehColor;       	//车牌颜色： 0蓝色; 1黄色; 2黑色; 3白色; 4渐变绿色; 5黄绿双拼色; 6蓝白渐变色; 9未确定
  char VehTime[15];   	//过车时间，YYYYMMDDhhmmss
  int  ImageType;    	//类型：1车头JPG图像、2车尾JPG图像、3车身JPG图像、4短视频MP4，其他值不处理
  int ImageLen;     	//长度：图像不超过300KB，短视频不超过1MB。
  BYTE	ImageInfo[IMAGE_INFO_MAX_LEN];  //图像或短视频数据
  char Reserve[9];   //备用
} TVehRecoImageInfo;

//心跳信息
typedef struct tag_VehRecoHeartInfo
{
  char CameraIP[17];     //识别器IP地址
  char StationHex[9];	//收费站编码
  BYTE LaneNum;    		//车道号
  UINT CurrentTime;   	//当前时间(unix时间)
  BYTE WorkStatus;  	//  工作状态: 0.正常 1.故障 2.时间同步异常(时差大于10min) 4.识别率异常（小于98%）
  char Reserve[9];      //备用
}TVehRecoHeartInfo;


typedef void ( *VehRecognizerMsgFunc) (int);

/*
    车型识别器有数据需要上报时，通过SendMessage()发送消息给主程序
    其中 (int) Message.WParam;
    0 、心跳信息
    1、车型信息(
    2、图片信息
*/

class IVehRecognizerCtrl:public QObject
{
    Q_OBJECT
public:
   //打开设备
   virtual int OpenVehRecognition(VehRecognizerMsgFunc CallBackFun,char *ServIP,int nPort)=0;
   //关闭设备
   virtual void CloseVehRecognition()=0;
   //输出日志配置
   virtual void SetLogger(const char *logFile, int logLevel)=0;
   //初始化
   virtual int SetConfig(char *StationHex,BYTE LaneNum,char *Reserve)=0;
   //设置状态上报URL
   virtual int SendStatusUploadURL(BYTE UpMinute,char *URL,int Len)=0;
//   //获取心跳状态
//   virtual int CheckStatus()=0;
   //获取车辆图片信息
   virtual int  GetVehImageInfo(TVehRecoImageInfo *veh) = 0;
   //获取车型信息
   virtual int  GetVehTypeInfo(TVehRecoTypeInfo *veh) = 0;
   //获取心跳状态
   virtual int GetHeartInfo(TVehRecoHeartInfo *hInfo) = 0;
   // LED显示
   virtual int ShowInfo(unsigned char color, char* data) = 0;
   // 语音播报
   virtual int PlayInfo(int count, char* text, int interval=3000) = 0;

signals:
   void RecRecoInfo(int Type);

};


extern "C" IVehRecognizerCtrl*  GetVehRecognizerCtrl();

#endif


