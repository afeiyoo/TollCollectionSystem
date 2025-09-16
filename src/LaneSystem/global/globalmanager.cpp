#include "globalmanager.h"
#include "ConsoleAppender.h"
#include "Jcon/json_rpc_tcp_client.h"
#include "Jcon/json_rpc_websocket_client.h"
#include "QSimpleUpdater.h"
#include "RollingFileAppender.h"
#include "config/bizenv.h"
#include "config/config.h"
#include "global/constant.h"
#include "global/modemanager.h"
#include "global/signalmanager.h"
#include "laneservice.h"
#include "utils/datadealutils.h"
#include "utils/fileutils.h"
#include "utils/stdafx.h"

using namespace Utils;

Q_GLOBAL_STATIC(GlobalManager, ins)

GlobalManager::GlobalManager(QObject *parent)
    : QObject{parent}
{
    m_signalMan = new SignalManager(this);
    m_modeMan = new ModeManager(this);
    m_updater = QSimpleUpdater::getInstance();

    m_jsonParser = new QJson::Parser();
    m_jsonSerializer = new QJson::Serializer();

    m_config = new Config(this);

    m_bizEnv = new BizEnv(this);
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
    LOG_ASSERT_X(m_config->loadConfig(), "系统初始化失败");

    // 环境参数初始化

    // 后端服务初始化
    m_laneService = new LaneService(this);
    m_laneService->init();
    if (m_config->m_systemConfig.serviceMode == 1) {
        if (m_config->m_systemConfig.serviceConnectType == 0) {
            m_jsonRpcClient = new jcon::JsonRpcTcpClient(this);
        } else {
            m_jsonRpcClient = new jcon::JsonRpcWebSocketClient(this);
        }
        m_jsonRpcClient->connectToServer(m_config->m_systemConfig.serviceIP, m_config->m_systemConfig.servicePort);
    } else {
        m_jsonRpcClient = nullptr; // 单机版直接调用动态库
    }

    LOG_INFO().noquote() << "系统正常启动，启动时间：" << Utils::DataDealUtils::curDateStr();
}
