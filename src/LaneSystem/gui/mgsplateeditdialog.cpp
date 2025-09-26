#include "mgsplateeditdialog.h"

#include "ElaWidgetTools/ElaListView.h"
#include "ElaWidgetTools/ElaText.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "gui/component/mgsbasedialog.h"
#include "gui/component/mgsplateedit.h"
#include "gui/delegate/mgscolordelegate.h"
#include "gui/delegate/mgskeydelegate.h"
#include "uiutils.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMap>
#include <QVBoxLayout>

MgsPlateEditDialog::MgsPlateEditDialog(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("车牌修改");
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(620, 400);
    setWindowIcon(QIcon(Constant::Path::APP_ICON));
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);

    Utils::UiUtils::moveToCenter(this);

    initUi();

    // 窗口关闭对话框
    // m_contentDialog = new MgsContentDialog(this);
    // connect(m_contentDialog, &MgsContentDialog::rightButtonClicked, this, &MgsPlateEditDialog::close);
    // connect(m_contentDialog, &MgsContentDialog::rightButtonClicked, this, [this]() {
    //     emit GM_INSTANCE->m_signalMan->sigPlateChanged(m_plateEdit->getPlate());
    // });

    // 跟踪当前光标位置
    connect(m_tab0->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [&](const QModelIndex &cur, const QModelIndex &) { m_curSelected = cur; });
    connect(m_tab1->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [&](const QModelIndex &cur, const QModelIndex &) { m_curSelected = cur; });
    connect(m_tab2->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [&](const QModelIndex &cur, const QModelIndex &) { m_curSelected = cur; });
    connect(m_tab3->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [&](const QModelIndex &cur, const QModelIndex &) { m_curSelected = cur; });
}

MgsPlateEditDialog::~MgsPlateEditDialog() {}

void MgsPlateEditDialog::setPlate(const QString &plate)
{
    m_plateEdit->setPlate(plate);
}

void MgsPlateEditDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    qDebug() << "KeyName:" << QKeySequence(key).toString();

    if (key == Qt::Key_Left) {
        if (m_isFocusEdit) {
            int nextIndex = m_plateEdit->getCurrentIndex() - 1;
            m_plateEdit->moveCursorTo(nextIndex);
        } else {
            int curRow = m_curSelected.row();

            if (m_curSelected.model() == m_tab0->model()) {
                int nextRow = getNextRowForTab1(curRow, key);
                if (nextRow >= 0)
                    m_tab0->setCurrentIndex(m_tab0->model()->index(nextRow, 0));
            } else if (m_curSelected.model() == m_tab1->model()) {
                if (curRow == 0) {
                    return event->accept();
                }
                int nextRow = curRow - 1;
                m_tab1->setCurrentIndex(m_model1->index(nextRow, 0));
            } else if (m_curSelected.model() == m_tab2->model()) {
                if (curRow == 0) {
                    m_tab2->clearSelection();
                    m_tab1->setCurrentIndex(m_model1->index(m_model1->rowCount() - 1, 0));
                    m_curSelected = m_tab1->currentIndex();
                } else {
                    int nextRow = curRow - 1;
                    m_tab2->setCurrentIndex(m_model2->index(nextRow, 0));
                }
            } else {
                if (curRow == 0) {
                    m_tab3->clearSelection();
                    m_tab2->setCurrentIndex(m_model2->index(m_model2->rowCount() - 1, 0));
                    m_curSelected = m_tab2->currentIndex();
                } else {
                    int nextRow = curRow - 1;
                    m_tab3->setCurrentIndex(m_model3->index(nextRow, 0));
                }
            }
        }
        event->accept();
    } else if (key == Qt::Key_Right) {
        if (m_isFocusEdit) {
            int nextIndex = m_plateEdit->getCurrentIndex() + 1;
            m_plateEdit->moveCursorTo(nextIndex);
        } else {
            int curRow = m_curSelected.row();

            if (m_curSelected.model() == m_tab0->model()) {
                int nextRow = getNextRowForTab1(curRow, key);
                if (nextRow >= 0)
                    m_tab0->setCurrentIndex(m_tab0->model()->index(nextRow, 0));
            } else if (m_curSelected.model() == m_tab1->model()) {
                if (curRow == m_model1->rowCount() - 1) {
                    m_tab1->clearSelection();
                    m_tab2->setCurrentIndex(m_model2->index(0, 0));
                    m_curSelected = m_tab2->currentIndex();
                } else {
                    int nextRow = curRow + 1;
                    m_tab1->setCurrentIndex(m_model1->index(nextRow, 0));
                }
            } else if (m_curSelected.model() == m_tab2->model()) {
                if (curRow == m_model2->rowCount() - 1) {
                    m_tab2->clearSelection();
                    m_tab3->setCurrentIndex(m_model3->index(0, 0));
                    m_curSelected = m_tab3->currentIndex();
                } else {
                    int nextRow = curRow + 1;
                    m_tab2->setCurrentIndex(m_model2->index(nextRow, 0));
                }
            } else {
                if (curRow == m_model3->rowCount() - 1) {
                    return event->accept();
                }
                int nextRow = curRow + 1;
                m_tab3->setCurrentIndex(m_model3->index(nextRow, 0));
            }
        }
        event->accept();
    } else if (key == Qt::Key_Up && !m_isFocusEdit) {
        int curRow = m_curSelected.row();
        if (m_curSelected.model() == m_tab0->model()) {
            int nextRow = getNextRowForTab1(curRow, key);
            if (nextRow >= 0)
                m_tab0->setCurrentIndex(m_tab0->model()->index(nextRow, 0));
        } else if (m_curSelected.model() == m_tab1->model()) {
            if (curRow < 16) {
                return event->accept();
            }
            int nextRow = curRow - 16;
            m_tab1->setCurrentIndex(m_model1->index(nextRow, 0));
        } else if (m_curSelected.model() == m_tab2->model()) {
            if (curRow < 15) {
                int nextRow = curRow + 16;
                m_tab2->clearSelection();
                m_tab1->setCurrentIndex(m_model1->index(nextRow, 0));
                m_curSelected = m_tab1->currentIndex();
            } else if (curRow == 15) {
                m_tab2->clearSelection();
                m_tab1->setCurrentIndex(m_model1->index(m_model1->rowCount() - 1, 0));
                m_curSelected = m_tab1->currentIndex();
            } else {
                int nextRow = curRow - 16;
                m_tab2->setCurrentIndex(m_model2->index(nextRow, 0));
            }
        } else {
            if (curRow < 8) {
                int nextRow = curRow + 16;
                m_tab3->clearSelection();
                m_tab2->setCurrentIndex(m_model2->index(nextRow, 0));
                m_curSelected = m_tab2->currentIndex();
            } else {
                m_tab3->clearSelection();
                m_tab2->setCurrentIndex(m_model2->index(m_model2->rowCount() - 1, 0));
                m_curSelected = m_tab2->currentIndex();
            }
        }
        event->accept();
    } else if (key == Qt::Key_Down && !m_isFocusEdit) {
        int curRow = m_curSelected.row();
        if (m_curSelected.model() == m_tab0->model()) {
            int nextRow = getNextRowForTab1(curRow, key);
            if (nextRow >= 0)
                m_tab0->setCurrentIndex(m_tab0->model()->index(nextRow, 0));
        } else if (m_curSelected.model() == m_tab1->model()) {
            if (curRow < 15) {
                int nextRow = curRow + 16;
                m_tab1->setCurrentIndex(m_model1->index(nextRow, 0));
            } else if (curRow == 15) {
                m_tab1->setCurrentIndex(m_model1->index(m_model1->rowCount() - 1, 0));
            } else {
                int nextRow = curRow - 16;
                m_tab1->clearSelection();
                m_tab2->setCurrentIndex(m_model2->index(nextRow, 0));
                m_curSelected = m_tab2->currentIndex();
            }
        } else if (m_curSelected.model() == m_tab2->model()) {
            if (curRow < 8) {
                int nextRow = curRow + 16;
                m_tab2->setCurrentIndex(m_model2->index(nextRow, 0));
            } else if (curRow < 16) {
                m_tab2->setCurrentIndex(m_model2->index(m_model2->rowCount() - 1, 0));
            } else {
                int nextRow = curRow - 16;
                m_tab2->clearSelection();
                m_tab3->setCurrentIndex(m_model3->index(nextRow, 0));
                m_curSelected = m_tab3->currentIndex();
            }
        } else {
            return event->accept();
        }
        event->accept();
    } else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
        if (m_isFocusEdit) {
            // 焦点在输入框，按确定键，进入键盘操作
            m_isFocusEdit = false;
            int curCharIndex = m_plateEdit->getCurrentIndex();
            m_plateEditIcon->setElaIcon(ElaIconType::HandBackPointDown);
            QModelIndex index;
            if (curCharIndex == 0) {
                m_tabLayout->setCurrentIndex(0);
                index = m_model0->index(0, 0);
                m_tab0->setCurrentIndex(index);
            } else if (curCharIndex == 1) {
                m_tabLayout->setCurrentIndex(1);
                index = m_model1->index(0, 0);
                m_tab1->setCurrentIndex(index);
            } else {
                m_tabLayout->setCurrentIndex(1);
                index = m_model2->index(0, 0);
                m_tab2->setCurrentIndex(index);
            }
            // 避免页面连续切换时，currentChanged信号不触发
            m_curSelected = index;
        } else {
            // 焦点在键盘，按确定键，选择字符，焦点回到输入框
            m_isFocusEdit = true;
            QString selected;
            if (m_curSelected.model() == m_tab0->model()) {
                selected = m_model0->data(m_curSelected).toString();
                m_tab0->clearSelection();
                m_tabLayout->setCurrentIndex(1);
            } else if (m_curSelected.model() == m_tab1->model()) {
                selected = m_model1->data(m_curSelected).toString();
                m_tab1->clearSelection();
            } else if (m_curSelected.model() == m_tab2->model()) {
                selected = m_model2->data(m_curSelected).toString();
                m_tab2->clearSelection();
            } else {
                selected = m_model3->data(m_curSelected).toString();
                m_tab3->clearSelection();
            }

            m_plateEdit->setCurChar(selected);
            m_plateEditIcon->setElaIcon(ElaIconType::HandBackPointLeft);
        }
        event->accept();
    } else if (key == Qt::Key_Backspace) {
        m_plateEdit->delCurChar();
        if (m_plateEdit->getCurrentIndex() == 0)
            m_tabLayout->setCurrentIndex(0);
        event->accept();
    } else if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        QString digit = QString(QChar(key)); // 将按键转为字符
        m_plateEdit->setCurChar(digit);
        event->accept();
    } else if (key == Qt::Key_S) {
        QString title = QString("车牌修改为: %1").arg(m_plateEdit->getPlate());
        // m_contentDialog->setContent(title, "按【确认】保存，按【返回】取消");
        // m_contentDialog->exec();
        event->accept();
    } else {
        qDebug() << "无效按键";
        QWidget::keyPressEvent(event); // 默认处理其他键
    }
}

void MgsPlateEditDialog::initUi()
{
    // 车牌
    ElaText *plateText = new ElaText("车牌: ", this);
    plateText->setTextStyle(ElaTextType::Subtitle);
    m_plateEdit = new MgsPlateEdit(this);
    m_plateEdit->setFixedSize(270, 50);
    m_plateEditIcon = new ElaText(this);
    m_plateEditIcon->setElaIcon(ElaIconType::HandBackPointLeft);
    m_plateEditIcon->setFixedSize(50, 50);

    QHBoxLayout *plateLayout = new QHBoxLayout();
    plateLayout->setContentsMargins(0, 0, 0, 0);
    plateLayout->addStretch(1);
    plateLayout->addWidget(plateText);
    plateLayout->addWidget(m_plateEdit);
    plateLayout->addWidget(m_plateEditIcon);
    plateLayout->addStretch(1);

    // 颜色选择
    m_colorDelegate = new MgsColorDelegate(this);

    ElaText *colorText = new ElaText("(可选车牌颜色如下)", this);
    colorText->setTextStyle(ElaTextType::BodyStrong);
    colorText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_model0 = new QStringListModel(this);
    m_model0->setStringList(m_colors);
    m_tab0 = new ElaListView(this);
    initKeyView(m_tab0, m_model0, 230, 10);
    m_tab0->setItemDelegate(m_colorDelegate);

    QWidget *colorWidget = new QWidget(this);
    colorWidget->setWindowTitle("ColorSelected");
    QVBoxLayout *colorLayout = new QVBoxLayout(colorWidget);
    colorLayout->setContentsMargins(0, 0, 0, 0);
    colorLayout->setSpacing(5);
    colorLayout->addWidget(colorText);
    colorLayout->addWidget(m_tab0);
    colorLayout->addStretch(1);

    // 省份选择
    m_keyDelegate = new MgsKeyDelegate(this);

    ElaText *provinceText = new ElaText("(可选省份简称如下)", this);
    provinceText->setTextStyle(ElaTextType::BodyStrong);
    provinceText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_model1 = new QStringListModel(this);
    m_model1->setStringList(m_provinces);
    m_tab1 = new ElaListView(this);
    initKeyView(m_tab1, m_model1, 80, 2);
    m_tab1->setItemDelegate(m_keyDelegate);

    // 字母选择
    ElaText *letterText = new ElaText("(可选英文字母如下)", this);
    letterText->setTextStyle(ElaTextType::BodyStrong);
    letterText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_model2 = new QStringListModel(this);
    m_model2->setStringList(m_letters);
    m_tab2 = new ElaListView(this);
    initKeyView(m_tab2, m_model2, 80, 2);
    m_tab2->setItemDelegate(m_keyDelegate);

    // 专用字符选择
    ElaText *specialText = new ElaText("(可选专用字符如下)", this);
    specialText->setTextStyle(ElaTextType::BodyStrong);
    specialText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_model3 = new QStringListModel(this);
    m_model3->setStringList(m_specialWords);
    m_tab3 = new ElaListView(this);
    initKeyView(m_tab3, m_model3, 40, 2);
    m_tab3->setItemDelegate(m_keyDelegate);

    QWidget *keyWidget = new QWidget(this);
    colorWidget->setWindowTitle("KeySelected");
    QVBoxLayout *keyLayout = new QVBoxLayout(keyWidget);
    keyLayout->setContentsMargins(0, 0, 0, 0);
    keyLayout->addSpacing(5);
    keyLayout->addWidget(provinceText);
    keyLayout->addWidget(m_tab1);
    keyLayout->addWidget(letterText);
    keyLayout->addWidget(m_tab2);
    keyLayout->addWidget(specialText);
    keyLayout->addWidget(m_tab3);
    keyLayout->addStretch(1);

    m_tabLayout = new QStackedLayout();
    m_tabLayout->setContentsMargins(0, 0, 0, 0);
    m_tabLayout->addWidget(colorWidget);
    m_tabLayout->addWidget(keyWidget);

    // 提示
    QHBoxLayout *tipLayout = Utils::UiUtils::createTipWidget("按左右方向键选择要修改的字符，按上下方向键选择内容，按【"
                                                             "维护】键选定并替换，按【确认】键保存，按【返回】键退出");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(9, 5, 9, 5);
    mainLayout->addLayout(plateLayout);
    mainLayout->addLayout(m_tabLayout, 1);
    mainLayout->addLayout(tipLayout);
}

void MgsPlateEditDialog::initKeyView(ElaListView *view, QStringListModel *model, int height, int spacing)
{
    view->setViewMode(QListView::IconMode);
    view->setFlow(QListView::LeftToRight);
    view->setSpacing(spacing);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);
    view->setResizeMode(QListView::Adjust);
    view->setFixedHeight(height);
    view->setIsTransparent(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setDragEnabled(false);
    if (model != nullptr)
        view->setModel(model);

    // 设置不响应鼠标事件
    view->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

int MgsPlateEditDialog::getNextRowForTab1(int currentRow, int key)
{
    const QList<int> downOrder = {0, 3, 6, 1, 4, 7, 2, 5};
    const QList<int> upOrder = {5, 2, 7, 4, 1, 6, 3, 0};
    const QList<int> rightOrder = {0, 1, 2, 3, 4, 5, 6, 7};
    const QList<int> leftOrder = {7, 6, 5, 4, 3, 2, 1, 0};

    const QList<int> *order = nullptr;

    switch (key) {
    case Qt::Key_Down:
        order = &downOrder;
        break;
    case Qt::Key_Up:
        order = &upOrder;
        break;
    case Qt::Key_Right:
        order = &rightOrder;
        break;
    case Qt::Key_Left:
        order = &leftOrder;
        break;
    default:
        return -1;
    }

    int idx = order->indexOf(currentRow);
    if (idx == -1 || idx + 1 >= order->size())
        return -1;

    return (*order)[idx + 1];
}
