#include "mgsmtcoutpage.h"

#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaTableView.h"
#include "ElaWidgetTools/ElaText.h"
#include "Logger.h"
#include "config/config.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/component/mgspagearea.h"
#include "gui/component/mgsrecenttradepanel.h"
#include "gui/component/mgsscrolltext.h"
#include "gui/mgsplateeditdialog.h"
#include "utils/bizutils.h"
#include "utils/uiutils.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <QVBoxLayout>

using namespace Utils;

MgsMtcOutPage::MgsMtcOutPage(QWidget *parent)
    : MgsBasePage(parent)
{
    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigPlateChanged, this, &MgsMtcOutPage::onPlateChanged);
}

MgsMtcOutPage::~MgsMtcOutPage() {}

void MgsMtcOutPage::setStartTicketNum(int num)
{
    if (!m_startTicketNum)
        return;
    m_startTicketNum->setText(QString("%1").arg(num));
}

void MgsMtcOutPage::setCurTicketNum(int num)
{
    if (!m_curTicketNum)
        return;
    m_curTicketNum->setText(QString("%1").arg(num));
}

void MgsMtcOutPage::setNormalTicketCnt(int cnt)
{
    if (!m_normalTicketCnt)
        return;
    m_normalTicketCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setScrapTicketCnt(int cnt)
{
    if (!m_scrapTicketCnt)
        return;
    m_scrapTicketCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setRestTicketCnt(int cnt)
{
    if (!m_restTicketCnt)
        return;
    m_restTicketCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setRecycleCardCnt(int cnt)
{
    if (!m_recycleCardCnt)
        return;
    m_recycleCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setThirdPayCnt(int cnt)
{
    if (!m_thirdPayCnt)
        return;
    m_thirdPayCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setTotalVehCnt(int cnt)
{
    if (!m_totalVehCnt)
        return;
    m_totalVehCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setTotalCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setNoFlagCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setPeccanyVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setFleetVehCnt(int cnt)
{
    if (!m_fleetVehCnt)
        return;
    m_fleetVehCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setCpcCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setEtcCardCnt(int cnt)
{
    if (!m_etcCardCnt)
        return;
    m_etcCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcOutPage::setBadCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setPaperCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setHolidayFreeVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setNormalVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setFreeVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setTotalToll(qreal toll)
{
    Q_UNUSED(toll);
}

void MgsMtcOutPage::setCreditCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setPrePayCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setLastShiftTotalVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setPlate(const QString &plate)
{
    if (!m_plate)
        return;

    m_plate->setText(plate);

    QString bgImage;
    QColor textColor = Qt::white; // 默认字体颜色

    if (plate.startsWith("白")) {
        bgImage = Constant::Path::WHITE_PLATE;
        textColor = Qt::black;
    } else if (plate.startsWith("黑")) {
        bgImage = Constant::Path::BLACK_PLATE;
        textColor = Qt::white;
    } else if (plate.startsWith("黄")) {
        bgImage = Constant::Path::YELLOW_PLATE;
        textColor = Qt::black;
    } else if (plate.startsWith("绿")) {
        bgImage = Constant::Path::GREEN_PLATE;
        textColor = Qt::black;
    } else if (plate.startsWith("渐")) {
        bgImage = Constant::Path::GRADIENT_PLATE;
        textColor = Qt::black;
    } else if (plate.startsWith("拼")) {
        bgImage = Constant::Path::MIX_PLATE;
        textColor = Qt::black;
    } else if (plate.startsWith("蓝")) {
        bgImage = Constant::Path::BLUE_PLATE;
        textColor = Qt::white;
    }

    if (!bgImage.isEmpty()) {
        m_plate->setStyleSheet(QString("border-image: url(%1) 0 0 0 0 stretch stretch; color: %2;").arg(bgImage, textColor.name()));
    } else {
        m_plate->setStyleSheet(""); // 没有匹配到颜色，清除样式
    }
}

void MgsMtcOutPage::setVehClass(const QString &vehClass)
{
    if (!m_vehClass)
        return;
    m_vehClass->setText(vehClass);
}

void MgsMtcOutPage::setVehStatus(const QString &vehStatus)
{
    if (!m_vehStatus)
        return;
    m_vehStatus->setText(vehStatus);
}

void MgsMtcOutPage::setSituation(const QString &situation)
{
    if (!m_situation)
        return;
    m_situation->setText(situation);
    m_situation->setStyleSheet(QString("color: %1").arg(Constant::Color::WARN_TEXT));
}

void MgsMtcOutPage::setCardType(const QString &cardType)
{
    if (!m_cardType)
        return;
    m_cardType->setText(cardType);
}

void MgsMtcOutPage::setCardNum(const QString &cardNum)
{
    if (!m_cardNum)
        return;
    m_cardNum->setText(cardNum);
}

void MgsMtcOutPage::setBalance(const QString &balance)
{
    Q_UNUSED(balance);
}

void MgsMtcOutPage::setEnTime(const QString &enTime)
{
    if (!m_enTime)
        return;
    m_enTime->setText(enTime);
}

void MgsMtcOutPage::setEnStationName(const QString &enStationName)
{
    if (!m_enStationName)
        return;
    m_enStationName->setText(enStationName);
}

void MgsMtcOutPage::setCardStatus(const QString &cardStatus)
{
    Q_UNUSED(cardStatus);
}

void MgsMtcOutPage::setWeightInfo(const QString &info)
{
    if (!m_weightInfo)
        return;
    m_weightInfo->setText(info);
}

void MgsMtcOutPage::setEnPlate(const QString &plate)
{
    if (!m_enPlate)
        return;
    m_enPlate->setText(plate);
}

void MgsMtcOutPage::setLabel1(const QString &info)
{
    if (!m_label1)
        return;
    m_label1->setText(info);
}

void MgsMtcOutPage::setLabel2(const QString &info)
{
    if (!m_splitProvincesInfo)
        return;
    m_splitProvincesInfo->setText(info);
}

void MgsMtcOutPage::setToll(const QString &toll)
{
    Q_UNUSED(toll);
}

void MgsMtcOutPage::setObuHint(const QString &obuHint, const QString &color /* = #1ed760*/)
{
    m_obuHint->setText(obuHint);
    m_obuHint->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsMtcOutPage::appendHintButton(const QString &hint, const QString &fontColor, const QString &bgColor)
{
    if (!m_hintButtonLayout)
        return;

    ElaPushButton *hintBtn = new ElaPushButton();
    hintBtn->setBorderRadius(5);
    hintBtn->setLightTextColor(fontColor);
    hintBtn->setLightDefaultColor(bgColor);
    hintBtn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    hintBtn->setFixedHeight(30);
    hintBtn->setMinimumWidth(50);
    QFont font = hintBtn->font();
    font.setPixelSize(13);
    font.setBold(true);
    hintBtn->setFont(font);

    hintBtn->setText(hint);

    m_hintButtonLayout->insertWidget(0, hintBtn);
}

void MgsMtcOutPage::onPlateChanged(const QString &plate)
{
    setPlate(plate);
}

void MgsMtcOutPage::setTradeHint(const QString &tradeHint, const QString &color /*= #007bff*/)
{
    m_tradeHint->setScrollText(tradeHint);
    m_tradeHint->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsMtcOutPage::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    LOG_INFO().noquote() << "接收到按键:" << key << "，" << BizUtils::getKeyName(GM_INSTANCE->m_config->m_keyboard, key);
    if (key == Qt::Key_S) {
        event->accept();
    } else if (key == Qt::Key_I) {
        event->accept();
    } else if (key == Qt::Key_Right) {
        event->accept();
    } else if (key == Qt::Key_W) {
        event->accept();
    } else {
        qDebug() << "无效按键";
        MgsBasePage::keyPressEvent(event); // 默认处理其他键
    }
}

MgsPageArea *MgsMtcOutPage::initTradeInfoArea()
{
    MgsPageArea *tradeInfoArea = new MgsPageArea();
    tradeInfoArea->setBorderRadius(0);
    tradeInfoArea->setBackgroundColor(Constant::Color::PAGEAREA_NORMAL_BG);
    tradeInfoArea->setMinimumHeight(160);
    tradeInfoArea->setMaximumHeight(260);

    ElaText *startTicketNumLabel = new ElaText("起始票据: ", tradeInfoArea);
    ElaText *curTicketNumLabel = new ElaText("当前票据: ", tradeInfoArea);
    ElaText *normalTicketCntLabel = new ElaText("正常票数: ", tradeInfoArea);
    ElaText *scrapTicketCntLabel = new ElaText("废票张数: ", tradeInfoArea);
    ElaText *restTicketCntLabel = new ElaText("剩余票数: ", tradeInfoArea);
    ElaText *totalVehCntLabel = new ElaText("总过车数: ", tradeInfoArea);
    ElaText *recycleCardCntLabel = new ElaText("回收卡数: ", tradeInfoArea);
    ElaText *etcCardCntLabel = new ElaText("闽通卡数: ", tradeInfoArea);
    ElaText *fleetVehCntLabel = new ElaText("车队车数: ", tradeInfoArea);
    ElaText *thirdPayCntLabel = new ElaText("第三方: ", tradeInfoArea);
    QList<ElaText *> labels = {startTicketNumLabel,
                               curTicketNumLabel,
                               normalTicketCntLabel,
                               scrapTicketCntLabel,
                               restTicketCntLabel,
                               totalVehCntLabel,
                               recycleCardCntLabel,
                               etcCardCntLabel,
                               fleetVehCntLabel,
                               thirdPayCntLabel};
    for (auto *l : labels) {
        l->setIsWrapAnywhere(false);
        QFont font = l->font();
        font.setPixelSize(Constant::FontSize::TRADEINFO_AREA_SIZE);
        l->setFont(font);
    }

    m_startTicketNum = new ElaText(tradeInfoArea);
    m_curTicketNum = new ElaText(tradeInfoArea);
    m_normalTicketCnt = new ElaText(tradeInfoArea);
    m_scrapTicketCnt = new ElaText(tradeInfoArea);
    m_restTicketCnt = new ElaText(tradeInfoArea);
    m_totalVehCnt = new ElaText(tradeInfoArea);
    m_recycleCardCnt = new ElaText(tradeInfoArea);
    m_etcCardCnt = new ElaText(tradeInfoArea);
    m_fleetVehCnt = new ElaText(tradeInfoArea);
    m_thirdPayCnt = new ElaText(tradeInfoArea);
    QList<ElaText *> texts = {m_startTicketNum,
                              m_curTicketNum,
                              m_normalTicketCnt,
                              m_scrapTicketCnt,
                              m_restTicketCnt,
                              m_totalVehCnt,
                              m_recycleCardCnt,
                              m_etcCardCnt,
                              m_fleetVehCnt,
                              m_thirdPayCnt};
    for (auto *t : texts) {
        t->setIsWrapAnywhere(false);
        QFont font = t->font();
        font.setWeight(QFont::Bold);
        font.setPixelSize(Constant::FontSize::TRADEINFO_AREA_SIZE);
        t->setFont(font);
        t->setStyleSheet(QString("color: %1;").arg(Constant::Color::STRESS_TEXT));
    }

    QGridLayout *tradeInfoAreaLayout = new QGridLayout(tradeInfoArea);
    tradeInfoAreaLayout->setContentsMargins(20, 5, 20, 5);
    tradeInfoAreaLayout->setVerticalSpacing(5);
    // 标签列紧凑，值列拉伸
    tradeInfoAreaLayout->setColumnStretch(0, 0);
    tradeInfoAreaLayout->setColumnStretch(1, 1);
    tradeInfoAreaLayout->setColumnStretch(2, 0);
    tradeInfoAreaLayout->setColumnStretch(3, 1);

    tradeInfoAreaLayout->addWidget(startTicketNumLabel, 0, 0);
    tradeInfoAreaLayout->addWidget(curTicketNumLabel, 1, 0);
    tradeInfoAreaLayout->addWidget(normalTicketCntLabel, 2, 0);
    tradeInfoAreaLayout->addWidget(scrapTicketCntLabel, 3, 0);
    tradeInfoAreaLayout->addWidget(restTicketCntLabel, 4, 0);
    tradeInfoAreaLayout->addWidget(m_startTicketNum, 0, 1);
    tradeInfoAreaLayout->addWidget(m_curTicketNum, 1, 1);
    tradeInfoAreaLayout->addWidget(m_normalTicketCnt, 2, 1);
    tradeInfoAreaLayout->addWidget(m_scrapTicketCnt, 3, 1);
    tradeInfoAreaLayout->addWidget(m_restTicketCnt, 4, 1);
    tradeInfoAreaLayout->addWidget(totalVehCntLabel, 0, 2);
    tradeInfoAreaLayout->addWidget(recycleCardCntLabel, 1, 2);
    tradeInfoAreaLayout->addWidget(etcCardCntLabel, 2, 2);
    tradeInfoAreaLayout->addWidget(fleetVehCntLabel, 3, 2);
    tradeInfoAreaLayout->addWidget(thirdPayCntLabel, 4, 2);
    tradeInfoAreaLayout->addWidget(m_totalVehCnt, 0, 3);
    tradeInfoAreaLayout->addWidget(m_recycleCardCnt, 1, 3);
    tradeInfoAreaLayout->addWidget(m_etcCardCnt, 2, 3);
    tradeInfoAreaLayout->addWidget(m_fleetVehCnt, 3, 3);
    tradeInfoAreaLayout->addWidget(m_thirdPayCnt, 4, 3);

    return tradeInfoArea;
}

MgsPageArea *MgsMtcOutPage::initVehInfoArea()
{
    MgsPageArea *vehInfoArea = new MgsPageArea();
    vehInfoArea->setBackgroundColor(QColor(Constant::Color::PAGEAREA_NORMAL_BG));
    vehInfoArea->setBorderRadius(0);

    // 车辆信息区：抓拍车牌，车型，车种，特情
    QWidget *carInfoWidget = new QWidget(vehInfoArea);
    carInfoWidget->setFixedHeight(40);

    m_plate = new ElaText(carInfoWidget);
    m_plate->setMinimumWidth(140);
    m_plate->setContentsMargins(9, 0, 9, 0);
    m_vehClass = new ElaText(carInfoWidget);
    m_vehClass->setMinimumWidth(145);
    m_vehStatus = new ElaText(carInfoWidget);
    m_vehStatus->setMinimumWidth(100);
    m_situation = new ElaText(carInfoWidget);
    m_situation->setMinimumWidth(100);

    QList<ElaText *> carTexts = {m_plate, m_vehClass, m_vehStatus, m_situation};
    for (auto *t : carTexts) {
        t->setTextPixelSize(Constant::FontSize::VEHINFO_AREA_SIZE);
        t->setAlignment(Qt::AlignCenter);
        t->setIsWrapAnywhere(false);
    }

    QHBoxLayout *carInfoHLayout = new QHBoxLayout(carInfoWidget);
    carInfoHLayout->setContentsMargins(10, 5, 10, 5);
    carInfoHLayout->setSpacing(5);

    carInfoHLayout->addWidget(m_plate);
    Utils::UiUtils::addLine(carInfoHLayout, Qt::Vertical, 2, Constant::Color::BORDER);
    carInfoHLayout->addWidget(m_vehClass);
    Utils::UiUtils::addLine(carInfoHLayout, Qt::Vertical, 2, Constant::Color::BORDER);
    carInfoHLayout->addWidget(m_vehStatus);
    Utils::UiUtils::addLine(carInfoHLayout, Qt::Vertical, 2, Constant::Color::BORDER);
    carInfoHLayout->addWidget(m_situation);
    carInfoHLayout->addStretch();

    // 卡内信息区
    QWidget *cardInfoWidget = new QWidget(vehInfoArea);
    cardInfoWidget->setMinimumHeight(140);
    cardInfoWidget->setMaximumHeight(150);

    ElaText *label1 = new ElaText("卡号:", cardInfoWidget);
    ElaText *label2 = new ElaText("类型:", cardInfoWidget);
    ElaText *label3 = new ElaText("站名:", cardInfoWidget);
    ElaText *label4 = new ElaText("时间:", cardInfoWidget);
    ElaText *label5 = new ElaText("称重:", cardInfoWidget);
    ElaText *label6 = new ElaText("车牌:", cardInfoWidget);
    ElaText *label7 = new ElaText("标签:", cardInfoWidget);
    ElaText *label8 = new ElaText("分省信息:", cardInfoWidget);
    QList<ElaText *> labels = {label1, label2, label3, label4, label5, label6, label7, label8};
    for (auto *l : labels) {
        l->setIsWrapAnywhere(false);
        QFont font = l->font();
        font.setBold(true);
        font.setPixelSize(Constant::FontSize::VEHINFO_AREA_SIZE);
        l->setFont(font);
    }

    m_cardType = new ElaText(cardInfoWidget);
    m_cardNum = new ElaText(cardInfoWidget);
    m_enStationName = new ElaText(cardInfoWidget);
    m_enTime = new ElaText(cardInfoWidget);
    m_weightInfo = new ElaText(cardInfoWidget);
    m_enPlate = new ElaText(cardInfoWidget);
    m_label1 = new ElaText(cardInfoWidget);
    m_splitProvincesInfo = new ElaText(cardInfoWidget);
    QList<ElaText *> texts = {m_cardType, m_cardNum, m_enStationName, m_enTime, m_weightInfo, m_enPlate, m_label1, m_splitProvincesInfo};
    for (auto *t : texts) {
        t->setTextPixelSize(Constant::FontSize::VEHINFO_AREA_SIZE);
        t->setIsWrapAnywhere(false);
    }

    // 每行都用 QHBoxLayout 控制
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->setSpacing(5);
    row1->addWidget(label1);
    row1->addWidget(m_cardNum, 1);
    row1->addWidget(label2);
    row1->addWidget(m_cardType, 1);

    QHBoxLayout *row2 = new QHBoxLayout();
    row2->setSpacing(5);
    row2->addWidget(label3);
    row2->addWidget(m_enStationName, 1);
    row2->addWidget(label4);
    row2->addWidget(m_enTime, 1);

    QHBoxLayout *row3 = new QHBoxLayout();
    row3->setSpacing(5);
    row3->addWidget(label5);
    row3->addWidget(m_weightInfo, 1);
    row3->addWidget(label6);
    row3->addWidget(m_enPlate, 1);

    QHBoxLayout *row4 = new QHBoxLayout();
    row4->setSpacing(5);
    row4->addWidget(label7);
    row4->addWidget(m_label1, 1);

    QHBoxLayout *row5 = new QHBoxLayout();
    row5->setSpacing(5);
    row5->addWidget(label8);
    row5->addWidget(m_splitProvincesInfo, 1);

    QVBoxLayout *cardInfoVLayout = new QVBoxLayout(cardInfoWidget);
    cardInfoVLayout->setContentsMargins(9, 5, 9, 5);
    cardInfoVLayout->setSpacing(5);
    cardInfoVLayout->addLayout(row1);
    cardInfoVLayout->addLayout(row2);
    cardInfoVLayout->addLayout(row3);
    cardInfoVLayout->addLayout(row4);
    cardInfoVLayout->addLayout(row5);

    QVBoxLayout *vehInfoAreaLayout = new QVBoxLayout(vehInfoArea);
    vehInfoAreaLayout->setSpacing(0);
    vehInfoAreaLayout->setContentsMargins(0, 0, 0, 0);
    vehInfoAreaLayout->addWidget(carInfoWidget);
    Utils::UiUtils::addLine(vehInfoAreaLayout, Qt::Horizontal, 2, Constant::Color::BORDER);
    vehInfoAreaLayout->addWidget(cardInfoWidget);

    return vehInfoArea;
}

MgsPageArea *MgsMtcOutPage::initTradeHintArea()
{
    MgsPageArea *tradeHintArea = new MgsPageArea();
    tradeHintArea->setBackgroundColor(QColor(Constant::Color::PAGEAREA_NORMAL_BG));
    tradeHintArea->setBorderRadius(0);
    tradeHintArea->setMinimumHeight(130);

    m_hintButtonLayout = new QHBoxLayout();
    m_hintButtonLayout->setContentsMargins(0, 0, 0, 0);
    m_hintButtonLayout->setSpacing(5);
    m_hintButtonLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_hintButtonLayout->addStretch();

    m_tradeHint = new MgsScrollText(tradeHintArea);
    m_tradeHint->setStyleSheet(QString("color: %1").arg(Constant::Color::INFO_TEXT));
    QFont tradeHintfont = m_tradeHint->font();
    tradeHintfont.setPixelSize(Constant::FontSize::TRADE_HINT_SIZE);
    tradeHintfont.setBold(true);
    m_tradeHint->setFont(tradeHintfont);

    m_obuHint = new ElaText(tradeHintArea);
    m_obuHint->setTextPixelSize(Constant::FontSize::OBU_HINT_SIZE);
    m_obuHint->setWordWrap(true);

    QVBoxLayout *tradeHintAreaLayout = new QVBoxLayout(tradeHintArea);
    tradeHintAreaLayout->setContentsMargins(10, 5, 10, 5);
    tradeHintAreaLayout->setSpacing(0);
    tradeHintAreaLayout->addLayout(m_hintButtonLayout);
    tradeHintAreaLayout->addStretch();
    tradeHintAreaLayout->addWidget(m_tradeHint);
    tradeHintAreaLayout->addStretch();
    tradeHintAreaLayout->addWidget(m_obuHint);

    return tradeHintArea;
}

MgsRecentTradePanel *MgsMtcOutPage::initRecentTradeArea()
{
    QStringList header = {"车牌", "车型", "出口时间", "支付金额(元)", "卡号"};
    MgsRecentTradePanel *recentTradePanel = new MgsRecentTradePanel(header);
    ElaTableView *view = recentTradePanel->getRecentTradeView();
    // 初始列表宽度
    connect(view, &ElaTableView::tableViewShow, this, [=]() {
        view->setColumnWidth(0, 100);
        view->setColumnWidth(1, 40);
        view->setColumnWidth(2, 120);
        view->setColumnWidth(3, 130);
    });

    return recentTradePanel;
}
