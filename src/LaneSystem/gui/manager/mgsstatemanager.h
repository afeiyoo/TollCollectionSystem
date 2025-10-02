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

    // 是否称重降级
    void setWeightLow(bool weightLow);

protected:
    bool m_weightLow = false; // 默认非称重降级
};

// 自定义信号参数
Q_DECLARE_METATYPE(State *);

//==============================================================================
// 具体状态：混合入口自动发卡模式，混合入口人工发卡模式，混合入口人工发纸券模式
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
    void onChangeState(State *newState);

private:
    State *m_curState = nullptr;           // 当前模式
    MgsMenuDialog *m_menuDialog = nullptr; // 全局唯一的菜单
};
