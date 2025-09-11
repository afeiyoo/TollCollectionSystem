#include "mgscontentdialog.h"

#include <QDebug>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "ElaPushButton.h"
#include "ElaText.h"

MgsContentDialog::MgsContentDialog(QWidget *parent)
    : ElaContentDialog{parent}
{
    setLeftButtonText("取消");
    setRightButtonText("确定");
    // 删除中间按钮
    removeMiddleButton();
}

void MgsContentDialog::setContent(const QString &titleText, const QString &content)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(15, 25, 15, 10);
    ElaText *title = new ElaText(titleText, this);
    title->setTextStyle(ElaTextType::Subtitle);
    ElaText *subTitle = new ElaText(content, this);
    subTitle->setTextStyle(ElaTextType::Body);
    layout->addWidget(title);
    layout->addSpacing(2);
    layout->addWidget(subTitle);
    layout->addStretch();

    setCentralWidget(centralWidget);
}

void MgsContentDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    qDebug() << "Key code:" << key << " KeyName:" << QKeySequence(key).toString();

    if (key == Qt::Key_S) {
        emit rightButtonClicked();
        close();
        event->accept();
    } else if (key == Qt::Key_X) {
        emit leftButtonClicked();
        close();
        event->accept();
    } else {
        qDebug() << "无效按键";
        ElaContentDialog::keyPressEvent(event); // 默认处理其他键
    }
}

void MgsContentDialog::removeMiddleButton()
{
    // 先找到中间按钮
    ElaPushButton *middleBtn = nullptr;
    auto btns = this->findChildren<ElaPushButton *>();
    if (btns.size() >= 1) {
        middleBtn = btns.at(1);
    }
    if (!middleBtn)
        return;

    // 找到它所在父布局
    QWidget *parentW = middleBtn->parentWidget();
    QLayout *layBase = parentW->layout();
    QHBoxLayout *hlay = qobject_cast<QHBoxLayout *>(layBase);
    if (!hlay)
        return;

    // 找出 middleBtn 在布局中的索引
    int count = hlay->count();
    int idx = -1;
    for (int i = 0; i < count; ++i) {
        QLayoutItem *item = hlay->itemAt(i);
        if (item->widget() == middleBtn) {
            idx = i;
            break;
        }
    }
    if (idx < 0)
        return;

    // 从布局移除并隐藏/删除按钮
    hlay->removeWidget(middleBtn);
    middleBtn->hide();
    middleBtn->deleteLater();

    hlay->insertStretch(0, 1);
}
