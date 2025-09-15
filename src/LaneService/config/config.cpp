#include "config.h"

#include "global/constant.h"

Config::Config(QObject *parent)
    : QObject(parent)
{
    m_logConfig.filesLimit = Constant::Log::LIMIT_FILES;
    m_logConfig.format = Constant::Log::FORMAT;
}

Config::~Config() {}

void Config::loadConfig(const Utils::FileName &configPath)
{
    QString str = Utils::FileUtils::canonicalPath(configPath).toString();
    m_ini = new Utils::IniUtils(str);

    m_dbConfig.type = m_ini->value("database", "type", "1").toUInt();
    m_dbConfig.host = m_ini->value("database", "host").toString();
    m_dbConfig.port = m_ini->value("database", "port").toUInt();
    m_dbConfig.dbName = m_ini->value("database", "dbName", "tolllanedb").toString();
    m_dbConfig.user = m_ini->value("database", "user", "tlman").toString();
    m_dbConfig.password = m_ini->value("database", "password", "ds18fjeit").toString();

    m_serverConfig.socketType = m_ini->value("rpc_server", "socketType", "1").toUInt();
    m_serverConfig.port = m_ini->value("rpc_server", "port", "5927").toUInt();
}

QString Config::dumpConfig()
{
    if (!m_ini)
        return "";
    return m_ini->dumpInfo();
}
