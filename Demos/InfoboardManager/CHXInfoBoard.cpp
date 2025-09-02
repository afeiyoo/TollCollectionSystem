#include "CHXInfoBoard.h"

#include "Logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

CHXInfoBoard::CHXInfoBoard(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::stateChanged, this, &CHXInfoBoard::onStateChanged);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(1000 * 60); // 每分钟重连一次
    connect(m_reconnectTimer, &QTimer::timeout, this, &CHXInfoBoard::tryReconnect);
}

void CHXInfoBoard::uploadFiles()
{
    QString lstFilesPath = QCoreApplication::applicationDirPath() + "/playfiles";
    QDir dir(lstFilesPath);

    // 获取该路径下所有以.lst为后缀的文件路径
    QStringList lstFiles = dir.entryList(QStringList() << "*.lst", QDir::Files);

    for (const auto &fileName : lstFiles) {
        QString filePath = dir.absoluteFilePath(fileName);
        QByteArray fileData = loadFiles(filePath);
        LOG_INFO("上传播放列表文件 %s", fileName.toStdString().c_str());

        // 上传文件名，并等待确认
        if (uploadFileName(fileName)) {
            LOG_INFO("文件名上传成功，开始上传文件内容");
            if (uploadFileData(fileData)) {
                // 等待设备的最终确认（0xF9 指令）
                if (m_socket->waitForReadyRead(1000)) {
                    QByteArray reply = m_socket->readAll();
                    LOG_INFO("上传完成响应: %s", byteArrayToHexStr(reply).toStdString().c_str());

                    if (static_cast<uchar>(reply[3]) == 0xF9 && static_cast<uchar>(reply[4]) == 1) {
                        LOG_INFO("文件 %s 下发成功", fileName.toStdString().c_str());
                    } else {
                        LOG_WARNING("文件 %s 下发失败，响应内容异常",
                                    fileName.toStdString().c_str());
                    }
                } else {
                    LOG_WARNING("未收到文件 %s 下发确认 (0xF9)", fileName.toStdString().c_str());
                }
            } else {
                LOG_WARNING("文件内容上传失败，跳过该文件: %s", fileName.toStdString().c_str());
            }
        } else {
            LOG_WARNING("文件名上传失败，跳过该文件: %s", fileName.toStdString().c_str());
        }
    }
}

void CHXInfoBoard::connectServer(const QString &ip, quint16 port)
{
    m_peerAddr = ip;
    m_peerPort = port;
    m_reconnectCount = 0;   // 重连尝试次数置0
    m_socket->connectToHost(ip, port);
}

bool CHXInfoBoard::playFile(uchar num)
{
    if (m_connected == false) {
        LOG_ERROR("设备未连接，无法请求播放列表文件");
        return false;
    }

    QByteArray content;
    content.append('\xFF'); // 设备地址
    content.append('\xFF');
    content.append('\x1B'); // 指令码
    content.append(static_cast<char>(num));

    QByteArray sendData = constructSendData(content);

    LOG_INFO("请求播放列表文件(编号: %d): %s",
             num,
             byteArrayToHexStr(sendData).toStdString().c_str());
    m_socket->write(sendData);
    if (!m_socket->waitForBytesWritten(1000)) {
        LOG_WARNING("请求播放列表文件(编号: %d)超时", num);
        return false;
    }
    if (!m_socket->waitForReadyRead(1000)) {
        LOG_WARNING("未收到播放列表文件(编号: %d)响应", num);
        return false;
    }

    QByteArray reply = m_socket->readAll();
    LOG_INFO("播放列表文件(编号: %d)响应: %s", num, byteArrayToHexStr(reply).toStdString().c_str());
    if (static_cast<uchar>(reply[3]) != 0x1C) {
        LOG_WARNING("播放列表文件(编号: %d)响应无效", num);
        return false;
    }

    uchar result = static_cast<uchar>(reply[4]);
    return result == 1;
}

QByteArray CHXInfoBoard::loadFiles(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    QString content = in.readAll();
    file.close();
    return content.toUtf8();
}

bool CHXInfoBoard::uploadFileName(const QString &fileName)
{
    QByteArray content;
    content.append('\xFF'); // 设备地址
    content.append('\xFF');
    content.append('\x11'); // 指令码
    content.append('\xFF'); // 块大小
    content.append('\xFF');
    content.append(fileName.toUtf8());

    QByteArray sendData = constructSendData(content);

    LOG_INFO("上传文件名: %s", byteArrayToHexStr(sendData).toStdString().c_str());
    m_socket->write(sendData);
    if (!m_socket->waitForBytesWritten(1000)) {
        LOG_WARNING("文件名上传超时");
        return false;
    }
    if (!m_socket->waitForReadyRead(1000)) {
        LOG_WARNING("未收到文件名上传响应");
        return false;
    }

    QByteArray reply = m_socket->readAll();
    LOG_INFO("文件名上传响应: %s", byteArrayToHexStr(reply).toStdString().c_str());
    if (static_cast<uchar>(reply[3]) != 0x12) {
        LOG_WARNING("文件名上传响应无效");
        return false;
    }

    uchar result = static_cast<uchar>(reply[4]);
    return result == 1;
}

bool CHXInfoBoard::uploadFileData(const QByteArray &data)
{
    int sent = 0;
    int blockNumber = 1; // 块号从1开始
    int totalSize = data.size();

    if (totalSize <= 0) {
        LOG_WARNING("文件内容为空");
        return false;
    }

    while (sent < totalSize) {
        int currentBlockSize = qMin(65535, totalSize - sent);
        QByteArray blockData = data.mid(sent, currentBlockSize);

        QByteArray content;
        content.append('\xFF'); // 设备地址
        content.append('\xFF');
        content.append('\x13'); // 指令码
        // 块号
        content.append(static_cast<char>(blockNumber & 0xFF));
        content.append(static_cast<char>((blockNumber >> 8) & 0xFF));
        content.append(blockData);

        QByteArray sendData = constructSendData(content);

        LOG_INFO("上传文件块 #%d: %s",
                 blockNumber,
                 byteArrayToHexStr(sendData).toStdString().c_str());
        m_socket->write(sendData);
        if (!m_socket->waitForBytesWritten(1000)) {
            LOG_ERROR("块 #%d 写入超时", blockNumber);
            return false;
        }

        if (!m_socket->waitForReadyRead(1000)) {
            LOG_ERROR("块 #%d 等待响应超时", blockNumber);
            return false;
        }

        QByteArray reply = m_socket->readAll();
        LOG_INFO("块 #%d 响应: %s", blockNumber, byteArrayToHexStr(reply).toStdString().c_str());

        if (static_cast<uchar>(reply[3]) != 0x14) {
            LOG_ERROR("块 #%d 响应无效", blockNumber);
            return false;
        }

        ushort replyBlock = static_cast<uchar>(reply[4]) | (static_cast<uchar>(reply[5]) << 8);
        uchar result = static_cast<uchar>(reply[6]);

        if (replyBlock != blockNumber) {
            LOG_ERROR("响应块号不匹配: 期望 #%d, 收到 #%d", blockNumber, replyBlock);
            return false;
        }

        if (result != 1) {
            LOG_ERROR("块 #%d 上传失败，终止该文件传输", blockNumber);
            return false;
        }

        // 更新偏移和块号
        sent += currentBlockSize;
        blockNumber += 1;
    }

    return true;
}

quint16 CHXInfoBoard::getX2516(quint8 *data, int len)
{
    const quint16 table16[256]
        = {0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF, 0x8C48, 0x9DC1, 0xAF5A,
           0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7, 0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C,
           0x75B7, 0x643E, 0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876, 0x2102,
           0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD, 0xAD4A, 0xBCC3, 0x8E58, 0x9FD1,
           0xEB6E, 0xFAE7, 0xC87C, 0xD9F5, 0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5,
           0x453C, 0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974, 0x4204, 0x538D,
           0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB, 0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868,
           0x99E1, 0xAB7A, 0xBAF3, 0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A,
           0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72, 0x6306, 0x728F, 0x4014,
           0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9, 0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3,
           0x8A78, 0x9BF1, 0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738, 0xFFCF,
           0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70, 0x8408, 0x9581, 0xA71A, 0xB693,
           0xC22C, 0xD3A5, 0xE13E, 0xF0B7, 0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76,
           0x7CFF, 0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036, 0x18C1, 0x0948,
           0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E, 0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E,
           0xF2A7, 0xC03C, 0xD1B5, 0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
           0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134, 0x39C3, 0x284A, 0x1AD1,
           0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C, 0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1,
           0xA33A, 0xB2B3, 0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB, 0xD68D,
           0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232, 0x5AC5, 0x4B4C, 0x79D7, 0x685E,
           0x1CE1, 0x0D68, 0x3FF3, 0x2E7A, 0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238,
           0x93B1, 0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9, 0xF78F, 0xE606,
           0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330, 0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3,
           0x2C6A, 0x1EF1, 0x0F78};

    quint16 crc = 0xFFFF;
    while (len-- > 0) {
        quint8 idx = static_cast<quint8>(crc ^ *data++) & 0xFF;
        crc = (crc >> 8) ^ table16[idx];
    }
    return crc;
}

QByteArray CHXInfoBoard::getCRCCode(const QByteArray &data)
{
    quint16 result = 0x0000;
    result = getX2516((quint8 *) data.data(), data.length());

    return ushortToByteRec(result);
}

QByteArray CHXInfoBoard::constructSendData(const QByteArray &data)
{
    QByteArray escapedContent;
    for (const auto &byte : data) {
        switch (static_cast<uchar>(byte)) {
        case 0xAA:
            escapedContent.append('\xEE');
            escapedContent.append('\x0A');
            break;
        case 0xCC:
            escapedContent.append('\xEE');
            escapedContent.append('\x0C');
            break;
        case 0xEE:
            escapedContent.append('\xEE');
            escapedContent.append('\x0E');
            break;
        default:
            escapedContent.append(byte);
            break;
        }
    }

    QByteArray sendData;
    sendData.append('\xAA');
    sendData.append(escapedContent);
    sendData.append('\xCC');
    QByteArray crc = getCRCCode(sendData);
    sendData.append(crc);

    return sendData;
}

void CHXInfoBoard::onStateChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::ConnectedState:
        m_connected = true;
        LOG_INFO("情报板连接成功");
        m_reconnectCount = 0;
        m_reconnectTimer->stop();
        uploadFiles();
        break;

    case QAbstractSocket::UnconnectedState:
        m_connected = false;
        LOG_WARNING("情报板断开连接，启动重连机制");
        m_reconnectTimer->start(); // 启动重连
        break;

    default:
        break;
    }
}

QByteArray CHXInfoBoard::ushortToByte(ushort i)
{
    QByteArray result;
    result.resize(2);
    result[1] = (uchar) (0x000000ff & i);
    result[0] = (uchar) ((0x0000ff00 & i) >> 8);
    return result;
}

QByteArray CHXInfoBoard::ushortToByteRec(ushort i)
{
    QByteArray result;
    result.resize(2);
    result[0] = (uchar) (0x000000ff & i);
    result[1] = (uchar) ((0x0000ff00 & i) >> 8);
    return result;
}

QString CHXInfoBoard::byteArrayToHexStr(const QByteArray &data)
{
    QString temp = "";
    QString hex = data.toHex();

    for (int i = 0; i < hex.length(); i = i + 2) {
        temp += hex.mid(i, 2) + " ";
    }

    return temp.trimmed().toUpper();
}


void CHXInfoBoard::tryReconnect()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_reconnectTimer->stop();
        return;
    }

    if (++m_reconnectCount > 3) {
        LOG_ERROR("无法连接情报板: 尝试重连 %d 次失败", 3);
        m_reconnectTimer->stop();
        return;
    }

    LOG_INFO("尝试重连情报板，第 %d 次", m_reconnectCount);
    m_socket->abort(); // 断开可能的连接
    m_socket->connectToHost(m_peerAddr, m_peerPort);
}

