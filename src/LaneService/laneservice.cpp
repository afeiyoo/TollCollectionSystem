#include "laneservice.h"

#include "Logger.h"
#include "config/config.h"
#include "global/globalmanager.h"
#include "utils/datadealutils.h"

using namespace Utils;

LaneService::LaneService(QObject *parent)
    : QObject(parent)
{}

LaneService::~LaneService() {}

void LaneService::init()
{
    GM_INSTANCE->init();
    GM_INSTANCE->m_rpcServer->registerServices({this});

    LOG_CINFO("service").noquote() << "LaneService 已注册, 监听端口 " << GM_INSTANCE->m_config->m_serverConfig.port;
}

QString LaneService::dbCreate(const QString &reqJson)
{
    QVariantMap reqMap = GM_INSTANCE->m_jsonParser->parse(reqJson.toUtf8()).toMap();
    QString sqlNamespace = reqMap.value("sqlNamespace", "").toString();
    QString sqlID = reqMap.value("sqlID", "").toString();

    QSqlDatabase sdb = GM_INSTANCE->m_sqlFactory->getDatabase("tolllanedb");
    // 获取sql语句
    QString sql = GM_INSTANCE->m_sqlDealer->getSql(sqlNamespace, sqlID);

    EasyQtSql::Transaction t(sdb);
    try {
        QVariantMap resMap;
        if (sqlNamespace != "Create") {
            resMap = genResMapForDBOperate(1, -1, "SQL命名空间类型错误", {});
        } else {
            EasyQtSql::NonQueryResult res = t.execNonQuery(sql);
            LOG_CINFO("service").noquote() << "执行sql语句: " << DataDealUtils::fullExecutedQuery(res.unwrappedQuery());
            resMap = genResMapForDBOperate(0, res.numRowsAffected(), "成功执行建表语句", {});

            t.commit();
        }
        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        QString desc = QString("%1\tsql: %2").arg(e.lastError.text(), e.lastQuery);
        LOG_CERROR("service").noquote() << desc;

        QVariantMap resMap = genResMapForDBOperate(1, -1, desc, {});

        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    }
}

QString LaneService::dbUpdate(const QString &reqJson)
{
    QVariantMap reqMap = GM_INSTANCE->m_jsonParser->parse(reqJson.toUtf8()).toMap();
    QString sqlNamespace = reqMap.value("sqlNamespace", "").toString();
    QString sqlID = reqMap.value("sqlID", "").toString();
    QVariantMap updateParams = reqMap.value("updateParams", {}).toMap();
    QVariantMap whereParams = reqMap.value("whereParam", {}).toMap();

    QSqlDatabase sdb = GM_INSTANCE->m_sqlFactory->getDatabase("tolllanedb");
    // 获取Sql语句
    QString sql = GM_INSTANCE->m_sqlDealer->getSql(sqlNamespace, sqlID);

    EasyQtSql::Transaction t(sdb);
    try {
        QVariantMap resMap;
        if (sqlNamespace != "Update") {
            resMap = genResMapForDBOperate(1, -1, "SQL命名空间类型错误", {});
        } else {
            QRegularExpression re(R"(UPDATE\s+(\w+)\s+SET\s+.+?\s+WHERE\s+(.+))",
                                  QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = re.match(sql);

            if (!match.hasMatch()) {
                resMap = genResMapForDBOperate(1, -1, "SQL语句解析格式错误", {});
            } else {
                QString tableName = match.captured(1);
                QString whereClause = match.captured(2);
                if (whereClause.isEmpty()) {
                    EasyQtSql::NonQueryResult res = t.update(tableName).set(updateParams).exec();
                    LOG_CINFO("service").noquote()
                        << "执行sql语句: " << DataDealUtils::fullExecutedQuery(res.unwrappedQuery());
                    resMap = genResMapForDBOperate(0, res.numRowsAffected(), "成功执行更新语句", {});
                } else {
                    EasyQtSql::NonQueryResult res = t.update(tableName).set(updateParams).where(whereClause, whereParams);
                    LOG_CINFO("service").noquote()
                        << "执行sql语句: " << DataDealUtils::fullExecutedQuery(res.unwrappedQuery());
                    resMap = genResMapForDBOperate(0, res.numRowsAffected(), "成功执行更新语句", {});
                }

                t.commit();
            }
        }
        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        QString desc = QString("%1\tsql: %2").arg(e.lastError.text(), e.lastQuery);
        LOG_CERROR("service").noquote() << desc;

        QVariantMap resMap = genResMapForDBOperate(1, -1, desc, {});

        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    }
}

QString LaneService::dbRead(const QString &reqJson)
{
    QVariantMap reqMap = GM_INSTANCE->m_jsonParser->parse(reqJson.toUtf8()).toMap();
    QString sqlNamespace = reqMap.value("sqlNamespace", "").toString();
    QString sqlID = reqMap.value("sqlID", "").toString();
    QVariantMap whereParams = reqMap.value("whereParam", {}).toMap();

    QSqlDatabase sdb = GM_INSTANCE->m_sqlFactory->getDatabase("tolllanedb");
    // 获取sql语句
    QString sql = GM_INSTANCE->m_sqlDealer->getSql(sqlNamespace, sqlID);

    EasyQtSql::Transaction t(sdb);
    try {
        QVariantMap resMap;
        if (sqlNamespace != "Read") {
            resMap = genResMapForDBOperate(1, -1, "SQL命名空间类型错误", {});
        } else {
            EasyQtSql::PreparedQuery query = t.prepare(sql);
            EasyQtSql::QueryResult res = query.exec(whereParams);
            LOG_CINFO("service").noquote() << "执行sql语句: " << DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

            QVariantList records;
            while (res.next()) {
                QVariantMap record = res.toMap();
                records.append(record);
            }

            QString desc;
            if (records.isEmpty()) {
                desc = "未查询到相关记录";
            } else {
                desc = QString("成功查询到%1条相关记录").arg(records.size());
            }
            resMap = genResMapForDBOperate(0, res.numRowsAffected(), desc, records);
        }
        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    } catch (EasyQtSql::DBException &e) {
        QString desc = QString("%1\tsql: %2").arg(e.lastError.text(), e.lastQuery);
        LOG_CERROR("service").noquote() << desc;

        QVariantMap resMap = genResMapForDBOperate(1, -1, desc, {});

        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    }
}

QString LaneService::dbDelete(const QString &reqJson)
{
    QVariantMap reqMap = GM_INSTANCE->m_jsonParser->parse(reqJson.toUtf8()).toMap();
    QString sqlNamespace = reqMap.value("sqlNamespace", "").toString();
    QString sqlID = reqMap.value("sqlID", "").toString();
    QVariantMap whereParams = reqMap.value("whereParam", {}).toMap();

    QSqlDatabase sdb = GM_INSTANCE->m_sqlFactory->getDatabase("tolllanedb");
    // 获取sql语句
    QString sql = GM_INSTANCE->m_sqlDealer->getSql(sqlNamespace, sqlID);

    EasyQtSql::Transaction t(sdb);
    try {
        QVariantMap resMap;
        if (sqlNamespace != "Delete") {
            resMap = genResMapForDBOperate(1, -1, "SQL命名空间类型错误", {});
        } else {
            QRegularExpression re(R"(DELETE\s+FROM\s+(\w+)(?:\s+WHERE\s+(.+))?)",
                                  QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = re.match(sql);

            if (!match.hasMatch()) {
                resMap = genResMapForDBOperate(1, -1, "SQL语句解析格式错误", {});
            } else {
                QString tableName = match.captured(1);
                QString whereClause = match.captured(2);

                if (whereClause.isEmpty()) {
                    EasyQtSql::NonQueryResult res = t.deleteFrom(tableName).exec();
                    LOG_CINFO("service").noquote()
                        << "执行sql语句: " << DataDealUtils::fullExecutedQuery(res.unwrappedQuery());
                    resMap = genResMapForDBOperate(0, res.numRowsAffected(), "成功执行删除语句", {});
                } else {
                    EasyQtSql::NonQueryResult res = t.deleteFrom(tableName).where(whereClause, whereParams);
                    LOG_CINFO("service").noquote()
                        << "执行sql语句: " << DataDealUtils::fullExecutedQuery(res.unwrappedQuery());
                    resMap = genResMapForDBOperate(0, res.numRowsAffected(), "成功执行删除语句", {});
                }

                t.commit();
            }
        }
        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        QString desc = QString("%1\tsql: %2").arg(e.lastError.text(), e.lastQuery);
        LOG_CERROR("service").noquote() << desc;

        QVariantMap resMap = genResMapForDBOperate(1, -1, desc, {});

        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    }
}

QString LaneService::dbInsert(const QString &reqJson)
{
    QVariantMap reqMap = GM_INSTANCE->m_jsonParser->parse(reqJson.toUtf8()).toMap();
    QString sqlNamespace = reqMap.value("sqlNamespace", "").toString();
    QString sqlID = reqMap.value("sqlID", "").toString();
    QVariantMap insertValues = reqMap.value("insertValues", {}).toMap();

    QSqlDatabase sdb = GM_INSTANCE->m_sqlFactory->getDatabase("tolllanedb");
    // 获取sql语句
    QString sql = GM_INSTANCE->m_sqlDealer->getSql(sqlNamespace, sqlID);

    EasyQtSql::Transaction t(sdb);
    try {
        QVariantMap resMap;
        if (sqlNamespace != "Insert") {
            resMap = genResMapForDBOperate(1, -1, "SQL命名空间类型错误", {});
        } else {
            QRegularExpression re(R"(INSERT\s+INTO\s+([A-Za-z_]\w*))", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = re.match(sql);

            if (!match.hasMatch()) {
                resMap = genResMapForDBOperate(1, -1, "SQL语句格式错误", {});
            } else {
                QString tableName = match.captured(1);

                EasyQtSql::NonQueryResult res = t.insertInto(tableName).values(insertValues).exec();
                LOG_CINFO("service").noquote() << "执行sql语句: " << res.executedQuery();
                resMap = genResMapForDBOperate(0, res.numRowsAffected(), "成功执行插入语句", {});

                t.commit();
            }
        }
        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        QString desc = QString("%1\tsql: %2").arg(e.lastError.text(), e.lastQuery);
        LOG_CERROR("service").noquote() << desc;

        QVariantMap resMap = genResMapForDBOperate(1, -1, desc, {});

        QString resJson = QString::fromUtf8(GM_INSTANCE->m_jsonSerializer->serialize(resMap));
        return resJson;
    }
}

QVariantMap LaneService::genResMapForDBOperate(int status, int recordCount, QString desc, QVariantList records)
{
    QVariantMap resMap;
    resMap["status"] = status;
    resMap["recordCount"] = recordCount;
    resMap["desc"] = desc;
    resMap["records"] = records;

    return resMap;
}
