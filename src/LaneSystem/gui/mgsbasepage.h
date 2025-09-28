#pragma once

#include "utils/defs.h"

#include <QPlainTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

class MgsWeightInfoPanel;
class WeightInfoItem;
class MgsIconButton;
class MgsDevicePanel;
class MgsRecentTradePanel;
class MgsScrollText;
class MgsPageArea;
class ElaImageCard;
class ElaPushButton;
class ElaToolButton;
class ElaText;
class MgsBasePage : public QWidget
{
    Q_OBJECT
public:
    explicit MgsBasePage(QWidget *parent = nullptr);
    virtual ~MgsBasePage() override;

    // 主显示区域初始化
    void initUi();

    // 顶部信息栏API
    void setStationInfo(const QString &stationInfo);
    void setUserInfo(const QString &userInfo);
    void setLaneID(uint laneID);
    void setShiftInfo(const QString &shiftInfo);
    void setModeText(const QString &mode);

    // 底部状态栏API（版本信息）
    void setFullBlackVer(const QString &ver);
    void setPartBlackVer(const QString &ver);
    void setVirtualGantryInfo(const QString &info);
    void setAppVer(const QString &ver);
    void setFeeRateVer(const QString &ver);

    // 抓拍及客货流程，情报板区域API
    void setCapImage(const QImage &img);
    void setVehMode(const QString &vehMode, const QString &color = "#007bff");
    void setInfoBoard(const QString info, const QString &color = "#1f9d5e");

    // 交易信息显示区域API
    virtual void setStartTicketNum(int num) = 0;       // 起始票据
    virtual void setCurTicketNum(int num) = 0;         // 当前票据
    virtual void setNormalTicketCnt(int cnt) = 0;      // 正常票数
    virtual void setScrapTicketCnt(int cnt) = 0;       // 废票张数
    virtual void setRestTicketCnt(int cnt) = 0;        // 剩余票据
    virtual void setRecycleCardCnt(int cnt) = 0;       // 回收卡数
    virtual void setThirdPayCnt(int cnt) = 0;          // 第三方支付
    virtual void setTotalVehCnt(int cnt) = 0;          // 总过车数
    virtual void setTotalCardCnt(int cnt) = 0;         // 总发卡数
    virtual void setNoFlagCardCnt(int cnt) = 0;        // 错标卡数
    virtual void setPeccanyVehCnt(int cnt) = 0;        // 冲关车数
    virtual void setFleetVehCnt(int cnt) = 0;          // 车队车数
    virtual void setCpcCardCnt(int cnt) = 0;           // 通行卡数
    virtual void setEtcCardCnt(int cnt) = 0;           // 闽通卡数
    virtual void setBadCardCnt(int cnt) = 0;           // 坏卡数
    virtual void setPaperCardCnt(int cnt) = 0;         // 发纸券数
    virtual void setHolidayFreeVehCnt(int cnt) = 0;    // 动免车数
    virtual void setNormalVehCnt(int cnt) = 0;         // 普通车数
    virtual void setFreeVehCnt(int cnt) = 0;           // 免征车数
    virtual void setTotalToll(qreal fee) = 0;          // 总收费
    virtual void setCreditCardCnt(int cnt) = 0;        // 记账卡数
    virtual void setPrePayCardCnt(int cnt) = 0;        // 储值卡数
    virtual void setLastShiftTotalVehCnt(int cnt) = 0; // 上个班次总车次

    // 滚动提示显示区域API
    void setScrollTip(const QString &tip);

    // 当前车辆与卡内信息显示区域API
    virtual void setPlate(const QString &plate) = 0;                 // 抓拍车牌
    virtual void setVehClass(const QString &vehClass) = 0;           // 车型
    virtual void setVehStatus(const QString &vehStatus) = 0;         // 车种
    virtual void setSituation(const QString &situation) = 0;         // 特情
    virtual void setCardType(const QString &cardType) = 0;           // 卡类型
    virtual void setCardNum(const QString &cardNum) = 0;             // 卡号
    virtual void setBalance(const QString &balance) = 0;             // 卡内余额
    virtual void setEnTime(const QString &enTime) = 0;               // 入口时间
    virtual void setEnStationName(const QString &enStationName) = 0; // 入口站名
    virtual void setCardStatus(const QString &cardStatus) = 0;       // 卡状态
    virtual void setWeightInfo(const QString &info) = 0;             // 称重
    virtual void setEnPlate(const QString &plate) = 0;               // 入口车牌
    virtual void setLabel1(const QString &info) = 0;                 // 标签1
    virtual void setLabel2(const QString &info) = 0;                 // 标签2
    virtual void setToll(const QString &toll) = 0;                   // 收费

    // 交易提示区域API
    virtual void setTradeHint(const QString &tradeHint, const QString &color = "#007bff") = 0;
    virtual void setObuHint(const QString &obuHint, const QString &color = "#000000") = 0;
    virtual void appendHintButton(const QString &hint,
                                  const QString &fontColor = "#ffffff",
                                  const QString &bgColor = "#08c134")
        = 0;

    // 称重信息显示区域API
    void setCurWeightInfo(const QString &curWeightInfo);
    void setCurWeightInfoCount(const QString &curWeightInfoCount);
    void appendWeightInfoItem(const WeightInfoItem &item);
    void setWeightLow(bool isLow);

    // 近期交易记录查看区域API
    void appendTradeItem(const QStringList &trade);

    // 设备图标显示区域API
    void setDeviceList(const QList<uint> &devList);

public slots:
    void onLogAppend(EM_LogLevel::LogLevel logLevel, const QString &log);

protected:
    // 抓拍与流程区域初始化
    MgsPageArea *initDisplayArea();
    // 交易信息区域初始化（由子类各自实现）
    virtual MgsPageArea *initTradeInfoArea() = 0;
    // 日志显示区域初始化
    QPlainTextEdit *initLogBrowseArea();
    // 滚动提示区域初始化
    MgsPageArea *initScrollTipArea();

    // 车辆及卡内信息区域初始化（由子类各自实现）
    virtual MgsPageArea *initVehInfoArea();

    // 卡内信息区域初始化（由子类各自实现）
    virtual MgsPageArea *initCardInfoArea(); // TODO 待删除

    // 交易提示信息区域初始化（由子类各自实现）
    virtual MgsPageArea *initTradeHintArea();
    // 称重信息区域初始化
    MgsPageArea *initWeightInfoArea();
    // 近期交易记录查看区域初始化（由子类各自实现）
    virtual MgsRecentTradePanel *initRecentTradeArea();
    // 设备状态栏区域初始化
    MgsDevicePanel *initDeviceIconArea();

    // 各子类公用，界面截图
    void screenShot();

private:
    // 创建顶部信息栏
    void createTopWidget();
    // 添加主显示区
    void addCentralWidget(QWidget *w);
    // 创建底部状态栏
    void createBottomWidget();

    void initLeftUi();
    void initRightUi();

private:
    // 顶部信息栏
    QWidget *m_topWidget = nullptr;
    ElaToolButton *m_stationInfo = nullptr; // 站信息
    ElaToolButton *m_userInfo = nullptr;    // 用户信息
    ElaToolButton *m_laneID = nullptr;      // 车道号
    ElaToolButton *m_shiftInfo = nullptr;   // 班次信息
    ElaPushButton *m_modeText = nullptr;    // 车道模式

    // 底部信息栏
    QWidget *m_bottomWidget = nullptr;
    ElaText *m_fullBlackVer = nullptr;      // 全量状态名单版本
    ElaText *m_partBlackVer = nullptr;      // 增量状态名单版本
    ElaText *m_virtualGantryInfo = nullptr; // 承载门架信息
    ElaText *m_appVer = nullptr;            // 软件版本
    ElaText *m_feeRateVer = nullptr;        // 费率版本

    // 主区域
    QWidget *m_mainWidget = nullptr;
    QSplitter *m_leftLayout = nullptr;  //  左侧主布局
    QSplitter *m_rightLayout = nullptr; // 右侧主布局

    // 抓拍与流程显示区域
    MgsPageArea *m_displayArea = nullptr;
    ElaImageCard *m_capImage = nullptr;
    ElaText *m_vehMode = nullptr;
    ElaText *m_infoBoard = nullptr;

    // 交易信息显示区域
    MgsPageArea *m_tradeInfoArea = nullptr;

    // 日志显示区域
    QPlainTextEdit *m_logBrowser = nullptr;
    QStringList m_logBuffer;

    // 滚动提示显示区域
    MgsPageArea *m_scrollTipArea = nullptr;
    MgsScrollText *m_scrollTip = nullptr;

    // 当前车辆信息显示区域
    MgsPageArea *m_vehInfoArea = nullptr;

    // 当前车辆卡内信息显示区域
    MgsPageArea *m_cardInfoArea = nullptr; // TODO 待删除

    // 交易提示区域
    MgsPageArea *m_tradeHintArea = nullptr;

    // 称重信息显示区域
    MgsPageArea *m_weightInfoArea = nullptr;
    ElaText *m_curWeightInfo = nullptr;
    ElaText *m_curWeightInfoCount = nullptr;
    MgsIconButton *m_weightLow = nullptr;
    MgsWeightInfoPanel *m_weightInfoPanel = nullptr;

    // 近期交易记录查看区
    MgsRecentTradePanel *m_recentTradePanel = nullptr;

    // 设备图标显示区域
    MgsDevicePanel *m_deviceIconPanel = nullptr;
};
