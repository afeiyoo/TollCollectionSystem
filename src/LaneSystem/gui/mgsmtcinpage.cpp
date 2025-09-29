#include "mgsmtcinpage.h"

#include "ElaWidgetTools/ElaTableView.h"
#include "ElaWidgetTools/ElaText.h"
#include "Logger.h"
#include "config/config.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/modemanager.h"
#include "global/signalmanager.h"
#include "gui/component/mgsdevicepanel.h"
#include "gui/component/mgsoptionsdialog.h"
#include "gui/component/mgspagearea.h"
#include "gui/component/mgsrecenttradepanel.h"
#include "gui/component/mgsscrolltext.h"
#include "gui/mgsauthdialog.h"
#include "utils/bizutils.h"
#include "utils/fileutils.h"
#include "utils/uiutils.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QSplitter>

using namespace Utils;

MgsMtcInPage::MgsMtcInPage(QWidget *parent)
    : MgsBasePage(parent)
{
    m_authDialog = new MgsAuthDialog(this);
    m_authDialog->hide();

    m_optionsDialog = new MgsOptionsDialog(this);
    m_optionsDialog->hide();
}

MgsMtcInPage::~MgsMtcInPage() {}

void MgsMtcInPage::setStartTicketNum(int num)
{
    Q_UNUSED(num);
}

void MgsMtcInPage::setCurTicketNum(int num)
{
    Q_UNUSED(num);
}

void MgsMtcInPage::setNormalTicketCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setScrapTicketCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setRestTicketCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setRecycleCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setThirdPayCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setTotalVehCnt(int cnt)
{
    if (!m_totalVehCnt)
        return;
    m_totalVehCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setTotalCardCnt(int cnt)
{
    if (!m_totalCardCnt)
        return;
    m_totalCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setNoFlagCardCnt(int cnt)
{
    if (!m_noFlagCardCnt)
        return;
    m_noFlagCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setPeccanyVehCnt(int cnt)
{
    if (!m_peccanyVehCnt)
        return;
    m_peccanyVehCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setFleetVehCnt(int cnt)
{
    if (!m_fleetVehCnt)
        return;
    m_fleetVehCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setCpcCardCnt(int cnt)
{
    if (!m_cpcCardCnt)
        return;
    m_cpcCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setEtcCardCnt(int cnt)
{
    if (!m_etcCardCnt)
        return;
    m_etcCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setBadCardCnt(int cnt)
{
    if (!m_badCardCnt)
        return;
    m_badCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setPaperCardCnt(int cnt)
{
    if (!m_paperCardCnt)
        return;
    m_paperCardCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setHolidayFreeVehCnt(int cnt)
{
    if (!m_holidayFreeVehCnt)
        return;
    m_holidayFreeVehCnt->setText(QString("%1").arg(cnt));
}

void MgsMtcInPage::setNormalVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setFreeVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setTotalToll(qreal toll)
{
    Q_UNUSED(toll);
}

void MgsMtcInPage::setCreditCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setPrePayCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setLastShiftTotalVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcInPage::setPlate(const QString &plate)
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
        m_plate->setStyleSheet(
            QString("border-image: url(%1) 0 0 0 0 stretch stretch; color: %2;").arg(bgImage).arg(textColor.name()));
    } else {
        m_plate->setStyleSheet(""); // 没有匹配到颜色，清除样式
    }
}

void MgsMtcInPage::setVehClass(const QString &vehClass)
{
    if (!m_vehClass)
        return;
    m_vehClass->setText(vehClass);
}

void MgsMtcInPage::setVehStatus(const QString &vehStatus)
{
    if (!m_vehStatus)
        return;
    m_vehStatus->setText(vehStatus);
}

void MgsMtcInPage::setSituation(const QString &situation)
{
    if (!m_situation)
        return;
    m_situation->setText(situation);
    m_situation->setStyleSheet(QString("color: %1").arg(Constant::Color::WARN_TEXT));
}

void MgsMtcInPage::setCardType(const QString &cardType)
{
    if (!m_cardType)
        return;
    m_cardType->setText(cardType);
}

void MgsMtcInPage::setCardNum(const QString &cardNum)
{
    if (!m_cardNum)
        return;
    m_cardNum->setText(cardNum);
}

void MgsMtcInPage::setBalance(const QString &balance)
{
    if (!m_balance)
        return;
    m_balance->setText(balance);
}

void MgsMtcInPage::setEnTime(const QString &enTime)
{
    if (!m_enTime)
        return;
    m_enTime->setText(enTime);
}

void MgsMtcInPage::setEnStationName(const QString &enStationName)
{
    if (!m_enStationName)
        return;
    m_enStationName->setText(enStationName);
}

void MgsMtcInPage::setCardStatus(const QString &cardStatus)
{
    if (!m_cardStatus)
        return;
    m_cardStatus->setText(cardStatus);
}

void MgsMtcInPage::setWeightInfo(const QString &info)
{
    Q_UNUSED(info);
}

void MgsMtcInPage::setEnPlate(const QString &plate)
{
    Q_UNUSED(plate);
}

void MgsMtcInPage::setLabel1(const QString &info)
{
    Q_UNUSED(info);
}

void MgsMtcInPage::setLabel2(const QString &info)
{
    Q_UNUSED(info);
}

void MgsMtcInPage::setToll(const QString &toll)
{
    Q_UNUSED(toll);
}

void MgsMtcInPage::setObuHint(const QString &obuHint, const QString &color)
{
    m_obuHint->setText(obuHint);
    m_obuHint->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsMtcInPage::appendHintButton(const QString &hint, const QString &fontColor, const QString &bgColor)
{
    Q_UNUSED(hint);
    Q_UNUSED(fontColor);
    Q_UNUSED(bgColor);
}

void MgsMtcInPage::setTradeHint(const QString &tradeHint, const QString &color)
{
    m_tradeHint->setText(tradeHint);
    m_tradeHint->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsMtcInPage::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    LOG_INFO().noquote() << "按键: " << BizUtils::getKeyName(GM_INSTANCE->m_config->m_keyboard, key)
                         << QString("【%1】").arg(BizUtils::getKeyDescByCode(GM_INSTANCE->m_config->m_keyboard, key));

    if (key == Qt::Key_S) {
        this->screenShot();
        event->accept();
    } else if (key == Qt::Key_W) {
        GM_INSTANCE->m_modeMan->showMenu();
        event->accept();
    } else if (key == Qt::Key_Y) {
        m_optionsDialog->setOptions(Constant::DialogID::TEST_DLG, "测试窗口", {"0. 功能1", "1. 功能2", "2. 功能3"});
        m_optionsDialog->exec();
        event->accept();
    } else if (key == Qt::Key_J) {
        static bool isLow = false;
        setWeightLow(isLow ? false : true);
        event->accept();
    } else if (key == Qt::Key_G) {
        emit GM_INSTANCE->m_signalMan->sigCloseLane();
        event->accept();
    } else if (key == Qt::Key_O) {
        emit GM_INSTANCE->m_signalMan->sigOpenLane();
        event->accept();
    } else if (key == Qt::Key_I) {
        emit GM_INSTANCE->m_signalMan->sigShiftIn();
        event->accept();
    } else if (key == Qt::Key_U) {
        // emit GM_INSTANCE->m_signalMan->sigShiftOut();
        static int count = 1;
        emit GM_INSTANCE->m_signalMan->sigLogAppend(
            EM_LogLevel::WARN,
            QString("测试数据 %1 封疆大吏深刻搭街坊立刻埃里克几十块的咖啡机拉进了房间啊撒旦立刻就").arg(count++));
        event->accept();
    } else {
        qDebug() << "按键功能未实现";
        MgsBasePage::keyPressEvent(event); // 默认处理其他键
    }
}

MgsPageArea *MgsMtcInPage::initTradeInfoArea()
{
    MgsPageArea *tradeInfoArea = new MgsPageArea();
    tradeInfoArea->setBorderRadius(0);
    tradeInfoArea->setBackgroundColor(Constant::Color::PAGEAREA_NORMAL_BG);
    tradeInfoArea->setMinimumHeight(160);
    tradeInfoArea->setMaximumHeight(260);

    ElaText *totalVehCntLabel = new ElaText("总过车数:", tradeInfoArea);
    ElaText *totalCardCntLabel = new ElaText("总发卡数:", tradeInfoArea);
    ElaText *noFlagCardCntLabel = new ElaText("错标卡数:", tradeInfoArea);
    ElaText *peccanyVehCntLabel = new ElaText("冲关车数:", tradeInfoArea);
    ElaText *fleetVehCntLabel = new ElaText("车队车数:", tradeInfoArea);
    ElaText *cpcCardCntLabel = new ElaText("通行卡数:", tradeInfoArea);
    ElaText *etcCardCntLabel = new ElaText("闽通卡数:", tradeInfoArea);
    ElaText *badCardCntLabel = new ElaText("损坏卡数:", tradeInfoArea);
    ElaText *paperCardCntLabel = new ElaText("纸质券数:", tradeInfoArea);
    ElaText *holidayFreeVehCntLabel = new ElaText("动免车数:", tradeInfoArea);
    QList<ElaText *> labels = {totalVehCntLabel,
                               totalCardCntLabel,
                               noFlagCardCntLabel,
                               peccanyVehCntLabel,
                               fleetVehCntLabel,
                               cpcCardCntLabel,
                               etcCardCntLabel,
                               badCardCntLabel,
                               paperCardCntLabel,
                               holidayFreeVehCntLabel};
    for (auto *l : labels) {
        l->setIsWrapAnywhere(false);
        QFont font = l->font();
        font.setPixelSize(Constant::FontSize::TRADEINFO_AREA_SIZE);
        l->setFont(font);
    }

    m_totalVehCnt = new ElaText(tradeInfoArea);
    m_totalCardCnt = new ElaText(tradeInfoArea);
    m_noFlagCardCnt = new ElaText(tradeInfoArea);
    m_peccanyVehCnt = new ElaText(tradeInfoArea);
    m_fleetVehCnt = new ElaText(tradeInfoArea);
    m_cpcCardCnt = new ElaText(tradeInfoArea);
    m_etcCardCnt = new ElaText(tradeInfoArea);
    m_badCardCnt = new ElaText(tradeInfoArea);
    m_paperCardCnt = new ElaText(tradeInfoArea);
    m_holidayFreeVehCnt = new ElaText(tradeInfoArea);
    QList<ElaText *> texts = {m_totalVehCnt,
                              m_totalCardCnt,
                              m_noFlagCardCnt,
                              m_peccanyVehCnt,
                              m_fleetVehCnt,
                              m_cpcCardCnt,
                              m_etcCardCnt,
                              m_badCardCnt,
                              m_paperCardCnt,
                              m_holidayFreeVehCnt};
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

    tradeInfoAreaLayout->addWidget(totalVehCntLabel, 0, 0);
    tradeInfoAreaLayout->addWidget(totalCardCntLabel, 1, 0);
    tradeInfoAreaLayout->addWidget(noFlagCardCntLabel, 2, 0);
    tradeInfoAreaLayout->addWidget(peccanyVehCntLabel, 3, 0);
    tradeInfoAreaLayout->addWidget(fleetVehCntLabel, 4, 0);
    tradeInfoAreaLayout->addWidget(m_totalVehCnt, 0, 1);
    tradeInfoAreaLayout->addWidget(m_totalCardCnt, 1, 1);
    tradeInfoAreaLayout->addWidget(m_noFlagCardCnt, 2, 1);
    tradeInfoAreaLayout->addWidget(m_peccanyVehCnt, 3, 1);
    tradeInfoAreaLayout->addWidget(m_fleetVehCnt, 4, 1);
    tradeInfoAreaLayout->addWidget(cpcCardCntLabel, 0, 2);
    tradeInfoAreaLayout->addWidget(etcCardCntLabel, 1, 2);
    tradeInfoAreaLayout->addWidget(badCardCntLabel, 2, 2);
    tradeInfoAreaLayout->addWidget(paperCardCntLabel, 3, 2);
    tradeInfoAreaLayout->addWidget(holidayFreeVehCntLabel, 4, 2);
    tradeInfoAreaLayout->addWidget(m_cpcCardCnt, 0, 3);
    tradeInfoAreaLayout->addWidget(m_etcCardCnt, 1, 3);
    tradeInfoAreaLayout->addWidget(m_badCardCnt, 2, 3);
    tradeInfoAreaLayout->addWidget(m_paperCardCnt, 3, 3);
    tradeInfoAreaLayout->addWidget(m_holidayFreeVehCnt, 4, 3);

    return tradeInfoArea;
}

MgsPageArea *MgsMtcInPage::initVehInfoArea()
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
    cardInfoWidget->setMinimumHeight(120);
    cardInfoWidget->setMaximumHeight(140);

    ElaText *label1 = new ElaText("卡类型:", cardInfoWidget);
    ElaText *label2 = new ElaText("卡内余额:", cardInfoWidget);
    ElaText *label3 = new ElaText("状态:", cardInfoWidget);
    ElaText *label4 = new ElaText("卡号:", cardInfoWidget);
    ElaText *label5 = new ElaText("入口站名:", cardInfoWidget);
    ElaText *label6 = new ElaText("入口时间:", cardInfoWidget);
    QList<ElaText *> labels = {label1, label2, label3, label4, label5, label6};
    for (auto *l : labels) {
        l->setIsWrapAnywhere(false);
        QFont font = l->font();
        font.setBold(true);
        font.setPixelSize(Constant::FontSize::VEHINFO_AREA_SIZE);
        l->setFont(font);
    }

    m_cardType = new ElaText(cardInfoWidget);
    m_cardNum = new ElaText(cardInfoWidget);
    m_balance = new ElaText(cardInfoWidget);
    m_enTime = new ElaText(cardInfoWidget);
    m_enStationName = new ElaText(cardInfoWidget);
    m_cardStatus = new ElaText(cardInfoWidget);
    QList<ElaText *> cardTexts = {m_cardType, m_cardNum, m_balance, m_enTime, m_enStationName, m_cardStatus};
    for (auto *t : cardTexts) {
        QFont font = t->font();
        font.setPixelSize(Constant::FontSize::VEHINFO_AREA_SIZE);
        t->setFont(font);
        t->setIsWrapAnywhere(false);
    }

    QGridLayout *cardInfoGLayout = new QGridLayout(cardInfoWidget);
    cardInfoGLayout->setContentsMargins(9, 5, 9, 5);
    cardInfoGLayout->setHorizontalSpacing(5);
    cardInfoGLayout->setVerticalSpacing(5);

    // 设置标签列最小化，值列弹性拉伸
    cardInfoGLayout->setColumnStretch(0, 0);
    cardInfoGLayout->setColumnStretch(2, 0);
    cardInfoGLayout->setColumnStretch(1, 1);
    cardInfoGLayout->setColumnStretch(3, 2);

    // 第一行：卡类型、卡号
    cardInfoGLayout->addWidget(label1, 0, 0);
    cardInfoGLayout->addWidget(m_cardType, 0, 1);
    cardInfoGLayout->addWidget(label4, 0, 2);
    cardInfoGLayout->addWidget(m_cardNum, 0, 3);

    // 第二行：卡内余额、入口站名
    cardInfoGLayout->addWidget(label2, 1, 0);
    cardInfoGLayout->addWidget(m_balance, 1, 1);
    cardInfoGLayout->addWidget(label5, 1, 2);
    cardInfoGLayout->addWidget(m_enStationName, 1, 3);

    // 第三行：状态，入口时间
    cardInfoGLayout->addWidget(label3, 2, 0);
    cardInfoGLayout->addWidget(m_cardStatus, 2, 1);
    cardInfoGLayout->addWidget(label6, 2, 2);
    cardInfoGLayout->addWidget(m_enTime, 2, 3);

    QVBoxLayout *vehInfoAreaLayout = new QVBoxLayout(vehInfoArea);
    vehInfoAreaLayout->setSpacing(0);
    vehInfoAreaLayout->setContentsMargins(0, 0, 0, 0);
    vehInfoAreaLayout->addWidget(carInfoWidget);
    Utils::UiUtils::addLine(vehInfoAreaLayout, Qt::Horizontal, 2, Constant::Color::BORDER);
    vehInfoAreaLayout->addWidget(cardInfoWidget);

    return vehInfoArea;
}

MgsPageArea *MgsMtcInPage::initTradeHintArea()
{
    MgsPageArea *tradeHintArea = new MgsPageArea();
    tradeHintArea->setBackgroundColor(QColor(Constant::Color::PAGEAREA_NORMAL_BG));
    tradeHintArea->setBorderRadius(0);
    tradeHintArea->setMinimumHeight(130);

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
    tradeHintAreaLayout->addStretch();
    tradeHintAreaLayout->addWidget(m_tradeHint);
    tradeHintAreaLayout->addStretch();
    tradeHintAreaLayout->addWidget(m_obuHint);

    return tradeHintArea;
}

MgsRecentTradePanel *MgsMtcInPage::initRecentTradeArea()
{
    QStringList header = {"车牌", "车型", "入口时间", "卡类型", "卡号"};
    MgsRecentTradePanel *recentTradePanel = new MgsRecentTradePanel(header);
    ElaTableView *view = recentTradePanel->getRecentTradeView();
    // 初始列表宽度
    connect(view, &ElaTableView::tableViewShow, this, [=]() {
        view->setColumnWidth(0, 100);
        view->setColumnWidth(1, 50);
        view->setColumnWidth(2, 130);
        view->setColumnWidth(3, 80);
    });
    return recentTradePanel;
}
