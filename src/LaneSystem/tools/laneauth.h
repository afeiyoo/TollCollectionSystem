#pragma once

#include "utils/sm4.h"
#include <QObject>

class LaneAuth : public QObject
{
    Q_OBJECT
public:
    explicit LaneAuth(QObject *parent = nullptr);

    // 从授权文件中获取授权信息
    bool getAuthInfo(const QString &fileName,
                     QString &stationID,
                     QString &laneID,
                     QString &laneIP,
                     QString &licenseCode,
                     QString &cnLaneID);

private:
    // 授权检查
    bool authCheck(QString stationID, int laneID, QString laneIP, QString cnLaneID);

    // 计算授权码
    QString computeID2License(QString cID, QString orgID);

private:
    Utils::sm4_context m_ctx;
};
