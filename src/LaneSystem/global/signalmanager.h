#pragma once

#include <QObject>

class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);
    ~SignalManager() override;

signals:
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

    //按键
    void sigKeyPress(uint key);
    // 界面日志区刷新
    void sigLogAppend(const QString &log);

    // 选项对话框请求
    void sigOptionSelected(int dlgID, uint option);
};
