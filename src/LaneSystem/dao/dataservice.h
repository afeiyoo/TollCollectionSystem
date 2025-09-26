#pragma once

#include <QObject>

class DataService : public QObject
{
    Q_OBJECT
public:
    explicit DataService(QObject *parent = nullptr);
    ~DataService() override;

    // 入口工班统计
    virtual void inStat() = 0;

    // 出口工班统计
    virtual void outStat() = 0;
};

