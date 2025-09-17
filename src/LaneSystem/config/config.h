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
    ST_BusinessCfg m_businessConfig;
    ST_SystemCfg m_systemConfig;
    QVariantMap m_keyboard;
};
