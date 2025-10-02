#pragma once

#include <QObject>

class MIBizEnv;
class MgsMainWindow;
class MIDeskProcess : public QObject
{
    Q_OBJECT
public:
    explicit MIDeskProcess(MgsMainWindow *mainWindow, QObject *parent = 0);
    ~MIDeskProcess() override;

public slots:
    // 闭道
    void onCloseLane();
    // 开道
    void onOpenLane();
    // 上班
    void onShiftIn();
    // 下班
    void onShiftOut();
    // 菜单功能执行
    void onMenuFuncDeal(uint tabIndex, uint funcIndex);

    void onUpdateCheckingFinished(const QString &url);

private:
    void dealWeightLow();

private:
    MIBizEnv *m_bizEnv = nullptr;
    QThread *m_thread = nullptr;
    MgsMainWindow *m_mainWindow = nullptr;
};
