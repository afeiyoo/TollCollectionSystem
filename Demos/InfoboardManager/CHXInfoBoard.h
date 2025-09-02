#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class CHXInfoBoard : public QObject
{
    Q_OBJECT
public:
    explicit CHXInfoBoard(QObject *parent = nullptr);

    // 上传播放列表文件
    void uploadFiles();

    // 连接设备
    void connectServer(const QString &ip, quint16 port);

    // 播放指定文件
    bool playFile(uchar num);

private:
    QByteArray loadFiles(const QString &path);
    bool uploadFileName(const QString &fileName);
    bool uploadFileData(const QByteArray &data);
    quint16 getX2516(quint8 *data, int len);
    QString byteArrayToHexStr(const QByteArray &data);
    QByteArray ushortToByte(ushort i);
    QByteArray ushortToByteRec(ushort i);
    QByteArray getCRCCode(const QByteArray &data);
    QByteArray constructSendData(const QByteArray &data);

    QByteArray m_content; // 播放列表文件内容
    QByteArray m_file;    // 播放列表文件名
    QTcpSocket *m_socket{nullptr};
    QString m_peerAddr;
    quint16 m_peerPort;
    bool m_connected{false};

    // 网络断连后，自动重连3次
    int m_reconnectCount = 0;
    QTimer* m_reconnectTimer = nullptr;

signals:
    void sigConnectStatus(bool status);

public slots:
    void onStateChanged(QAbstractSocket::SocketState state);
    void tryReconnect();
};
