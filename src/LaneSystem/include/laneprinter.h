#ifndef LANEPRINTER_H
#define LANEPRINTER_H


class ILanePrinter {
public:

    virtual int Open() = 0;
    virtual int Close() = 0;
    virtual int print(const char *OutStationName, const char *InStationName, const char *VehClass,const char *FactPay,const char *ExceedRate,const char *OperatorID,const char *ExDate,const char *ExTime, const char *PrintID) = 0;
    // 出口 入口 车型 金额 超限比例 工号 时间 打印号
    virtual int GetStatus() = 0;
    virtual int feed() = 0;
    virtual int SetComPort(const char* ComPort,int BaudRate) = 0; //串口时调用
    virtual int SetBasePort(long basePort) = 0;//并口时候调用
    //virtual int SetInitEveryTime(bool flag) = 0;
    virtual void SetType(int Type) = 0;//1 ds , 2 star
    virtual void Release() = 0;
    virtual int printTaxTicket(const char *OutStationName, const char *InStationName,const char *VehClass,const char *FactPay,const char *OperatorID, const char *ExDate,const char *ExTime,const char *PrintID) = 0; // 出口 入口 车型 金额 工号 时间 打印号


};

extern "C" ILanePrinter *GetLanePrinterCOM();
extern "C" ILanePrinter *GetLanePrinterLPT();

#endif // LANEPRINTER_H



