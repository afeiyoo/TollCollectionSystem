#pragma once

namespace Constant {

//日志常量
namespace Log {
const char FORMAT_BRIEF[] = "[%{time}{yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] | %{message}\n\n";
const char FORMAT_DETAIL[] = "[%{time}{yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] [%{threadid}] [%{file}:%{line}] | %{message}\n\n";
const int MAX_SAVE_DAY = 90;
} // namespace Log

// 路径常量
namespace Path {
const char APP_ICON[] = ":/static/images/mgskj_icon.png";
const char CAP_AREA_BACKGROUND[] = ":/static/images/demo_pic.jpg";
const char APP_BIG_ICON[] = ":/static/images/banner.png";
const char WHITE_PLATE[] = ":/static/images/white_plate.png";
const char BLACK_PLATE[] = ":/static/images/black_plate.png";
const char YELLOW_PLATE[] = ":/static/images/yellow_plate.png";
const char GREEN_PLATE[] = ":/static/images/green_plate.png";
const char GRADIENT_PLATE[] = ":/static/images/gradient_plate.png";
const char MIX_PLATE[] = ":/static/images/mix_plate.png";
const char BLUE_PLATE[] = ":/static/images/blue_plate.png";
const char TEMP_PLATE[] = ":/static/images/temp_plate.png";
const char LANE_CONFIG_PATH[] = ":/static/config/Lane.ini";
const char KEYBOARD_FILE_PATH[] = ":/static/config/Keyboard_%1.json";
} // namespace Path

// 界面颜色
namespace Color {
const char MAIN_BG[] = "#f8fbff";            // 主界面背景色
const char TOP_BAR_BG[] = "#ffffff";         // 顶栏背景色
const char PAGEAREA_STRESS_BG[] = "#e2ecf6"; // 主界面强调背景色
const char PAGEAREA_NORMAL_BG[] = "#ffffff"; // 主机面常规背景色
const char CONFIRM_BUTTON_BG[] = "#007bff";  // 强调按钮背景色
const char CANCEL_BUTTON_BG[] = "#cdcdcd";   // 取消按钮背景色
const char DIALOG_BG[] = "#f8fbff";          // 对话框背景色
const char SELECTED_BG[] = "#F0F8FF";        // 光标选中背景色

const char SPLITTER[] = "#cdcdcd"; // 分割线颜色
const char BORDER[] = "#e5e5e5";   // 边界颜色

const char CONFIRM_BUTTON_TEXT[] = "#ffffff"; // 强调按钮字体色
const char STATUS_TEXT[] = "#1f9d5e";         // 绿色状态颜色
const char INFO_TEXT[] = "#007bff";           // 蓝色信息颜色
const char WARN_TEXT[] = "#ff0000";           // 红色警告颜色
const char STRESS_TEXT[] = "#9d4d56";         // 强调提醒颜色
const char SELECTED_TEXT[] = "#3399ff";       // 光标选中字体色
} // namespace Color

// 字体大小
namespace FontSize {
const char TOP_BAR_SIZE = 18;
const char BOTTOM_BAR_SIZE = 14;
const char DISPLAY_AREA_SIZE = 20;
const char TRADEINFO_AREA_SIZE = 16;
const char LOG_AREA_SIZE = 14;
const char SCROLLTIP_AREA_SIZE = 18;
const char VEHINFO_AREA_SIZE = 17;
const char TRADE_HINT_SIZE = 28;
const char OBU_HINT_SIZE = 15;
const char WEIGHTINFO_AREA_SIZE = 15;
const char TRADE_VIEW_SIZE = 14;

const char DIALOG_BUTTON_SIZE = 15;
const char DIALOG_TITLE_SIZE = 20;
const char DIALOG_BODY_SIZE = 16;
} // namespace FontSize

// 特殊字符
namespace Character {
const char EMPTY_CHARACTER[] = "*";
} // namespace Character

// 交互对话框ID
namespace DialogID {
const int INVALID_DLG = 0x0000;
const int TEST_DLG = 0x0001;
} // namespace DialogID

} // namespace Constant
