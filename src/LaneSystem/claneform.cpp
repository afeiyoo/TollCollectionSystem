#include "claneform.h"
#include "ui_claneform.h"

CLaneForm::CLaneForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CLaneForm)
{
    ui->setupUi(this);

    //日志对象
    // QString logFile = QCoreApplication::applicationDirPath()+"/log/main.log";
    // m_logger = new CommLogger(logFile.toStdString().c_str());

    //窗体显示信号
    m_form = new ILaneForm(this);
    connect(m_form,SIGNAL(sigTradeInfo(int,QString)),this,SLOT(onTradeInfo(int,QString)));
    connect(m_form,SIGNAL(sigCapImage(QByteArray)),this,SLOT(onCapImage(QByteArray)));
    connect(m_form,SIGNAL(sigDeviceStatus(int)),this,SLOT(onDeviceStatus(int)));
    m_form->initBaseForm();

}

CLaneForm::~CLaneForm()
{
    delete ui;
}

//窗体按键事件
void CLaneForm::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        // 处理Esc键的按下事件
        close(); // 例如，关闭窗口
    } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        // 处理Enter键的按下事件

    }

    //传桌面类传递按键事件
    emit m_form->sigKeyPress(event->key());

    // 确保调用基类的keyPressEvent以确保事件不被忽略
    QWidget::keyPressEvent(event);
}


void CLaneForm::onTradeInfo(int infoType,QString infoData)
{
    switch(infoType){
    case IT_EN:
        showEnInfo(infoData);
        break;
    case IT_EX:
        showExInfo(infoData);
        break;
    case IT_HISTRADE:
        showHisTrade(infoData);
        break;
    case IT_ROBOT:
        showRobotInfo(infoData);
        break;
    case IT_SPECIAL:
        showSpecialInfo(infoData);
        break;
    case IT_SPLIT:
        showSplitInfo(infoData);
        break;
    case IT_TLOG:
        showTradeLog(infoData);
        break;
    case IT_WEIGHT:
        showWeightInfo(infoData);
        break;
    }
}

//显示设备状态
void CLaneForm::onDeviceStatus(int nStatus)
{

}

//显示抓拍图片
void CLaneForm::onCapImage(QByteArray imgBuf)
{

}

//显示入口信息
void CLaneForm::showEnInfo(QString jsonInfo)
{

}

//显示出口信息
void CLaneForm::showExInfo(QString jsonInfo)
{

}

//显示特情信息
void CLaneForm::showSpecialInfo(QString jsonInfo)
{

}

//显示出口分省信息
void CLaneForm::showSplitInfo(QString splitInfo)
{

}

//显示称重信息
void CLaneForm::showWeightInfo(QString jsonInfo)
{

}

//显示卡机(自助缴费机、自助终端）设备信息（包括工位状态/卡数量等）
void CLaneForm::showRobotInfo(QString jsonInfo)
{

}

//显示交易日志
void CLaneForm::showTradeLog(QString sLog)
{
   ui->teLog->insertPlainText(sLog);
}

//显示历史交易
void CLaneForm::showHisTrade(QString jsonInfo)
{

}
