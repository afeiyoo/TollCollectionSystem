#pragma once

#include "dataservice.h"

class MIDataService : public DataService
{
    Q_OBJECT
public:
    explicit MIDataService(QObject *parent = nullptr);
    ~MIDataService() override;

    // 入口工班统计
    void inStat() override;

    // 出口工班统计
    void outStat() override;
};
