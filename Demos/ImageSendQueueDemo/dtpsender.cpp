#include "dtpsender.h"

#include <QDebug>
#include <QTextCodec>

fn_dtp_connect dtp_connect = NULL;
fn_dtp_disconn dtp_disconn = NULL;
fn_dtp_login dtp_login = NULL;
fn_dtp_logout dtp_logout = NULL;
fn_dtp_putmsg dtp_putmsg = NULL;
fn_dtp_getmsg dtp_getmsg = NULL;
fn_dtp_storemsg dtp_storemsg = NULL;

QLibrary *dtp_lib = NULL;

DtpSender::DtpSender(QObject *parent)
    : QObject{parent}
{}

DtpSender::~DtpSender() {}

bool DtpSender::initDtp(QString s)
{
    if (dtp_lib == NULL) {
        dtp_lib = new QLibrary(s);
        if (dtp_lib->load()) {
            dtp_connect = (fn_dtp_connect) dtp_lib->resolve("dtp_connect");
            std::string errorStr = dtp_lib->errorString().toStdString();
            if (!dtp_connect) {
                qDebug("dtp_connect resolve failed: %s", errorStr.c_str());
            }
            dtp_disconn = (fn_dtp_disconn) dtp_lib->resolve("dtp_disconn");
            if (!dtp_disconn) {
                qDebug("dtp_disconn resolve failed: %s", errorStr.c_str());
            }
            dtp_login = (fn_dtp_login) dtp_lib->resolve("dtp_login");
            if (!dtp_login) {
                qDebug("dtp_login resolve failed: %s", errorStr.c_str());
            }
            dtp_logout = (fn_dtp_logout) dtp_lib->resolve("dtp_logout");
            if (!dtp_logout) {
                qDebug("dtp_logout resolve failed: %s", errorStr.c_str());
            }
            dtp_putmsg = (fn_dtp_putmsg) dtp_lib->resolve("dtp_putmsg");
            if (!dtp_putmsg) {
                qDebug("dtp_putmsg resolve failed: %s", errorStr.c_str());
            }
            dtp_getmsg = (fn_dtp_getmsg) dtp_lib->resolve("dtp_getmsg");
            if (!dtp_getmsg) {
                qDebug("dtp_getmsg resolve failed: %s", errorStr.c_str());
            }
            dtp_storemsg = (fn_dtp_storemsg) dtp_lib->resolve("dtp_storemsg");
            if (!dtp_storemsg) {
                qDebug("dtp_storemsg resolve failed: %s", errorStr.c_str());
            }
        } else {
            std::string errorStr = dtp_lib->errorString().toStdString();
            qDebug("Load dtp dll failed: %s", errorStr.c_str());
            delete dtp_lib;
            dtp_lib = NULL;
        }
    }
    return dtp_lib && dtp_connect && dtp_disconn && dtp_login && dtp_logout && dtp_putmsg && dtp_getmsg && dtp_storemsg;
}

int DtpSender::connectDtpServer(QString serverIP, int port)
{
    int nRes = 0;
    if (m_dtpHandle > 0)
        return 0;
    if (serverIP.isEmpty()) {
        return -9;
    }
    int nRole = 0;
    // 连接数据库
    nRes = dtp_connect(&m_dtpHandle, serverIP.toStdString().c_str(), port);
    qDebug("连接DtpServer,返回Handle:%d,Result:%d", m_dtpHandle, nRes);
    if (nRes != 0) {
        m_LoginDtp = false;
        qDebug("连接[%s:%d]DTP服务失败,尝试重连!ErrCode:%d", serverIP.toStdString().c_str(), port, nRes);
        m_dtpHandle = 0;
        return -1;
    }
    // 如果没有登陆过，尝试登陆
    if (!m_LoginDtp)
        nRes = dtp_login(m_dtpHandle, "admin", "admin", &nRole);
    qDebug().noquote() << QString("开始登录DtpServer,返回handle:%1,Res:%2").arg(m_dtpHandle).arg(nRes);
    // 登陆失败处理
    if (nRes != 0) {
        if (m_dtpHandle) {
            qDebug("断开DtpServer连接Hanlde:%d", m_dtpHandle);
            dtp_disconn(m_dtpHandle);
        }
        qDebug("%s登陆DTP服务失败,再次尝试!", "admin");
        m_dtpHandle = 0;
        m_LoginDtp = false;
        return -2;
    }
    m_LoginDtp = true;
    return 0;
}

int DtpSender::sendMsgToDtp(QString serverIP, int port, QString queue, QString laneId, const QString &msg)
{
    int nRes = connectDtpServer(serverIP, port);
    if (nRes)
        return nRes;
    // 发送数据
    queue += laneId;
    QByteArray arrQueue = queue.toLatin1();
    char *queueData = arrQueue.data();

    // 将 UTF-8 编码的 msg 转换为 GBK 编码
    QTextCodec *gbkCodec = QTextCodec::codecForName("GBK");
    QByteArray gbkMsg = gbkCodec->fromUnicode(msg);
    nRes = dtp_putmsg(m_dtpHandle,
                      queueData,
                      const_cast<char *>(gbkMsg.toStdString().c_str()),
                      gbkMsg.toStdString().length(),
                      3);
    // 发送失败
    if (nRes) {
        qDebug().noquote() << QString("向[%1: %2]DTP发送数据失败重连.错误码:%3").arg(serverIP, queue).arg(nRes);
        if (m_dtpHandle) {
            qDebug().noquote() << QString("断开DtpServer连接Hanlde:%1").arg(m_dtpHandle);
            dtp_disconn(m_dtpHandle);
        }
        m_dtpHandle = 0;
    }
    // 发送成功
    else {
        qDebug().noquote() << QString("向[%1: %2]发送 Done.").arg(serverIP, queue);
    }
    return nRes;
}

int DtpSender::storeMsgToDtp(QString serverIP, int port, QString queue, QString msgId, QByteArray msgContent, int msgLen)
{
    int nRes = connectDtpServer(serverIP, port);
    if (nRes)
        return nRes;
    // 发送数据
    QByteArray arrQueue = queue.toLatin1();
    char *queueName = arrQueue.data();

    qDebug().noquote() << "MsgId: " << msgId << " MsgLen: " << msgLen;
    nRes = dtp_storemsg(m_dtpHandle, queueName, msgId.toStdString().c_str(), msgContent.data(), msgLen);

    // 发送失败
    if (nRes) {
        qDebug().noquote() << QString("向[%1: %2]DTP发送数据失败重连.错误码:%3").arg(serverIP, queue).arg(nRes);
        if (m_dtpHandle) {
            qDebug().noquote() << QString("断开DtpServer连接Hanlde:%1").arg(m_dtpHandle);
            dtp_disconn(m_dtpHandle);
        }
        m_dtpHandle = 0;
    }
    // 发送成功
    else {
        qDebug().noquote() << QString("向[%1: %2]发送 Done.").arg(serverIP, queue);
    }
    return nRes;
}
