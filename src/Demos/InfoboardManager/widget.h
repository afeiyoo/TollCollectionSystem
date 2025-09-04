#pragma once

#include "CHXInfoBoard.h"

#include "fileutils.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setItems(const Utils::FileNameList &fnList);

private slots:
    void on_btnConnect_clicked();

    void onConnectStatusChange(bool status);

    void on_btnSend_clicked();

    void onShowLog(const QString &log);

private:
    Ui::Widget *ui;

    std::unique_ptr<CHXInfoBoard> m_infoBoard;
};
