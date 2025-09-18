#include "config.h"
#include "Logger.h"
#include "global/constant.h"
#include "utils/fileutils.h"
#include "utils/iniutils.h"

using namespace Utils;

Config::Config(QObject *parent)
    : QObject(parent)
{
    m_logConfig.filesLimit = Constant::Log::LIMIT_FILES;
    m_logConfig.format = Constant::Log::FORMAT;
}

Config::~Config() {}

bool Config::loadConfig()
{
    FileName configDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/config");
    FileUtils::makeSureDirExist(configDir);

    // 服务配置加载
    FileName configPath = configDir + QString("/Service.ini");
    LOG_INFO().noquote() << "加载服务配置";
    if (!configPath.exists()) {
        LOG_WARNING().noquote() << "服务配置文件 " << configPath.fileName(0) << "不存在，开始创建默认服务配置";
        QByteArray configData = FileReader::fetchQrc(Constant::Path::SERVICE_CONFIG_PATH);
        FileSaver saver(configPath.toString());
        saver.write(configData);
        if (!saver.finalize()) {
            LOG_ERROR().noquote() << "创建默认服务配置失败 " << saver.errorString();
            return false;
        }
        LOG_INFO().noquote() << "创建默认服务配置成功";
    }

    IniUtils ini(configPath.toString());
    m_dbConfig.type = ini.value("database", "type", 1).toUInt();
    m_dbConfig.ip = ini.value("database", "ip", "127.0.0.1").toString();
    m_dbConfig.port = ini.value("database", "port", 3306).toUInt();
    m_dbConfig.dbName = ini.value("database", "dbName", "tolllanedb").toString();
    m_dbConfig.user = ini.value("database", "user", "tlman").toString();
    m_dbConfig.password = ini.value("database", "password", "ds18fjeit").toString();

    m_serverConfig.socketType = ini.value("service", "socketType", 1).toUInt();
    m_serverConfig.port = ini.value("service", "port", 5927).toUInt();
    m_serverConfig.mode = ini.value("service", "mode", 0).toUInt();

    LOG_INFO().noquote() << ini.dumpInfo();
    LOG_INFO().noquote() << "服务配置加载完成";

    return true;
}
