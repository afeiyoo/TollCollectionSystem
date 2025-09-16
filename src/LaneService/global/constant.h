#pragma once

namespace Constant {

namespace Log {
const char FORMAT[] = "[%{time}{yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] [%{threadid}] [%{file}:%{line}] | %{message}\n\n";
int LIMIT_FILES = 90;
} // namespace Log

// 路径常量
namespace Path {
const char SERVICE_CONFIG_PATH[] = ":/static/config/Service.ini";
} // namespace Path

} // namespace Constant
