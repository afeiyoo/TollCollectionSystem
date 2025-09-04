#pragma once

#include <QObject>
#include <QSet>

#include "EasyQtSql/EasyQtSql.h"
#include "QJson/include/parser.h"
#include "QJson/include/serializer.h"
#include "utils/fileutils.h"

#define GM_INSTANCE GlobalManager::instance()

class Config;
class GlobalManager : public QObject
{
    Q_OBJECT
public:
    explicit GlobalManager(QObject *parent = nullptr);
    ~GlobalManager() override;

    static GlobalManager *instance();

    void init();

    void createSqlServerConnection(const QString &stationIP);

public:
    // 站级数据库连接缓存
    QSet<QString> m_sqlServerConfiguredStation;
    // 配置对象
    Config *m_config = nullptr;
    // 数据库连接池
    EasyQtSql::SqlFactory *m_dbFactory = nullptr;
    // 图片保存目录
    Utils::FileName m_pictureDir;
    // Json解析对象
    QJson::Serializer *m_jsonSerializer = nullptr;
    QJson::Parser *m_jsonParser = nullptr;
};
