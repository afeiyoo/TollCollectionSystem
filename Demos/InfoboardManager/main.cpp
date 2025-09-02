//==============================================================================
// 2025-07-12 昊星情报板调试Demo
//==============================================================================

#include "widget.h"

#include "Logger.h"
#include "RollingFileAppender.h"
#include "fileutils.h"

#include <QApplication>

using namespace Utils;

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    #else
    // 根据实际屏幕缩放比例更改
    qputenv("QT_SCALE_FACTOR", "1.5");
    #endif
#endif
    QApplication a(argc, argv);

    FileName fn = FileName::fromString(QCoreApplication::applicationDirPath() + "/logs");
    FileUtils::makeSureDirExist(fn);

    FileName logPath = fn + QString("/infoboard.log");
    RollingFileAppender *rollFileAppender = new RollingFileAppender(logPath.toString());
    rollFileAppender->setLogFilesLimit(90);
    rollFileAppender->setFormat("%{time} [%{type}] %{message}\n\n");
    rollFileAppender->setFlushOnWrite(true);
    rollFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerAppender(rollFileAppender);

    Widget w;

    LOG_INFO() << "程序初始化";

    FileName lstDir = FileName::fromString(QCoreApplication::applicationDirPath() + "/playfiles");
    FileUtils::makeSureDirExist(lstDir);

    FileNameList lsts = FileUtils::getFilesWithSuffix(lstDir, "lst");
    if (lsts.isEmpty())
        LOG_ERROR() << "无播放列表文件";

    w.setItems(lsts);

    w.show();
    return a.exec();
}
