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

    // 错误弹窗提示
    void showFormErrorHint(const QString &title, const QStringList &strs);
    // 提示弹窗提示
    void showFormInfoHint(const QString &title, const QStringList &strs);
    // 选择弹窗提示
    void showFormQuestionHint(const QString &title, const QStringList &strs);
    // 报警弹窗提示
    void showFormWarningHint(const QString &title, const QStringList &strs);

protected:
    void showEvent(QShowEvent *event) override;

private:
    // 主页面
    MgsBasePage *m_mainPage = nullptr;
};
