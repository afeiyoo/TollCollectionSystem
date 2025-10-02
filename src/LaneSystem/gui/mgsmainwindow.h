#pragma once

#include "ElaWidgetTools/ElaWindow.h"
#include "utils/defs.h"

class MgsOptionsDialog;
class MgsAuthDialog;
class MgsBasePage;
class MgsStateManager;
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
    // 选项窗口显示
    void onShowFormOptions(uint dlgID, const QString &title, const QStringList &options);
    // 功能菜单窗口显示
    void onShowFormMenu();
    // 日志显示区刷新
    void onShowLogAppend(EM_LogLevel::LogLevel logLevel, const QString &log);

protected:
    void showEvent(QShowEvent *event) override;

private:
    // 主页面
    MgsBasePage *m_mainPage = nullptr;
    // 登录窗口
    MgsAuthDialog *m_authDialog = nullptr;
    // 选项对话框
    MgsOptionsDialog *m_optionsDialog = nullptr;
    // 状态控件管理对象
    MgsStateManager *m_stateMan = nullptr;
};
