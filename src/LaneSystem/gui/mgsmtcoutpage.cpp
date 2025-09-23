#include "mgsmtcoutpage.h"

#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaTableView.h"
#include "ElaWidgetTools/ElaText.h"
#include "Logger.h"
#include "config/config.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/component/mgsmenu.h"
#include "gui/component/mgspagearea.h"
#include "gui/component/mgsrecenttradepanel.h"
#include "gui/component/mgsscrolltext.h"
#include "gui/mgsauthdialog.h"
#include "gui/mgsplateeditdialog.h"
#include "utils/bizutils.h"
#include "utils/uiutils.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QVBoxLayout>

using namespace Utils;

MgsMtcOutPage::MgsMtcOutPage(QWidget *parent)
    : MgsBasePage(parent)
{
    menu = new MgsMenu(this);
    menu->setFixedSize(350, 450);
    menu->hide();

    m_authDialog = new MgsAuthDialog(this);
    m_authDialog->hide();

    connect(GM_INSTANCE->m_signalMan, &SignalManager::sigPlateChanged, this, &MgsMtcOutPage::onPlateChanged);
}

MgsMtcOutPage::~MgsMtcOutPage() {}

void MgsMtcOutPage::setStartTicketNum(int num)
{
    if (!m_startTicketNum)
        return;
    m_startTicketNum->setText(QString("起始票据: %1").arg(num));
}

void MgsMtcOutPage::setCurTicketNum(int num)
{
    if (!m_curTicketNum)
        return;
    m_curTicketNum->setText(QString("当前票据: %1").arg(num));
}

void MgsMtcOutPage::setNormalTicketCnt(int cnt)
{
    if (!m_normalTicketCnt)
        return;
    m_normalTicketCnt->setText(QString("正常票数: %1").arg(cnt));
}

void MgsMtcOutPage::setScrapTicketCnt(int cnt)
{
    if (!m_scrapTicketCnt)
        return;
    m_scrapTicketCnt->setText(QString("废票张数: %1").arg(cnt));
}

void MgsMtcOutPage::setRestTicketCnt(int cnt)
{
    if (!m_restTicketCnt)
        return;
    m_restTicketCnt->setText(QString("剩余票据: %1").arg(cnt));
}

void MgsMtcOutPage::setRecycleCardCnt(int cnt)
{
    if (!m_recycleCardCnt)
        return;
    m_recycleCardCnt->setText(QString("回收卡数: %1").arg(cnt));
}

void MgsMtcOutPage::setThirdPayCnt(int cnt)
{
    if (!m_thirdPayCnt)
        return;
    m_thirdPayCnt->setText(QString("第三方: %1").arg(cnt));
}

void MgsMtcOutPage::setTotalVehCnt(int cnt)
{
    if (!m_totalVehCnt)
        return;
    m_totalVehCnt->setText(QString("总过车数: %1").arg(cnt));
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
    m_fleetVehCnt->setText(QString("车队车数: %1").arg(cnt));
}

void MgsMtcOutPage::setCpcCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsMtcOutPage::setEtcCardCnt(int cnt)
{
    if (!m_etcCardCnt)
        return;
    m_etcCardCnt->setText(QString("闽通卡数: %1").arg(cnt));
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
        m_plate->setStyleSheet(
            QString("border-image: url(%1) 0 0 0 0 stretch stretch; color: %2;").arg(bgImage).arg(textColor.name()));
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
    m_situation->setStyleSheet(QString("color: %1").arg(Constant::Color::RED_COLOR));
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
    m_tradeHint->setText(tradeHint);
    m_tradeHint->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsMtcOutPage::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    LOG_INFO().noquote() << "接收到按键:" << key << "，"
                         << BizUtils::getKeyName(GM_INSTANCE->m_config->m_keyboard, key);
    // if (key == Qt::Key_S) {
    //     event->accept();
    // } else if (key == Qt::Key_I) {
    //     m_authDialog->show();
    //     event->accept();
    // } else if (key == Qt::Key_Right) {
    //     MgsPlateEditDialog *dlg = new MgsPlateEditDialog();
    //     dlg->setAttribute(Qt::WA_DeleteOnClose);
    //     dlg->setPlate(m_plate->text());
    //     dlg->show();
    //     event->accept();
    // } else if (key == Qt::Key_W) {
    //     menu->show();
    //     menu->setFocus();
    //     event->accept();
    // } else {
    //     qDebug() << "无效按键";
    //     MgsBasePage::keyPressEvent(event); // 默认处理其他键
    // }
}

MgsPageArea *MgsMtcOutPage::initTradeInfoArea()
{
    MgsPageArea *tradeInfoArea = new MgsPageArea();
    tradeInfoArea->setBackgroundColor(Qt::transparent);
    tradeInfoArea->setMinimumHeight(140);
    tradeInfoArea->setMaximumHeight(180);

    m_startTicketNum = new ElaText("起始票据: ", tradeInfoArea);
    m_curTicketNum = new ElaText("当前票据: ", tradeInfoArea);
    m_normalTicketCnt = new ElaText("正常票数: ", tradeInfoArea);
    m_scrapTicketCnt = new ElaText("废票张数: ", tradeInfoArea);
    m_restTicketCnt = new ElaText("剩余票数: ", tradeInfoArea);
    m_totalVehCnt = new ElaText("总过车数: ", tradeInfoArea);
    m_recycleCardCnt = new ElaText("回收卡数: ", tradeInfoArea);
    m_etcCardCnt = new ElaText("闽通卡数: ", tradeInfoArea);
    m_fleetVehCnt = new ElaText("车队车数: ", tradeInfoArea);
    m_thirdPayCnt = new ElaText("第三方: ", tradeInfoArea);

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
        t->setTextPixelSize(17);
    }

    QGridLayout *tradeInfoAreaLayout = new QGridLayout(tradeInfoArea);
    tradeInfoAreaLayout->setContentsMargins(10, 5, 10, 5);
    tradeInfoAreaLayout->setVerticalSpacing(5);
    tradeInfoAreaLayout->addWidget(m_startTicketNum, 0, 0);
    tradeInfoAreaLayout->addWidget(m_curTicketNum, 1, 0);
    tradeInfoAreaLayout->addWidget(m_normalTicketCnt, 2, 0);
    tradeInfoAreaLayout->addWidget(m_scrapTicketCnt, 3, 0);
    tradeInfoAreaLayout->addWidget(m_restTicketCnt, 4, 0);
    tradeInfoAreaLayout->addWidget(m_totalVehCnt, 0, 1);
    tradeInfoAreaLayout->addWidget(m_recycleCardCnt, 1, 1);
    tradeInfoAreaLayout->addWidget(m_etcCardCnt, 2, 1);
    tradeInfoAreaLayout->addWidget(m_fleetVehCnt, 3, 1);
    tradeInfoAreaLayout->addWidget(m_thirdPayCnt, 4, 1);

    return tradeInfoArea;
}

MgsPageArea *MgsMtcOutPage::initVehInfoArea()
{
    MgsPageArea *vehInfoArea = new MgsPageArea();
    vehInfoArea->setMinimumHeight(45);
    vehInfoArea->setMaximumHeight(50);
    Utils::UiUtils::applyShadow(vehInfoArea);

    m_plate = new ElaText(vehInfoArea);
    m_plate->setContentsMargins(9, 0, 9, 0);
    m_plate->setMinimumWidth(145);
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

MgsPageArea *MgsMtcOutPage::initCardInfoArea()
{
    MgsPageArea *cardInfoArea = new MgsPageArea();
    cardInfoArea->setMinimumHeight(130);
    cardInfoArea->setMaximumHeight(150);

    ElaText *text1 = new ElaText("卡 号:", cardInfoArea);
    ElaText *text2 = new ElaText("类 型:", cardInfoArea);
    ElaText *text3 = new ElaText("站 名:", cardInfoArea);
    ElaText *text4 = new ElaText("时 间:", cardInfoArea);
    ElaText *text5 = new ElaText("称 重:", cardInfoArea);
    ElaText *text6 = new ElaText("车 牌:", cardInfoArea);
    ElaText *text7 = new ElaText("标 签:", cardInfoArea);
    ElaText *text8 = new ElaText("分省信息:", cardInfoArea);

    QFont font = text1->font();
    font.setBold(true);
    for (auto *t : {text1, text2, text3, text4, text5, text6, text7, text8}) {
        t->setFont(font);
    }

    m_cardType = new ElaText(cardInfoArea);
    m_cardNum = new ElaText(cardInfoArea);
    m_enStationName = new ElaText(cardInfoArea);
    m_enTime = new ElaText(cardInfoArea);
    m_weightInfo = new ElaText(cardInfoArea);
    m_enPlate = new ElaText(cardInfoArea);
    m_label1 = new ElaText(cardInfoArea);
    m_splitProvincesInfo = new ElaText(cardInfoArea);

    QList<ElaText *> texts = {text1,
                              text2,
                              text3,
                              text4,
                              text5,
                              text6,
                              text7,
                              text8,
                              m_cardType,
                              m_cardNum,
                              m_enStationName,
                              m_enTime,
                              m_weightInfo,
                              m_enPlate,
                              m_label1,
                              m_splitProvincesInfo};
    for (auto *t : texts) {
        t->setTextPixelSize(17);
        t->setIsWrapAnywhere(false);
    }

    // 每行都用 QHBoxLayout 控制
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->setSpacing(10);
    row1->addWidget(text1);
    row1->addWidget(m_cardNum, 1);
    row1->addWidget(text2);
    row1->addWidget(m_cardType, 1);

    QHBoxLayout *row2 = new QHBoxLayout();
    row2->setSpacing(10);
    row2->addWidget(text3);
    row2->addWidget(m_enStationName, 1);
    row2->addWidget(text4);
    row2->addWidget(m_enTime, 1);

    QHBoxLayout *row3 = new QHBoxLayout();
    row3->setSpacing(10);
    row3->addWidget(text5);
    row3->addWidget(m_weightInfo, 1);
    row3->addWidget(text6);
    row3->addWidget(m_enPlate, 1);

    QHBoxLayout *row4 = new QHBoxLayout();
    row4->setSpacing(10);
    row4->addWidget(text7);
    row4->addWidget(m_label1, 1);

    QHBoxLayout *row5 = new QHBoxLayout();
    row5->setSpacing(10);
    row5->addWidget(text8);
    row5->addWidget(m_splitProvincesInfo, 1);

    QVBoxLayout *vbox = new QVBoxLayout(cardInfoArea);
    vbox->setContentsMargins(9, 5, 9, 5);
    vbox->setSpacing(5);
    vbox->addLayout(row1);
    vbox->addLayout(row2);
    vbox->addLayout(row3);
    vbox->addLayout(row4);
    vbox->addLayout(row5);

    return cardInfoArea;
}

MgsPageArea *MgsMtcOutPage::initTradeHintArea()
{
    MgsPageArea *tradeHintArea = new MgsPageArea();
    tradeHintArea->setMinimumHeight(120);
    tradeHintArea->setMaximumHeight(160);

    m_hintButtonLayout = new QHBoxLayout();
    m_hintButtonLayout->setContentsMargins(0, 0, 0, 0);
    m_hintButtonLayout->setSpacing(5);
    m_hintButtonLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_hintButtonLayout->addStretch();

    m_tradeHint = new MgsScrollText(tradeHintArea);
    m_tradeHint->setTextPixelSize(30);

    m_obuHint = new ElaText(tradeHintArea);
    m_obuHint->setTextPixelSize(15);
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
        view->setColumnWidth(0, 90);
        view->setColumnWidth(1, 40);
        view->setColumnWidth(2, 140);
        view->setColumnWidth(3, 120);
    });

    return recentTradePanel;
}
