#include "dtpsender.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QTextStream>

// 自定义 Qt 消息处理函数
void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QTextStream out(stdout);
    QTextStream err(stderr);

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    switch (type) {
    case QtDebugMsg:
        out << "[DEBUG " << time << "] " << msg << "\n";
        out.flush();
        break;
    case QtInfoMsg:
        out << "[INFO  " << time << "] " << msg << "\n";
        out.flush();
        break;
    case QtWarningMsg:
        err << "[WARN  " << time << "] " << msg << "\n";
        err.flush();
        break;
    case QtCriticalMsg:
        err << "[ERROR " << time << "] " << msg << "\n";
        err.flush();
        break;
    case QtFatalMsg:
        err << "[FATAL " << time << "] " << msg << "\n";
        err.flush();
        abort();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 安装消息处理器，所有 qDebug()/qWarning() 等都会走这里
    qInstallMessageHandler(messageHandler);

    // Dtp初始化
    DtpSender dtpSender;
    dtpSender.initDtp("./libDtp-Client.so");

    QStringList args = app.arguments();
    if (args.size() < 4) {
        qDebug().noquote() << "用法: " << args.at(0) << " <文件夹路径> <IP地址> <端口号>";
        return -1;
    }

    QString folderPath = args.at(1);
    QString host = args.at(2);
    bool ok = false;
    int port = args.at(3).toInt(&ok);
    if (!ok) {
        qDebug().noquote() << "无效端口号: " << args.at(3);
        return -1;
    }

    QDir dir(folderPath);
    if (!dir.exists()) {
        qDebug().noquote() << "文件夹不存在: " << folderPath;
        return -1;
    }

    QDirIterator it(folderPath, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    int count = 0;
    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fi(filePath);
        QString baseName = fi.fileName();

        // 筛选目标
        if (!(baseName.endsWith("_W1.jpg", Qt::CaseInsensitive) || baseName.endsWith("_W2.jpg", Qt::CaseInsensitive)
              || baseName.endsWith("_W3.jpg", Qt::CaseInsensitive) || baseName.endsWith("_W4.jpg", Qt::CaseInsensitive)
              || baseName.endsWith("_image.jpg", Qt::CaseInsensitive)
              || baseName.endsWith("_binimage.jpg", Qt::CaseInsensitive))) {
            continue;
        }

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug().noquote() << "读取图片数据失败:" << baseName << "原因:" << file.errorString();
            continue;
        }
        QByteArray data = file.readAll();
        file.close();

        if (baseName.endsWith("_W1.jpg", Qt::CaseInsensitive) || baseName.endsWith("_W2.jpg", Qt::CaseInsensitive)
            || baseName.endsWith("_W3.jpg", Qt::CaseInsensitive) || baseName.endsWith("_W4.jpg", Qt::CaseInsensitive)) {
            QString msgId = baseName;
            msgId.chop(4);
            dtpSender.storeMsgToDtp(host, port, "ImageQ", msgId, data, data.length());
            count++;
        } else if (baseName.endsWith("_image.jpg", Qt::CaseInsensitive)) {
            int idx = baseName.indexOf('_');
            if (idx > 0) {
                QString msgId = baseName.left(idx);
                dtpSender.storeMsgToDtp(host, port, "ImageQ", msgId, data, data.length());
                count++;
            }
        } else if (baseName.endsWith("_binimage.jpg", Qt::CaseInsensitive)) {
            int idx = baseName.indexOf('_');
            if (idx > 0) {
                QString prefix = baseName.left(idx);
                QString msgId = prefix + "_b1";
                dtpSender.storeMsgToDtp(host, port, "ImageQ", msgId, data, data.length());
                count++;
            }
        }
    }

    qDebug().noquote() << "传输完成，共传输图片 " << count << " 张";

    return 0;
}
