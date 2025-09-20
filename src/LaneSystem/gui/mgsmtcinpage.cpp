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
#include "gui/component/mgspagearea.h"
#include "gui/component/mgsrecenttradepanel.h"
#include "gui/component/mgsscrolltext.h"
#include "gui/mgsauthdialog.h"
#include "utils/bizutils.h"
#include "utils/fileutils.h"
#include "utils/uiutils.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
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
    m_totalVehCnt->setText(QString("总过车数: %1").arg(cnt));
}

void MgsMtcInPage::setTotalCardCnt(int cnt)
{
    if (!m_totalCardCnt)
        return;
    m_totalCardCnt->setText(QString("总发卡数: %1").arg(cnt));
}

void MgsMtcInPage::setNoFlagCardCnt(int cnt)
{
    if (!m_noFlagCardCnt)
        return;
    m_noFlagCardCnt->setText(QString("错标卡数: %1").arg(cnt));
}

void MgsMtcInPage::setPeccanyVehCnt(int cnt)
{
    if (!m_peccanyVehCnt)
        return;
    m_peccanyVehCnt->setText(QString("冲关车数: %1").arg(cnt));
}

void MgsMtcInPage::setFleetVehCnt(int cnt)
{
    if (!m_fleetVehCnt)
        return;
    m_fleetVehCnt->setText(QString("车队车数: %1").arg(cnt));
}

void MgsMtcInPage::setCpcCardCnt(int cnt)
{
    if (!m_cpcCardCnt)
        return;
    m_cpcCardCnt->setText(QString("通行卡数: %1").arg(cnt));
}

void MgsMtcInPage::setEtcCardCnt(int cnt)
{
    if (!m_etcCardCnt)
        return;
    m_etcCardCnt->setText(QString("闽通卡数: %1").arg(cnt));
}

void MgsMtcInPage::setBadCardCnt(int cnt)
{
    if (!m_badCardCnt)
        return;
    m_badCardCnt->setText(QString("坏卡数: %1").arg(cnt));
}

void MgsMtcInPage::setPaperCardCnt(int cnt)
{
    if (!m_paperCardCnt)
        return;
    m_paperCardCnt->setText(QString("发纸券数: %1").arg(cnt));
}

void MgsMtcInPage::setHolidayFreeVehCnt(int cnt)
{
    if (!m_holidayFreeVehCnt)
        return;
    m_holidayFreeVehCnt->setText(QString("动免车数: %1").arg(cnt));
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
    m_situation->setStyleSheet(QString("color: %1").arg(Constant::Color::RED_COLOR));
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
                         << QString("【%1】").arg(BizUtils::getKeyDesc(GM_INSTANCE->m_config->m_keyboard, key));

    if (key == Qt::Key_S) {
        Utils::FileName saveDir = Utils::FileName::fromString(qApp->applicationDirPath() + "/captures");
        QString error;
        bool ok = Utils::UiUtils::screenShot(saveDir, &error);
        if (!ok) {
            qDebug() << error;
        }
        event->accept();
    } else if (key == Qt::Key_W) {
        GM_INSTANCE->m_modeMan->showMenu();
        event->accept();
    } else if (key == Qt::Key_Y) {
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
        emit GM_INSTANCE->m_signalMan->sigShiftOut();
        event->accept();
    } else {
        qDebug() << "按键功能未实现";
        MgsBasePage::keyPressEvent(event); // 默认处理其他键
    }
}

MgsPageArea *MgsMtcInPage::initTradeInfoArea()
{
    MgsPageArea *tradeInfoArea = new MgsPageArea();
    tradeInfoArea->setBackgroundColor(Qt::transparent);
    tradeInfoArea->setMinimumHeight(140);
    tradeInfoArea->setMaximumHeight(180);

    m_totalVehCnt = new ElaText("总过车数: ", tradeInfoArea);
    m_totalCardCnt = new ElaText("总发卡数: ", tradeInfoArea);
    m_noFlagCardCnt = new ElaText("错标卡数: ", tradeInfoArea);
    m_peccanyVehCnt = new ElaText("冲关车数: ", tradeInfoArea);
    m_fleetVehCnt = new ElaText("车队车数: ", tradeInfoArea);
    m_cpcCardCnt = new ElaText("通行卡数: ", tradeInfoArea);
    m_etcCardCnt = new ElaText("闽通卡数: ", tradeInfoArea);
    m_badCardCnt = new ElaText("坏卡数: ", tradeInfoArea);
    m_paperCardCnt = new ElaText("纸券数: ", tradeInfoArea);
    m_holidayFreeVehCnt = new ElaText("动免车数: ", tradeInfoArea);
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
        t->setTextPixelSize(17);
    }

    QGridLayout *tradeInfoAreaLayout = new QGridLayout(tradeInfoArea);
    tradeInfoAreaLayout->setContentsMargins(10, 5, 10, 5);
    tradeInfoAreaLayout->setVerticalSpacing(5);
    tradeInfoAreaLayout->addWidget(m_totalVehCnt, 0, 0);
    tradeInfoAreaLayout->addWidget(m_totalCardCnt, 1, 0);
    tradeInfoAreaLayout->addWidget(m_noFlagCardCnt, 2, 0);
    tradeInfoAreaLayout->addWidget(m_peccanyVehCnt, 3, 0);
    tradeInfoAreaLayout->addWidget(m_fleetVehCnt, 4, 0);
    tradeInfoAreaLayout->addWidget(m_cpcCardCnt, 0, 1);
    tradeInfoAreaLayout->addWidget(m_etcCardCnt, 1, 1);
    tradeInfoAreaLayout->addWidget(m_badCardCnt, 2, 1);
    tradeInfoAreaLayout->addWidget(m_paperCardCnt, 3, 1);
    tradeInfoAreaLayout->addWidget(m_holidayFreeVehCnt, 4, 1);

    return tradeInfoArea;
}

MgsPageArea *MgsMtcInPage::initVehInfoArea()
{
    MgsPageArea *vehInfoArea = new MgsPageArea();
    vehInfoArea->setMinimumHeight(45);
    vehInfoArea->setMaximumHeight(50);
    Utils::UiUtils::applyShadow(vehInfoArea);

    m_plate = new ElaText(vehInfoArea);
    m_plate->setMinimumWidth(145);
    m_plate->setContentsMargins(9, 0, 9, 0);
    m_vehClass = new ElaText(vehInfoArea);
    m_vehClass->setMinimumWidth(145);
    m_vehStatus = new ElaText(vehInfoArea);
    m_vehStatus->setMinimumWidth(100);
    m_situation = new ElaText(vehInfoArea);
    m_situation->setMinimumWidth(100);

    QList<ElaText *> texts = {m_plate, m_vehClass, m_vehStatus, m_situation};
    for (auto *t : texts) {
        t->setTextPixelSize(18);
        t->setAlignment(Qt::AlignCenter);
        t->setIsWrapAnywhere(false);
    }

    QHBoxLayout *vehInfoAreaLayout = new QHBoxLayout(vehInfoArea);
    vehInfoAreaLayout->setContentsMargins(10, 5, 10, 5);
    vehInfoAreaLayout->setSpacing(5);

    vehInfoAreaLayout->addWidget(m_plate);
    Utils::UiUtils::addLine(vehInfoAreaLayout);
    vehInfoAreaLayout->addWidget(m_vehClass);
    Utils::UiUtils::addLine(vehInfoAreaLayout);
    vehInfoAreaLayout->addWidget(m_vehStatus);
    Utils::UiUtils::addLine(vehInfoAreaLayout);
    vehInfoAreaLayout->addWidget(m_situation);
    vehInfoAreaLayout->addStretch();

    return vehInfoArea;
}

MgsPageArea *MgsMtcInPage::initCardInfoArea()
{
    MgsPageArea *cardInfoArea = new MgsPageArea();
    cardInfoArea->setMinimumHeight(130);
    cardInfoArea->setMaximumHeight(150);

    ElaText *text1 = new ElaText("卡类型:", cardInfoArea);
    ElaText *text2 = new ElaText("卡内余额:", cardInfoArea);
    ElaText *text3 = new ElaText("状态:", cardInfoArea);
    ElaText *text4 = new ElaText("卡号:", cardInfoArea);
    ElaText *text5 = new ElaText("入口站名:", cardInfoArea);
    ElaText *text6 = new ElaText("入口时间:", cardInfoArea);

    QFont font = text1->font();
    font.setBold(true);
    text1->setFont(font);
    text2->setFont(font);
    text3->setFont(font);
    text4->setFont(font);
    text5->setFont(font);
    text6->setFont(font);

    m_cardType = new ElaText(cardInfoArea);
    m_cardNum = new ElaText(cardInfoArea);
    m_balance = new ElaText(cardInfoArea);
    m_enTime = new ElaText(cardInfoArea);
    m_enStationName = new ElaText(cardInfoArea);
    m_cardStatus = new ElaText(cardInfoArea);

    QList<ElaText *> texts = {text1,
                              text2,
                              text3,
                              text4,
                              text5,
                              text6,
                              m_cardType,
                              m_cardNum,
                              m_balance,
                              m_enTime,
                              m_enStationName,
                              m_cardStatus};
    for (auto *t : texts) {
        t->setTextPixelSize(17);
        t->setIsWrapAnywhere(false);
    }

    QGridLayout *cardInfoAreaLayout = new QGridLayout(cardInfoArea);
    cardInfoAreaLayout->setContentsMargins(9, 5, 9, 5);
    cardInfoAreaLayout->setHorizontalSpacing(10);
    cardInfoAreaLayout->setVerticalSpacing(5);

    // 设置标签列最小化
    cardInfoAreaLayout->setColumnStretch(0, 0);
    cardInfoAreaLayout->setColumnStretch(2, 0);

    // 设置值列弹性填充
    cardInfoAreaLayout->setColumnStretch(1, 1);
    cardInfoAreaLayout->setColumnStretch(3, 2);

    // 第一行：卡类型、卡号
    cardInfoAreaLayout->addWidget(text1, 0, 0);
    cardInfoAreaLayout->addWidget(m_cardType, 0, 1);
    cardInfoAreaLayout->addWidget(text4, 0, 2);
    cardInfoAreaLayout->addWidget(m_cardNum, 0, 3);

    // 第二行：卡内余额、入口站名
    cardInfoAreaLayout->addWidget(text2, 1, 0);
    cardInfoAreaLayout->addWidget(m_balance, 1, 1);
    cardInfoAreaLayout->addWidget(text5, 1, 2);
    cardInfoAreaLayout->addWidget(m_enStationName, 1, 3);

    // 第三行：状态，入口时间
    cardInfoAreaLayout->addWidget(text3, 2, 0);
    cardInfoAreaLayout->addWidget(m_cardStatus, 2, 1);
    cardInfoAreaLayout->addWidget(text6, 2, 2);
    cardInfoAreaLayout->addWidget(m_enTime, 2, 3);

    return cardInfoArea;
}

MgsPageArea *MgsMtcInPage::initTradeHintArea()
{
    MgsPageArea *tradeHintArea = new MgsPageArea();
    tradeHintArea->setMinimumHeight(120);
    tradeHintArea->setMaximumHeight(160);

    m_tradeHint = new MgsScrollText(tradeHintArea);
    m_tradeHint->setTextPixelSize(30);

    m_obuHint = new ElaText(tradeHintArea);
    m_obuHint->setTextPixelSize(15);
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
        view->setColumnWidth(0, 90);
        view->setColumnWidth(1, 50);
        view->setColumnWidth(2, 140);
        view->setColumnWidth(3, 80);
    });
    return recentTradePanel;
}
