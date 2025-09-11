#ifndef CCAPTUREDEF_H
#define CCAPTUREDEF_H





//抓拍类型
#define CAP_TYPE_NONE      0
#define CAP_TYPE_HW        1
#define CAP_TYPE_GDW_IP    2
#define CAP_TYPE_GDW_CARD  3
#define CAP_TYPE_GDW_HIGH  4
#define CAP_TYPE_XNW_HD    5  //信诺威高清
#define CAP_TYPE_GDW_NEWHD 6  //高德威高清(新版)
#define CAP_TYPE_SOCKET    7  //新协议,网络通讯


//特征图数据不低于100K(扩大4倍-高清) 409600/409600/5120
#define SPECIAL_IMG_LEN  409600
//全景图不低于100K(扩大24倍-高清)
#define FULL_IMG_LEN  409600
//车牌图不低于5K
#define PLATE_IMG_LEN 5120
//二值化图不低于280字节(长20宽14,一个点用一个字节表示)
#define TWO_IMG_LEN 280
#define HW_IP_LEN  16
#define HW_PLATE_LEN  64

#define BIZ_RET_F1  0        //发送F1正应答
#define BIZ_RET_F2  1        //发送F2负应答




typedef unsigned char BYTE;
typedef unsigned int  UINT;
typedef unsigned short USHORT;

typedef struct tag_CapVehInfo
{
  char chHWYIP[HW_IP_LEN];
  char chPlate[HW_PLATE_LEN];
  int  nColor;
  BYTE chSpecialImage[SPECIAL_IMG_LEN];
  int  nSpecialLen;
  BYTE chFullImage[FULL_IMG_LEN];
  int  nFullLen;
  BYTE chPlateImage[PLATE_IMG_LEN];
  int  nPlateLen;
  BYTE chTwoImage[TWO_IMG_LEN];
  int  nTwoLen;
  UINT CameraTime; //Unix
  BYTE chVehicleColor;  //车身颜色
  BYTE chVehicleClass; //车型
  char Reserve[9];   //备用
} TCapVehInfo;

typedef struct tag_heartInfo   //E1
{
  char CameraIP[17];
  char StationHex[9];
  BYTE LaneNum;
  UINT CurrentTime;
  BYTE WorkStatus;
  char Reserve[9];
}THeartInfo;

typedef struct tagCapCmd{
   BYTE OldRsctl;
   BYTE NewRsctl;
   BYTE CmdType;
   BYTE Version;
   UINT DataLen;
} TCapCmd;


#endif // CCAPTUREDEF_H
