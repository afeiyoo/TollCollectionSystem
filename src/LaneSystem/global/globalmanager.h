#pragma once

#include <QObject>

#define GM_INSTANCE GlobalManager::instance()

class QSimpleUpdater;
class SignalManager;
class ModeManager;
class GlobalManager : public QObject
{
    Q_OBJECT
public:
    explicit GlobalManager(QObject *parent = nullptr);
    ~GlobalManager() override;

    static GlobalManager *instance();

    void init();

public:
    SignalManager *m_signalMan = nullptr;
    ModeManager *m_modeMan = nullptr;
    QSimpleUpdater *m_updater = nullptr;
};
