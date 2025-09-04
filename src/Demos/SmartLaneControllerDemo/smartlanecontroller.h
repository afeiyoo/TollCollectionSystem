#ifndef SMARTLANECONTROLLER_H
#define SMARTLANECONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class SmartLaneController : public QObject
{
    Q_OBJECT
public:
    struct PendingCommand
    {
        QString type;
        QByteArray data;
        int retryCount;
        QTimer *timer;
    };

    explicit SmartLaneController(QObject *parent = nullptr);
    void connectServer(const QString &ip, quint16 port);
    void disconnectServer();

    void sendCommand(const QString &type, const QByteArray &data);

    ~SmartLaneController();

public slots:
    void doConnected();
    void doDisconnected();
    void doReadyRead();

private:
    void dealSocketData();
    void dealCommand(const QByteArray &command, uchar seq);
    void sendResponse(uchar cmdType, uchar status);
    uchar getClientSeq();

signals:
    void sigSmartLaneController(uchar type, QByteArray data);
    void connectStatus(bool status);

private:
    QTcpSocket *m_tcpSocket = nullptr;
    bool m_isOk = false;
    QString m_peerAddr;
    quint16 m_peerPort;
    QByteArray m_buffer; // 用于缓存从 socket 中读到但还未解析完的数据（处理粘包问题）

    // 重发等待队列
    QMap<QString, PendingCommand *> m_pendingCommands;
};

#endif // SMARTLANECONTROLLER_H
