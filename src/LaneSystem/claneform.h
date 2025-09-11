#ifndef CLANEFORM_H
#define CLANEFORM_H

/*车道系统主窗体*/

#include <QMainWindow>
#include <QKeyEvent>
#include "cbizenv.h"
#include "ilaneform.h"
// #include "CommLogger.h"


namespace Ui {
class CLaneForm;
}

class CLaneForm : public QMainWindow
{
    Q_OBJECT
private:
    Ui::CLaneForm *ui;
    ILaneForm* m_form;
    // CommLogger* m_logger;
public:
    explicit CLaneForm(QWidget *parent = 0);
    ~CLaneForm();
protected:
    //窗体按键事件
    void keyPressEvent(QKeyEvent *event) override;
public:
    //显示入口信息
    void showEnInfo(QString jsonInfo);
    //显示出口信息
    void showExInfo(QString jsonInfo);
    //显示特情信息
    void showSpecialInfo(QString jsonInfo);
    //显示出口分省信息
    void showSplitInfo(QString splitInfo);
    //显示称重信息
    void showWeightInfo(QString jsonInfo);
    //显示卡机(自助缴费机、自助终端）设备信息（包括工位状态/卡数量等）
    void showRobotInfo(QString jsonInfo);
    //显示交易日志
    void showTradeLog(QString sLog);
    //显示历史交易
    void showHisTrade(QString jsonInfo);
public slots:
    void onTradeInfo(int infoType,QString infoData);
    void onDeviceStatus(int nStatus);
    void onCapImage(QByteArray imgBuf);
};

#endif // CLANEFORM_H
