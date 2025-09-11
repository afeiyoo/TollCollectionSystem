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
};
