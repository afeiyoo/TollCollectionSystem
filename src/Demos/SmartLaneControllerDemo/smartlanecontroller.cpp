#include "smartlanecontroller.h"

#include "Logger.h"
#include "utils/datadealutils.h"

#include <QCoreApplication>
#include <QDir>
#include <QNetworkProxy>

static const QByteArray STX = QByteArray::fromHex("FFFF"); // 帧头
static const QByteArray VER = QByteArray::fromHex("00");   // 版本号
static const int STX_LEN = 2;                              // 帧头 2 字节
static const int VER_LEN = 1;                              // 版本号 1 字节，固定为 0x00
static const int SEQ_LEN = 1;                              // 序列号 1 字节（服务端格式：0x0X, X=1~9）
static const int LEN_FIELD_LEN = 4;                        // 数据长度字段 4 字节（大端）
static const int CRC_LEN = 2;                              // CRC 校验码 2 字节

// 允许的最大数据长度（4MB），超过则判为非法
static const int MAX_BUFF_LEN = 4 * 1024 * 1024;

SmartLaneController::SmartLaneController(QObject *parent)
    : QObject{parent}
{
    m_isOk = false;
    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->setProxy(QNetworkProxy::NoProxy);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &SmartLaneController::doConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &SmartLaneController::doDisconnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &SmartLaneController::doReadyRead);
//    connect(m_tcpSocket,
//            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
//            this,
//            [=](QAbstractSocket::SocketError socketError) {
//                LOG_INFO() << "[ERROR] Socket error:" << socketError << m_tcpSocket->errorString();
//            });
}

void SmartLaneController::connectServer(const QString &ip, quint16 port)
{
    LOG_INFO("正在连接服务端, %s:%d", ip.toStdString().c_str(), port);
    // 连接服务端
    m_peerAddr = ip;
    m_peerPort = port;
    m_tcpSocket->connectToHost(ip, port);
}

void SmartLaneController::disconnectServer()
{
    m_tcpSocket->disconnectFromHost();
}

void SmartLaneController::sendCommand(const QString &type, const QByteArray &data)
{
    // 组装发送帧
    QByteArray buffer;
    buffer.append(STX);
    buffer.append(VER);
    buffer.append(getClientSeq());
    buffer.append(Utils::DataDealUtils::intToByte(data.size()));
    buffer.append(data);
    QByteArray crc = Utils::DataDealUtils::getCRCCode(buffer.mid(STX_LEN));
    buffer.append(crc);

    if (!m_isOk) {
        LOG_INFO("==>发送%s帧失败: 连接未建立", type.toStdString().c_str());
    }

    LOG_INFO("==>发送%s帧: %s",
             type.toStdString().c_str(),
             Utils::DataDealUtils::byteArrayToHexStr(buffer).toStdString().c_str());
    m_tcpSocket->write(buffer);
    m_tcpSocket->waitForBytesWritten(1000);

    // 如果是 A1/A2/A3，就加到重发队列
    if (type == "A1" || type == "A2" || type == "A3") {
        if (!m_pendingCommands.contains(type)) {
            PendingCommand *cmd = new PendingCommand{type, data, 0, new QTimer(this)};
            cmd->timer->setSingleShot(true);

            connect(cmd->timer, &QTimer::timeout, this, [=]() {
                if (!m_isOk) {
                    LOG_INFO("%s 网络断开，停止重传", type.toStdString().c_str());
                    return;
                }
                if (cmd->retryCount < 3) {
                    cmd->retryCount++;
                    LOG_INFO("%s 超时，进行第%d次重传", type.toStdString().c_str(), cmd->retryCount);
                    sendCommand(type, data); // 调用原始 sendCommand 发送帧
                    cmd->timer->start(1000); // 再次等待超时
                } else {
                    LOG_INFO("%s 重传3次失败，放弃", type.toStdString().c_str());
                    m_pendingCommands.remove(type);
                    cmd->timer->stop();
                    delete cmd;
                }
            });

            m_pendingCommands[type] = cmd;
            cmd->timer->start(1000); // 第一次启动超时检测
        }
    }
}

SmartLaneController::~SmartLaneController()
{
    m_tcpSocket->disconnectFromHost();
    m_tcpSocket->deleteLater();
}

void SmartLaneController::doConnected()
{
    LOG_INFO("与服务端建立连接 %s:%d", m_peerAddr.toStdString().c_str(), m_peerPort);
    m_isOk = true;
    emit connectStatus(m_isOk);
}

void SmartLaneController::doDisconnected()
{
    LOG_INFO("与服务端断开连接");
    m_isOk = false;
    emit connectStatus(m_isOk);
}

void SmartLaneController::doReadyRead()
{
    QByteArray chunk = m_tcpSocket->readAll();
    LOG_INFO("<==接收数据: %s", Utils::DataDealUtils::byteArrayToHexStr(chunk).toStdString().c_str());
    if (chunk.size() <= 0) {
        return;
    }
    if (chunk.size() >= MAX_BUFF_LEN) {
        LOG_INFO("接收的数据大小(%dB)超过数据缓冲区大小限制(%dB), 忽略处理", chunk.size(), MAX_BUFF_LEN);
        return;
    }
    if (m_buffer.size() + chunk.size() >= MAX_BUFF_LEN) {
        LOG_INFO("数据缓冲区现有大小(%dB) + 接收的数据大小(%dB) 超过数据缓冲区大小限制(%dB), 清空缓冲区",
                 m_buffer.size(),
                 chunk.size(),
                 MAX_BUFF_LEN);
        m_buffer.clear();
    }

    m_buffer.append(chunk);
    // 数据长度不足，不做处理
    if (m_buffer.size() <= (STX_LEN + VER_LEN + SEQ_LEN + LEN_FIELD_LEN))
        return;
    dealSocketData();
}

void SmartLaneController::dealSocketData()
{
    while (true) {
        // 在 m_buffer 中查找第一对连续的 0xFF,0xFF
        int idx = m_buffer.indexOf(STX);
        if (idx >= 0) {
            // 则0~idx-1前的数据为垃圾数据
            m_buffer.remove(0, idx);
        } else {
            // 说明没有找到帧头，该数据无效，直接丢弃
            m_buffer.clear();
            return;
        }

        // 验证版本号是否为 0x00
        uchar ver = static_cast<uchar>(m_buffer.at(2));
        if (ver != 0x00) {
            LOG_INFO("版本号错误");
            m_buffer.remove(0, STX_LEN);
            continue;
        }

        // 验证序列号是否正确
        uchar seq = static_cast<uchar>(m_buffer.at(3));
        bool ok = ((seq & 0xF0) == 0x00) && ((seq & 0x0F) >= 1 && (seq & 0x0F) <= 9);
        if (!ok) {
            LOG_INFO("序列号错误");
            m_buffer.remove(0, 2);
            continue;
        }

        // 解析数据长度
        quint32 dataLen = Utils::DataDealUtils::byteToUInt(m_buffer.mid(STX_LEN + VER_LEN + SEQ_LEN, LEN_FIELD_LEN));

        // 计算数据总大小（单位：B）
        quint32 totalSize = STX_LEN + VER_LEN + SEQ_LEN + LEN_FIELD_LEN + dataLen + CRC_LEN;
        // 数据长度不足，等待更多后续数据到来
        if ((quint32) m_buffer.size() < totalSize)
            return;

        QByteArray fullFrame = m_buffer.left(totalSize);
        // 进行CRC校验
        QByteArray remoteCrc = fullFrame.right(CRC_LEN);
        QByteArray localCrc = Utils::DataDealUtils::getCRCCode(fullFrame.mid(STX_LEN, totalSize - STX_LEN - CRC_LEN));

        if (remoteCrc != localCrc) {
            QString localCrcStr = QString::fromLatin1(localCrc.toHex()).toUpper();
            QString remoteCrcStr = QString::fromLatin1(remoteCrc.toHex()).toUpper();
            LOG_INFO("CRC校验失败(localCrc: %s, remoteCrc: %s)",
                     localCrcStr.toStdString().c_str(),
                     remoteCrcStr.toStdString().c_str());
            m_buffer.remove(0, STX_LEN);
            continue;
        }

        QByteArray command = fullFrame.mid(STX_LEN + VER_LEN + SEQ_LEN + LEN_FIELD_LEN, dataLen);
        // 处理单条指令
        dealCommand(command, seq);
        m_buffer.remove(0, totalSize);
        continue;
    }
}

void SmartLaneController::dealCommand(const QByteArray &command, uchar seq)
{
    uchar cmdType = static_cast<uchar>(command.at(0));
    LOG_INFO("开始处理指令(%02X): %s", cmdType, Utils::DataDealUtils::byteArrayToHexStr(command).toStdString().c_str());

    int status = 0;
    switch (cmdType) {
    case 0xD2:
        // IO状态信息
        emit sigSmartLaneController(cmdType, command);
        sendResponse(cmdType, 0x01);
        break;
    case 0xD3:
        // 透传接收信息
        emit sigSmartLaneController(cmdType, command);
        sendResponse(cmdType, 0x01);
        break;
    case 0xD6:
        // 心跳及设备状态
        emit sigSmartLaneController(cmdType, command);
        sendResponse(cmdType, 0x01);
        break;
    case 0xA1: {
        status = static_cast<uchar>(command.at(1));
        if (m_pendingCommands.contains("A1")) {
            auto *cmd = m_pendingCommands["A1"];
            cmd->timer->stop();
            delete cmd;
            m_pendingCommands.remove("A1");
        }
        if (status == 0x01) {
            LOG_INFO("车控器返回A1指令处理失败");
        } else {
            LOG_INFO("车控器返回A1指令处理成功");
        }
    } break;
    case 0xA2: {
        status = static_cast<uchar>(command.at(1));
        if (m_pendingCommands.contains("A2")) {
            auto *cmd = m_pendingCommands["A2"];
            cmd->timer->stop();
            delete cmd;
            m_pendingCommands.remove("A2");
        }
        if (status == 0x01) {
            LOG_INFO("车控器返回A2指令处理失败");
        } else {
            LOG_INFO("车控器返回A2指令处理成功");
        }
    } break;
    case 0xA3: {
        status = static_cast<uchar>(command.at(1));
        if (m_pendingCommands.contains("A3")) {
            auto *cmd = m_pendingCommands["A3"];
            cmd->timer->stop();
            delete cmd;
            m_pendingCommands.remove("A3");
        }
        if (status == 0x01) {
            LOG_INFO("车控器返回A3指令处理失败");
        } else {
            LOG_INFO("车控器返回A3指令处理成功");
        }
    } break;
    default:
        LOG_INFO("未知的命令类型: %s", QString("0x%1").arg(cmdType, 2, 16, QChar('0')).toUpper().toStdString().c_str());
        break;
    }
}

void SmartLaneController::sendResponse(uchar cmdType, uchar status)
{
    QByteArray command;
    command.append(cmdType);
    command.append(status);

    // 组装发送帧
    QByteArray buffer;
    buffer.append(STX);
    buffer.append(VER);
    buffer.append(getClientSeq());
    buffer.append(Utils::DataDealUtils::intToByte(command.size()));
    buffer.append(command);
    QByteArray crc = Utils::DataDealUtils::getCRCCode(buffer.mid(STX_LEN));
    buffer.append(crc);

    if (!m_isOk) {
        LOG_INFO("返回应答失败: 连接未建立");
    }

    LOG_INFO("==>返回应答: %s", Utils::DataDealUtils::byteArrayToHexStr(buffer).toStdString().c_str());
    m_tcpSocket->write(buffer);
    m_tcpSocket->waitForBytesWritten(1000);
}

uchar SmartLaneController::getClientSeq()
{
    static uchar x = 1; // X 范围 1~9

    uchar ret = (x << 4); // X0H，其实就是 x * 0x10

    x++;
    if (x > 9) {
        x = 1; // 回到 1
    }

    return ret;
}
