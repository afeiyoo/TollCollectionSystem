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
    ConfigDef::ST_LogCfg m_logConfig;
    ConfigDef::ST_DatabaseCfg m_dbConfig;
    ConfigDef::ST_RpcServerCfg m_serverConfig;
};
