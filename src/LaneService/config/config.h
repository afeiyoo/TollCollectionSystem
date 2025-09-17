#pragma once

#include "utils/defs.h"

#include <QObject>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);

    ~Config() override;

    bool loadConfig();

public:
    ST_DatabaseCfg m_dbConfig;
    ST_RpcServerCfg m_serverConfig;
    ST_LogCfg m_logConfig;
};
