#include "mgsdevicepanel.h"

#include "ElaWidgetTools/ElaIconButton.h"

#include <QDebug>
#include <QHBoxLayout>

MgsDevicePanel::MgsDevicePanel(QWidget *parent)
    : MgsPageArea{parent}
{
    initUi();
}

MgsDevicePanel::~MgsDevicePanel() {}

void MgsDevicePanel::setDeviceList(const QList<uint> &devices)
{
    // 清除之前的图标（保留首尾的两个 Stretch）
    for (auto btn : m_devMap.values()) {
        delete btn;
    }
    m_devMap.clear();
    m_devices = devices;

    while (m_iconLayout->count() > 2) {
        QLayoutItem *item = m_iconLayout->takeAt(1);
        if (QWidget *w = item->widget())
            delete w;
        delete item;
    }

    for (uint dev : m_devices) {
        // 设备初始状态为 1
        QString path = iconPath(dev, 1);
        ElaIconButton *btn = new ElaIconButton(QPixmap(path), this);
        btn->setFixedSize(50, 50);
        btn->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        // 保存映射
        m_devMap.insert(dev, btn);
        // 插入布局倒数第二位
        m_iconLayout->insertWidget(m_iconLayout->count() - 1, btn);
    }
}

void MgsDevicePanel::onDevStatusChange(uint dev, uint status)
{
    if (auto btn = m_devMap.value(dev, nullptr)) {
        QString path = iconPath(dev, status);
        QPixmap pix(path);
        btn->setPixmap(pix);
        btn->update();
    }
}

void MgsDevicePanel::initUi()
{
    setBackgroundColor(Qt::transparent);

    m_iconLayout = new QHBoxLayout();
    m_iconLayout->setSpacing(5);
    m_iconLayout->addStretch();
    m_iconLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 5, 0);
    mainLayout->addStretch();
    mainLayout->addLayout(m_iconLayout);
    mainLayout->addStretch();
}

// 根据 dev 和 status 返回不同图标路径
QString MgsDevicePanel::iconPath(uint dev, uint status) const
{
    QString path;
    switch (static_cast<DeviceType>(dev)) {
    case Capture: {
        if (status == 1 || status == 2) {
            path = QString(":/static/images/cap_%1.png").arg(status);
        }
    } break;
    case CellingLamp: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/celling_lamp_%1.png").arg(status);
        }
    } break;
    case FirstCoil: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/first_coil_%1.png").arg(status);
        }
    } break;
    case SecondCoil: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/second_coil_%1.png").arg(status);
        }
    } break;
    case ThirdCoil: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/third_coil_%1.png").arg(status);
        }
    } break;
    case FourthCoil: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/fourth_coil_%1.png").arg(status);
        }
    } break;
    case CapCoil: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/capcoil_%1.png").arg(status);
        }
    } break;
    case RailingCoil: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/railcoil_%1.png").arg(status);
        }
    } break;
    case PassingLamp: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/passing_lamp_%1.png").arg(status);
        }
    } break;
    case RailingMachine: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/railing_machine_%1.png").arg(status);
        }
    } break;
    case RSU: {
        if (status == 1 || status == 2 || status == 3) {
            path = QString(":/static/images/rsu_%1.png").arg(status);
        }
    } break;
    case Weight: {
        if (status == 1 || status == 2) {
            path = QString(":/static/images/weight_%1.png").arg(status);
        }
    } break;
    default:
        return QString();
    }

    return path;
}
