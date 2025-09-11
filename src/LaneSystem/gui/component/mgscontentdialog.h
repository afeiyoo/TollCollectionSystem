#pragma once

#include <QWidget>

#include "ElaContentDialog.h"

class MgsContentDialog : public ElaContentDialog
{
    Q_OBJECT
public:
    explicit MgsContentDialog(QWidget *parent = nullptr);
    void setContent(const QString &titleText, const QString &content);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void removeMiddleButton();
};
