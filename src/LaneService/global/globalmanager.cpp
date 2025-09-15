#include "globalmanager.h"

#include "ConsoleAppender.h"
#include "Jcon/json_rpc_tcp_server.h"
#include "Jcon/json_rpc_websocket_server.h"
#include "Logger.h"
#include "RollingFileAppender.h"
#include "config/config.h"
#include "global/defs.h"
#include "utils/fileutils.h"
#include "utils/stdafx.h"

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

    void logDebug(const QString &message) override { LOG_CDEBUG("service").noquote() << message; }
    void logInfo(const QString &message) override { LOG_CINFO("service").noquote() << message; }
    void logWarning(const QString &message) override { LOG_CWARNING("service").noquote() << message; }
    void logError(const QString &message) override { LOG_CERROR("service").noquote() << message; }
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

void GlobalManager::init()
{
    // 日志初始化
    ConsoleAppender *consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat(m_config->m_logConfig.format);
    cuteLogger->registerAppender(consoleAppender);

    //总日志
    FileName logPath = FileName::fromString(FileUtils::curApplicationDirPath() + "/logs/LaneService.log");
    FileUtils::makeSureDirExist(logPath.parentDir());
    RollingFileAppender *serviceFileAppender = new RollingFileAppender(FileUtils::canonicalPath(logPath).toString());
    serviceFileAppender->setFormat(m_config->m_logConfig.format);
    serviceFileAppender->setLogFilesLimit(m_config->m_logConfig.filesLimit);
    serviceFileAppender->setFlushOnWrite(true);
    serviceFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerCategoryAppender("service", serviceFileAppender);

    // 加载配置
    FileName configPath = FileName::fromString(FileUtils::curApplicationDirPath() + "/config/service.ini");
    m_config->loadConfig(configPath);
    LOG_CINFO("service").noquote() << m_config->dumpConfig();

    // json解析对象初始化
    m_jsonSerializer->setIndentMode(QJson::IndentCompact); // 序列化时不保留空格

    // sql语句解析对象初始化
    FileName sqlsDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/sqls");
    FileNameList sqlFiles = FileUtils::getFilesWithSuffix(sqlsDir, "xml");
    m_sqlDealer->loadSqlFiles(sqlFiles);

    // 服务初始化
    if (m_config->m_serverConfig.socketType == EM_SocketType::TCP) {
        m_rpcServer = new jcon::JsonRpcTcpServer(this, std::make_shared<JsonRpcCuteLogger>());
    } else {
        m_rpcServer = new jcon::JsonRpcWebSocketServer(this, std::make_shared<JsonRpcCuteLogger>());
    }
    m_rpcServer->enableSendNotification(true);
    m_rpcServer->listen(m_config->m_serverConfig.port);

    // 数据库连接池初始化
    EasyQtSql::SqlFactory::DBSetting dbSetting;
    if (m_config->m_dbConfig.type == EM_DataBaseType::MYSQL) {
        dbSetting = EasyQtSql::SqlFactory::DBSetting("QMYSQL",
                                                     m_config->m_dbConfig.host,
                                                     m_config->m_dbConfig.port,
                                                     m_config->m_dbConfig.user,
                                                     m_config->m_dbConfig.password,
                                                     m_config->m_dbConfig.dbName);
    } else if (m_config->m_dbConfig.type == EM_DataBaseType::DAMENG) {
        // TODO 达梦数据库连接
    }
    m_sqlFactory = EasyQtSql::SqlFactory::getInstance()->config(dbSetting, "tolllanedb");
}
