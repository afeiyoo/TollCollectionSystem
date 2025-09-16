#pragma once

#include "global/defs.h"
#include <QObject>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    ~Config() override;

    bool loadConfig();

public:
    ST_BusinessConfig m_businessConfig;
    ST_SystemConfig m_systemConfig;
    QVariantMap m_keyboard;
};
