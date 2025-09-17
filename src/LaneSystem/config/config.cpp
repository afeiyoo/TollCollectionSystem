#include "config.h"

#include "Logger.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "utils/fileutils.h"
#include "utils/iniutils.h"

using namespace Utils;

Config::Config(QObject *parent)
    : QObject{parent}
{}

Config::~Config() {}

bool Config::loadConfig()
{
    FileName configDir = FileName::fromString(FileUtils::curApplicationDirPath() + "/config");
    FileUtils::makeSureDirExist(configDir);

    // 车道基础配置加载
    LOG_INFO().noquote() << "加载车道基础配置";
    FileName configFile = configDir + QString("/Lane.ini");
    if (!configFile.exists()) {
        LOG_WARNING().noquote() << "车道基础配置文件 " << configFile.fileName(0) << "不存在，开始创建车道默认配置";
        // 创建默认车道配置
        QByteArray configData = FileReader::fetchQrc(Constant::Path::LANE_CONFIG_PATH);
        FileSaver saver(configFile.toString());
        saver.write(configData);
        if (!saver.finalize()) {
            LOG_ERROR().noquote() << "创建车道默认基础配置失败 " << saver.errorString();
            return false;
        }
        LOG_INFO().noquote() << "创建车道默认基础配置成功";
    }

    IniUtils ini(FileUtils::canonicalPath(configFile).toString());
    m_systemConfig.serviceMode = ini.value("system", "serviceMode", 0).toInt();
    m_systemConfig.serviceSocketType = ini.value("system", "serviceSocketType", 0).toInt();
    m_systemConfig.serviceIP = ini.value("system", "serviceIP", "127.0.0.1").toString();
    m_systemConfig.servicePort = ini.value("system", "servicePort", 5927).toInt();
    m_systemConfig.versionCheckTime = ini.value("system", "versionCheckTime", 600).toInt();
    m_systemConfig.keyboardType = ini.value("system", "keyboardType", 0).toInt();

    m_businessConfig.laneMode = ini.value("business", "laneMode", 1).toInt();
    m_businessConfig.netID = ini.value("business", "netID", "3501").toString();
    m_businessConfig.stationID = ini.value("business", "stationID").toString();
    m_businessConfig.laneID = ini.value("business", "laneID", 0).toInt();
    m_businessConfig.virEnstation = ini.value("business", "virEnstation").toString();
    m_businessConfig.laneIP = ini.value("business", "laneIP", "127.0.0.1").toString();
    m_businessConfig.gantryID = ini.value("business", "gantryID").toString();
    m_businessConfig.CNLaneID = ini.value("business", "CNLaneID").toString();
    m_businessConfig.doubleLane = ini.value("business", "doubleLane").toInt();
    m_businessConfig.heartBeatTime = ini.value("business", "heartBeatTime", 600).toInt();

    LOG_INFO().noquote() << ini.dumpInfo();
    LOG_INFO().noquote() << "车道基础配置加载完成";

    // 车道键盘参数加载
    LOG_INFO().noquote() << "加载车道键盘参数 键盘类型" << m_systemConfig.keyboardType;
    FileName keyboardFile = configDir + QString("/Keyboard_%1.json").arg(m_systemConfig.keyboardType);
    if (!keyboardFile.exists()) {
        LOG_WARNING().noquote() << "键盘参数文件 " << keyboardFile.fileName(0) << "不存在，开始创建默认键盘参数";
        // 创建默认键盘参数
        QString keyboardPath = QString(Constant::Path::KEYBOARD_FILE_PATH).arg(m_systemConfig.keyboardType);
        QByteArray keyboardJson = FileReader::fetchQrc(keyboardPath);
        FileSaver saver(keyboardFile.toString());
        saver.write(keyboardJson);
        if (!saver.finalize()) {
            LOG_ERROR().noquote() << "创建默认键盘参数失败 " << saver.errorString();
            return false;
        }
        LOG_INFO().noquote() << "创建默认键盘参数成功";
    }
    FileReader reader;
    bool success = reader.fetch(keyboardFile.toString());
    if (!success) {
        LOG_ERROR().noquote() << "读取车道键盘参数文件失败 " << reader.errorString();
        return false;
    }
    bool ok = false;
    QVariant var = GM_INSTANCE->m_jsonParser->parse(reader.data(), &ok);
    if (!ok) {
        LOG_ERROR().noquote() << "车道键盘参数文件内容解析错误 " << GM_INSTANCE->m_jsonParser->errorString();
        return false;
    }
    m_keyboard = var.toMap();
    LOG_INFO().noquote() << "车道键盘参数加载完成";

    return true;
}
