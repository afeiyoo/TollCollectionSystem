#include "globalmanager.h"

#include "ConsoleAppender.h"
#include "Jcon/json_rpc_tcp_server.h"
#include "Jcon/json_rpc_websocket_server.h"
#include "Logger.h"
#include "RollingFileAppender.h"
#include "config/config.h"
#include "global/defs.h"
#include "utils/fileutils.h"

Q_GLOBAL_STATIC(GlobalManager, ins)

GlobalManager::GlobalManager(QObject *parent)
    : QObject{parent}
{
    m_config = new Config(this);

    m_jsonParser = new QJson::Parser();
    m_jsonSerializer = new QJson::Serializer();

    m_sqlDealer = new Utils::SqlUtils(this);
}

GlobalManager::~GlobalManager()
{
    delete m_jsonParser;
    delete m_jsonSerializer;
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

    //总日志，负责打接收到的报文信息
    Utils::FileName logPath = Utils::FileName::fromString(Utils::FileUtils::curApplicationDirPath()
                                                          + "/logs/LaneService.log");
    Utils::FileUtils::makeSureDirExist(logPath.parentDir());
    RollingFileAppender *serviceFileAppender = new RollingFileAppender(
        Utils::FileUtils::canonicalPath(logPath).toString());
    serviceFileAppender->setFormat(m_config->m_logConfig.format);
    serviceFileAppender->setLogFilesLimit(m_config->m_logConfig.filesLimit);
    serviceFileAppender->setFlushOnWrite(true);
    serviceFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerCategoryAppender("service",serviceFileAppender);

    //TODO 注册计费日志

    //TODO 注册SQL日志

    // 加载配置
    Utils::FileName configPath = Utils::FileName::fromString(Utils::FileUtils::curApplicationDirPath()
                                                             + "/config/service.ini");
    m_config->loadConfig(configPath);
    m_config->dumpConfig();

    // json解析对象初始化
    m_jsonSerializer->setIndentMode(QJson::IndentCompact); // 序列化时不保留空格

    // sql语句解析对象初始化
    Utils::FileName sqlsDir = Utils::FileName::fromString(Utils::FileUtils::curApplicationDirPath() + "/sqls");
    Utils::FileNameList sqlFiles = Utils::FileUtils::getFilesWithSuffix(sqlsDir, "xml");
    m_sqlDealer->loadSqlFiles(sqlFiles);

    // 服务初始化
    if (m_config->m_serverConfig.socketType == SocketType::tcp) {
        m_rpcServer = new jcon::JsonRpcTcpServer(this);
    } else {
        m_rpcServer = new jcon::JsonRpcWebSocketServer(this);
    }
    m_rpcServer->enableSendNotification(true);
    m_rpcServer->listen(m_config->m_serverConfig.port);

    // 数据库连接池初始化
    EasyQtSql::SqlFactory::DBSetting dbSetting;
    if (m_config->m_dbConfig.type == DataBaseType::MySql) {
        dbSetting = EasyQtSql::SqlFactory::DBSetting("QMYSQL",
                                                     m_config->m_dbConfig.host,
                                                     m_config->m_dbConfig.port,
                                                     m_config->m_dbConfig.user,
                                                     m_config->m_dbConfig.password,
                                                     m_config->m_dbConfig.dbName);
    } else if (m_config->m_dbConfig.type == DataBaseType::Dameng) {
        // TODO 达梦数据库连接
    }
    m_sqlFactory = EasyQtSql::SqlFactory::getInstance()->config(dbSetting, "tolllanedb");
}
