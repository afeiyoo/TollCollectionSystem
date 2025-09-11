#pragma once

#include "gui/component/mgspagearea.h"

class ElaIconButton;
class QHBoxLayout;
class MgsDevicePanel : public MgsPageArea
{
    Q_OBJECT
public:
    enum DeviceType {
        Capture = 1,
        CellingLamp,
        FirstCoil,
        SecondCoil,
        ThirdCoil,
        FourthCoil,
        CapCoil,
        RailingCoil,
        PassingLamp,
        RailingMachine,
        RSU,
        Weight
    };

    explicit MgsDevicePanel(QWidget *parent = nullptr);
    ~MgsDevicePanel() override;

    void setDeviceList(const QList<uint> &devices);

public slots:
    void onDevStatusChange(uint dev, uint status);

private:
    void initUi();
    QString iconPath(uint dev, uint status) const;

private:
    QList<uint> m_devices;
    QHBoxLayout *m_iconLayout = nullptr;
    // 设备与图标的映射关系
    QHash<uint, ElaIconButton *> m_devMap;
};
