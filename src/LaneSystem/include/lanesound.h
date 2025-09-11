#ifndef LANESOUND_H
#define LANESOUND_H
#include<QObject>
/*
int PlayMAV(int SoundType,int MsgType)
SoundType: 1 通行费   MsgType:0--999
SoundType: 2 车型     MsgType:1--6类车
SoundType: 3 车情
MsgType:1             车牌不符
MsgType:2:            U转车
MsgType:3:      超时车
MsgType:4:      变更车
MsgType:5:      未付车
MsgType:6:      免费车
MsgType:7:      超限车
SoundType: 4 卡机
MsgType:1:      请取卡
MsgType:2:      卡机故障请走入工通道
MsgType:3:      卡机无卡请走入工通道
MsgType:4:      上工位卡机故障,请从下工位取卡
MsgType:5:      下工位卡机故障,请从上工位取卡
MsgType:6:      上工位卡机无卡,请从下工位取卡
MsgType:7:      下工位卡机无卡,请从上工位取卡
MsgType:8:      闽通卡读写失败，请取通行卡
SoundType: 5  入口即出口，文明用语
MsgType:1:      //您好
MsgType:2:      //请稍后
MsgType:3:      //谢谢
MsgType:4:      //一路平安
MsgType:5:      //请按键取卡
MsgType:6:      //请取卡
*/

class ILaneSound :public QObject{
public slots:
    virtual int init(QString WAVPath) = 0;
    virtual void PlaySound(QString FileName) = 0;// 语音文件名 test.wav
    virtual int PlayMAV(int SoundType,int MsgType) = 0;
};

extern "C" ILaneSound *GetLaneSound();

#endif // LANESOUND_H
