#include "mgsmainwindow.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QStackedLayout>
#include <QTimer>
#include <QVBoxLayout>

#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/component/mgsdevicepanel.h"
#include "gui/component/mgsweightinfopanel.h"
#include "gui/mgsbasepage.h"
#include "gui/mgsetcpage.h"
#include "gui/mgsmtcinpage.h"
#include "gui/mgsmtcoutpage.h"
#include "utils/uiutils.h"

using namespace Utils;

MgsMainWindow::MgsMainWindow(QWidget *parent)
    : ElaWindow(parent)
{
    setAppBarHeight(0);
    setIsFixedSize(true);
    setIsNavigationBarEnable(false);
    setWindowButtonFlags(ElaAppBarType::None);
    setWindowIcon(QIcon(Constant::Path::APP_ICON));

    // 程序退出时，清理界面资源
    connect(qApp, &QCoreApplication::aboutToQuit, this, [this]() { MgsMainWindow::deleteLater(); });
}

MgsMainWindow::~MgsMainWindow() {}

void MgsMainWindow::initMtcIn()
{
    m_mainPage = new MgsMtcInPage(this);
    m_mainPage->initUi();

    addPageNode("主页面", m_mainPage, ElaIconType::House);

    m_mainPage->setStationInfo("南靖靖城(4001)");
    m_mainPage->setUserInfo("王博艺(40090)");
    m_mainPage->setLaneID(5);
    m_mainPage->setShiftInfo("2025-07-31 晚班");
    m_mainPage->setModeText("混合入口发卡/券");

    m_mainPage->setVehMode("客车流程");
    m_mainPage->setInfoBoard("车道关闭", Constant::Color::WARN_TEXT);

    m_mainPage->setTotalVehCnt(20);
    m_mainPage->setTotalCardCnt(123);
    m_mainPage->setNoFlagCardCnt(22);
    m_mainPage->setPeccanyVehCnt(20);
    m_mainPage->setFleetVehCnt(23);
    m_mainPage->setCpcCardCnt(21);
    m_mainPage->setEtcCardCnt(214);
    m_mainPage->setBadCardCnt(1);
    m_mainPage->setPaperCardCnt(23);
    m_mainPage->setHolidayFreeVehCnt(23);

    m_mainPage->setFullBlackVer("20250801");
    m_mainPage->setPartBlackVer("202508011215");
    m_mainPage->setVirtualGantryInfo("福州西B向门架(340901.H)");
    m_mainPage->setAppVer("1.0.1");
    m_mainPage->setFeeRateVer("1098");

    m_mainPage->setScrollTip("欢迎使用福建省高速公路收费软件（如遇到软件问题请致电福建省高速公路信息科技有限公司）");

    m_mainPage->setPlate("绿闽ZZ00002");
    m_mainPage->setVehClass("四类专项作业车");
    m_mainPage->setVehStatus("抢险救灾车");
    m_mainPage->setSituation("超限车");

    m_mainPage->setCardType("CPC卡");
    m_mainPage->setCardNum("35012215200600004842");
    m_mainPage->setBalance("888.2");
    m_mainPage->setEnTime("2025-07-12 12:00:04");
    m_mainPage->setEnStationName("福建莆田西站");
    m_mainPage->setCardStatus("正常");

    m_mainPage->setTradeHint("正在交易中，请勿进行其他操作");
    m_mainPage->setObuHint("大件运输车; 蓝闽Z8699A; 普通车; 专二; OBUSN:35011603000799998;");

    m_mainPage->setCurWeightInfo("12轴型 2轴 10.00吨 轴型18.00吨 超限0.00%");
    m_mainPage->setCurWeightInfoCount("车辆数: 4");
    WeightInfoItem item1;
    item1.plate = "闽A12345";
    item1.axisType = 1127;
    item1.axisNum = 6;
    item1.weight = 25.74;
    item1.status = 1;
    m_mainPage->appendWeightInfoItem(item1);
    WeightInfoItem item2;
    item2.plate = "闽B100000";
    item2.axisType = 1127;
    item2.axisNum = 6;
    item2.weight = 25.74;
    item2.status = 0;
    m_mainPage->appendWeightInfoItem(item2);
    WeightInfoItem item3;
    item3.plate = "闽BA50430";
    item3.axisType = 125;
    item3.axisNum = 4;
    item3.weight = 22.74;
    item3.status = 0;
    m_mainPage->appendWeightInfoItem(item3);
    WeightInfoItem item4;
    item4.plate = "闽BA50430";
    item4.axisType = 125;
    item4.axisNum = 4;
    item4.weight = 22.74;
    item4.status = 0;
    m_mainPage->appendWeightInfoItem(item4);

    m_mainPage->appendTradeItem({"闽A12345", "货一", "2025-07-12 12:00:04", "CPC卡", "3501222233334444"});
    m_mainPage->appendTradeItem({"闽A234312", "货二", "2025-07-12 12:00:04", "CPC卡", "3501222233334444"});
    m_mainPage->appendTradeItem({"闽A12345D", "货一", "2025-07-12 12:00:04", "CPC卡", "3501222233334444"});

    m_mainPage->setDeviceList({MgsDevicePanel::CellingLamp,
                               MgsDevicePanel::CapCoil,
                               MgsDevicePanel::Capture,
                               MgsDevicePanel::RailingCoil,
                               MgsDevicePanel::PassingLamp,
                               MgsDevicePanel::RailingMachine,
                               MgsDevicePanel::RSU});

    m_mainPage->setFocusPolicy(Qt::StrongFocus);
    m_mainPage->setFocus();
}

void MgsMainWindow::initMtcOut()
{
    m_mainPage = new MgsMtcOutPage(this);
    m_mainPage->initUi();

    addPageNode("主页面", m_mainPage, ElaIconType::House);

    m_mainPage->setStationInfo("南靖靖城(4001)");
    m_mainPage->setUserInfo("王博艺(40090)");
    m_mainPage->setLaneID(5);
    m_mainPage->setShiftInfo("2025-07-31 晚班");
    m_mainPage->setModeText("正常过车模式");

    m_mainPage->setVehMode("客车流程");

    m_mainPage->setStartTicketNum(20240429);
    m_mainPage->setCurTicketNum(20240429);
    m_mainPage->setNormalTicketCnt(24);
    m_mainPage->setScrapTicketCnt(0);
    m_mainPage->setRestTicketCnt(6990);
    m_mainPage->setTotalVehCnt(20);
    m_mainPage->setRecycleCardCnt(23);
    m_mainPage->setEtcCardCnt(13);
    m_mainPage->setFleetVehCnt(0);
    m_mainPage->setThirdPayCnt(34);

    m_mainPage->setScrollTip("欢迎使用福建省高速公路收费软件（如遇到软件问题请致电福建省高速公路信息科技有限公司）");

    m_mainPage->setPlate("蓝闽HZ0006");
    m_mainPage->setVehClass("一类货车");
    m_mainPage->setVehStatus("J1集装箱车");
    m_mainPage->setSituation("超限");

    m_mainPage->setCardType("CPC卡");
    m_mainPage->setCardNum("35012042230602103792");
    m_mainPage->setEnTime("2025-07-12 12:00:04");
    m_mainPage->setEnStationName("福建莆田西站");
    m_mainPage->setWeightInfo("轴数:2 称重:5600 车型:11");
    m_mainPage->setEnPlate("蓝闽HZ0006");
    m_mainPage->setLabel1("350116030008000023432489258;3249803;蓝闽HZ0006，货一");
    m_mainPage->setLabel2("蓝闽HZ0006，货一");

    m_mainPage->appendHintButton("第三方支付");
    m_mainPage->appendHintButton("现金支付", "#ffffff", "#e5720d");
    m_mainPage->setTradeHint("正在交易中，请勿进行其他操作");
    m_mainPage->setObuHint("蓝闽HZ0006;客车5座;非集装箱运输车;卡号:35012042230602103792;");

    m_mainPage->setCurWeightInfo("12轴型 2轴 10.00吨 轴型18.00吨 超限0.00%");
    m_mainPage->setCurWeightInfoCount("车辆数: 4");
    WeightInfoItem item1;
    item1.plate = "闽A12345";
    item1.axisType = 1127;
    item1.axisNum = 6;
    item1.weight = 25.74;
    item1.status = 1;
    m_mainPage->appendWeightInfoItem(item1);
    WeightInfoItem item2;
    item2.plate = "闽B100000";
    item2.axisType = 1127;
    item2.axisNum = 6;
    item2.weight = 25.74;
    item2.status = 0;
    m_mainPage->appendWeightInfoItem(item2);
    WeightInfoItem item3;
    item3.plate = "闽BA50430";
    item3.axisType = 125;
    item3.axisNum = 4;
    item3.weight = 22.74;
    item3.status = 0;
    m_mainPage->appendWeightInfoItem(item3);
    WeightInfoItem item4;
    item4.plate = "闽BA50430";
    item4.axisType = 125;
    item4.axisNum = 4;
    item4.weight = 22.74;
    item4.status = 0;
    m_mainPage->appendWeightInfoItem(item4);

    for (int i = 0; i < 101; i++) {
        m_mainPage->appendTradeItem(
            {"闽A12345", "货一", "2025-07-12 12:00:04", "2.39/电子支付", "35012042230602103792"});
    }
    m_mainPage->appendTradeItem({"闽A12345", "货一", "2025-07-12 12:00:04", "1321.2/电子支付", "35012042230602103792"});
    m_mainPage->appendTradeItem(
        {"闽A12345", "货一", "2025-07-12 12:00:04", "1323.2/第三方支付", "35012042230602103792"});
    m_mainPage->appendTradeItem({"闽A12345F", "货一", "2025-07-12 12:00:04", "234/现金支付", "35012042230602103792"});
    m_mainPage->appendTradeItem(
        {"闽A12345", "货一", "2025-07-12 12:00:04", "1234.5/第三方支付", "35012042230602103792"});
    m_mainPage->appendTradeItem({"闽A12345", "货一", "2025-07-12 12:00:04", "4/现金支付", "35012042230602103792"});

    m_mainPage->setDeviceList({MgsDevicePanel::Weight,
                               MgsDevicePanel::FirstCoil,
                               MgsDevicePanel::CellingLamp,
                               MgsDevicePanel::CapCoil,
                               MgsDevicePanel::Capture,
                               MgsDevicePanel::RailingCoil,
                               MgsDevicePanel::PassingLamp,
                               MgsDevicePanel::RailingMachine,
                               MgsDevicePanel::RSU});

    m_mainPage->setFocusPolicy(Qt::StrongFocus);
    m_mainPage->setFocus();
}

void MgsMainWindow::initEtc()
{
    m_mainPage = new MgsEtcPage(this);
    m_mainPage->initUi();

    addPageNode("主页面", m_mainPage, ElaIconType::House);

    m_mainPage->setStationInfo("南靖靖城(4001)");
    m_mainPage->setUserInfo("王博艺(40090)");
    m_mainPage->setLaneID(5);
    m_mainPage->setShiftInfo("2025-07-31 晚班");
    m_mainPage->setModeText("ETC入口");

    m_mainPage->setVehMode("称重降级", Constant::Color::RED_COLOR);

    m_mainPage->setTotalVehCnt(32);
    m_mainPage->setNormalVehCnt(20);
    m_mainPage->setFreeVehCnt(1);
    m_mainPage->setTotalToll(234.1);
    m_mainPage->setCreditCardCnt(24);
    m_mainPage->setPrePayCardCnt(2);
    m_mainPage->setHolidayFreeVehCnt(1);
    m_mainPage->setPeccanyVehCnt(2);
    m_mainPage->setLastShiftTotalVehCnt(18);

    m_mainPage->setScrollTip("欢迎使用福建省高速公路收费软件（如遇到软件问题请致电福建省高速公路信息科技有限公司）");

    m_mainPage->setPlate("蓝闽HZ0006");
    m_mainPage->setVehClass("一类货车");
    m_mainPage->setVehStatus("普通车");

    m_mainPage->setCardType("CPC卡");
    m_mainPage->setCardNum("35012042230602103792");
    m_mainPage->setEnTime("2025-07-12 12:00:04");
    m_mainPage->setEnStationName("福建莆田西站");
    m_mainPage->setBalance("2304.4");
    m_mainPage->setToll("0");

    m_mainPage->setTradeHint("等待交易...");

    m_mainPage->setCurWeightInfo("无称重数据");
    m_mainPage->setCurWeightInfoCount("车辆数: 4");
    WeightInfoItem item1;
    item1.plate = "闽A12345";
    item1.axisType = 0;
    item1.axisNum = 0;
    item1.weight = 0;
    item1.status = 1;
    m_mainPage->appendWeightInfoItem(item1);
    WeightInfoItem item2;
    item2.plate = "闽B100000";
    item2.axisType = 0;
    item2.axisNum = 0;
    item2.weight = 0;
    item2.status = 0;
    m_mainPage->appendWeightInfoItem(item2);

    for (int i = 0; i < 101; i++) {
        m_mainPage->appendTradeItem(
            {"闽A123BDK", "货一", "12:00:04", "莆田西", "记账卡", "23.4", "35011621230300068933"});
    }

    m_mainPage->setDeviceList({
        MgsDevicePanel::FirstCoil,
        MgsDevicePanel::SecondCoil,
        MgsDevicePanel::ThirdCoil,
        MgsDevicePanel::FourthCoil,
        MgsDevicePanel::CellingLamp,
        MgsDevicePanel::CapCoil,
        MgsDevicePanel::Capture,
        MgsDevicePanel::RSU,
        MgsDevicePanel::RailingCoil,
        MgsDevicePanel::PassingLamp,
        MgsDevicePanel::RailingMachine,
    });

    m_mainPage->setFocusPolicy(Qt::StrongFocus);
    m_mainPage->setFocus();
}

void MgsMainWindow::showFormErrorHint(const QString &title, const QStringList &strs)
{
    emit GM_INSTANCE->m_signalMan->sigLogAppend(EM_LogLevel::ERROR, title);
    QString message = strs.join("<br/>");
    UiUtils::showMessageBoxError(title, message, QMessageBox::Yes | QMessageBox::Cancel);
}

void MgsMainWindow::showFormInfoHint(const QString &title, const QStringList &strs)
{
    emit GM_INSTANCE->m_signalMan->sigLogAppend(EM_LogLevel::INFO, title);
    QString message = strs.join("<br/>");
    UiUtils::showMessageBoxInfo(title, message, QMessageBox::Yes | QMessageBox::Cancel);
}

void MgsMainWindow::showFormQuestionHint(const QString &title, const QStringList &strs)
{
    emit GM_INSTANCE->m_signalMan->sigLogAppend(EM_LogLevel::INFO, title);
    QString message = strs.join("<br/>");
    UiUtils::showMessageBoxQuestion(title, message, QMessageBox::Yes | QMessageBox::Cancel);
}

void MgsMainWindow::showFormWarningHint(const QString &title, const QStringList &strs)
{
    emit GM_INSTANCE->m_signalMan->sigLogAppend(EM_LogLevel::WARN, title);
    QString message = strs.join("<br/>");
    UiUtils::showMessageBoxWarning(title, message, QMessageBox::Yes | QMessageBox::Cancel);
}

void MgsMainWindow::showEvent(QShowEvent *event)
{
    // setIsStayTop(true); // 窗口置顶
    ElaWindow::showEvent(event);
}
