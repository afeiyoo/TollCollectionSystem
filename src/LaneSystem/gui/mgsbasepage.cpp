#include "mgsbasepage.h"

#include "ElaWidgetTools/ElaIconButton.h"
#include "ElaWidgetTools/ElaImageCard.h"
#include "ElaWidgetTools/ElaPlainTextEdit.h"
#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaText.h"
#include "ElaWidgetTools/ElaToolBar.h"
#include "ElaWidgetTools/ElaToolButton.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/component/mgsdevicepanel.h"
#include "gui/component/mgsiconbutton.h"
#include "gui/component/mgspagearea.h"
#include "gui/component/mgsrecenttradepanel.h"
#include "gui/component/mgsscrolltext.h"
#include "gui/component/mgsweightinfopanel.h"
#include "gui/mgsplateeditdialog.h"

#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>

MgsBasePage::MgsBasePage(QWidget *parent)
    : QWidget(parent)
{
    setFocus(); // 设置焦点

    createTopWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_topWidget);
    mainLayout->addStretch();

    m_mainWidget = new QWidget(this);
    addCentralWidget(m_mainWidget);

    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigLogAppend, this, &MgsBasePage::onLogAppend);
}

MgsBasePage::~MgsBasePage() {}

void MgsBasePage::createTopWidget()
{
    m_topWidget = new QWidget(this);
    m_topWidget->setFixedHeight(50);
    m_topWidget->setStyleSheet(QString("background-color: %1;").arg(Constant::Color::GRAY_COLOR));
    m_topWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // 图标
    ElaImageCard *pixmap = new ElaImageCard(m_topWidget);
    pixmap->setFixedSize(170, 50);
    pixmap->setIsPreserveAspectCrop(false);
    pixmap->setCardImage(QImage(Constant::Path::APP_BANNER));

    // 信息栏
    ElaToolBar *infoBar = new ElaToolBar(m_topWidget);
    infoBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    infoBar->setToolBarSpacing(10);
    infoBar->setFixedHeight(50);

    m_stationInfo = new ElaToolButton(m_topWidget);
    m_userInfo = new ElaToolButton(m_topWidget);
    m_laneID = new ElaToolButton(m_topWidget);
    m_shiftInfo = new ElaToolButton(m_topWidget);
    QList<ElaToolButton *> toolButtons = {m_stationInfo, m_userInfo, m_laneID, m_shiftInfo};
    for (auto *toolButton : toolButtons) {
        toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        infoBar->addWidget(toolButton);
        QFont font = toolButton->font();
        font.setPixelSize(20);
        font.setBold(true);
        toolButton->setFont(font);
    }

    // 车道模式标签
    m_modeText = new ElaPushButton(m_topWidget);
    m_modeText->setBorderRadius(15);
    m_modeText->setFixedHeight(45);
    m_modeText->setLightDefaultColor(QColor(Constant::Color::BLUE_COLOR));
    m_modeText->setLightTextColor(QColor(Constant::Color::WHITE_COLOR));
    QFont font = m_modeText->font();
    font.setPixelSize(20);
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

ElaImageCard *MgsBasePage::initCapImageArea()
{
    ElaImageCard *capImage = new ElaImageCard();
    capImage->setIsPreserveAspectCrop(false);
    capImage->setCardImage(QImage(Constant::Path::CAP_AREA_BACKGROUND));
    capImage->setMinimumHeight(290);

    return capImage;
}

MgsPageArea *MgsBasePage::initVehModeAndInfoBoardArea()
{
    MgsPageArea *vehModeAndInfoBoardArea = new MgsPageArea();
    vehModeAndInfoBoardArea->setBackgroundColor(QColor(Constant::Color::BLUE_COLOR));
    vehModeAndInfoBoardArea->setMinimumHeight(45);
    vehModeAndInfoBoardArea->setMaximumHeight(65);

    m_infoBoard = new ElaText(vehModeAndInfoBoardArea);
    m_infoBoard->setStyleSheet(QString("color: %1;").arg(Constant::Color::RED_COLOR));
    m_infoBoard->setMinimumWidth(100);
    m_vehMode = new ElaText(vehModeAndInfoBoardArea);
    m_vehMode->setStyleSheet(QString("color: %1;").arg(Constant::Color::WHITE_COLOR));
    m_vehMode->setMinimumWidth(100);
    ElaText *splitter = new ElaText("|", vehModeAndInfoBoardArea);
    splitter->setStyleSheet(QString("color: %1;").arg(Constant::Color::WHITE_COLOR));

    QList<ElaText *> texts = {m_infoBoard, m_vehMode, splitter};
    for (auto *t : texts) {
        t->setAlignment(Qt::AlignCenter);
        t->setIsWrapAnywhere(false);
        t->setTextPixelSize(25);
        QFont font = t->font();
        font.setBold(true);
        t->setFont(font);
    }

    QHBoxLayout *vehModeAreaLayout = new QHBoxLayout(vehModeAndInfoBoardArea);
    vehModeAreaLayout->setContentsMargins(0, 0, 0, 0);
    vehModeAreaLayout->setSpacing(0);
    vehModeAreaLayout->addStretch(0);
    vehModeAreaLayout->addWidget(m_infoBoard);
    vehModeAreaLayout->addStretch(0);
    vehModeAreaLayout->addWidget(splitter);
    vehModeAreaLayout->addStretch(0);
    vehModeAreaLayout->addWidget(m_vehMode);
    vehModeAreaLayout->addStretch(0);

    return vehModeAndInfoBoardArea;
}

MgsPageArea *MgsBasePage::initTradeInfoArea()
{
    MgsPageArea *tradeInfoArea = new MgsPageArea();
    return tradeInfoArea;
}

ElaPlainTextEdit *MgsBasePage::initLogBrowseArea()
{
    ElaPlainTextEdit *logBrowser = new ElaPlainTextEdit();
    logBrowser->setReadOnly(true);
    logBrowser->setTextInteractionFlags(Qt::NoTextInteraction); // 禁止选择、复制等
    QFont font = logBrowser->font();
    font.setPixelSize(13);
    logBrowser->setFont(font);

    return logBrowser;
}

MgsPageArea *MgsBasePage::initVersionInfoArea()
{
    MgsPageArea *versionInfoArea = new MgsPageArea();
    versionInfoArea->setBackgroundColor(QColor(Constant::Color::BLUE_COLOR));
    versionInfoArea->setMinimumHeight(65);
    versionInfoArea->setMaximumHeight(90);

    m_fullBlackVer = new ElaText("全量状态名单: 20250716", versionInfoArea);
    m_partBlackVer = new ElaText("增量状态名单: 202507311750", versionInfoArea);
    m_appVer = new ElaText("当前软件版本: 25051301", versionInfoArea);
    m_feeRateVer = new ElaText("当前费率版本: 1271", versionInfoArea);
    m_virtualGantryInfo = new ElaText("承载门架: 福州西B向门架(35013C)", versionInfoArea);

    QList<ElaText *> texts = {m_fullBlackVer, m_partBlackVer, m_virtualGantryInfo, m_appVer, m_feeRateVer};
    for (auto *t : texts) {
        t->setIsWrapAnywhere(false);
        t->setTextPixelSize(13);
        t->setStyleSheet(QString("color: %1").arg(Constant::Color::WHITE_COLOR));
    }

    QGridLayout *versionInfoAreaLayout = new QGridLayout(versionInfoArea);
    versionInfoAreaLayout->setContentsMargins(9, 0, 9, 0);
    versionInfoAreaLayout->setVerticalSpacing(0);

    versionInfoAreaLayout->addWidget(m_appVer, 0, 0);
    versionInfoAreaLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Preferred, QSizePolicy::Minimum), 0, 1);
    versionInfoAreaLayout->addWidget(m_feeRateVer, 0, 2);

    versionInfoAreaLayout->addWidget(m_fullBlackVer, 1, 0);
    versionInfoAreaLayout->addItem(new QSpacerItem(10, 0, QSizePolicy::Preferred, QSizePolicy::Minimum), 1, 1);
    versionInfoAreaLayout->addWidget(m_partBlackVer, 1, 2);

    // 独占一行（占满三列）
    versionInfoAreaLayout->addWidget(m_virtualGantryInfo, 2, 0, 1, 3);

    return versionInfoArea;
}

MgsPageArea *MgsBasePage::initScrollTipArea()
{
    MgsPageArea *scrollTipArea = new MgsPageArea();
    scrollTipArea->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    scrollTipArea->setBackgroundColor(QColor(Constant::Color::BLUE_COLOR));
    scrollTipArea->setBorderRadius(15);
    scrollTipArea->setMinimumHeight(40);
    scrollTipArea->setMaximumHeight(60);

    m_scrollTip = new MgsScrollText(scrollTipArea);
    m_scrollTip->setStyleSheet(QString("color: %1;").arg(Constant::Color::WHITE_COLOR));
    m_scrollTip->setTextPixelSize(20);
    QFont font = m_scrollTip->font();
    font.setBold(true);
    m_scrollTip->setFont(font);

    int pixelSize = m_scrollTip->getTextPixelSize();
    ElaIconButton *tipIcon = new ElaIconButton(ElaIconType::MessageCaptions,
                                               pixelSize - 1,
                                               pixelSize,
                                               pixelSize,
                                               scrollTipArea);
    tipIcon->setLightIconColor(QColor(Constant::Color::WHITE_COLOR));

    QHBoxLayout *scrollTipAreaLayout = new QHBoxLayout(scrollTipArea);
    scrollTipAreaLayout->setContentsMargins(18, 5, 18, 5);
    scrollTipAreaLayout->setSpacing(10);
    scrollTipAreaLayout->addWidget(tipIcon, 0);
    scrollTipAreaLayout->addWidget(m_scrollTip, 1);

    return scrollTipArea;
}

MgsPageArea *MgsBasePage::initVehInfoArea()
{
    MgsPageArea *vehInfoArea = new MgsPageArea();

    return vehInfoArea;
}

MgsPageArea *MgsBasePage::initCardInfoArea()
{
    MgsPageArea *cardInfoArea = new MgsPageArea();

    return cardInfoArea;
}

MgsPageArea *MgsBasePage::initTradeHintArea()
{
    MgsPageArea *tradeHintArea = new MgsPageArea();

    return tradeHintArea;
}

MgsPageArea *MgsBasePage::initWeightInfoArea()
{
    MgsPageArea *weightInfoArea = new MgsPageArea();
    weightInfoArea->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    weightInfoArea->setFixedHeight(125);

    m_curWeightInfo = new ElaText(weightInfoArea);
    m_curWeightInfo->setTextPixelSize(16);
    m_curWeightInfo->setIsWrapAnywhere(false);

    m_weightLow = new MgsIconButton(ElaIconType::TriangleExclamation, 16, weightInfoArea);
    m_weightLow->setColor("#ff0000");
    m_weightLow->setText("称重降级");
    m_weightLow->hide();

    m_weightLowEffect = new QGraphicsOpacityEffect(m_weightLow);
    m_weightLow->setGraphicsEffect(m_weightLowEffect);

    m_weightLowBlinkTimer = new QTimer(this);
    m_weightLowBlinkTimer->setInterval(500);
    connect(m_weightLowBlinkTimer, &QTimer::timeout, this, [=]() {
        qreal opacity = m_weightLowEffect->opacity();
        m_weightLowEffect->setOpacity(opacity > 0.5 ? 0.0 : 1.0);
    });

    m_curWeightInfoCount = new ElaText("车辆数:", weightInfoArea);
    m_curWeightInfoCount->setTextPixelSize(16);
    m_curWeightInfoCount->setIsWrapAnywhere(false);

    QHBoxLayout *weightInfoSubLayout = new QHBoxLayout();
    weightInfoSubLayout->setContentsMargins(0, 0, 0, 0);
    weightInfoSubLayout->addWidget(m_curWeightInfo);
    weightInfoSubLayout->addStretch();
    weightInfoSubLayout->addWidget(m_weightLow);
    weightInfoSubLayout->addStretch();
    weightInfoSubLayout->addWidget(m_curWeightInfoCount);

    // 称重信息队列
    m_weightInfoPanel = new MgsWeightInfoPanel(weightInfoArea);

    QVBoxLayout *weightInfoAreaLayout = new QVBoxLayout(weightInfoArea);
    weightInfoAreaLayout->setContentsMargins(5, 5, 5, 5);
    weightInfoAreaLayout->setSpacing(0);
    weightInfoAreaLayout->addLayout(weightInfoSubLayout);
    weightInfoAreaLayout->addStretch();
    weightInfoAreaLayout->addWidget(m_weightInfoPanel);

    return weightInfoArea;
}

MgsRecentTradePanel *MgsBasePage::initRecentTradeArea()
{
    MgsRecentTradePanel *m_recentTradePanel = new MgsRecentTradePanel({});
    return m_recentTradePanel;
}

MgsDevicePanel *MgsBasePage::initDeviceIconArea()
{
    MgsDevicePanel *deviceIconPanel = new MgsDevicePanel();
    deviceIconPanel->setMinimumHeight(65);
    deviceIconPanel->setMaximumHeight(90);

    return deviceIconPanel;
}

void MgsBasePage::initUi()
{
    initLeftUi();
    initRightUi();

    QHBoxLayout *layout = new QHBoxLayout(m_mainWidget);
    layout->setContentsMargins(5, 1, 5, 1);
    layout->setSpacing(5);
    layout->addWidget(m_leftLayout, 40);
    layout->addWidget(m_rightLayout, 60);
}

void MgsBasePage::setFullBlackVer(const QString &ver)
{
    if (!m_fullBlackVer)
        return;
    m_fullBlackVer->setText(QString("全量状态名单: %1").arg(ver));
}

void MgsBasePage::setPartBlackVer(const QString &ver)
{
    if (!m_partBlackVer)
        return;
    m_partBlackVer->setText(QString("增量状态名单: %1").arg(ver));
}

void MgsBasePage::setVirtualGantryInfo(const QString &info)
{
    if (!m_virtualGantryInfo)
        return;
    m_virtualGantryInfo->setText(QString("承载门架信息: %1").arg(info));
}

void MgsBasePage::setAppVer(const QString &ver)
{
    if (!m_appVer)
        return;
    m_appVer->setText(QString("当前软件版本: %1").arg(ver));
}

void MgsBasePage::setFeeRateVer(const QString &ver)
{
    if (!m_feeRateVer)
        return;
    m_feeRateVer->setText(QString("当前费率版本: %1").arg(ver));
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

void MgsBasePage::appendWeightInfoItem(const WeightInfoItem &item)
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
    if (isLow) {
        m_weightLowEffect->setOpacity(1.0); // 初始为可见
        m_weightLowBlinkTimer->start();
    } else {
        m_weightLowBlinkTimer->stop();
        m_weightLowEffect->setOpacity(1.0); // 停止时保持可见状态
    }
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

void MgsBasePage::onLogAppend(const QString &log)
{
    const int maxCount = 100;
    const int trimCount = 50;

    // 维护日志缓存
    m_logBuffer.append(log);
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
    // 抓拍显示区域
    m_capImageArea = initCapImageArea();
    m_capImageArea->setParent(this);

    // 客货流程及情报板显示区域
    m_vehModeAndInfoBoardArea = initVehModeAndInfoBoardArea();
    m_vehModeAndInfoBoardArea->setParent(this);

    // 交易信息显示区域
    m_tradeInfoArea = initTradeInfoArea();
    m_tradeInfoArea->setParent(this);

    QWidget *leftSubWidget1 = new QWidget(this);
    QVBoxLayout *leftSubWidgetLayout1 = new QVBoxLayout(leftSubWidget1);
    leftSubWidgetLayout1->setContentsMargins(0, 0, 0, 0);
    leftSubWidgetLayout1->setSpacing(1);
    leftSubWidgetLayout1->addWidget(m_capImageArea, 65);
    leftSubWidgetLayout1->addWidget(m_vehModeAndInfoBoardArea, 10);
    leftSubWidgetLayout1->addWidget(m_tradeInfoArea, 25);

    // 日志显示区域
    m_logBrowser = initLogBrowseArea();
    m_logBrowser->setParent(this);

    // 版本信息显示区域
    m_versionInfoArea = initVersionInfoArea();
    m_versionInfoArea->setParent(this);

    QWidget *leftSubWidget2 = new QWidget(this);
    QVBoxLayout *leftSubLayout2 = new QVBoxLayout(leftSubWidget2);
    leftSubLayout2->setContentsMargins(0, 0, 0, 0);
    leftSubLayout2->setSpacing(1);
    leftSubLayout2->addWidget(m_logBrowser, 70);
    leftSubLayout2->addWidget(m_versionInfoArea, 30);

    // 左侧主布局
    m_leftLayout = new QSplitter(Qt::Vertical, this);
    m_leftLayout->setChildrenCollapsible(false);
    m_leftLayout->addWidget(leftSubWidget1);
    m_leftLayout->addWidget(leftSubWidget2);
    m_leftLayout->setHandleWidth(1);
    m_leftLayout->setSizes({700, 300}); // 初始按比例设置总高度为 1000 时的分配
}

void MgsBasePage::initRightUi()
{
    // 滚动提示显示区域
    m_scrollTipArea = initScrollTipArea();
    m_scrollTipArea->setParent(this);

    // 当前车辆信息显示区域
    m_vehInfoArea = initVehInfoArea();
    m_vehInfoArea->setParent(this);

    // 当前车辆卡内信息显示区域
    m_cardInfoArea = initCardInfoArea();
    m_cardInfoArea->setParent(this);

    // 交易提示区域
    m_tradeHintArea = initTradeHintArea();
    m_tradeHintArea->setParent(this);

    // 称重信息显示区域
    m_weightInfoArea = initWeightInfoArea();
    m_weightInfoArea->setParent(this);

    QWidget *rightSubWidget1 = new QWidget(this);
    QVBoxLayout *rightSubLayout1 = new QVBoxLayout(rightSubWidget1);
    rightSubLayout1->setContentsMargins(0, 0, 0, 0);
    rightSubLayout1->setSpacing(1);
    rightSubLayout1->addWidget(m_scrollTipArea, 10);
    rightSubLayout1->addWidget(m_vehInfoArea, 10);
    rightSubLayout1->addWidget(m_cardInfoArea, 30);
    rightSubLayout1->addWidget(m_tradeHintArea, 20);
    rightSubLayout1->addWidget(m_weightInfoArea, 30);

    // 近期交易记录查看区域
    m_recentTradePanel = initRecentTradeArea();
    m_recentTradePanel->setParent(this);

    // 设备图标显示区域
    m_deviceIconPanel = initDeviceIconArea();
    m_deviceIconPanel->setParent(this);

    QWidget *rightSubWidget2 = new QWidget(this);
    QVBoxLayout *rightSubLayout2 = new QVBoxLayout(rightSubWidget2);
    rightSubLayout2->setContentsMargins(0, 0, 0, 0);
    rightSubLayout2->setSpacing(1);
    rightSubLayout2->addWidget(m_recentTradePanel, 80);
    rightSubLayout2->addWidget(m_deviceIconPanel, 20);

    m_rightLayout = new QSplitter(Qt::Vertical, this);
    m_rightLayout->setChildrenCollapsible(false);
    m_rightLayout->addWidget(rightSubWidget1);
    m_rightLayout->addWidget(rightSubWidget2);
    m_rightLayout->setHandleWidth(1);
    m_rightLayout->setSizes({580, 420});
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
    if (!m_capImageArea)
        return;
    m_capImageArea->setCardImage(img);
}

void MgsBasePage::setVehMode(const QString &vehMode, const QString &color /*= #ffffff */)
{
    if (!m_vehMode)
        return;
    m_vehMode->setText(vehMode);
    if (color != "#ffffff")
        m_vehMode->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsBasePage::setInfoBoard(const QString info, const QString &color)
{
    if (!m_infoBoard)
        return;
    m_infoBoard->setText(info);
    if (color != "#ff0000")
        m_infoBoard->setStyleSheet(QString("color: %1;").arg(color));
}
