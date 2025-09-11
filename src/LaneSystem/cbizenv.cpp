#include "cbizenv.h"
#include "fileutils.h"
#include "global/constant.h"
#include "iniutils.h"

Q_GLOBAL_STATIC(CBizEnv, bizEnv)
CBizEnv::CBizEnv(QObject *parent)
    : QObject(parent)
{
    m_cap_list.clear();
    m_weight_list.clear();
}

CBizEnv::~CBizEnv() {}

bool CBizEnv::loadLaneEnv()
{
    LOG_CINFO("sys") << "开始读取车道基础配置(LaneEnv)";
    using namespace Utils;
    FileName configFile;
    configFile = FileName::fromString(FileUtils::curApplicationDirPath() + QString(Constant::Path::CONFIG_RELATIVE_PATH)
                                      + Constant::Path::LANE_CONFIG_NAME);
    //检查文件是否存在
    if (!configFile.exists()) {
        LOG_CWARNING("sys") << "配置文件" << configFile.toString() << "不存在";
        if (!createDefaultCfg()) {
            LOG_CERROR("sys") << "创建默认配置失败，车道初始化失败！";
            return false;
        }
    }
    //读取配置
    IniUtils iniUtils(configFile.toString());
    m_lane_env.AppVersion = QCoreApplication::applicationVersion();
    m_lane_env.ServiceMode = iniUtils.value("system", "ServiceMode", 0).toInt();
    if (m_lane_env.ServiceMode > 1)
        m_lane_env.ServiceMode = 0;
    m_lane_env.ServiceIP = iniUtils.value("system", "ServiceIP", "127.0.0.1").toString();
    m_lane_env.ServicePort = iniUtils.value("system", "ServicePort", 5927).toInt();
    m_lane_env.VersionCheckTime = iniUtils.value("system", "VersionCheckTime", 600).toInt();
    if (m_lane_env.VersionCheckTime <= 0)
        m_lane_env.VersionCheckTime = 600;
    m_lane_env.Keyboard = iniUtils.value("system", "Keyboard", "DefaultKeyBoard").toString();

    m_lane_env.LaneMode = iniUtils.value("business", "LaneMode", 1).toInt();
    m_lane_env.NetID = iniUtils.value("business", "NetID", "3501").toString();
    m_lane_env.StationID = iniUtils.value("business", "StationID").toString();
    m_lane_env.LaneID = iniUtils.value("business", "LaneID", 0).toInt();
    m_lane_env.VirEnstation = iniUtils.value("business", "VirEnstation").toString();
    m_lane_env.LaneIP = iniUtils.value("business", "LaneIP", "127.0.0.1").toString();
    m_lane_env.FlagID = iniUtils.value("business", "FlagID").toString();
    m_lane_env.CNLaneID = iniUtils.value("business", "CNLaneID").toString();
    m_lane_env.DoubleLane = iniUtils.value("business", "DoubleLane").toInt();
    m_lane_env.HeartBeatTime = iniUtils.value("business", "HeartBeatTime", 600).toInt();
    //读取配置结束
    LOG_CDEBUG("sys") << "车道模式："
                      << QString(m_lane_env.ServiceMode == 0 ? "单机版" : "在线式").toStdString().c_str();
    LOG_CDEBUG("sys") << "计费服务IP：" << m_lane_env.ServiceIP.toStdString().c_str() << ":" << m_lane_env.ServicePort;
    LOG_CINFO("sys") << "检查软件升级间隔：" << m_lane_env.VersionCheckTime << "s";
    LOG_CINFO("biz") << "站代码（8位）：" << m_lane_env.NetID.toStdString().c_str()
                     << m_lane_env.StationID.toStdString().c_str() << "车道号：" << m_lane_env.LaneID << "CNLaneID："
                     << m_lane_env.CNLaneID.toStdString().c_str();
    LOG_CINFO("biz") << "车道模式：" << m_lane_env.LaneMode;
    if (m_lane_env.DoubleLane)
        LOG_CINFO("biz") << "复式票亭";
    if (!m_lane_env.VirEnstation.isEmpty())
        LOG_CINFO("biz") << "虚拟入口站：" << m_lane_env.VirEnstation.toStdString().c_str() << ",开放段";
    LOG_CINFO("biz") << "门架ID：" << m_lane_env.FlagID.toStdString().c_str();
    LOG_CINFO("biz") << "心跳上传间隔：" << m_lane_env.HeartBeatTime << "s";
    LOG_CINFO("sys") << "基础配置加载完毕（system and business）";
    return true;
}

bool CBizEnv::loadKeyboard()
{
    LOG_CINFO("sys") << "开始加载车道键盘参数";
    using namespace Utils;
    FileName keyboardFile = FileName::fromString(QCoreApplication::applicationDirPath()
                                                 + Constant::Path::CONFIG_RELATIVE_PATH + m_lane_env.Keyboard);
    if (!keyboardFile.exists()) {
        LOG_CWARNING("sys") << "键盘文件" << keyboardFile.toString() << "不存在";
        if (!createDefaultKeyboardCfg(0)) {
            for (int i = 0; Constant::Config::KEYBOARD_DEFAULT_KEY[i] != nullptr; i++) {
                if (Constant::Config::KEYBOARD_DEFAULT_VALUE[i] != nullptr)
                    m_keyboardMap.insert(Constant::Config::KEYBOARD_DEFAULT_KEY[i],
                                         Constant::Config::KEYBOARD_DEFAULT_VALUE[i]);
            }
            return false;
        }
    }
    IniUtils iniUtils(keyboardFile.toString());
    for (int i = 0; Constant::Config::KEYBOARD_DEFAULT_KEY[i] != nullptr; i++) {
        QString value = iniUtils.value("keyboard", Constant::Config::KEYBOARD_DEFAULT_KEY[i]).toString();
        m_keyboardMap.insert("keyboard", value);
    }
    LOG_CINFO("sys") << "车道键盘参数加载完成";
    return true;
}

bool CBizEnv::loadDevCfg()
{
    LOG_CINFO("sys") << "开始读取车道设备配置(LaneDev)";
    return true;
}

bool CBizEnv::createDefaultCfg()
{
    LOG_CINFO("sys") << "开始创建车道默认配置";
    using namespace Utils;
    IniUtils iniUtils(QCoreApplication::applicationDirPath() + Constant::Path::CONFIG_RELATIVE_PATH
                      + Constant::Path::LANE_CONFIG_NAME);
    //创建系统默认配置
    for (int i = 0; Constant::Config::SYSTEM_DEFAULT_CONFIG_KEY[i] != nullptr; i++) {
        if (Constant::Config::SYSTEM_DEFAULT_CONFIG_VALUE[i] != nullptr) {
            iniUtils.setValue("system",
                              Constant::Config::SYSTEM_DEFAULT_CONFIG_KEY[i],
                              Constant::Config::SYSTEM_DEFAULT_CONFIG_VALUE[i],
                              Qt::CaseSensitive);
        }
    }
    //创建业务默认配置
    for (int i = 0; Constant::Config::BUSINESS_DEFAULT_CONFIG_KEY[i] != nullptr; i++) {
        if (Constant::Config::BUSINESS_DEFAULT_CONFIG_VALUE[i] != nullptr) {
            iniUtils.setValue("business",
                              Constant::Config::BUSINESS_DEFAULT_CONFIG_KEY[i],
                              Constant::Config::BUSINESS_DEFAULT_CONFIG_VALUE[i],
                              Qt::CaseSensitive);
        }
    }
    //TODO 还有设备相关
    return iniUtils.save();
}

/**
*    @brief:   创建默认键盘映射
*    @param:   0：默认mgs键盘
*              后续根据厂商情况可扩展
*    @returns: 是否创建成功
*    @author:  SheMinglei
*/
bool CBizEnv::createDefaultKeyboardCfg(int type)
{
    LOG_CDEBUG("sys") << "开始生成默认键盘映射，类型:" << type;
    switch (type) {
    case 0:
        using namespace Utils;
        FileName configAbsolutePath = FileName::fromString(QCoreApplication::applicationDirPath()
                                                           + Constant::Path::CONFIG_RELATIVE_PATH);
        FileUtils::makeSureDirExist(configAbsolutePath);
        IniUtils iniUtils(configAbsolutePath.toString() + Constant::Path::KEYBOARD_FILE_NAME);
        for (int i = 0; Constant::Config::KEYBOARD_DEFAULT_KEY[i] != nullptr; i++) {
            if (Constant::Config::KEYBOARD_DEFAULT_VALUE[i] != nullptr)
                iniUtils.setValue("keyboard",
                                  Constant::Config::KEYBOARD_DEFAULT_KEY[i],
                                  Constant::Config::KEYBOARD_DEFAULT_VALUE[i]);
        }
        if (!iniUtils.save()) {
            LOG_CERROR("sys") << "生成键盘映射失败";
            return false;
        }
        break;
    }
    return true;
}

//运行环境初始化
void CBizEnv::InitLaneEnv()
{
    Utils::FileName configAbsolutePath = Utils::FileName::fromString(QCoreApplication::applicationDirPath()
                                                                     + Constant::Path::CONFIG_RELATIVE_PATH);
    Utils::FileUtils::makeSureDirExist(configAbsolutePath);
    if (!loadLaneEnv()) {
        LOG_CERROR("sys") << "读取车道基础配置失败，车道初始化失败！";
        QCoreApplication::exit(-1);
        return;
    }
    if (!loadKeyboard()) {
        LOG_CWARNING("sys") << "读取键盘基础配置失败，强制使用基础键盘配置";
    }
    if (!loadDevCfg()) {
        LOG_CERROR("sys") << "读取车道设备配置失败，车道初始化失败！";
        QCoreApplication::exit(-1);
        return;
    }
}

//业务环境初始化
void CBizEnv::InitBizEnv()
{
    //创建车道后端服务对象
    // m_service = getBizService();
}

void CBizEnv::RegistLogs()
{
    using namespace Utils;
    //设置路径
    FileName fn = FileName::fromString(QCoreApplication::applicationDirPath() + Constant::Path::LOG_RELATIVE_PATH);
    FileUtils::makeSureDirExist(fn);
    //设置控制台日志，用于定位
    ConsoleAppender *console = new ConsoleAppender;
    console->setDetailsLevel("trace");
    console->setFormat(Constant::Log::FORMAT_DETAIL);
    // console->onlyPrintSetLevel(true);
    cuteLogger->registerCategoryAppender("sys", console);
    //设置sys日志，打详细系统信息
    FileName sysLog = fn + QString(Constant::Path::SYS_LOG_NAME); //系统日志，用于打详细系统信息
    RollingFileAppender *sysAppender = new RollingFileAppender(sysLog.toString());
    sysAppender->setLogFilesLimit(90);
    sysAppender->setFormat(Constant::Log::FORMAT_DETAIL);
    sysAppender->setFlushOnWrite(true);
    sysAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    sysAppender->setDetailsLevel("trace");
    cuteLogger->registerCategoryAppender("sys", sysAppender);
    m_loggerAppenders["sys"] = sysAppender;
    //设置biz日志，打详细业务日志
    FileName bizLog = fn + QString(Constant::Path::BIZ_LOG_NAME); //业务日志，业务类往里头放
    RollingFileAppender *bizAppender = new RollingFileAppender(bizLog.toString());
    bizAppender->setLogFilesLimit(90);
    bizAppender->setFormat(Constant::Log::FORMAT_BRIEF);
    bizAppender->setFlushOnWrite(true);
    bizAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    bizAppender->setDetailsLevel("info");
    cuteLogger->registerCategoryAppender("biz", bizAppender);
    m_loggerAppenders["biz"] = bizAppender;
    //设置dev日志，打详细设备日志
    FileName devLog = fn + QString(Constant::Path::DEV_LOG_NAME); //设备日志，设备类往里头放
    bizAppender = new RollingFileAppender(devLog.toString());
    bizAppender->setLogFilesLimit(90);
    bizAppender->setFormat(Constant::Log::FORMAT_BRIEF);
    bizAppender->setFlushOnWrite(true);
    bizAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
    bizAppender->setDetailsLevel("debug");
    cuteLogger->registerCategoryAppender("dev", bizAppender);
    m_loggerAppenders["dev"] = bizAppender;
}

TLaneEnv CBizEnv::GetLaneEnv() const
{
    return m_lane_env;
}

TDevConfig CBizEnv::GetDevConfig() const
{
    return m_dev_cfg;
}

QString CBizEnv::GetKeyName(int keyCode) const
{
    if (m_keyboardMap.isEmpty())
        return " ";
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
    QString keyStr = metaEnum.valueToKey(keyCode); // 如Qt::Key_J → "Key_J"
    return m_keyboardMap[keyStr];
}

//特别说明，返回-1表示未获取到对应keyName的key
int CBizEnv::GetKeyCode(QString keyName) const
{
    QString enumName = m_keyboardMap.key(keyName);
    if (enumName.isEmpty())
        return -1;
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
    return metaEnum.keyToValue(enumName.toLocal8Bit().data());
}

CBizEnv *CBizEnv::GetInstance()
{
    return bizEnv();
}

void CBizEnv::onWeightInfo(TVehicleWeightInfo weightInfo)
{
    m_mutex.lock();

    //倒车要删掉称重信息
    //不是倒车要加上称重信息
    m_weight_list.append(weightInfo);

    m_mutex.unlock();
}

void CBizEnv::onCapInfo(TCapVehInfo capInfo)
{
    m_mutex.lock();

    //检查是否有重复车牌
    m_cap_list.append(capInfo);

    m_mutex.unlock();
}

void CBizEnv::onVehRecognize() {}
