#include "widget.h"
#include "Logger.h"
#include "ui_widget.h"

using namespace Utils;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_infoBoard = std::make_unique<CHXInfoBoard>();
    connect(m_infoBoard.get(), &CHXInfoBoard::sigConnectStatus, this, &Widget::onConnectStatusChange);
    connect(cuteLogger, &Logger::sigLogWrite, this, &Widget::onShowLog);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setItems(const Utils::FileNameList &fnList)
{
    if (fnList.isEmpty()) {
        ui->comboBox->setDisabled(true);
        ui->btnSend->setDisabled(true);
    }

    FileName appDir = FileName::fromString(qApp->applicationDirPath());
    for (const auto &fn : fnList) {
        FileName relativeFn = fn.relativeChildPath(appDir);
        ui->comboBox->addItem(relativeFn.toUserOutput());
    }
}

void Widget::on_btnConnect_clicked()
{
    QString ip = ui->ledtIP->text();
    QString port = ui->ledtPort->text();

    if (!ip.isEmpty() && !port.isEmpty()) {
        m_infoBoard->connectServer(ip, port.toUInt());
    } else {
        ui->textBrowser->append("请填写连接信息\n");
    }
}

void Widget::onConnectStatusChange(bool status)
{
    if (status) {
        ui->btnConnect->setText("断开");
        ui->textBrowser->append("设备已连接\n");
    } else {
        ui->btnConnect->setText("连接");
        ui->textBrowser->append("设备已断开\n");
    }
}

void Widget::on_btnSend_clicked()
{
    QString curStr = ui->comboBox->currentText();

    QRegularExpression re(R"((\d+))"); // 匹配一个或多个数字
    QRegularExpressionMatch match = re.match(curStr);

    if (match.hasMatch()) {
        QString num = match.captured(1);
        ui->textBrowser->append(QString("执行播放文件编号 %1\n").arg(num));
        m_infoBoard->playFile(static_cast<uchar>(num.toInt()));
    } else {
        ui->textBrowser->append("播放文件格式错误\n");
    }
}

void Widget::onShowLog(const QString &log)
{
    ui->textBrowser->append(log + "\n");
}
