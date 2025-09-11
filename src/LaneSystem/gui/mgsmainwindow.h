#pragma once

#include "ElaWindow.h"

class MgsBasePage;
class ElaToolButton;
class ElaStatusBar;
class MgsMainWindow : public ElaWindow
{
    Q_OBJECT
public:
    explicit MgsMainWindow(QWidget *parent = nullptr);
    ~MgsMainWindow() override;

    void initMtcIn();
    void initMtcOut();
    void initEtc();

private:
    void initUi();

private:
    // 状态栏
    ElaStatusBar *m_statusBar = nullptr;
    // 主页面
    MgsBasePage *m_mainPage = nullptr;
};
