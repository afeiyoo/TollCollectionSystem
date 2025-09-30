#pragma once

#include "utils/defs.h"
#include <QObject>

class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);
    ~SignalManager() override;

signals:
    //==============================================================================
    // 前端发往后端的信号
    //==============================================================================
    // 车牌修改
    void sigPlateChanged(const QString &plate);
    // 功能不可用
    void sigFuncUnavaliable(const QString &func);
    // 上班
    void sigShiftIn();
    // 下班
    void sigShiftOut();
    // 菜单功能请求
    void sigMenuRequest(uint index);
    // 闭道
    void sigCloseLane();
    // 开道
    void sigOpenLane();
    // 选项对话框请求
    void sigOptionSelected(int dlgID, uint option);

    //按键
    void sigKeyPress(uint key);

    //==============================================================================
    // 后端发往前端的信号
    //==============================================================================
    // 错误弹窗提示
    void sigShowFormErrorHint(const QString &title, const QStringList &strs);
    // 提示弹窗提示
    void sigShowFormInfoHint(const QString &title, const QStringList &strs);
    // 选择弹窗提示
    void sigShowFormQuestionHint(const QString &title, const QStringList &strs);
    // 报警弹窗提示
    void sigShowFormWarningHint(const QString &title, const QStringList &strs);
    // 登录窗口显示
    void sigShowFormLogin();
    // 界面日志区刷新
    void sigLogAppend(EM_LogLevel::LogLevel logLevel, const QString &log);
};
