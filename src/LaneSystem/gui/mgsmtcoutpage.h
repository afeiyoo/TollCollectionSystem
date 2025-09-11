#pragma once

#include "gui/component/mgsmenu.h"
#include "gui/mgsbasepage.h"

#include <QHBoxLayout>

class MgsAuthDialog;
class MgsMtcOutPage : public MgsBasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit MgsMtcOutPage(QWidget *parent = nullptr);
    ~MgsMtcOutPage() override;

    // 交易信息显示区域API
    void setStartTicketNum(int num) override;       // 起始票据
    void setCurTicketNum(int num) override;         // 当前票据
    void setNormalTicketCnt(int cnt) override;      // 正常票数
    void setScrapTicketCnt(int cnt) override;       // 废票张数
    void setRestTicketCnt(int cnt) override;        // 剩余票据
    void setRecycleCardCnt(int cnt) override;       // 回收卡数
    void setThirdPayCnt(int cnt) override;          // 第三方支付
    void setTotalVehCnt(int cnt) override;          // 总过车数
    void setTotalCardCnt(int cnt) override;         // 总发卡数
    void setNoFlagCardCnt(int cnt) override;        // 错标卡数
    void setPeccanyVehCnt(int cnt) override;        // 冲关车数
    void setFleetVehCnt(int cnt) override;          // 车队车数
    void setCpcCardCnt(int cnt) override;           // 通行卡数
    void setEtcCardCnt(int cnt) override;           // 闽通卡数
    void setBadCardCnt(int cnt) override;           // 坏卡数
    void setPaperCardCnt(int cnt) override;         // 发纸券数
    void setHolidayFreeVehCnt(int cnt) override;    // 动免车数
    void setNormalVehCnt(int cnt) override;         // 普通车数
    void setFreeVehCnt(int cnt) override;           // 免征车数
    void setTotalToll(qreal toll) override;         // 总收费
    void setCreditCardCnt(int cnt) override;        // 记账卡数
    void setPrePayCardCnt(int cnt) override;        // 储值卡数
    void setLastShiftTotalVehCnt(int cnt) override; // 上个班次总车次

    // 当前车辆信息显示区域API
    void setPlate(const QString &plate) override;
    void setVehClass(const QString &vehClass) override;
    void setVehStatus(const QString &vehStatus) override;
    void setSituation(const QString &situation) override;

    // 当前车辆卡内信息显示区域API
    void setCardType(const QString &cardType) override;           // 卡类型
    void setCardNum(const QString &cardNum) override;             // 卡号
    void setBalance(const QString &balance) override;             // 卡内余额
    void setEnTime(const QString &enTime) override;               // 入口时间
    void setEnStationName(const QString &enStationName) override; // 入口站名
    void setCardStatus(const QString &cardStatus) override;       // 卡状态
    void setWeightInfo(const QString &info) override;             // 称重
    void setEnPlate(const QString &plate) override;               // 入口车牌
    void setLabel1(const QString &info) override;                 // 标签1
    void setLabel2(const QString &info) override;                 // 标签2
    void setToll(const QString &toll) override;                   // 收费

    // 交易提示区域API
    void setTradeHint(const QString &tradeHint, const QString &color = "#007bff") override;
    void setObuHint(const QString &obuHint, const QString &color = "#08c134") override;
    void appendHintButton(const QString &hint,
                          const QString &fontColor = "#ffffff",
                          const QString &bgColor = "#08c134") override;

public slots:
    void onPlateChanged(const QString &plate);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    MgsPageArea *initTradeInfoArea() override;
    MgsPageArea *initVehInfoArea() override;
    MgsPageArea *initCardInfoArea() override;
    MgsPageArea *initTradeHintArea() override;
    MgsRecentTradePanel *initRecentTradeArea() override;

private:
    // 交易信息显示区域
    ElaText *m_startTicketNum = nullptr;
    ElaText *m_curTicketNum = nullptr;
    ElaText *m_normalTicketCnt = nullptr;
    ElaText *m_scrapTicketCnt = nullptr;
    ElaText *m_restTicketCnt = nullptr;
    ElaText *m_totalVehCnt = nullptr;
    ElaText *m_recycleCardCnt = nullptr;
    ElaText *m_etcCardCnt = nullptr;
    ElaText *m_fleetVehCnt = nullptr;
    ElaText *m_thirdPayCnt = nullptr;
    // 当前车辆信息显示区域
    ElaText *m_plate = nullptr;     // 车牌
    ElaText *m_vehClass = nullptr;  // 车型
    ElaText *m_vehStatus = nullptr; // 车种
    ElaText *m_situation = nullptr; // 特情信息
    // 当前车辆卡信息显示区域
    ElaText *m_cardType = nullptr;      // 卡类型
    ElaText *m_cardNum = nullptr;       // 卡号
    ElaText *m_enStationName = nullptr; // 入口站名
    ElaText *m_enTime = nullptr;        // 入口时间
    ElaText *m_weightInfo = nullptr;    // 称重信息
    ElaText *m_enPlate = nullptr;       // 入口车牌
    ElaText *m_label1 = nullptr;        // 标签1
    ElaText *m_splitProvincesInfo = nullptr;        // 标签2
    // 交易提示区域
    QHBoxLayout *m_hintButtonLayout = nullptr;
    MgsScrollText *m_tradeHint = nullptr;
    ElaText *m_obuHint = nullptr;

    // 子窗口
    MgsMenu *menu = nullptr;
    MgsAuthDialog *m_authDialog = nullptr;
};
