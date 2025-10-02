#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class HttpReply;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void postToPrinter(bool checked);

private:
    QString blockUtilResponse(HttpReply *reply, qint32 timeoutMs);

private:
    Ui::Widget *ui;
};
