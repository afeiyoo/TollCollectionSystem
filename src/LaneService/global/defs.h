#pragma once

#include <QString>
#include <QtGlobal>

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
enum DataBaseType { MySql = 1, Dameng };

// 服务的socket类型
enum SocketType { tcp = 1, websocket };
