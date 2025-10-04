#pragma once

#include "ElaWidgetTools/ElaWindow.h"
#include "utils/defs.h"

#include <QMessageBox>

class MgsOptionsDialog;
class MgsAuthDialog;
class MgsBasePage;
class StateManager;
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
    QMessageBox::StandardButton onShowFormErrorHint(const QString &title, const QStringList &strs);
    // 提示弹窗提示
    QMessageBox::StandardButton onShowFormInfoHint(const QString &title, const QStringList &strs);
    // 选择弹窗提示
    QMessageBox::StandardButton onShowFormQuestionHint(const QString &title, const QStringList &strs);
    // 报警弹窗提示
    QMessageBox::StandardButton onShowFormWarningHint(const QString &title, const QStringList &strs);
    // 登录窗口显示
    void onShowFormLogin();
    // 选项窗口显示
    void onShowFormOptions(uint dlgID, const QString &title, const QStringList &options);
    // 功能菜单窗口显示
    void onShowFormMenu();
    // 日志显示区刷新
    void onShowUpdateLog(EM_LogLevel::LogLevel logLevel, const QString &log);
    // 称重降级
    void onShowUpdateWeightLow(bool isLow);
    // 界面提示信息刷新
    void onShowUpdateTradeHint(const QString &hint, bool isWarning = false);

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
    StateManager *m_stateMan = nullptr;
};
