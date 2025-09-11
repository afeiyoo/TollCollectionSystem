#include <QApplication>

#include "ElaWidgetTools/ElaApplication.h"
// #include "claneform.h"
#include "cbizenv.h"
#include "fileutils.h"
#include "gui/component/mgsmenu.h"
#include "gui/mgsauthdialog.h"
#include "gui/mgsmainwindow.h"
#include "gui/mgsplateeditdialog.h"

#include "jcon/json_rpc_tcp_client.h"
#include "laneservice.h"

#include "utils/uiutils.h"

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

    //ENV初始化
    GENV->RegistLogs(); //注册日志文件
    LOG_CINFO("sys") << "LaneSystem start Init";
    GENV->InitLaneEnv();

    eApp->init(); // 界面库初始化
    //TODO 初始化界面
    MgsMainWindow m;
    m.initMtcIn();
    m.show();

    // MgsPlateEditDialog w;
    // w.show();

    // LaneService *server = new LaneService(nullptr);
    // server->init();

    // auto rpc_client = new jcon::JsonRpcTcpClient(nullptr);
    // rpc_client->connectToServer("127.0.0.1", 9588);

    // QString title2 = QString("请注意搭街坊卡拉介绍了快点放假啊势力扩大解放法撒旦看见了");
    // Utils::UiUtils::showMessageBoxInfo(title2, "好的合", QMessageBox::Yes);

    return a.exec();
}
