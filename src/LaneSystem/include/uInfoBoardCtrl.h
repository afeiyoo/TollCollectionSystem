/*---------------------------------------------------------------------------
Module: ETC车道情报板控制
Author：Huanglr
Version：
   0.1 支持三思+新软情报板控制
---------------------------------------------------------------------------*/
#ifndef uInfoBoardCtrlH
#define uInfoBoardCtrlH
//---------------------------------------------------------------------------
//#include "windows.h"

class IInfoBoardCtrl
{
public:
   virtual int OpenBoard(const char * nPort,int nRate,int DevType)=0;  //打开情报板,DevType为厂家类型
   virtual void CloseBoard()=0;                     //关闭情报板
   virtual int GetComPort()=0;
   virtual int ShowWelcomeInfo()=0;  //显示"电子收费车道"
   virtual int ShowCloseInfo()=0;    //显示"车道关闭"信息
   virtual int ShowRepairInfo()=0;   //显示"车道维护"信息
};

extern "C" IInfoBoardCtrl* GetInfoBoardCtrl();


#endif
