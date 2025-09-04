#ifndef WIDGET_H
#define WIDGET_H

#include <QTextBrowser>
#include <QWidget>

namespace Ui {
class Widget;
} // namespace Ui

class SmartLaneController;
class RelayController;
class QButtonGroup;
class QRadioButton;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_resetButton_clicked();

    void on_sendButton_clicked();

    void onLogShow(const QString &log);

    void on_connectButton_clicked();

private:
    void initUi();
    void initIOPage();

private:
    Ui::Widget *ui;
    QRadioButton *m_radioBtnOn = nullptr;
    QRadioButton *m_radioBtnOff = nullptr;
    QButtonGroup *m_switchGroup = nullptr;
    QButtonGroup *m_controlGroup = nullptr;
    QButtonGroup *m_levelGroup = nullptr;
    QButtonGroup *m_addOffGroup = nullptr;
    QTextBrowser *m_logBrowser = nullptr;

    RelayController *m_relayController = nullptr;

    // 智能车控器
    SmartLaneController *m_smartLaneController = nullptr;
    bool m_connected = false;
};

#endif // WIDGET_H
