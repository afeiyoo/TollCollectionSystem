#pragma once

#include <QDateTime>
#include <QObject>

class MIBizEnv : public QObject
{
    Q_OBJECT
public:
    explicit MIBizEnv(QObject *parent = nullptr);
    ~MIBizEnv() override;

public:
    // 是否已上班
    bool m_isShifted = false;
    // 收费员名称
    QString m_operateName;
    // 收费员工号
    uint m_operatorID;
    // 当前工班日期
    QDateTime m_shiftDate;
    // 当前班次号
    uint m_shiftID;
    // 当前车道启用与否
    bool m_isOpen = false;
};
