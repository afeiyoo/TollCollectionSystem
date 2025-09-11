#include "globalmanager.h"
#include "ConsoleAppender.h"
#include "QSimpleUpdater.h"
#include "RollingFileAppender.h"
#include "global/constant.h"
#include "global/modemanager.h"
#include "global/signalmanager.h"
#include "utils/fileutils.h"

using namespace Utils;

Q_GLOBAL_STATIC(GlobalManager, ins)

GlobalManager::GlobalManager(QObject *parent)
    : QObject{parent}
{
    m_signalMan = new SignalManager(this);
    m_modeMan = new ModeManager(this);
    m_updater = QSimpleUpdater::getInstance();
}

GlobalManager::~GlobalManager() {}

GlobalManager *GlobalManager::instance()
{
    return ins();
}

void GlobalManager::init()
{
    // 日志初始化
    // FileName logDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/logs");
    // FileUtils::makeSureDirExist(logDir);
    // //设置控制台日志，用于定位
    // ConsoleAppender *console = new ConsoleAppender;
    // console->setDetailsLevel("trace");
    // console->setFormat(Constant::Log::FORMAT_DETAIL);
    // // console->onlyPrintSetLevel(true);
    // cuteLogger->registerCategoryAppender("sys", console);
    // //设置sys日志，打详细系统信息
    // FileName sysLog = fn + QString(Constant::Path::SYS_LOG_NAME); //系统日志，用于打详细系统信息
    // RollingFileAppender *sysAppender = new RollingFileAppender(sysLog.toString());
    // sysAppender->setLogFilesLimit(90);
    // sysAppender->setFormat(Constant::Log::FORMAT_DETAIL);
    // sysAppender->setFlushOnWrite(true);
    // sysAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    // sysAppender->setDetailsLevel("trace");
    // cuteLogger->registerCategoryAppender("sys", sysAppender);
    // m_loggerAppenders["sys"] = sysAppender;
    // //设置biz日志，打详细业务日志
    // FileName bizLog = fn + QString(Constant::Path::BIZ_LOG_NAME); //业务日志，业务类往里头放
    // RollingFileAppender *bizAppender = new RollingFileAppender(bizLog.toString());
    // bizAppender->setLogFilesLimit(90);
    // bizAppender->setFormat(Constant::Log::FORMAT_BRIEF);
    // bizAppender->setFlushOnWrite(true);
    // bizAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    // bizAppender->setDetailsLevel("info");
    // cuteLogger->registerCategoryAppender("biz", bizAppender);
    // m_loggerAppenders["biz"] = bizAppender;
    // //设置dev日志，打详细设备日志
    // FileName devLog = fn + QString(Constant::Path::DEV_LOG_NAME); //设备日志，设备类往里头放
    // bizAppender = new RollingFileAppender(devLog.toString());
    // bizAppender->setLogFilesLimit(90);
    // bizAppender->setFormat(Constant::Log::FORMAT_BRIEF);
    // bizAppender->setFlushOnWrite(true);
    // bizAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    // bizAppender->setDetailsLevel("debug");
    // cuteLogger->registerCategoryAppender("dev", bizAppender);
    // m_loggerAppenders["dev"] = bizAppender;
}
