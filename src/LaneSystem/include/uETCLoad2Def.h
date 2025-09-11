
//---------------------------------------------------------------------------

#ifndef uETCLoad2DefH
#define uETCLoad2DefH

//---------------------------------------------------------------------------

#include "../include/CXCardCtrlDef.h"


#define ERR_OK           0
#define ERR_RETCODE      1     //报文返回错误码
#define ERR_RETCHECK     2     //接收数据不完整或解码失败
#define ERR_SOCKETINIT   3
#define ERR_SOCKETCONN   4
#define ERR_SOCKETSEND   5
#define ERR_SOCKETRECV   6
#define ERR_PROGRAM      99    //程序异常出错
//---------------------------------------------------------------------------
class IETCLoad2
{
public:
   //StationID:收费站代码；LaneID:车道号，loadUrl:圈存服务器URL；
   virtual int Init(int StationID,int LaneID,char* loadUrl,int httpType)=0;
   //圈存签到,返回TermCode,WorkKey
   virtual int SignIn()=0;
   //获取可圈存额度   执行前初始化 TCardLoadData (hexCardNo,hexTermNo,LoadMoney=0,LoadTime)
   virtual int GetAmountReq(TCardLoadData& clData,QString &Msg)=0;
   //圈存请求        执行前初始化 TCardLoadData
   virtual int LoadReq(TCardLoadData& clData,QString &Msg)=0;
   //圈存结果报告(目的是校验TAC)    执行前初始化 TCardLoadData
   virtual int LoadReport(TCardLoadData& clData,QString &Msg)=0;
};


extern "C"  IETCLoad2* GetETCLoad2Ctrl();

#endif
