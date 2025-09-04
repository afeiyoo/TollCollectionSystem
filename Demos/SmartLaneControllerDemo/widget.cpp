#include "widget.h"
#include "ui_widget.h"

#include "Logger.h"
#include "relaycontroller.h"
#include "smartlanecontroller.h"
#include "utils/uiutils.h"

#include <QButtonGroup>
#include <QGroupBox>
#include <QMessageBox>
#include <QRadioButton>
#include <QTextBrowser>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    initUi();

    setWindowTitle("智能车道控制器Demo");
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setFixedSize(650, 450);
    Utils::UiUtils::moveToCenter(this);

    m_smartLaneController = new SmartLaneController(this);
    m_relayController = new RelayController(this);
    m_relayController->setSmartLaneCtrl(m_smartLaneController);

    connect(m_smartLaneController, &SmartLaneController::connectStatus, this, [&](bool status) {
        m_connected = status;
        if (m_connected == false) {
            ui->connectButton->setText("连接");
        } else {
            ui->connectButton->setText("断连");
        }
    });

    connect(cuteLogger, &Logger::sigLogWrite, this, &Widget::onLogShow);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initUi()
{
    ui->setupUi(this);

    QList<QLabel *> labels = {ui->ipLabel, ui->portLabel};
    for (auto *label : labels) {
        QFont font = label->font();
        font.setPixelSize(16);
        label->setFont(font);
    }

    ui->portLineEdit->setFixedWidth(50);
    QList<QLineEdit *> lineEdits = {ui->ipLineEdit, ui->portLineEdit};
    for (auto *lineEdit : lineEdits) {
        QFont font = lineEdit->font();
        font.setPixelSize(16);
        lineEdit->setFont(font);
        lineEdit->setFixedHeight(30);
    }

    QList<QPushButton *> buttons = {ui->connectButton, ui->sendButton, ui->resetButton};
    for (auto *button : buttons) {
        QFont font = ui->connectButton->font();
        font.setPixelSize(16);
        button->setFont(font);
        button->setFixedSize(75, 30);
    }

    // 内容页面初始化
    initIOPage();
}

void Widget::initIOPage()
{
    QGroupBox *switchBox = new QGroupBox("设备开关", ui->IOPage);
    QGroupBox *controlBitBox = new QGroupBox("控制位", ui->IOPage);
    QGroupBox *levelBitBox = new QGroupBox("电平位", ui->IOPage);
    QGroupBox *addOffBitBox = new QGroupBox("偏移位", ui->IOPage);

    // --- Switch Box ---
    m_switchGroup = new QButtonGroup(switchBox);
    m_radioBtnOn = new QRadioButton("设备打开", switchBox);
    m_radioBtnOff = new QRadioButton("设备关闭", switchBox);
    m_radioBtnOff->setChecked(true);

    QVBoxLayout *switchLayout = new QVBoxLayout(switchBox);
    switchBox->setLayout(switchLayout);

    QList<QRadioButton *> radioButtons = {m_radioBtnOn, m_radioBtnOff};
    int i = 0;
    for (auto *radioButton : radioButtons) {
        switchLayout->addWidget(radioButton);
        QFont font = radioButton->font();
        font.setPixelSize(16);
        radioButton->setFont(font);
        m_switchGroup->addButton(radioButton, i++);
    }

    // --- Control Bit Box ---
    m_controlGroup = new QButtonGroup(controlBitBox);
    QGridLayout *controlLayout = new QGridLayout(controlBitBox);
    controlLayout->setContentsMargins(5, 5, 5, 5);
    for (int i = 0; i < 16; ++i) {
        QPushButton *btn = new QPushButton(QString::number(i + 1), controlBitBox);
        btn->setFixedSize(45, 45);
        btn->setCheckable(true); // 设置为可切换
        btn->setChecked(false);  // 初始状态，选中或未选中
        QFont font = btn->font();
        font.setPixelSize(16);
        btn->setFont(font);
        m_controlGroup->addButton(btn, i);
        int row = i / 4;
        int col = i % 4;
        controlLayout->addWidget(btn, row, col);
    }

    // --- Level Bit Box ---
    m_levelGroup = new QButtonGroup(levelBitBox);
    QHBoxLayout *levelLayout = new QHBoxLayout(levelBitBox);
    for (int i = 0; i < 2; ++i) {
        QPushButton *btn = new QPushButton(QString::number(i), levelBitBox);
        btn->setFixedSize(45, 45);
        btn->setCheckable(true);
        btn->setChecked(false);
        QFont font = btn->font();
        font.setPixelSize(16);
        btn->setFont(font);
        m_levelGroup->addButton(btn, i);
        levelLayout->addWidget(btn);
    }

    // --- Address Off Bit Box ---
    m_addOffGroup = new QButtonGroup(addOffBitBox);
    QHBoxLayout *addOffLayout = new QHBoxLayout(addOffBitBox);
    for (int i = 0; i < 2; ++i) {
        QPushButton *btn = new QPushButton(QString::number(i), addOffBitBox);
        btn->setFixedSize(45, 45);
        btn->setCheckable(true);
        btn->setChecked(false);
        QFont font = btn->font();
        font.setPixelSize(16);
        btn->setFont(font);
        m_addOffGroup->addButton(btn, i);
        addOffLayout->addWidget(btn);
    }

    QHBoxLayout *boxLayout = new QHBoxLayout();
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->addWidget(addOffBitBox);
    boxLayout->addWidget(levelBitBox);

    QVBoxLayout *subLayout1 = new QVBoxLayout();
    subLayout1->addWidget(switchBox);
    subLayout1->addWidget(controlBitBox, 1);
    subLayout1->addLayout(boxLayout);

    m_logBrowser = new QTextBrowser(ui->IOPage);

    QHBoxLayout *layout = new QHBoxLayout(ui->IOPage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    layout->addLayout(subLayout1, 40);
    layout->addWidget(m_logBrowser, 60);
}

void Widget::on_resetButton_clicked()
{
    // Control Bit 按钮组
    m_controlGroup->setExclusive(false);
    for (QAbstractButton *btn : m_controlGroup->buttons()) {
        btn->setChecked(false);
    }
    m_controlGroup->setExclusive(true);

    // Level Bit 按钮组
    m_levelGroup->setExclusive(false);
    for (QAbstractButton *btn : m_levelGroup->buttons()) {
        btn->setChecked(false);
    }
    m_levelGroup->setExclusive(true);

    m_radioBtnOff->setChecked(true);

    m_relayController->reset();

    m_logBrowser->clear();
}

void Widget::on_sendButton_clicked()
{
    QAbstractButton *ctrlBtn = m_controlGroup->checkedButton();
    if (!ctrlBtn) {
        QMessageBox::warning(this, "提示", "请选择控制位！");
        return;
    }
    int relay = m_controlGroup->id(ctrlBtn) + 1;

    QAbstractButton *levelBtn = m_levelGroup->checkedButton();
    if (!levelBtn) {
        QMessageBox::warning(this, "提示", "请选择电平位！");
        return;
    }
    int level = m_levelGroup->id(levelBtn);
    m_relayController->setRelayForDev(relay, level); // 设置路数与电平位

    int switchState = !(m_switchGroup->checkedId());
    m_relayController->controlDev(relay, switchState);
}

void Widget::onLogShow(const QString &log)
{
    m_logBrowser->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " " + log + "\n");
}

void Widget::on_connectButton_clicked()
{
    if (ui->ipLineEdit->text().isEmpty() || ui->portLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择输入连接信息！");
        return;
    }

    if (m_connected == false) {
        QString ip = ui->ipLineEdit->text();
        uint port = ui->portLineEdit->text().toUInt();
        m_smartLaneController->connectServer(ip, port);
    } else {
        m_smartLaneController->disconnectServer();
    }
}
