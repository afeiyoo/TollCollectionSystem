#pragma once

#include "laneservice_global.h"

#include <QObject>

class LANESERVICE_EXPORT LaneService : public QObject
{
    Q_OBJECT
public:
    LaneService(QObject *parent = nullptr);
    ~LaneService() override;

    // 服务初始化
    void init();

    // 命令01：通用数据库操作
    Q_INVOKABLE QString dbCreate(const QString &reqJson);
    Q_INVOKABLE QString dbUpdate(const QString &reqJson);
    Q_INVOKABLE QString dbRead(const QString &reqJson);
    Q_INVOKABLE QString dbDelete(const QString &reqJson);
    Q_INVOKABLE QString dbInsert(const QString &reqJson);

private:
    QVariantMap genResMapForDBOperate(int status, int recordCount, QString desc, QVariantList records);
};
