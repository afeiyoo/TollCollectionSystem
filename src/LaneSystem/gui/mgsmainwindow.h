#pragma once

#include "ElaWindow.h"

class MgsAuthDialog;
class MgsBasePage;
class MgsMainWindow : public ElaWindow
{
    Q_OBJECT
public:
    explicit MgsMainWindow(QWidget *parent = nullptr);
    ~MgsMainWindow() override;

    void initMtcIn();
    void initMtcOut();
    void initEtc();

public slots:
    // 错误弹窗提示
    void onShowFormErrorHint(const QString &title, const QStringList &strs);
    // 提示弹窗提示
    void onShowFormInfoHint(const QString &title, const QStringList &strs);
    // 选择弹窗提示
    void onShowFormQuestionHint(const QString &title, const QStringList &strs);
    // 报警弹窗提示
    void onShowFormWarningHint(const QString &title, const QStringList &strs);
    // 登录窗口显示
    void onShowFormLogin();

protected:
    void showEvent(QShowEvent *event) override;

private:
    // 主页面
    MgsBasePage *m_mainPage = nullptr;
    // 登录窗口
    MgsAuthDialog *m_authDialog = nullptr;
};
