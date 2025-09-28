#include "mgspagearea.h"

#include <QPainter>

MgsPageArea::MgsPageArea(QWidget *parent)
    : QWidget(parent)
{
    // 允许透明绘制
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    m_borderRadius = 6;
    m_backgroundColor = QColor(245, 245, 245);
}

MgsPageArea::~MgsPageArea() {}

void MgsPageArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect(1, 1, width() - 2, height() - 2);
    painter.setPen(QColor("#e5e5e5"));
    // 设置背景刷（根据透明度判断）
    if (m_backgroundColor.alpha() > 0) {
        painter.setBrush(m_backgroundColor);
    } else {
        painter.setBrush(Qt::NoBrush);
    }
    painter.drawRoundedRect(rect, m_borderRadius, m_borderRadius);
    painter.restore();
}

QColor MgsPageArea::getBackgroundColor() const
{
    return m_backgroundColor;
}

void MgsPageArea::setBackgroundColor(const QColor &newBackgroundColor)
{
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_backgroundColor = newBackgroundColor;
    emit backgroundColorChanged();
}

int MgsPageArea::getBorderRadius() const
{
    return m_borderRadius;
}

void MgsPageArea::setBorderRadius(int newBorderRadius)
{
    if (m_borderRadius == newBorderRadius)
        return;
    m_borderRadius = newBorderRadius;
    emit borderRadiusChanged();
}
