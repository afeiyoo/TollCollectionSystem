#include "globalmanager.h"

#include "ConsoleAppender.h"
#include "Logger.h"
#include "RollingFileAppender.h"
#include "bend/dtpsender.h"
#include "config/config.h"
#include "utils/fileutils.h"

Q_GLOBAL_STATIC(GlobalManager, ins);

GlobalManager::GlobalManager(QObject *parent)
    : QObject{parent}
{
    m_config = new Config(this);
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

    Utils::FileName logPath = Utils::FileName::fromString(Utils::FileUtils::curApplicationDirPath()
                                                          + "/logs/PadService.log");
    Utils::FileUtils::makeSureDirExist(logPath.parentDir());
    RollingFileAppender *rollingFileAppender = new RollingFileAppender(
        Utils::FileUtils::canonicalPath(logPath).toString());
    rollingFileAppender->setFormat(m_config->m_logConfig.format);
    rollingFileAppender->setLogFilesLimit(m_config->m_logConfig.filesLimit);
    rollingFileAppender->setFlushOnWrite(true);
    rollingFileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    cuteLogger->registerAppender(rollingFileAppender);

    // 配置加载
    Utils::FileName configPath = Utils::FileName::fromString(Utils::FileUtils::curApplicationDirPath()
                                                             + "/config/config.ini");
    m_config->loadConfig(configPath);

    // 系统环境初始化
    m_pictureDir = Utils::FileName::fromString(QCoreApplication::applicationDirPath() + "/pictures");
    Utils::FileUtils::makeSureDirExist(m_pictureDir);
    Utils::FileUtils::autoDeleteFiles(m_pictureDir.toString(), ".jpg", 30);

#if QT_VERSION <= QT_VERSION_CHECK(5, 10, 0)
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime())); // 随机数种子初始化
#endif

    // 数据库连接初始化
    QString dbType = m_config->m_dbConfig.type;
    EasyQtSql::SqlFactory::DBSetting setting;
    if (dbType == "QODBC") {
        QString connectionString = QString("Driver={%1};DBQ=%2;UID=%3;PWD=%4")
                                       .arg(m_config->m_dbConfig.driver,
                                            m_config->m_dbConfig.dbName,
                                            m_config->m_dbConfig.user,
                                            m_config->m_dbConfig.password);
        setting = EasyQtSql::SqlFactory::DBSetting(dbType, connectionString);
    } else {
        setting = EasyQtSql::SqlFactory::DBSetting(dbType,
                                                   m_config->m_dbConfig.host,
                                                   m_config->m_dbConfig.port,
                                                   m_config->m_dbConfig.user,
                                                   m_config->m_dbConfig.password,
                                                   m_config->m_dbConfig.dbName);
    }
    m_dbFactory = EasyQtSql::SqlFactory::getInstance()->config(setting, "oracle");

    // Json解析对象初始化
    m_jsonParser = new QJson::Parser();
    m_jsonSerializer = new QJson::Serializer();
    m_jsonSerializer->setIndentMode(QJson::IndentCompact);

    // Dtp发送对象初始化
    m_dtpSender = new DtpSender(this);
    m_dtpSender->initDtp("./libDtp-Client.so");
}

void GlobalManager::createSqlServerConnection(const QString &stationIP)
{
    QString connectionStr = QString("Driver={ODBC Driver 18 for SQL Server};"
                                    "Server=%1;Database=TollStationDB;UID=tsman;PWD=ds18fjeit;"
                                    "Encrypt=yes;TrustServerCertificate=yes;")
                                .arg(stationIP);

    EasyQtSql::SqlFactory::DBSetting setting("QODBC", connectionStr);

    if (!m_sqlServerConfiguredStation.contains(stationIP)) {
        QString id = QString("sqlserver_%1").arg(stationIP);
        EasyQtSql::SqlFactory::getInstance()->config(setting, id);
        m_sqlServerConfiguredStation.insert(stationIP); // 用于标记该IP已配置
    }
}
