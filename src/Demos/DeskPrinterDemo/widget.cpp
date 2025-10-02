#include "widget.h"
#include "ui_widget.h"

#include "HttpClient/src/http.h"
#include "utils/datadealutils.h"

#include <QButtonGroup>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTcpSocket>

using namespace Utils;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setFixedSize(550, 400);
    setWindowTitle("桌面打印机Demo");

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(ui->chkBoxGBK);
    group->addButton(ui->chkBoxUTF);
    group->setExclusive(true); //开启互斥

    ui->peditLog->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->peditLog->setReadOnly(true);

    connect(ui->btnSend, &QPushButton::clicked, this, &Widget::postToPrinter);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::postToPrinter(bool checked)
{
    Q_UNUSED(checked);

    if (ui->peditSend->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "内容错误", "请检查发送内容!", QMessageBox::Cancel);
        return;
    }

    QString content = ui->peditSend->toPlainText();
    QString urlStr = ui->leditUrl->text();
    QUrl url(urlStr);
    if (!url.isValid()) {
        QMessageBox::critical(this, "URL错误", "请检查URL是否正确!", QMessageBox::Cancel);
        return;
    }

    QByteArray encodingContent;
    if (ui->chkBoxGBK->isChecked()) {
        // 转GBK发送
        encodingContent = DataDealUtils::encodeString(content, 1);
    } else {
        // 转UTF8发送
        encodingContent = DataDealUtils::encodeString(content, 2);
    }

    ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | 发送内容:" + encodingContent + "\n");

    auto reply = Http::instance().post(url, encodingContent, "application/json");
    QString res = blockUtilResponse(reply, Http::instance().getReadTimeout());
    ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | " + res + "\n");
}

QString Widget::blockUtilResponse(HttpReply *reply, qint32 timeoutMs)
{
    bool isSuccessful = false;
    QString error;
    // 局部事件循环 + 超时保护
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, [&]() {
        error = QString("响应失败：网络等待超时(%1s)").arg(timeoutMs / 1000);
        loop.quit();
    });
    timer.start(timeoutMs);

    QString result;

    // 连接 finished 信号，一到就拿 body 并退出循环
    QObject::connect(reply, &HttpReply::finished, &loop, [&]() {
        if (reply->isSuccessful()) {
            isSuccessful = true;
            result = reply->body();
        } else {
            error = QString("响应失败: 网络错误(%1, %2)").arg(reply->statusCode()).arg(reply->reasonPhrase());
        }
        loop.quit();
    });

    // 阻塞到 finished() 或者超时
    loop.exec();

    // 清理
    if (timer.isActive())
        timer.stop();
    reply->deleteLater();
    if (!isSuccessful) {
        ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | " + error + "\n");
    }
    return result;
}
