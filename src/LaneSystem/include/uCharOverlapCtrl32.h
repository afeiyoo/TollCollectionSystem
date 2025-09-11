/*---------------------------------------------------------------------------
Module:字符叠加模块
Author:Huanglr
Version:
   0.1  2014-09-03 海康字符叠加
   0.2  2015-03-21 增加新软-金盛安
   0.3  2015-05-17 增加华工,各厂家代码单元分开
Remark:
---------------------------------------------------------------------------*/
#ifndef uCharOverlapCtrl32H
#define uCharOverlapCtrl32H
#include <QtCore>

typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef unsigned int  UINT;


//字符叠加类型
#define COL_TYPE_NONE    0   //无
#define COL_TYPE_HK      1   //海康
#define COL_TYPE_JSA     2   //新软
#define COL_TYPE_HG      3   //华工1(ETCIO箱中带的)
#define COL_TYPE_HG2     4   //华工2(水官李戈新)
#define COL_TYPE_YS      5   //广州英沙
//---------------------------------------------------------------------------
class ICharOverlapCtrl32:public QObject
{
    Q_OBJECT
public:                                           //灵活叠加
   virtual int   OpenOverlap(char * com_port,int BaudRate)=0;     //打开叠加器
   virtual void  CloseOverlap()=0;                //关闭叠加器
   virtual void  ClearOverlap()=0;                //清除叠加器
   virtual void  SetBigChar(BYTE row)= 0;         //设置第row行为大字体
   virtual void  ShowTime()=0;                    //显示时间
   virtual void  HideTime()=0;                    //隐藏时间
   virtual void  SetDatePos(BYTE dayRow,BYTE dayCol,BYTE timeRow,BYTE timeCol)=0;  //设置日期显示位置
   virtual void  SetCurDate()=0;                  //设置当前时间(用于工控机与叠加器校时,每5钟进行一次,实际上下班设置一次即可)
   virtual void  Overlap(int nRow, int nCol, char *info)=0;           //字符叠加
   virtual void  CleanRow(int nRow, int nStartCol)=0;  //用空格刷空一行(从nCol开始刷)
   virtual int   GetComPort()=0;                  //获取串口号
};

extern "C" ICharOverlapCtrl32* GetCharOverlapCtrl32(BYTE nType);

#endif
