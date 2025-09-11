//---------------------------------------------------------------------------

#ifndef uSPTCtrlH
#define uSPTCtrlH
//#include <QtCore>
#include <QObject>
//---------------------------------------------------------------------------

//typedef unsigned char BYTE;
//typedef unsigned int  UINT;
//typedef unsigned short USHORT;

typedef struct __SptOperation {
    int       cmd;       // 1 应答  2 插卡  3  用户选择  4  卡取走  5 状态
    int       pos;       // 工位  1 上   2 下
    int       option;    // 选项值
    char      code[64];  // 支付码 等等
    int       rsv;
    unsigned char pic[409600];  //
} TSptOperation;

class ISptTcpListener
{
public:
    virtual void OnSptMsgNotify(TSptOperation sptInfo) = 0;
};


class ISPTCtrl:public QObject
{
    Q_OBJECT
public:
    //打开设备
    virtual int Open(ISptTcpListener *sptListener,const char *ServIP,int nPort)=0;
    //关闭设备
    virtual void Close()=0;
    //输出日志配置
    virtual void SetLogger(const char *logFile, int logLevel)=0;
    //
    virtual bool Send2View(int vid, int timeout, int errcode, const char * msg) = 0;
    virtual bool Send2Voice(int vid, int cnt, const char * msg, int interval = 0) = 0;
    virtual bool Send2RejectCard(int pos)  = 0 ;
    virtual bool Send2Print(int pos, const char * msg)  = 0;
    virtual bool Send2EatCard(int pos)  = 0;
    virtual bool Send2ChangeCoilStatus(int sts)  = 0;
    virtual bool Send2ChangeRailingStatus(int sts) = 0;
    virtual bool Send2OcrTicket(int pos) = 0;
signals:
    void RecSptInfo(TSptOperation sptInfo);

};


extern "C" ISPTCtrl*  GetSPTCtrl(int type,int laneid);

#endif


