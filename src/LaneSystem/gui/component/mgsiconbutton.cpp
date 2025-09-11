#include "MgsIconButton.h"

#include <QDebug>
#include <QHBoxLayout>

MgsIconButton::MgsIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget *parent)
    : QWidget(parent)
{
    m_text = new ElaText(this);
    m_text->setTextPixelSize(pixelSize);
    m_text->setContentsMargins(0, 0, 0, 0);
    m_text->setIsWrapAnywhere(false);

    m_icon = new ElaIconButton(awesome, pixelSize, pixelSize, pixelSize, this);
    m_icon->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->addWidget(m_icon);
    layout->addWidget(m_text);
    layout->addStretch();
}

MgsIconButton::MgsIconButton(ElaIconType::IconName awesome, QString text, int pixelSize, QWidget *parent)
    : MgsIconButton(awesome, pixelSize, parent)
{
    m_text->setText(text);
}

void MgsIconButton::setText(const QString &text)
{
    m_text->setText(text);
}

void MgsIconButton::setColor(const QString &color)
{
    m_icon->setLightIconColor(color);

    QString style = QString("color: %1;").arg(color);
    m_text->setStyleSheet(style);
}
