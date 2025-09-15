#pragma once

#include <QString>
#include <QtGlobal>

#include "utils/stdafx.h"

// 数据库配置
struct ST_DatabaseConfig
{
    uint type; // 数据库类型
    QString host;
    uint port;
    QString dbName;
    QString user;
    QString password;
};

// 服务端配置
struct ST_RpcServerConfig
{
    uint socketType; // 连接类型 0-tcp, 1-websocket
    uint port;
};

// 日志配置
struct ST_LogConfig
{
    QString format;
    uint filesLimit;
};

// 数据库类型枚举
Q_BEGIN_ENUM_CREATE(EM_DataBaseType)
enum DataBaseType { MYSQL = 1, DAMENG };
Q_ENUM_CREATE(DataBaseType)
Q_END_ENUM_CREATE(EM_DataBaseType)

// 服务的socket类型
Q_BEGIN_ENUM_CREATE(EM_SocketType)
enum SocketType { TCP = 1, WEBSOCKET };
Q_ENUM_CREATE(SocketType)
Q_END_ENUM_CREATE(EM_SocketType)
