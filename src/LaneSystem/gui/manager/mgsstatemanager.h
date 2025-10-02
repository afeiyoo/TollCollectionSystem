#pragma once

#include <QObject>

class MgsMenuDialog;

//==============================================================================
// 抽象状态类
//==============================================================================
class State
{
public:
    explicit State() = default;
    virtual ~State() = default;

    virtual void setupMenu(MgsMenuDialog *menu) = 0;
};

//==============================================================================
// 具体状态：混合入口自动发卡模式，混合入口手动发卡模式，混合入口手动发纸券模式
//==============================================================================
class MtcInAutoMode : public State
{
public:
    void setupMenu(MgsMenuDialog *menu) override;
};

class MtcInManaualMode : public State
{
public:
    void setupMenu(MgsMenuDialog *menu) override;
};

class MtcInPaperMode : public State
{
public:
    void setupMenu(MgsMenuDialog *menu) override;
};

//==============================================================================
// 状态管理类
//==============================================================================
class MgsStateManager : public QObject
{
    Q_OBJECT
public:
    explicit MgsStateManager(QWidget *parent = nullptr);
    ~MgsStateManager() override;

    void showMenu();

public slots:
    void onStateChanged(State *newState);

private:
    State *m_curState = nullptr;     // 当前模式
    MgsMenuDialog *m_menuDialog = nullptr; // 全局唯一的菜单
};
