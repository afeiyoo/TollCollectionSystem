#pragma once

#include <QObject>

class MgsMenu;

// 抽象模式类
class Mode
{
public:
    explicit Mode() = default;
    virtual ~Mode() = default;

    virtual void setupMenu(MgsMenu *menu) = 0;
};

// 混合入口自动发卡模式
class MtcInAutoMode : public Mode
{
public:
    void setupMenu(MgsMenu *menu) override;
};

// 混合入口手动发卡模式
class MtcInManaualMode : public Mode
{
public:
    void setupMenu(MgsMenu *menu) override;
};

// 混合入口手动发纸券模式
class MtcInPaperMode : public Mode
{
public:
    void setupMenu(MgsMenu *menu) override;
};

// 车道模式管理类
class ModeManager : public QObject
{
    Q_OBJECT
public:
    explicit ModeManager(QObject *parent = nullptr);
    ~ModeManager() override;

    void showMenu();

public slots:
    void onModeChanged(Mode *newMode);

private:
    Mode *m_curMode = nullptr; // 当前模式
    MgsMenu *m_menu = nullptr; // 全局唯一的菜单
};
