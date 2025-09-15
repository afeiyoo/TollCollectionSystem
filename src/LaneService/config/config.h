#pragma once

#include "global/defs.h"
#include "utils/fileutils.h"
#include "utils/iniutils.h"

#include <QObject>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);

    ~Config() override;

    void loadConfig(const Utils::FileName &configPath);

    QString dumpConfig();

public:
    ST_DatabaseConfig m_dbConfig;
    ST_RpcServerConfig m_serverConfig;
    ST_LogConfig m_logConfig;

private:
    Utils::IniUtils *m_ini = nullptr;
};
