#ifndef CMGSLANEGANTRY_H
#define CMGSLANEGANTRY_H

#include <QtCore>
#include "fcdefine.h"
#include <QSqlDatabase>
#include "umgslanegantry.h"
#include "RollingFileAppender.h"
#include "Logger.h"

typedef int (*libfeecalc_init)(const char *curTollGantry, const char *soPath);
typedef int (*libfeecalc_deInit)();
typedef int (*libfeecalc_getLibVersion)(char *version, int verLen,
                                        char *dataVersion, int dataVerLen);
typedef int (*libfeecalc_getFeeDataVersion)(char *version, int verLen);
typedef int (*libfeecalc_getFee)(const TradingInfo &tradingInfo,
                                 FeeInfo &feeInfo);
typedef int (*libfeecalc_loadFlagFee)(
        const T_ParamVersion *p1, int p1cnt, const T_ETCFlag *p2, int p2cnt,
        const T_FlagFee *p3, int p3cnt, const T_HolidayFree *p4, int p4cnt,
        const T_FF_Node *p5, int p5cnt, const T_FF_NodeRelation *p6, int p6cnt,
        const T_ParamAxisRatio *p7, int p7cnt, const T_DiscountFee *p8, int p8cnt);
typedef int (*libfeecalc_getFlagFee)(const FlagTradingInfo &tradingInfo,
                                     FlagFeeInfo &feeInfo);

struct TGrantryDBCfg{
    QString dbName;
    QString dbUser;
    QString dbPwd;
    QString dbHost;
    int LanedID;
};

class CMgsLaneGantry : public IMgsLaneGantry
{
private:
    RollingFileAppender *m_fileAppender;
    QString m_ErrMsg;
    QLibrary *_handle;
    int m_index;
    QSqlDatabase db;
    TGrantryDBCfg gantryDB;
    libfeecalc_init _libfeecalc_init;
    libfeecalc_deInit _libfeecalc_deInit;
    libfeecalc_getLibVersion _libfeecalc_getLibVersion;
    libfeecalc_getFeeDataVersion _libfeecalc_getFeeDataVersion;
    libfeecalc_getFee _libfeecalc_getFee;
    libfeecalc_loadFlagFee _libfeecalc_loadFlagFee;
    libfeecalc_getFlagFee _libfeecalc_getFlagFee;
    void ConnectDB(TGrantryDBCfg dbCfg);
    void CheckDB();
    int GetParamVersion(int paramtype);
    T_ParamVersion* Load_T_ParamVersion(int *cnt);
    T_ETCFlag* Load_T_ETCFlag(int *cnt);
    T_FlagFee* Load_T_FlagFee(int *cnt, int ver);
    T_HolidayFree* Load_T_HolidayFree(int *cnt);
    T_FF_Node* Load_T_FF_Node(int *cnt);
    T_FF_NodeRelation* Load_T_FF_NodeRelation(int *cnt, int ver);
    T_ParamAxisRatio* Load_T_ParamAxisRatio(int *cnt, int ver);
    T_DiscountFee* Load_T_DiscountFee(int *cnt, int ver);
    QString NormalizeFlagID(char *itrvid);
    QString NormalizeSpecialType(char *sp, int rateCompute, int fitResult);
    void LogGantryTradeInfo(GantryTradeInfo *info);
    void LogGantryFeeInfo(GantryFeeInfo *info);
    QString GBKArray2String(char *gbk);
    bool is_str_utf8(const char *str);
    QString NormalizeIntervalID(char *itrvid);
    QByteArray String2GBKArray(const QString &s);
public:
    CMgsLaneGantry();
    // 获取函数调用错误信息，建议分配足够空间存放errmsg
    bool MgsLaneGantry_GetLastErr(char *errmsg);
    // 初始化,载入动态库，调用一次
    bool MgsLaneGantry_Init(char *dbHost,char *dbName,char *dbUser,char *dbPwd,int laneId,int index=0);
    // 上班时调用，载入初始参数
    bool MgsLaneGantry_LoadParam();
    // 补点算费，传入交易信息，传出补点算费信息，ETC出口车道调用判断是否需要补点
    bool MgsLaneGantry_GetFlagFee(GantryTradeInfo *t, GantryFeeInfo *f);
    // 车道新版本参数生效后调用，传入参数类型，版本号
    bool MgsLaneGantry_LoadNewParam(int paramtype, int newver);
};


#endif // CMGSLANEGANTRY_H
