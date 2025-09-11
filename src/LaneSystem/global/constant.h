#pragma once

namespace Constant {

//日志常量
namespace Log {
const char FORMAT_BRIEF[] = "%{time}{yyyy-MM-dd HH:mm:ss.zzz} [%{type}] %{message}\n\n";
const char FORMAT_DETAIL[] = "%{time}{yyyy-MM-dd HH:mm:ss.zzz} [%{type}:%{threadid}]"
                             "[%{file}:%{function}] %{message}\n\n";
const int MAX_SAVE_DAY = 90;
} // namespace Log

//配置常量
namespace Config {
const char *const SYSTEM_DEFAULT_CONFIG_KEY[]
    = {"ServiceMode", "ServiceIP", "ServicePort", "VersionCheckTime", "Keyboard", nullptr};

const char *const SYSTEM_DEFAULT_CONFIG_VALUE[] = {"0", "127.0.0.1", "5927", "600", "DefaultKeyBoard", nullptr};

const char *const BUSINESS_DEFAULT_CONFIG_KEY[] = {
    //:1.ETC入口 2.ETC出口 3.ETC/人工入口 4.ETC/人工出口 5.ETC/自助入口
    //6.ETC/自助出口 7.站前预交易入口 8.站前预交易出口 9.手持机
    "LaneMode",
    "NetID",
    "StationID",
    "LaneID",
    "VirEnstation", //虚拟入口站，开放段时填写
    "LaneIP",
    "FlagID", //门架编号
    "CNLaneID",
    "HeartBeatTime", //心跳上传间隔，单位秒
    "DoubleLane",    //复式票亭
    nullptr};

const char *const BUSINESS_DEFAULT_CONFIG_VALUE[] = {"1",         //LaneMode
                                                     "3501",      //NetID
                                                     " ",         //StationID
                                                     " ",         //LaneID
                                                     " ",         //VirEnstation
                                                     "127.0.0.1", //LaneIP
                                                     " ",         //FlagID
                                                     " ",         //CNLaneID
                                                     "600",       //HeartBeatTimer
                                                     "0",         //DoubleLane
                                                     nullptr};

const char *const KEYBOARD_DEFAULT_KEY[] = {"Key_Enter",     "Key_Return",      "Key_Escape",
                                            "Key_Minus",     "Key_Semicolon",   "Key_Equal",
                                            "Key_Backslash", "Key_BracketLeft", "Key_BracketRight",
                                            "Key_A",         "Key_B",           "Key_C",
                                            "Key_D",         "Key_E",           "Key_F",
                                            "Key_G",         "Key_H",           "Key_I",
                                            "Key_J",         "Key_K",           "Key_L",
                                            "Key_O",         "Key_R",           "Key_S",
                                            "Key_T",         "Key_U",           "Key_V",
                                            "Key_W",         "Key_X",           nullptr};

const char *const KEYBOARD_DEFAULT_VALUE[]
    = {"KeyPaid",  "KeyPaid", "KeyChange", "KeyCar",  "KeyV",      "KeyOtherFreeType", "KeyV1",     "KeyV5",    "KeyV4",
       "KeyMaint", "KeyJC",   "KeyV2",     "KeyGW",   "KeyAdd",    "KeyReprint",       "KeyClose",  "KeyBreak", "KeyOn",
       "KeyOP",    "KeySim",  "KeyTruck",  "KeyOpen", "KeyCancel", "KeyEnter",         "KeyUnpaid", "KeyOff",   "KeyV3",
       "KeyPass",  "KeyEsc",  nullptr};
} // namespace Config

// 路径常量
namespace Path {
const char CONFIG_RELATIVE_PATH[] = "/config/";
const char LANE_CONFIG_NAME[] = "Lane.ini";
const char LOG_RELATIVE_PATH[] = "/logs/";
const char SYS_LOG_NAME[] = "sys.log";
const char BIZ_LOG_NAME[] = "biz.log";
const char DEV_LOG_NAME[] = "dev.log";
const char BACK_LOG_NAME[] = "lane_back.log";
const char KEYBOARD_FILE_NAME[] = "DefaultKeyBoard";
const char APP_ICON[] = ":/static/images/mgskj_icon.png";
const char CAP_AREA_BACKGROUND[] = ":/static/images/demo_pic.jpg";
const char APP_BANNER[] = ":/static/images/banner.png";
const char WHITE_PLATE[] = ":/static/images/white_plate.png";
const char BLACK_PLATE[] = ":/static/images/black_plate.png";
const char YELLOW_PLATE[] = ":/static/images/yellow_plate.png";
const char GREEN_PLATE[] = ":/static/images/green_plate.png";
const char GRADIENT_PLATE[] = ":/static/images/gradient_plate.png";
const char MIX_PLATE[] = ":/static/images/mix_plate.png";
const char BLUE_PLATE[] = ":/static/images/blue_plate.png";
const char TEMP_PLATE[] = ":/static/images/temp_plate.png";
} // namespace Path

// 界面颜色
namespace Color {
const char GRAY_COLOR[] = "#cdcdcd";
const char BLUE_COLOR[] = "#007bff";
const char GREEN_COLOR[] = "#00f91a";
const char WHITE_COLOR[] = "#ffffff";
const char RED_COLOR[] = "#ff0000";
const char ALICE_BULE_COLOR[] = "#F0F8FF"; // 光标选中背景色
const char NAVY_BLUE_COLOR[] = "#3399ff";
} // namespace Color

// 特殊字符
namespace Character {
const char EMPTY_CHARACTER[] = "*";
} // namespace Character

} // namespace Constant
