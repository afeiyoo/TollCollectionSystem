#include <QApplication>

#include "ElaWidgetTools/ElaApplication.h"
#include "Logger.h"
#include "bend/mtcin/mideskprocess.h"
#include "config/config.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/mgsmainwindow.h"
#include "utils/datadealutils.h"

#include <QObject>

using namespace Utils;

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    #else
    // 根据实际屏幕缩放比例更改
    qputenv("QT_SCALE_FACTOR", "1.5");
    #endif
#endif
    QApplication a(argc, argv);

    eApp->init();
    GM_INSTANCE->init();

    // 界面初始化
    LOG_INFO().noquote() << "前端初始化";
    MgsMainWindow mainWindow;
    if (GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::MTC_IN) {
        LOG_INFO().noquote() << "加载混合入口前端";
        mainWindow.initMtcIn();
    } else if (GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::MTC_OUT) {
        LOG_INFO().noquote() << "加载混合出口前端";
        mainWindow.initMtcOut();
    } else if (GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::ETC_IN
               || GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::ETC_OUT) {
        LOG_INFO().noquote() << "加载ETC车道前端";
        mainWindow.initEtc();
    } else {
        LOG_ASSERT_X(false, "系统初始化失败：前端初始化失败（无对应车道类型）");
    }
    mainWindow.showNormal();
    LOG_INFO().noquote() << "前端初始化完成";

    // 业务处理模块初始化
    LOG_INFO().noquote() << "业务处理模块初始化";
    if (GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::MTC_IN) {
        LOG_INFO().noquote() << "加载混合入口业务处理模块";
        new MIDeskProcess();
    } else if (GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::MTC_OUT) {
        // TODO 混合出口业务处理模块
    } else if (GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::ETC_IN
               || GM_INSTANCE->m_config->m_businessConfig.laneMode == EM_LaneMode::ETC_OUT) {
        // TODO ETC业务处理模块
    } else {
        LOG_ASSERT_X(false, "系统初始化失败：业务处理模块初始化失败（无对应车道类型）");
    }
    LOG_INFO().noquote() << "业务处理模块初始化完成";
    LOG_INFO().noquote() << "系统正常启动: " << DataDealUtils::curDateTimeStr();
    emit GM_INSTANCE->m_signalMan->sigLogAppend(EM_LogLevel::INFO, "系统正常启动");

    return a.exec();
}
