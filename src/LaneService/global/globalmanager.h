#pragma once

#include "EasyQtSql/EasyQtSql.h"
#include "Jcon/json_rpc_server.h"
#include "QJson/include/parser.h"
#include "QJson/include/serializer.h"
#include "utils/sqlutils.h"
#include <QObject>

#define GM_INSTANCE GlobalManager::instance()

class Config;
class GlobalManager : public QObject
{
    Q_OBJECT
public:
    explicit GlobalManager(QObject *parent = nullptr);
    ~GlobalManager() override;

    static GlobalManager *instance();

    bool initForLocal();
    bool initForOnline();

public:
    // 全局配置信息
    Config *m_config = nullptr;
    // 通信服务端
    jcon::JsonRpcServer *m_rpcServer = nullptr;
    // 数据库连接池
    EasyQtSql::SqlFactory *m_sqlFactory = nullptr;
    // Json解析对象
    QJson::Parser *m_jsonParser = nullptr;
    QJson::Serializer *m_jsonSerializer = nullptr;
    // sql语句解析对象
    Utils::SqlUtils *m_sqlDealer = nullptr;
};
