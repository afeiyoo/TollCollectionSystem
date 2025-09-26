#pragma once

namespace Constant {

//日志常量
namespace Log {
const char FORMAT_BRIEF[] = "[%{time}{yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] | %{message}\n\n";
const char FORMAT_DETAIL[]
    = "[%{time}{yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] [%{threadid}] [%{file}:%{line}] | %{message}\n\n";
const int MAX_SAVE_DAY = 90;
} // namespace Log

// 路径常量
namespace Path {
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
const char LANE_CONFIG_PATH[] = ":/static/config/Lane.ini";
const char KEYBOARD_FILE_PATH[] = ":/static/config/Keyboard_%1.json";
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

// 字体大小
namespace FontSize {
const char LARGE_TITLE_SIZE = 30;
const char TITLE_SIZE = 25;
const char SUBTITLE_SIZE = 20;
const char BODY_SIZE = 17;
const char CONTROL_SIZE = 15;
const char CAPTION_SIZE = 12;
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
