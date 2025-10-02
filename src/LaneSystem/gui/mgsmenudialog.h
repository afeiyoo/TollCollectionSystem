#pragma once

#include <QDialog>
#include <QKeyEvent>

class ElaPivot;
class QStackedLayout;
class MgsMenuDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MgsMenuDialog(QWidget *parent = nullptr);
    ~MgsMenuDialog() override;

    void addNewTab(const QString &tabName, const QStringList &funcs, const QList<uint> &enableFuncs);
    void reset();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void initUi();
    void focusTab(int tabIndex);

private:
    QWidget *m_mainWidget = nullptr;
    // 功能导航栏
    ElaPivot *m_pivot = nullptr;
    // 功能堆栈
    QStackedLayout *m_tabLayout = nullptr;
    // 可用功能列表
    QList<QList<uint>> m_enableFuncsPerTab;
    // 标签列表
    QStringList m_tabNames;
};
