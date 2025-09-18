#include "globalmanager.h"

#include "Jcon/json_rpc_tcp_server.h"
#include "Jcon/json_rpc_websocket_server.h"
#include "Logger.h"
#include "RollingFileAppender.h"
#include "config/config.h"
#include "utils/defs.h"
#include "utils/fileutils.h"
#include "utils/stdafx.h"

#ifdef LANESERVICE_NETWORK
    #include "ConsoleAppender.h"
#endif

using namespace Utils;

Q_GLOBAL_STATIC(GlobalManager, ins)

//==============================================================================
// Rpc 内部日志输出类
//==============================================================================
class JsonRpcCuteLogger : public jcon::JsonRpcLogger
{
public:
    JsonRpcCuteLogger() {}
    ~JsonRpcCuteLogger() override {}

    void logDebug(const QString &message) override { LOG_DEBUG().noquote() << message; }
    void logInfo(const QString &message) override { LOG_INFO().noquote() << message; }
    void logWarning(const QString &message) override { LOG_WARNING().noquote() << message; }
    void logError(const QString &message) override { LOG_ERROR().noquote() << message; }
};

//==============================================================================
// 全局管理类：进行日志初始化，配置加载，工具类初始化，数据库连接等
//==============================================================================
GlobalManager::GlobalManager(QObject *parent)
    : QObject{parent}
{
    m_config = new Config(this);

    m_jsonParser = new QJson::Parser();
    m_jsonSerializer = new QJson::Serializer();

    m_sqlDealer = new SqlUtils(this);
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

bool GlobalManager::init()
{
    FileName logDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/logs");
    FileUtils::makeSureDirExist(logDir);
#ifdef LANESERVICE_NETWORK
    // 日志初始化
    ConsoleAppender *consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat(m_config->m_logConfig.format);
    cuteLogger->registerAppender(consoleAppender);

    // 总日志
    FileName serviceLogPath = logDir + QString("/service.log");
    RollingFileAppender *serviceFileAppender = new RollingFileAppender(
        FileUtils::canonicalPath(serviceLogPath).toString());
    serviceFileAppender->setFormat(m_config->m_logConfig.format);
    serviceFileAppender->setLogFilesLimit(m_config->m_logConfig.filesLimit);
    serviceFileAppender->setFlushOnWrite(true);
    serviceFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerAppender(serviceFileAppender);
#endif
    // 计费日志
    FileName calFeeLogPath = logDir + QString("/fee.log");
    RollingFileAppender *calFeeFileAppender = new RollingFileAppender(
        FileUtils::canonicalPath(calFeeLogPath).toString());
    calFeeFileAppender->setFormat(m_config->m_logConfig.format);
    calFeeFileAppender->setLogFilesLimit(m_config->m_logConfig.filesLimit);
    calFeeFileAppender->setFlushOnWrite(true);
    calFeeFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerCategoryAppender("fee", calFeeFileAppender);

    // 加载配置
    if (!m_config->loadConfig())
        return false;

    // json解析对象初始化
    m_jsonSerializer->setIndentMode(QJson::IndentCompact); // 序列化时不保留空格

    // sql语句解析对象初始化
    FileName sqlsDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/sqls");
    FileUtils::makeSureDirExist(sqlsDir);
    LOG_INFO().noquote() << "加载车道服务SQL文件";
    FileNameList sqlFiles = FileUtils::getFilesWithSuffix(sqlsDir, "xml");
    if (sqlFiles.isEmpty()) {
        LOG_ERROR().noquote() << "车道服务SQL文件不存在";
        return false;
    }
    if (!m_sqlDealer->loadSqlFiles(sqlFiles))
        return false;

    LOG_INFO().noquote() << "车道服务SQL文件加载完成";

    // 数据库连接池初始化
    LOG_INFO().noquote() << "初始化车道服务数据库连接";
    EasyQtSql::SqlFactory::DBSetting dbSetting;
    if (m_config->m_dbConfig.type == EM_DataBaseType::MYSQL) {
        dbSetting = EasyQtSql::SqlFactory::DBSetting("QMYSQL",
                                                     m_config->m_dbConfig.ip,
                                                     m_config->m_dbConfig.port,
                                                     m_config->m_dbConfig.user,
                                                     m_config->m_dbConfig.password,
                                                     m_config->m_dbConfig.dbName);
    } else if (m_config->m_dbConfig.type == EM_DataBaseType::DAMENG) {
        // TODO 达梦数据库连接
    }
    m_sqlFactory = EasyQtSql::SqlFactory::getInstance()->config(dbSetting, "tolllanedb");
    // TODO 测试数据库连通性
    LOG_INFO().noquote() << "车道服务数据库连接初始化完成";

    // 服务初始化
    if (m_config->m_serverConfig.mode == EM_ServiceMode::ONLINE) {
        if (m_config->m_serverConfig.socketType == EM_SocketType::TCP) {
            m_rpcServer = new jcon::JsonRpcTcpServer(this, std::make_shared<JsonRpcCuteLogger>());
        } else {
            m_rpcServer = new jcon::JsonRpcWebSocketServer(this, std::make_shared<JsonRpcCuteLogger>());
        }
        m_rpcServer->enableSendNotification(true);
        if (!m_rpcServer->listen(m_config->m_serverConfig.port))
            return false;
    } else {
        m_rpcServer = nullptr; // 单机版
    }

    return true;
}
