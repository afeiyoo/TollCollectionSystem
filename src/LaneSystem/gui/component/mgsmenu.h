#pragma once

#include <QKeyEvent>
#include <QWidget>

class ElaPivot;
class QStackedLayout;
class MgsMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MgsMenu(QWidget *parent = nullptr);
    ~MgsMenu() override;

    void initUi();

    void addNewTab(const QString &tabName, const QStringList &funcs, const QList<uint> &enableFuncs);
    void reset();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void focusTab(int tabIndex);

private:
    // 功能导航栏
    ElaPivot *m_pivot = nullptr;
    // 功能堆栈
    QStackedLayout *m_tabLayout = nullptr;
    // 可用功能列表
    QList<QList<uint>> m_enableFuncsPerTab;
    // 标签列表
    QStringList m_tabNames;
};
