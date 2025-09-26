#include "Constant.h"

namespace Constant {

namespace Log {
int MAX_SAVE_DAY = 90;
const char FORMAT[] = "[%{time}{yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] [%{threadid}] [%{file}:%{line}] | %{message}\n\n";
} // namespace Log

namespace Path {
const char SERVICE_CONFIG_PATH[] = ":/static/config/Service.ini";
} // namespace Path

} // namespace Constant
