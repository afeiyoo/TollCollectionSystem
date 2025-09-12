#include "dataservice.h"

#include "EasyQtSql/EasyQtSql.h"
#include "Logger.h"
#include "global/globalmanager.h"
#include "utils/datadealutils.h"

DataService::DataService(QObject *parent)
    : QObject{parent}
{}

DataService::~DataService() {}

bool DataService::getLatestOutTrade(const QString &vehPlate, QObject *obj, int type)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(
        R"(SELECT * FROM %1 WHERE exvehplate = ? AND extime = (SELECT max(extime) FROM (SELECT max(extime) AS extime FROM t_etc_out WHERE exvehplate = ? UNION SELECT max(extime) AS extime FROM t_mtc_out WHERE exvehplate =?) t))");

    if (type == 0) {
        sql = sql.arg("t_etc_out");
    } else {
        sql = sql.arg("t_mtc_out");
    }

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(vehPlate, vehPlate, vehPlate);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return false;

        res.fetchObject(*obj);
        return true;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return false;
    }
}

QVariantMap DataService::getEnInfo(const QString &passID)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(
        R"(SELECT tradeid, TO_CHAR(entime, 'yyyy-mm-dd HH24:mi:ss') AS entime, cnlaneid, enstation, stationname FROM t_mtc_in WHERE passid = ? UNION ALL SELECT tradeid, TO_CHAR(entime, 'yyyy-mm-dd HH24:mi:ss') AS entime, cnlaneid, enstation, stationname FROM t_etc_in WHERE passid = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(passID, passID);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return {};

        QVariantMap resMap = res.toMap();
        return resMap;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return {};
    }
}

QList<QVariantMap> DataService::getGantryInfos(const QString &passId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(
        R"(SELECT g.gantryid AS gantryid, e.tradetime AS tradetime, e.flagid AS flagid, g.flagname AS flagname, e.errcode FROM t_ff_etcTrade e, t_etcflag g WHERE e.passid = ? AND e.flagid = g.flagid UNION ALL SELECT g.gantryid AS gantryid, e.tradetime AS tradetime, e.flagid AS flagid, g.flagname AS flagname, e.errcode FROM t_ff_cpcrecord e, t_etcflag g WHERE e.passid = ? AND e.flagid = g.flagid ORDER BY tradetime)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(passId, passId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        QList<QVariantMap> records;
        while (res.next()) {
            QVariantMap record = res.toMap();
            records.append(record);
        }

        return records;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return {};
    }
}

QString DataService::getGantryNodeID(const QString &nodeHex)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT nodeid FROM t_cnfeenode WHERE nodetype = 1 AND hexnode = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(nodeHex);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("nodeid").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

QString DataService::getGantryNodeName(const QString &nodeId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT nodename FROM t_cnfeenode WHERE nodetype = 1 AND nodeid = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(nodeId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("nodename").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

QString DataService::getGantryHexNode(const QString &nodeId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT hexnode FROM t_cnfeenode WHERE nodetype = 1 AND nodeid = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(nodeId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("hexnode").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

bool DataService::getSplitOut(const QString &tradeId, QObject *obj)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT * FROM t_split_out WHERE tradeid = ? AND provincenum = 35)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(tradeId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return false;

        res.fetchObject(*obj);
        return true;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return false;
    }
}

QString DataService::getUserID(const QString &cardId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT userid FROM t_user WHERE isvalid = 1 AND idtcardnum = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(cardId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("userid").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

QString DataService::getUserName(const QString &param, int type)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT username FROM t_user WHERE isvalid = 1 AND %1 = ?)");

    if (type == 0) {
        sql = sql.arg("idtcardnum");
    } else {
        sql = sql.arg("userid");
    }

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(param);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("username").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

QString DataService::getStationIP(const QString &stationId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT ipaddr FROM t_station WHERE stationid = ? AND ipaddr NOT IN ('0000', '0.0.0.0'))");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(stationId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("ipaddr").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

QString DataService::getStationName(const QString &nodeId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT nodename FROM t_cnfeenode WHERE hexnode = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(nodeId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("nodename").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

QString DataService::getGrayCardRemark(const QString &cardId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT remark FROM t_graycard  WHERE cardid = ? AND isvalid = 1)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(cardId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("remark").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

QString DataService::getGrayVehicleRemark(const QString &plate)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT remark FROM t_grayvehicle  WHERE vehplate = ? AND isvalid = 1)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(plate);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "";

        QString data = res.value("remark").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

int DataService::getGreenPassBanType(const QString &plate)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(
        R"(SELECT bantype FROM t_greenpassban WHERE vehplate = ? AND isvalid = 1 AND CURRENT_TIMESTAMP BETWEEN starttime AND endtime)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(plate);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return -1;

        int data = res.value("bantype").toInt();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return -1;
    }
}

bool DataService::getGreenPassAppointment(const QString &plate)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT count(*) as count FROM t_appointment WHERE vehicleid = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(plate);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return false;

        int data = res.value("count").toInt();
        return data > 0;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return false;
    }
}

QVariantList DataService::getFreeTempVehicles(const QString &plate)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT * FROM t_freetempvehicle WHERE vehplate = ? AND isvalid = 1)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(plate);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        QVariantList records;
        while (res.next()) {
            QVariantMap record = res.toMap();
            records.append(record);
        }
        return records;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return {};
    }
}

QString DataService::getEmgcSeqNum(const QString &stationId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");
    QString sql(R"(SELECT kitem FROM t_emgcdict WHERE stationid = ? AND laneid = 0 AND ktype = 1)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(stationId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return "0";

        QString data = res.value("kitem").toString();
        return data;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return "";
    }
}

bool DataService::updateEmgcSeqNum(const QString &stationId, const QString &newSeqNum)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::NonQueryResult res = t.update("t_emgcdict")
                                            .set("kitem", newSeqNum)
                                            .where("stationid = ? AND laneid = 0 AND ktype = 1", stationId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        return t.commit();
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return false;
    }
}

bool DataService::insertEmgcSeqNum(const QString &stationId)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase("oracle");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::NonQueryResult res = t.insertInto(
                                             "t_emgcdict (stationid, laneid, ktype, kitem, kvalueint, kvaluestring)")
                                            .values(stationId, 0, 1, "1", 0, "")
                                            .exec();

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        return t.commit();
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return false;
    }
}

QVariantMap DataService::getTicketUseInfo(int laneId, const QString &id)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase(id);

    QString sql(
        R"(SELECT TOP 1 * FROM t_ticketusemanage WHERE laneid = ? AND isused = 1 AND lastnum != stopnum AND remark LIKE '%SPT-POS:30%' ORDER BY operatetime)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(laneId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return {};

        QVariantMap resMap = res.toMap();
        return resMap;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return {};
    }
}

bool DataService::updateTicketUseInfo(int laneId, int newSeqNum, const QString &id)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase(id);

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::NonQueryResult res = t.update("t_ticketusemanage")
                                            .set("lastnum", newSeqNum)
                                            .where("laneid = ? AND isused = 1 AND lastnum != stopnum AND startnum <= ? "
                                                   "AND stopnum >= ? AND remark LIKE '%SPT-POS:30%'",
                                                   laneId,
                                                   newSeqNum,
                                                   newSeqNum);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        return t.commit();
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return false;
    }
}

int DataService::getOutShiftSettleCount(const QString &stationId,
                                        const QString &shiftDate,
                                        int shiftId,
                                        const QString &id)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase(id);
    QString sql(
        R"(SELECT COUNT(*) FROM t_outshiftsettle WHERE shiftdate = ? AND laneid = 249 AND shiftid = ? AND isvalid = 1 AND stationid = ?)");

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::PreparedQuery query = t.prepare(sql);
        EasyQtSql::QueryResult res = query.exec(shiftDate, shiftId, stationId);

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        if (!res.next())
            return 0;

        int count = res.scalar<int>();
        return count;
    } catch (EasyQtSql::DBException &e) {
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return -1;
    }
}

bool DataService::insertOutShiftSettle(const QString &dataId,
                                       const QString &shiftDate,
                                       int shiftId,
                                       const QString &stationId,
                                       const QString &operatorId,
                                       const QString &operatorName,
                                       const QString &id)
{
    QSqlDatabase sdb = GM_INSTANCE->m_dbFactory->getDatabase(id);

    EasyQtSql::Transaction t(sdb);
    try {
        EasyQtSql::NonQueryResult res
            = t.insertInto("t_outshiftsettle (dataid, stationid, laneid, operator, "
                           "operatorname, shiftid, shiftdate, finoperator, isvalid, nextoperator)")
                  .values(dataId, stationId, 249, operatorId, operatorName, shiftId, shiftDate, "0000000", 1, "0000000")
                  .exec();

        LOG_INFO().noquote() << "执行SQL语句: " << Utils::DataDealUtils::fullExecutedQuery(res.unwrappedQuery());

        return t.commit();
    } catch (EasyQtSql::DBException &e) {
        t.rollback();
        LOG_ERROR().noquote() << e.lastError.text() << '\t' << e.lastQuery;
        return false;
    }
}
