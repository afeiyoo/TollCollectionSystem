#include "widget.h"
#include "ui_widget.h"

#include "HttpClient/src/http.h"
#include "utils/datadealutils.h"
#include "utils/uiutils.h"

#include <QButtonGroup>
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

    ui->peditLog->setReadOnly(true);

    connect(ui->btnSend, &QPushButton::clicked, this, &Widget::postToPrinter);
}

Widget::~Widget()
{
    delete ui;
}

#if 0
void Widget::postToPrinter(bool checked)
{
    Q_UNUSED(checked);

    if (ui->peditSend->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "发送内容为空", "请检查发送内容!", QMessageBox::Cancel);
        return;
    }

    QString content = ui->peditSend->toPlainText();
    QString urlStr = ui->leditUrl->text();
    QUrl url(urlStr);
    QByteArray encodingContent;
    if (ui->chkBoxGBK->isChecked()) {
        // 转GBK发送
        encodingContent = DataDealUtils::encodeString(content, 1);
    } else {
        // 转UTF8发送
        encodingContent = DataDealUtils::encodeString(content, 2);
    }

    ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | 发送内容:" + encodingContent + "\n");
    QMap<QByteArray, QByteArray> reqHeader;
    reqHeader["User-Agent"] = "QtClient/1.0";

    Http::instance().setRequestHeaders(reqHeader);
    auto reply = Http::instance().post(url, encodingContent, "application/json");
    QString res = blockUtilResponse(reply, Http::instance().getReadTimeout());
    ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | " + res + "\n");
}
#endif

#if 1
void Widget::postToPrinter(bool checked)
{
    Q_UNUSED(checked);

    if (ui->peditSend->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "发送内容为空", "请检查发送内容!", QMessageBox::Cancel);
        return;
    }

    QString content = ui->peditSend->toPlainText();
    QByteArray body;
    if (ui->chkBoxGBK->isChecked()) {
        body = DataDealUtils::encodeString(content, 1); // GBK
    } else {
        body = content.toUtf8(); // UTF-8
    }

    ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | 发送内容: " + QString::fromUtf8(body));

    QString host = "192.168.41.101";
    quint16 port = 9588;
    QString path = "/paper";

    // 构建 HTTP POST 请求
    QByteArray request;
    request.append("POST " + path.toUtf8() + " HTTP/1.1\r\n");
    request.append("Host: " + host.toUtf8() + ":" + QByteArray::number(port) + "\r\n");
    request.append("Content-Type: application/json\r\n");
    request.append("Content-Length: " + QByteArray::number(body.size()) + "\r\n");
    request.append("Connection: close\r\n");
    request.append("\r\n");
    request.append(body);

    QTcpSocket *socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, [socket, request]() {
        // 禁用 Nagle 算法，尽量一次性发送
        socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        socket->write(request);
        socket->flush();
    });

    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        QByteArray response = socket->readAll();
        ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | 响应: " + QString::fromUtf8(response));
    });

    connect(socket, &QTcpSocket::errorOccurred, this, [this, socket](QAbstractSocket::SocketError) {
        ui->peditLog->appendPlainText(DataDealUtils::curDateTimeStr() + " | 网络错误: " + socket->errorString());
        socket->deleteLater();
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
    socket->connectToHost(host, port);
}
#endif

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
