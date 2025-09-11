#ifndef LANEWEIGHT_H
#define LANEWEIGHT_H

#include <QByteArray>



//#include<QDateTime>


enum LaneWeightCmd
{
  Cmd_WeightAllAx = 0,        //0：读取一辆车的整车信息(轴组方式)
  Cmd_WeightAll = 1,          //1：读取一辆车的整车信息(单轴方式)
  Cmd_WeightAx = 2,           //2：只读取一辆车的轴组重信息
  Cmd_Weight = 3,             //3：只读取一辆车的轴重信息
  Cmd_WeightNum = 4,          //4：读取子机数据缓冲区车辆数信息
  Cmd_WeightStatus = 5,       //5：轴重仪自检
  Cmd_WeightExit = 6,         //6：车道车辆队列中最后一辆车退出收费车道(数据流方向：从机->主机)
  Cmd_WeightZero = 7,         //7：设备置零
  Cmd_WeightDel = 8,          //8：删除从机数据缓冲区中最前面一条数据
  Cmd_WeightIdxAx = 9,        //9：从从机数据缓冲区中读取指定序号的车辆称重数据(轴组方式)
  Cmd_WeightIdx = 10,         //10：从从机数据缓冲区读取指定序号的车辆称重数据(单轴方式)
  Cmd_WeightTime = 11         //11：重新设定从机时间(消息格式见后)
};

struct TVehicleWeightInfo
{
  int cmd;                  // 命令
  int idx;                  // 序号
  int err;                  // 0成功 1失败
  int carnum;               // 缓冲区数量
  int status;               // 设备状态
  int year;
  int mon;
  int day;
  int hour;
  int min;
  int sec;                  //
  int flag;                 // 超限标志
  int velocity;             // 速度
  int acceleration;         // 加速度
  int axnum;                  // 轴数
  int axgnum;                // 轴组数
  int weight[256];            // 轴重
  int axtype[256];            // 轴型
  //int axlen[8];             // 轴距
};


typedef bool ( *WeightFunc) (TVehicleWeightInfo *);

class ILaneWeight
{
public:
    virtual int Open(const char *ComPort, int nRate, WeightFunc proc /* 数据回调 */,int WeightAddr) = 0;
    virtual int Close() = 0;
    virtual int SendCmd(int cmd, int idx) = 0;
    virtual void Release() = 0;                          //释放资源
    virtual void dealBuffer(const QByteArray& buffer) = 0;
    //virtual QDateTime GetWeihtDateTime() = 0;
};


extern "C"  ILaneWeight*  GetLaneWeight();




#endif // LANEWEIGHT_H
