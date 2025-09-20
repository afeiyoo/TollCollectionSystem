#pragma once

#include <QObject>

class MgsMainWindow;
class MIBizEnv;
class MIDeskProcess : public QObject
{
    Q_OBJECT
public:
    explicit MIDeskProcess(MgsMainWindow *mainWindow, QObject *parent = 0);
    ~MIDeskProcess() override;

public slots:
    void onVDChanged(unsigned int newVD);
    // 菜单功能请求
    void onMenuRequest(uint index);
    // 闭道
    void onCloseLane();
    // 开道
    void onOpenLane();
    // 上班
    void onShiftIn();
    // 下班
    void onShiftOut();

private:
    MgsMainWindow *m_mainWindow = nullptr;
    MIBizEnv *m_bizEnv = nullptr;
};
