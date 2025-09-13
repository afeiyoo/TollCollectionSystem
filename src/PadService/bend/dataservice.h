#pragma once

#include <QObject>

class DataService : public QObject
{
    Q_OBJECT
public:
    explicit DataService(QObject *parent = nullptr);
    ~DataService() override;

    // 获取某辆车最近一次出口通行记录，返回该记录 type=0，etc出口，type=1，mtc出口
    bool getLatestOutTrade(const QString &vehPlate, QObject *obj, int type);

    // 根据 passId 获取车辆的入口交易信息，并返回
    QVariantMap getEnInfo(const QString &passID);

    // 获取门架节点信息
    QList<QVariantMap> getGantryInfos(const QString &passId);

    // 获取门架信息-节点编号
    QString getGantryNodeID(const QString &nodeHex);

    // 获取门架信息-节点名称
    QString getGantryNodeName(const QString &nodeId);

    // 获取门架信息-十六进制编码
    QString getGantryHexNode(const QString &nodeId);

    // 获取分省信息
    bool getSplitOut(const QString &tradeId, QObject *obj);

    // 获取收费员工号
    QString getUserID(const QString &cardId);

    // 获取收费员名字 type=0,传入cardID type=1,传入userID
    QString getUserName(const QString &param, int type);

    // 获取收费站IP地址
    QString getStationIP(const QString &stationId);

    // 获取收费站名称
    QString getStationName(const QString &nodeId);

    // 获取灰名单卡信息
    QString getGrayCardRemark(const QString &cardId);

    // 获取灰名单车信息
    QString getGrayVehicleRemark(const QString &plate);

    // 获取绿通禁止免费车辆的禁止类型
    int getGreenPassBanType(const QString &plate);

    // 获取绿通车辆是否预约情况
    bool getGreenPassAppointment(const QString &plate);

    // 依据车牌获取临时免征车信息
    QVariantList getFreeTempVehicles(const QString &plate);

    // 获取手持机序列号
    QString getEmgcSeqNum(const QString &stationId);

    // 更新手持机序列号
    bool updateEmgcSeqNum(const QString &stationId, const QString &lastSeqNum);

    // 插入手持机序列号
    bool insertEmgcSeqNum(const QString &stationId);

    // 获取票据段信息
    QVariantMap getTicketUseInfo(int laneId, const QString &ticketNum, const QString &id);

    // 更新票据段信息
    bool updateTicketUseInfo(int dataId, int laneId, int newSeqNum, const QString &id);

    // 获取相关班次数据记录条数
    int getOutShiftSettleCount(const QString &stationId, const QString &shiftDate, int shiftId, const QString &id);

    // 插入相关班次数据
    bool insertOutShiftSettle(const QString &dataId,
                              const QString &shiftDate,
                              int shiftId,
                              const QString &stationId,
                              const QString &operatorId,
                              const QString &operatorName,
                              const QString &id);
};
