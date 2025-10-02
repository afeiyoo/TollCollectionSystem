#include "globalmanager.h"

#include "ConsoleAppender.h"
#include "RollingFileAppender.h"
#include "config/config.h"
#include "global/constant.h"
#include "global/signalmanager.h"
#include "laneservice.h"
#include "tools/laneauth.h"
#include "utils/fileutils.h"
#include "utils/stdafx.h"

using namespace Utils;

Q_GLOBAL_STATIC(GlobalManager, ins)

GlobalManager::GlobalManager(QObject *parent)
    : QObject{parent}
{
    m_signalMan = new SignalManager(this);
    m_updater = QSimpleUpdater::getInstance();

    m_jsonParser = new QJson::Parser();
    m_jsonSerializer = new QJson::Serializer();

    m_config = new Config(this);

    m_laneAuth = new LaneAuth(this);
}

GlobalManager::~GlobalManager()
{
    SAFE_DELETE(m_jsonSerializer);
    SAFE_DELETE(m_jsonParser);
}

GlobalManager *GlobalManager::instance()
{
    return ins();
}

void GlobalManager::init()
{
    // 日志初始化
    FileName logDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/logs");
    FileUtils::makeSureDirExist(logDir);
    // 控制台日志
    ConsoleAppender *consoleAppender = new ConsoleAppender();
    consoleAppender->setFormat(Constant::Log::FORMAT_DETAIL);
    cuteLogger->registerAppender(consoleAppender);
    // 详细完整系统日志
    FileName sysLogPath = logDir + QString("/system.log");
    RollingFileAppender *sysFileAppender = new RollingFileAppender(FileUtils::canonicalPath(sysLogPath).toString());
    sysFileAppender->setLogFilesLimit(90);
    sysFileAppender->setFormat(Constant::Log::FORMAT_DETAIL);
    sysFileAppender->setFlushOnWrite(true);
    sysFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerAppender(sysFileAppender);
    // 详细业务日志
    FileName bizLogPath = logDir + QString("/biz.log");
    RollingFileAppender *bizFileAppender = new RollingFileAppender(FileUtils::canonicalPath(bizLogPath).toString());
    bizFileAppender->setLogFilesLimit(90);
    bizFileAppender->setFormat(Constant::Log::FORMAT_BRIEF);
    bizFileAppender->setFlushOnWrite(true);
    bizFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    bizFileAppender->setDetailsLevel("info");
    cuteLogger->registerCategoryAppender("biz", bizFileAppender);
    // 详细设备日志
    FileName devLogPath = logDir + QString("/dev.log");
    RollingFileAppender *devFileAppender = new RollingFileAppender(FileUtils::canonicalPath(devLogPath).toString());
    devFileAppender->setLogFilesLimit(90);
    devFileAppender->setFormat(Constant::Log::FORMAT_BRIEF);
    devFileAppender->setFlushOnWrite(true);
    devFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerCategoryAppender("dev", devFileAppender);

    LOG_INFO().noquote() << "开始进行系统初始化......";

    // Json序列化时不保留空格
    m_jsonSerializer->setIndentMode(QJson::IndentCompact);

    // 软件配置加载
    LOG_ASSERT_X(m_config->loadConfig(), "系统初始化失败: 系统基础配置加载异常");

    // 更新对象初始化
    FileName downloadDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/download");
    FileUtils::makeSureDirExist(downloadDir);

    m_updater->setDownloadDir(m_config->m_systemConfig.updateUrl, downloadDir.toString() + "/update");
    m_updater->setNotifyOnUpdate(m_config->m_systemConfig.updateUrl, true);
    m_updater->setMandatoryUpdate(m_config->m_systemConfig.updateUrl, true);
    m_updater->setModuleVersion(m_config->m_systemConfig.updateUrl, "0.1");

    // 后端服务初始化
    m_laneService = new LaneService(this);
    if (m_config->m_systemConfig.serviceMode == EM_ServiceMode::LOCAL) {
        // 单机版需要进行车道服务初始化
        LOG_INFO().noquote() << "服务初始化";
        LOG_ASSERT_X(m_laneService->init(), "系统初始化失败：服务加载异常");
    }

    // 系统环境参数初始化
    // TODO
}
