#pragma once

#include <QDateTime>
#include <QObject>

class BizEnv : public QObject
{
    Q_OBJECT
public:
    explicit BizEnv(QObject *parent = nullptr);
    ~BizEnv() override;

public:
    // 收费员名称
    QString m_operateName;
    // 收费员工号
    uint m_operatorID;
    // 当前工班日期
    QDateTime m_shiftDate;
    // 当前班次号
    uint m_shiftID;
};
