#include "mgsbasepage.h"

#include "ElaWidgetTools/ElaIconButton.h"
#include "ElaWidgetTools/ElaImageCard.h"
#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaText.h"
#include "ElaWidgetTools/ElaToolBar.h"
#include "ElaWidgetTools/ElaToolButton.h"
#include "Logger.h"
#include "global/constant.h"
#include "gui/component/mgsdevicepanel.h"
#include "gui/component/mgsiconbutton.h"
#include "gui/component/mgspagearea.h"
#include "gui/component/mgsrecenttradepanel.h"
#include "gui/component/mgsscrolltext.h"
#include "gui/component/mgsweightinfopanel.h"
#include "gui/mgsplateeditdialog.h"
#include "utils/datadealutils.h"
#include "utils/fileutils.h"
#include "utils/uiutils.h"

#include <QHBoxLayout>
#include <QTimer>

using namespace Utils;

MgsBasePage::MgsBasePage(QWidget *parent)
    : QWidget(parent)
{
    createTopWidget();
    createBottomWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 3);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_topWidget);
    mainLayout->addStretch();
    mainLayout->addWidget(m_bottomWidget);

    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("mainWidget");
    m_mainWidget->setStyleSheet(QString("#mainWidget { background-color: %1; }").arg(Constant::Color::MAIN_BG));
    UiUtils::disableMouseEvents(m_mainWidget);
    addCentralWidget(m_mainWidget);
}

MgsBasePage::~MgsBasePage() {}

void MgsBasePage::createTopWidget()
{
    m_topWidget = new QWidget(this);
    m_topWidget->setObjectName("topWidget");
    m_topWidget->setStyleSheet(QString("#topWidget { background-color: %1; }").arg(Constant::Color::TOP_BAR_BG));
    m_topWidget->setFixedHeight(40);
    UiUtils::disableMouseEvents(m_topWidget);

    // 图标
    ElaImageCard *pixmap = new ElaImageCard(m_topWidget);
    pixmap->setFixedSize(140, 40);
    pixmap->setIsPreserveAspectCrop(false);
    pixmap->setCardImage(QImage(Constant::Path::APP_BIG_ICON));

    // 信息栏
    ElaToolBar *infoBar = new ElaToolBar(m_topWidget);
    infoBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    infoBar->setToolBarSpacing(10);
    infoBar->setFixedHeight(40);

    m_stationInfo = new ElaToolButton(infoBar);
    m_userInfo = new ElaToolButton(infoBar);
    m_laneID = new ElaToolButton(infoBar);
    m_shiftInfo = new ElaToolButton(infoBar);
    QList<ElaToolButton *> toolButtons = {m_stationInfo, m_userInfo, m_laneID, m_shiftInfo};
    for (auto *toolButton : toolButtons) {
        toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        QFont font = toolButton->font();
        font.setWeight(QFont::DemiBold);
        font.setPixelSize(Constant::FontSize::TOP_BAR_SIZE);
        toolButton->setFont(font);
        infoBar->addWidget(toolButton);
    }

    // 车道模式标签
    m_modeText = new ElaPushButton(m_topWidget);
    m_modeText->setBorderRadius(8);
    m_modeText->setMinimumWidth(100);
    m_modeText->setFixedHeight(35);
    m_modeText->setLightDefaultColor(QColor(Constant::Color::CONFIRM_BUTTON_BG));
    m_modeText->setLightTextColor(QColor(Constant::Color::CONFIRM_BUTTON_TEXT));
    QFont font = m_modeText->font();
    font.setPixelSize(Constant::FontSize::TOP_BAR_SIZE);
    font.setBold(true);
    m_modeText->setFont(font);

    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setContentsMargins(5, 0, 5, 0);
    topLayout->addWidget(pixmap);
    topLayout->addStretch();
    topLayout->addWidget(infoBar);
    topLayout->addStretch();
    topLayout->addWidget(m_modeText);
}

void MgsBasePage::addCentralWidget(QWidget *w)
{
    if (!w)
        return;

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(this->layout());
    layout->insertWidget(layout->indexOf(m_topWidget) + 1, w, 1);
}

void MgsBasePage::createBottomWidget()
{
    m_bottomWidget = new QWidget(this);
    m_bottomWidget->setFixedHeight(40);
    m_bottomWidget->setObjectName("bottomWidget");
    m_bottomWidget->setStyleSheet(QString("#bottomWidget { background-color: %1; }").arg(Constant::Color::MAIN_BG));
    UiUtils::disableMouseEvents(m_bottomWidget);

    // 版本信息区
    ElaText *virtualGantryLabel = new ElaText("承载门架: ", m_bottomWidget);
    ElaText *appVerLabel = new ElaText("系统版本: ", m_bottomWidget);
    ElaText *feeRateVerLabel = new ElaText("费率版本: ", m_bottomWidget);
    ElaText *blackVerLabel = new ElaText("状态名单版本: ", m_bottomWidget);
    m_virtualGantryInfo = new ElaText(m_bottomWidget);
    m_appVer = new ElaText(m_bottomWidget);
    m_feeRateVer = new ElaText(m_bottomWidget);
    m_fullBlackVer = new ElaText(m_bottomWidget);
    m_partBlackVer = new ElaText(m_bottomWidget);

    QList<ElaText *> verTexts = {virtualGantryLabel,
                                 appVerLabel,
                                 feeRateVerLabel,
                                 blackVerLabel,
                                 m_fullBlackVer,
                                 m_partBlackVer,
                                 m_appVer,
                                 m_feeRateVer,
                                 m_virtualGantryInfo};
    for (auto *t : verTexts) {
        t->setStyleSheet(QString("color: %1;").arg(Constant::Color::INFO_TEXT));
        t->setIsWrapAnywhere(false);
        QFont font = t->font();
        font.setPixelSize(Constant::FontSize::BOTTOM_BAR_SIZE);
        t->setFont(font);
    }

    QHBoxLayout *versionHLayout = new QHBoxLayout();
    versionHLayout->setContentsMargins(5, 0, 5, 0);
    versionHLayout->setSpacing(0);
    versionHLayout->setAlignment(Qt::AlignCenter);
    versionHLayout->addWidget(virtualGantryLabel);
    versionHLayout->addWidget(m_virtualGantryInfo);
    versionHLayout->addSpacing(40);
    versionHLayout->addWidget(appVerLabel);
    versionHLayout->addWidget(m_appVer);
    versionHLayout->addSpacing(30);
    versionHLayout->addWidget(feeRateVerLabel);
    versionHLayout->addWidget(m_feeRateVer);
    versionHLayout->addSpacing(30);
    versionHLayout->addWidget(blackVerLabel);
    versionHLayout->addWidget(m_fullBlackVer);
    versionHLayout->addSpacing(15);
    versionHLayout->addWidget(m_partBlackVer);

    // 状态区（技术支持，时钟）
    ElaText *supportInfo = new ElaText(m_bottomWidget);
    supportInfo->setText("技术支持: 福建省高速公路信息科技有限公司");

    ElaText *clock = new ElaText(m_bottomWidget);
    clock->setText(DataDealUtils::curDateTimeStr());
    QTimer *timer = new QTimer(clock);
    connect(timer, &QTimer::timeout, clock, [clock]() { clock->setText(DataDealUtils::curDateTimeStr()); });
    timer->start(1000); // 每秒刷新

    QList<ElaText *> statusTexts = {supportInfo, clock};
    for (auto *t : statusTexts) {
        t->setIsWrapAnywhere(false);
        QFont font = t->font();
        font.setPixelSize(Constant::FontSize::BOTTOM_BAR_SIZE);
        t->setFont(font);
    }

    QHBoxLayout *statusHLayout = new QHBoxLayout();
    statusHLayout->setAlignment(Qt::AlignCenter);
    statusHLayout->setContentsMargins(0, 0, 0, 0);
    statusHLayout->setSpacing(70);
    statusHLayout->addWidget(supportInfo);
    statusHLayout->addWidget(clock);

    QVBoxLayout *bottomLayout = new QVBoxLayout(m_bottomWidget);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(5);
    bottomLayout->addLayout(versionHLayout);
    bottomLayout->addLayout(statusHLayout);
}

MgsPageArea *MgsBasePage::initDisplayArea()
{
    MgsPageArea *displayArea = new MgsPageArea();
    displayArea->setBorderRadius(0);

    // 抓拍显示区
    m_capImage = new ElaImageCard(displayArea);
    m_capImage->setBorderRadius(0);
    m_capImage->setIsPreserveAspectCrop(false);
    m_capImage->setCardImage(QImage(Constant::Path::CAP_AREA_BACKGROUND));
    m_capImage->setMinimumHeight(310);

    // 情报板与流程显示区
    QWidget *vehModeAndInfoBoardArea = new QWidget(displayArea);
    vehModeAndInfoBoardArea->setStyleSheet(QString("background-color: %1;").arg(Constant::Color::PAGEAREA_STRESS_BG));
    vehModeAndInfoBoardArea->setMinimumHeight(45);
    vehModeAndInfoBoardArea->setMaximumHeight(50);

    m_infoBoard = new ElaText(vehModeAndInfoBoardArea);
    m_infoBoard->setStyleSheet(QString("color: %1;").arg(Constant::Color::STATUS_TEXT));
    m_infoBoard->setMinimumWidth(100);
    m_vehMode = new ElaText(vehModeAndInfoBoardArea);
    m_vehMode->setStyleSheet(QString("color: %1;").arg(Constant::Color::INFO_TEXT));
    m_vehMode->setMinimumWidth(100);
    ElaText *splitter = new ElaText("|", vehModeAndInfoBoardArea);
    splitter->setStyleSheet(QString("color: %1;").arg(Constant::Color::SPLITTER));

    QList<ElaText *> texts = {m_infoBoard, m_vehMode, splitter};
    for (auto *t : texts) {
        t->setAlignment(Qt::AlignCenter);
        t->setIsWrapAnywhere(false);
        QFont font = t->font();
        font.setPixelSize(Constant::FontSize::DISPLAY_AREA_SIZE);
        font.setBold(true);
        t->setFont(font);
    }

    QHBoxLayout *vehModeAndInfoBoardAreaLayout = new QHBoxLayout(vehModeAndInfoBoardArea);
    vehModeAndInfoBoardAreaLayout->setContentsMargins(0, 0, 0, 0);
    vehModeAndInfoBoardAreaLayout->setSpacing(0);
    vehModeAndInfoBoardAreaLayout->addStretch();
    vehModeAndInfoBoardAreaLayout->addWidget(m_infoBoard);
    vehModeAndInfoBoardAreaLayout->addStretch();
    vehModeAndInfoBoardAreaLayout->addWidget(splitter);
    vehModeAndInfoBoardAreaLayout->addStretch();
    vehModeAndInfoBoardAreaLayout->addWidget(m_vehMode);
    vehModeAndInfoBoardAreaLayout->addStretch();

    QVBoxLayout *displayLayout = new QVBoxLayout(displayArea);
    displayLayout->setContentsMargins(0, 0, 0, 0);
    displayLayout->setSpacing(0);
    displayLayout->addWidget(m_capImage);
    displayLayout->addWidget(vehModeAndInfoBoardArea);

    return displayArea;
}

QPlainTextEdit *MgsBasePage::initLogBrowseArea()
{
    QPlainTextEdit *logBrowser = new QPlainTextEdit();
    logBrowser->setStyleSheet(QString("border: 1px solid %1;").arg(Constant::Color::BORDER));
    logBrowser->setReadOnly(true);
    logBrowser->setTextInteractionFlags(Qt::NoTextInteraction); // 禁止选择、复制等
    logBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    logBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QFont font = logBrowser->font();
    font.setPixelSize(Constant::FontSize::LOG_AREA_SIZE);
    logBrowser->setFont(font);

    return logBrowser;
}

MgsPageArea *MgsBasePage::initScrollTipArea()
{
    MgsPageArea *scrollTipArea = new MgsPageArea();
    scrollTipArea->setBackgroundColor(QColor(Constant::Color::PAGEAREA_STRESS_BG));
    scrollTipArea->setBorderRadius(0);
    scrollTipArea->setMinimumHeight(35);
    scrollTipArea->setMaximumHeight(45);

    m_scrollTip = new MgsScrollText(scrollTipArea);
    m_scrollTip->setStyleSheet(QString("color: %1;").arg(Constant::Color::INFO_TEXT));
    QFont font = m_scrollTip->font();
    font.setPixelSize(Constant::FontSize::SCROLLTIP_AREA_SIZE);
    font.setBold(true);
    m_scrollTip->setFont(font);

    QWidget *tipIconWidget = new QWidget(scrollTipArea);
    tipIconWidget->setStyleSheet(QString("background-color: %1;").arg(Constant::Color::CONFIRM_BUTTON_BG));
    tipIconWidget->setFixedWidth(40);

    ElaIconButton *tipIcon = new ElaIconButton(ElaIconType::MessageCaptions, 20, 30, 30, tipIconWidget);
    tipIcon->setLightIconColor(QColor(Constant::Color::CONFIRM_BUTTON_TEXT));

    QHBoxLayout *iconHLayout = new QHBoxLayout(tipIconWidget);
    iconHLayout->setContentsMargins(0, 0, 0, 0);
    iconHLayout->setSpacing(0);
    iconHLayout->setAlignment(Qt::AlignCenter);
    iconHLayout->addWidget(tipIcon);

    QHBoxLayout *scrollTipHLayout = new QHBoxLayout(scrollTipArea);
    scrollTipHLayout->setContentsMargins(0, 0, 0, 0);
    scrollTipHLayout->setSpacing(0);
    scrollTipHLayout->addWidget(tipIconWidget);
    scrollTipHLayout->addSpacing(10);
    scrollTipHLayout->addWidget(m_scrollTip, 1);
    scrollTipHLayout->addSpacing(10);

    return scrollTipArea;
}

MgsPageArea *MgsBasePage::initWeightInfoArea()
{
    MgsPageArea *weightInfoArea = new MgsPageArea();
    weightInfoArea->setBackgroundColor(QColor(Constant::Color::PAGEAREA_NORMAL_BG));
    weightInfoArea->setBorderRadius(0);
    weightInfoArea->setFixedHeight(125);

    // 当前车辆称重信息
    m_curWeightInfo = new ElaText(weightInfoArea);
    m_curWeightInfo->setTextPixelSize(Constant::FontSize::WEIGHTINFO_AREA_SIZE);
    m_curWeightInfo->setIsWrapAnywhere(false);

    // 称重降级提示
    m_weightLow = new MgsIconButton(ElaIconType::TriangleExclamation, Constant::FontSize::WEIGHTINFO_AREA_SIZE, weightInfoArea);
    m_weightLow->setColor(Constant::Color::WARN_TEXT);
    m_weightLow->setText("称重降级已开启!");
    m_weightLow->hide();

    // 称重队列车辆数
    m_curWeightInfoCount = new ElaText("车辆数:", weightInfoArea);
    m_curWeightInfoCount->setTextPixelSize(Constant::FontSize::WEIGHTINFO_AREA_SIZE);
    m_curWeightInfoCount->setIsWrapAnywhere(false);

    QHBoxLayout *weightSubHLayout = new QHBoxLayout();
    weightSubHLayout->setContentsMargins(0, 0, 0, 0);
    weightSubHLayout->addWidget(m_curWeightInfo);
    weightSubHLayout->addStretch();
    weightSubHLayout->addWidget(m_weightLow);
    weightSubHLayout->addStretch();
    weightSubHLayout->addWidget(m_curWeightInfoCount);

    // 称重信息队列
    m_weightInfoPanel = new MgsWeightInfoPanel(weightInfoArea);

    QVBoxLayout *weightInfoAreaLayout = new QVBoxLayout(weightInfoArea);
    weightInfoAreaLayout->setContentsMargins(5, 5, 5, 5);
    weightInfoAreaLayout->setSpacing(0);
    weightInfoAreaLayout->addLayout(weightSubHLayout);
    weightInfoAreaLayout->addStretch();
    weightInfoAreaLayout->addWidget(m_weightInfoPanel);

    return weightInfoArea;
}

MgsDevicePanel *MgsBasePage::initDeviceIconArea()
{
    MgsDevicePanel *deviceIconPanel = new MgsDevicePanel();
    deviceIconPanel->setMinimumHeight(55);
    deviceIconPanel->setMaximumHeight(70);

    return deviceIconPanel;
}

void MgsBasePage::initUi()
{
    initLeftUi();
    initRightUi();

    QHBoxLayout *mainHlayout = new QHBoxLayout(m_mainWidget);
    mainHlayout->setContentsMargins(5, 5, 5, 5);
    mainHlayout->setSpacing(5);
    mainHlayout->addWidget(m_leftLayout, 40);
    mainHlayout->addWidget(m_rightLayout, 60);
}

void MgsBasePage::setFullBlackVer(const QString &ver)
{
    if (!m_fullBlackVer)
        return;
    m_fullBlackVer->setText(QString("全量%1").arg(ver));
}

void MgsBasePage::setPartBlackVer(const QString &ver)
{
    if (!m_partBlackVer)
        return;
    m_partBlackVer->setText(QString("增量%1").arg(ver));
}

void MgsBasePage::setVirtualGantryInfo(const QString &info)
{
    if (!m_virtualGantryInfo)
        return;
    m_virtualGantryInfo->setText(QString("%1").arg(info));
}

void MgsBasePage::setAppVer(const QString &ver)
{
    if (!m_appVer)
        return;
    m_appVer->setText(QString("%1").arg(ver));
}

void MgsBasePage::setFeeRateVer(const QString &ver)
{
    if (!m_feeRateVer)
        return;
    m_feeRateVer->setText(QString("%1").arg(ver));
}

void MgsBasePage::setCurWeightInfoCount(const QString &curWeightInfoCount)
{
    if (!m_curWeightInfoCount)
        return;
    m_curWeightInfoCount->setText(curWeightInfoCount);
}

void MgsBasePage::setCurWeightInfo(const QString &curWeightInfo)
{
    if (!m_curWeightInfo)
        return;
    m_curWeightInfo->setText(curWeightInfo);
}

void MgsBasePage::appendWeightInfoItem(const ST_WeightInfoItem &item)
{
    if (!m_weightInfoPanel)
        return;
    m_weightInfoPanel->appendItem(item);
}

void MgsBasePage::setWeightLow(bool isLow)
{
    if (!m_weightLow)
        return;
    m_weightLow->setVisible(isLow);
}

void MgsBasePage::appendTradeItem(const QStringList &trade)
{
    if (!m_recentTradePanel)
        return;
    m_recentTradePanel->appendTrade(trade);
}

void MgsBasePage::setDeviceList(const QList<uint> &devList)
{
    if (!m_deviceIconPanel)
        return;
    m_deviceIconPanel->setDeviceList(devList);
}

void MgsBasePage::setScrollTip(const QString &tip)
{
    if (!m_scrollTip)
        return;

    m_scrollTip->setScrollText(tip);
}

void MgsBasePage::logAppend(EM_LogLevel::LogLevel logLevel, const QString &log)
{
    const int maxCount = 100;
    const int trimCount = 50;

    // 维护日志缓存
    QString level;
    if (logLevel == EM_LogLevel::INFO) {
        level = "INFO";
    } else if (logLevel == EM_LogLevel::WARN) {
        level = "WARN";
    } else if (logLevel == EM_LogLevel::ERROR) {
        level = "ERROR";
    } else {
        level = "DEBUG";
    }
    m_logBuffer.append(QString("%1 [%2] | %3").arg(DataDealUtils::curDateTimeStr("hh:mm:ss"), level, log));
    if (m_logBuffer.size() > maxCount) {
        m_logBuffer.erase(m_logBuffer.begin(), m_logBuffer.begin() + trimCount);
    }

    // 清空后重建整个日志区域
    m_logBrowser->clear();
    for (int i = 0; i < m_logBuffer.size(); ++i) {
        const QString &line = m_logBuffer[i];
        QColor textColor = Qt::black;
        if (line.contains("[ERROR]") || line.contains("[WARN]"))
            textColor = Qt::red;

        QTextCursor cursor = m_logBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);

        QTextCharFormat format;
        format.setForeground(textColor);
        cursor.setCharFormat(format);
        cursor.insertText(line);
        if (i != m_logBuffer.size() - 1)
            cursor.insertText("\n");
    }

    m_logBrowser->moveCursor(QTextCursor::End);
    m_logBrowser->ensureCursorVisible();
}

void MgsBasePage::initLeftUi()
{
    // 抓拍与流程显示区域
    m_displayArea = initDisplayArea();
    m_displayArea->setParent(this);

    // 交易信息显示区域
    m_tradeInfoArea = initTradeInfoArea();
    m_tradeInfoArea->setParent(this);

    QWidget *leftSubWidget = new QWidget(this);
    QVBoxLayout *leftSubVLayout = new QVBoxLayout(leftSubWidget);
    leftSubVLayout->setContentsMargins(0, 0, 0, 4);
    leftSubVLayout->setSpacing(5);
    leftSubVLayout->addWidget(m_displayArea, 65);
    leftSubVLayout->addWidget(m_tradeInfoArea, 35);

    // 日志显示区域
    m_logBrowser = initLogBrowseArea();
    m_logBrowser->setParent(this);

    // 左侧主布局
    m_leftLayout = new QSplitter(Qt::Vertical, this);
    m_leftLayout->setChildrenCollapsible(false);
    m_leftLayout->addWidget(leftSubWidget);
    m_leftLayout->addWidget(m_logBrowser);
    m_leftLayout->setHandleWidth(1);
    m_leftLayout->setSizes({750, 250}); // 初始按比例设置总高度为 1000 时的分配
}

void MgsBasePage::initRightUi()
{
    // 滚动提示显示区域
    m_scrollTipArea = initScrollTipArea();
    m_scrollTipArea->setParent(this);

    // 当前车辆与卡内信息显示区域
    m_vehInfoArea = initVehInfoArea();
    m_vehInfoArea->setParent(this);

    // 交易提示区域
    m_tradeHintArea = initTradeHintArea();
    m_tradeHintArea->setParent(this);

    // 称重信息显示区域
    m_weightInfoArea = initWeightInfoArea();
    m_weightInfoArea->setParent(this);

    QWidget *rightSubWidget1 = new QWidget(this);
    QVBoxLayout *rightSubLayout1 = new QVBoxLayout(rightSubWidget1);
    rightSubLayout1->setContentsMargins(0, 0, 0, 4);
    rightSubLayout1->setSpacing(5);
    rightSubLayout1->addWidget(m_scrollTipArea, 1);
    rightSubLayout1->addWidget(m_vehInfoArea, 1);
    rightSubLayout1->addWidget(m_tradeHintArea, 1);
    rightSubLayout1->addWidget(m_weightInfoArea);

    // 近期交易记录查看区域
    m_recentTradePanel = initRecentTradeArea();
    m_recentTradePanel->setParent(this);

    // 设备图标显示区域
    m_deviceIconPanel = initDeviceIconArea();
    m_deviceIconPanel->setParent(this);

    QWidget *rightSubWidget2 = new QWidget(this);
    QVBoxLayout *rightSubLayout2 = new QVBoxLayout(rightSubWidget2);
    rightSubLayout2->setContentsMargins(0, 0, 0, 0);
    rightSubLayout2->setSpacing(5);
    rightSubLayout2->addWidget(m_recentTradePanel, 75);
    rightSubLayout2->addWidget(m_deviceIconPanel, 25);

    m_rightLayout = new QSplitter(Qt::Vertical, this);
    m_rightLayout->setChildrenCollapsible(false);
    m_rightLayout->addWidget(rightSubWidget1);
    m_rightLayout->addWidget(rightSubWidget2);
    m_rightLayout->setHandleWidth(1);
    m_rightLayout->setSizes({600, 400});
}

void MgsBasePage::setStationInfo(const QString &stationInfo)
{
    if (!m_stationInfo)
        return;
    m_stationInfo->setText(stationInfo);
}

void MgsBasePage::setUserInfo(const QString &userInfo)
{
    if (!m_userInfo)
        return;
    m_userInfo->setText(userInfo);
}

void MgsBasePage::setLaneID(uint laneID)
{
    if (!m_laneID)
        return;
    m_laneID->setText(QString::number(laneID) + "#");
}

void MgsBasePage::setShiftInfo(const QString &shiftInfo)
{
    if (!m_shiftInfo)
        return;
    m_shiftInfo->setText(shiftInfo);
}

void MgsBasePage::setModeText(const QString &mode)
{
    if (!m_modeText)
        return;
    m_modeText->setText(mode);
}

void MgsBasePage::setCapImage(const QImage &img)
{
    if (!m_capImage)
        return;
    m_capImage->setCardImage(img);
}

void MgsBasePage::setVehMode(const QString &vehMode, const QString &color /*= #007bff */)
{
    if (!m_vehMode)
        return;
    m_vehMode->setText(vehMode);
    if (color != "#007bff")
        m_vehMode->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsBasePage::setInfoBoard(const QString info, const QString &color /*= "#1f9d5e"*/)
{
    if (!m_infoBoard)
        return;
    m_infoBoard->setText(info);
    if (color != "#1f9d5e")
        m_infoBoard->setStyleSheet(QString("color: %1;").arg(color));
}
