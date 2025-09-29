#include "mgsetcpage.h"

#include "ElaWidgetTools/ElaTableView.h"
#include "ElaWidgetTools/ElaText.h"
#include "global/constant.h"
#include "gui/component/mgspagearea.h"
#include "gui/component/mgsrecenttradepanel.h"
#include "gui/component/mgsscrolltext.h"
#include "utils/uiutils.h"

#include <QDebug>
#include <QGridLayout>
#include <QKeyEvent>

MgsEtcPage::MgsEtcPage(QWidget *parent)
    : MgsBasePage(parent)
{}

MgsEtcPage::~MgsEtcPage() {}

void MgsEtcPage::setStartTicketNum(int num)
{
    Q_UNUSED(num);
}

void MgsEtcPage::setCurTicketNum(int num)
{
    Q_UNUSED(num);
}

void MgsEtcPage::setNormalTicketCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setScrapTicketCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setRestTicketCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setRecycleCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setThirdPayCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setTotalVehCnt(int cnt)
{
    if (!m_totalVehCnt)
        return;
    m_totalVehCnt->setText(QString("总过车数: %1").arg(cnt));
}

void MgsEtcPage::setTotalCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setNoFlagCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setPeccanyVehCnt(int cnt)
{
    if (!m_peccanyVehCnt)
        return;
    m_peccanyVehCnt->setText(QString("冲关车数: %1").arg(cnt));
}

void MgsEtcPage::setFleetVehCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setCpcCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setEtcCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setBadCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setPaperCardCnt(int cnt)
{
    Q_UNUSED(cnt);
}

void MgsEtcPage::setHolidayFreeVehCnt(int cnt)
{
    if (!m_holidayFreeVehCnt)
        return;
    m_holidayFreeVehCnt->setText(QString("动免车数: %1").arg(cnt));
}

void MgsEtcPage::setNormalVehCnt(int cnt)
{
    if (!m_normalVehCnt)
        return;
    m_normalVehCnt->setText(QString("普通车数: %1").arg(cnt));
}

void MgsEtcPage::setFreeVehCnt(int cnt)
{
    if (!m_freeVehCnt)
        return;
    m_freeVehCnt->setText(QString("免征车数: %1").arg(cnt));
}

void MgsEtcPage::setTotalToll(qreal toll)
{
    if (!m_totalToll)
        return;
    m_totalToll->setText(QString("收费总额: %1").arg(toll));
}

void MgsEtcPage::setCreditCardCnt(int cnt)
{
    if (!m_creditCardCnt)
        return;
    m_creditCardCnt->setText(QString("记账卡数: %1").arg(cnt));
}

void MgsEtcPage::setPrePayCardCnt(int cnt)
{
    if (!m_prePayCardCnt)
        return;
    m_prePayCardCnt->setText(QString("储值卡数: %1").arg(cnt));
}

void MgsEtcPage::setLastShiftTotalVehCnt(int cnt)
{
    if (!m_lastShiftTotalVehCnt)
        return;
    m_lastShiftTotalVehCnt->setText(QString("上个班次总车次: %1").arg(cnt));
}

void MgsEtcPage::setPlate(const QString &plate)
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

void MgsEtcPage::setVehClass(const QString &vehClass)
{
    if (!m_vehClass)
        return;
    m_vehClass->setText(vehClass);
}

void MgsEtcPage::setVehStatus(const QString &vehStatus)
{
    if (!m_vehStatus)
        return;
    m_vehStatus->setText(vehStatus);
}

void MgsEtcPage::setSituation(const QString &situation)
{
    Q_UNUSED(situation);
}

void MgsEtcPage::setCardType(const QString &cardType)
{
    if (!m_cardType)
        return;
    m_cardType->setText(cardType);
}

void MgsEtcPage::setCardNum(const QString &cardNum)
{
    if (!m_cardNum)
        return;
    m_cardNum->setText(cardNum);
}

void MgsEtcPage::setBalance(const QString &balance)
{
    if (!m_balance)
        return;
    m_balance->setText(balance);
}

void MgsEtcPage::setEnTime(const QString &enTime)
{
    if (!m_enTime)
        return;
    m_enTime->setText(enTime);
}

void MgsEtcPage::setEnStationName(const QString &enStationName)
{
    if (!m_enStationName)
        return;
    m_enStationName->setText(enStationName);
}

void MgsEtcPage::setCardStatus(const QString &cardStatus)
{
    Q_UNUSED(cardStatus);
}

void MgsEtcPage::setWeightInfo(const QString &info)
{
    Q_UNUSED(info);
}

void MgsEtcPage::setEnPlate(const QString &plate)
{
    Q_UNUSED(plate);
}

void MgsEtcPage::setLabel1(const QString &info)
{
    Q_UNUSED(info);
}

void MgsEtcPage::setLabel2(const QString &info)
{
    Q_UNUSED(info);
}

void MgsEtcPage::setToll(const QString &toll)
{
    if (!m_toll)
        return;
    m_toll->setText(toll);
}

void MgsEtcPage::setObuHint(const QString &obuHint, const QString &color)
{
    Q_UNUSED(obuHint);
    Q_UNUSED(color);
}

void MgsEtcPage::appendHintButton(const QString &hint, const QString &fontColor, const QString &bgColor)
{
    Q_UNUSED(hint);
    Q_UNUSED(fontColor);
    Q_UNUSED(bgColor);
}

void MgsEtcPage::setTradeHint(const QString &tradeHint, const QString &color)
{
    m_tradeHint->setText(tradeHint);
    m_tradeHint->setStyleSheet(QString("color: %1;").arg(color));
}

void MgsEtcPage::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    qDebug() << "KeyName:" << QKeySequence(key).toString();

    if (key == Qt::Key_S) {
        event->accept();
    } else if (key == Qt::Key_I) {
        event->accept();
    } else {
        qDebug() << "无效按键";
        MgsBasePage::keyPressEvent(event); // 默认处理其他键
    }
}

MgsPageArea *MgsEtcPage::initTradeInfoArea()
{
    MgsPageArea *tradeInfoArea = new MgsPageArea();
    tradeInfoArea->setBackgroundColor(Qt::transparent);
    tradeInfoArea->setMinimumHeight(140);
    tradeInfoArea->setMaximumHeight(180);

    m_totalVehCnt = new ElaText("总过车数: ", tradeInfoArea);
    m_normalVehCnt = new ElaText("普通车数: ", tradeInfoArea);
    m_freeVehCnt = new ElaText("免征车数: ", tradeInfoArea);
    m_totalToll = new ElaText("收费总额: ", tradeInfoArea);
    m_creditCardCnt = new ElaText("记账卡数: ", tradeInfoArea);
    m_prePayCardCnt = new ElaText("储值卡数: ", tradeInfoArea);
    m_holidayFreeVehCnt = new ElaText("动免车数: ", tradeInfoArea);
    m_peccanyVehCnt = new ElaText("冲关车数: ", tradeInfoArea);
    m_lastShiftTotalVehCnt = new ElaText("上个班次总车次: ", tradeInfoArea);
    QList<ElaText *> texts = {m_totalVehCnt,
                              m_normalVehCnt,
                              m_freeVehCnt,
                              m_totalToll,
                              m_creditCardCnt,
                              m_prePayCardCnt,
                              m_holidayFreeVehCnt,
                              m_peccanyVehCnt,
                              m_lastShiftTotalVehCnt};
    for (auto *t : texts) {
        t->setIsWrapAnywhere(false);
        t->setTextPixelSize(17);
    }

    QGridLayout *tradeInfoAreaLayout = new QGridLayout(tradeInfoArea);
    tradeInfoAreaLayout->setContentsMargins(10, 5, 10, 5);
    tradeInfoAreaLayout->setVerticalSpacing(5);
    tradeInfoAreaLayout->addWidget(m_totalVehCnt, 0, 0);
    tradeInfoAreaLayout->addWidget(m_normalVehCnt, 1, 0);
    tradeInfoAreaLayout->addWidget(m_freeVehCnt, 2, 0);
    tradeInfoAreaLayout->addWidget(m_totalToll, 3, 0);
    tradeInfoAreaLayout->addWidget(m_creditCardCnt, 0, 1);
    tradeInfoAreaLayout->addWidget(m_prePayCardCnt, 1, 1);
    tradeInfoAreaLayout->addWidget(m_holidayFreeVehCnt, 2, 1);
    tradeInfoAreaLayout->addWidget(m_peccanyVehCnt, 3, 1);
    tradeInfoAreaLayout->addWidget(m_lastShiftTotalVehCnt, 4, 0, 1, 2);

    return tradeInfoArea;
}

MgsPageArea *MgsEtcPage::initVehInfoArea()
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
    m_vehClass->setMinimumWidth(150);
    m_vehStatus = new ElaText(carInfoWidget);
    m_vehStatus->setMinimumWidth(150);
    QList<ElaText *> carTexts = {m_plate, m_vehClass, m_vehStatus};
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
    carInfoHLayout->addStretch();

    // 卡内信息区
    QWidget *cardInfoWidget = new QWidget(vehInfoArea);
    cardInfoWidget->setMinimumHeight(120);
    cardInfoWidget->setMaximumHeight(140);

    ElaText *label1 = new ElaText("卡类型:", cardInfoWidget);
    ElaText *label2 = new ElaText("卡号:", cardInfoWidget);
    ElaText *label3 = new ElaText("卡内余额:", cardInfoWidget);
    ElaText *label4 = new ElaText("收费金额:", cardInfoWidget);
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
    m_toll = new ElaText(cardInfoWidget);
    QList<ElaText *> cardTexts = {m_cardType, m_cardNum, m_balance, m_enTime, m_enStationName, m_toll};
    for (auto *t : cardTexts) {
        QFont font = t->font();
        font.setPixelSize(Constant::FontSize::VEHINFO_AREA_SIZE);
        t->setFont(font);
        t->setIsWrapAnywhere(false);
    }

    QGridLayout *cardInfoAreaLayout = new QGridLayout(cardInfoWidget);
    cardInfoAreaLayout->setContentsMargins(9, 5, 9, 5);
    cardInfoAreaLayout->setHorizontalSpacing(5);
    cardInfoAreaLayout->setVerticalSpacing(5);

    // 设置标签列最小化，值列弹性拉伸
    cardInfoAreaLayout->setColumnStretch(0, 0);
    cardInfoAreaLayout->setColumnStretch(2, 0);
    cardInfoAreaLayout->setColumnStretch(1, 1);
    cardInfoAreaLayout->setColumnStretch(3, 2);

    // 第一行：卡类型、卡号
    cardInfoAreaLayout->addWidget(label1, 0, 0);
    cardInfoAreaLayout->addWidget(m_cardType, 0, 1);
    cardInfoAreaLayout->addWidget(label2, 0, 2);
    cardInfoAreaLayout->addWidget(m_cardNum, 0, 3);

    // 第二行：卡内余额、收费
    cardInfoAreaLayout->addWidget(label3, 1, 0);
    cardInfoAreaLayout->addWidget(m_balance, 1, 1);
    cardInfoAreaLayout->addWidget(label4, 1, 2);
    cardInfoAreaLayout->addWidget(m_toll, 1, 3);

    // 第三行：状态，入口时间
    cardInfoAreaLayout->addWidget(label5, 2, 0);
    cardInfoAreaLayout->addWidget(m_enStationName, 2, 1);
    cardInfoAreaLayout->addWidget(label6, 2, 2);
    cardInfoAreaLayout->addWidget(m_enTime, 2, 3);

    QVBoxLayout *vehInfoAreaLayout = new QVBoxLayout(vehInfoArea);
    vehInfoAreaLayout->setSpacing(0);
    vehInfoAreaLayout->setContentsMargins(0, 0, 0, 0);
    vehInfoAreaLayout->addWidget(carInfoWidget);
    Utils::UiUtils::addLine(vehInfoAreaLayout, Qt::Horizontal, 2, Constant::Color::BORDER);
    vehInfoAreaLayout->addWidget(cardInfoWidget);

    return vehInfoArea;
}

MgsPageArea *MgsEtcPage::initTradeHintArea()
{
    MgsPageArea *tradeHintArea = new MgsPageArea();
    tradeHintArea->setMinimumHeight(120);
    tradeHintArea->setMaximumHeight(160);

    m_tradeHint = new MgsScrollText(tradeHintArea);
    m_tradeHint->setTextPixelSize(30);

    QVBoxLayout *tradeHintAreaLayout = new QVBoxLayout(tradeHintArea);
    tradeHintAreaLayout->setContentsMargins(10, 5, 10, 5);
    tradeHintAreaLayout->setSpacing(0);
    tradeHintAreaLayout->addStretch();
    tradeHintAreaLayout->addWidget(m_tradeHint);
    tradeHintAreaLayout->addStretch();

    return tradeHintArea;
}

MgsRecentTradePanel *MgsEtcPage::initRecentTradeArea()
{
    QStringList header = {"车牌", "车型", "时间", "入口站", "卡类型", "收费", "卡号"};
    MgsRecentTradePanel *recentTradePanel = new MgsRecentTradePanel(header);
    ElaTableView *view = recentTradePanel->getRecentTradeView();
    // 初始列表宽度
    connect(view, &ElaTableView::tableViewShow, this, [=]() {
        view->setColumnWidth(0, 85);
        view->setColumnWidth(1, 45);
        view->setColumnWidth(2, 70);
        view->setColumnWidth(3, 100);
        view->setColumnWidth(4, 50);
        view->setColumnWidth(5, 50);
    });
    return recentTradePanel;
}
