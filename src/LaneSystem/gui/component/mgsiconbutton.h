#pragma once

#include <QColor>
#include <QWidget>

#include "ElaIconButton.h"
#include "ElaText.h"

class MgsIconButton : public QWidget
{
    Q_OBJECT
public:
    explicit MgsIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget *parent = nullptr);
    explicit MgsIconButton(ElaIconType::IconName awesome, QString text, int pixelSize, QWidget *parent = nullptr);

    void setText(const QString &text);
    void setColor(const QString &color);

private:
    ElaIconButton *m_icon = nullptr;
    ElaText *m_text = nullptr;
};
