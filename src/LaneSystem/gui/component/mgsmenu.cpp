#include "mgsmenu.h"

#include <QGuiApplication>
#include <QScreen>
#include <QStackedLayout>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include "ElaWidgetTools/ElaListView.h"
#include "ElaWidgetTools/ElaPivot.h"
#include "Logger.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "utils/uiutils.h"

using namespace Utils;

MgsMenu::MgsMenu(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("功能选择");
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(350, 450);
    setWindowIcon(QIcon(Constant::Path::APP_ICON));
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);

    initUi();
    UiUtils::disableMouseEvents(this);

    UiUtils::moveToCenter(this);
}

MgsMenu::~MgsMenu() {}

void MgsMenu::initUi()
{
    m_pivot = new ElaPivot(this);

    m_tabLayout = new QStackedLayout();
    QHBoxLayout *tipLayout = UiUtils::createTipWidget(
        "按左右方向键选择分类，按上下方向键选择功能并按【确定】键确认选择");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    mainLayout->addWidget(m_pivot);
    mainLayout->addLayout(m_tabLayout, 1);
    mainLayout->addLayout(tipLayout);
}

void MgsMenu::addNewTab(const QString &tabName, const QStringList &funcs, const QList<uint> &enableFuncs)
{
    m_pivot->appendPivot(tabName);
    m_tabNames.append(tabName);

    QStandardItemModel *model = new QStandardItemModel(this);
    for (int i = 0; i < funcs.size(); ++i) {
        QStandardItem *item = new QStandardItem(funcs[i]);

        // 设置item样式
        QFont font = item->font();
        font.setPixelSize(17);
        item->setFont(font);
        // 确定是否启用
        QColor color = enableFuncs.contains(i) ? Qt::black : QColor(Constant::Color::GRAY_COLOR);
        item->setForeground(color);

        model->appendRow(item);
    }
    // 可用功能列表更新
    m_enableFuncsPerTab.append(enableFuncs);

    ElaListView *view = new ElaListView(this);
    view->setModel(model);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QFont font = view->font();
    font.setPixelSize(17);
    view->setFont(font);
    // 由于tab是窗口的子控件，子控件拦截并处理事件A后，事件A不会传到父控件, 为了让窗口统一处理事件，使子控件无法获取焦点。
    view->setFocusPolicy(Qt::NoFocus);
    m_tabLayout->addWidget(view);

    // 聚焦初始化
    if (m_tabLayout->count() == 1) {
        m_tabLayout->setCurrentIndex(0);
        m_pivot->setCurrentIndex(0);
        focusTab(0);
    }
}

void MgsMenu::reset()
{
    for (const auto &name : m_tabNames) {
        m_pivot->removePivot(name);
    }
    m_tabNames.clear();
    m_enableFuncsPerTab.clear();

    // 清空布局中的视图
    while (m_tabLayout->count() > 0) {
        QWidget *w = m_tabLayout->widget(0);
        m_tabLayout->removeWidget(w);
        if (w) {
            w->deleteLater(); // 释放控件
        }
    }

    m_tabLayout->setCurrentIndex(-1);
    m_pivot->setCurrentIndex(-1);
}

void MgsMenu::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    int tabIndex = m_tabLayout->currentIndex();
    int count = m_tabLayout->count();
    if (key == Qt::Key_Left) {
        if (tabIndex > 0) { // 不是第一个才切
            tabIndex -= 1;
            m_tabLayout->setCurrentIndex(tabIndex);
            m_pivot->setCurrentIndex(tabIndex);
            focusTab(tabIndex);
        }
        event->accept();
    } else if (key == Qt::Key_Right) {
        if (tabIndex < count - 1) { // 不是最后一个才切
            tabIndex += 1;
            m_tabLayout->setCurrentIndex(tabIndex);
            m_pivot->setCurrentIndex(tabIndex);
            focusTab(tabIndex);
        }
        event->accept();
    } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
        QWidget *w = m_tabLayout->widget(tabIndex);
        if (ElaListView *view = qobject_cast<ElaListView *>(w)) {
            QModelIndex current = view->currentIndex();
            int row = current.isValid() ? current.row() : 0;

            if (key == Qt::Key_Up) {
                if (row > 0)
                    row -= 1;
            } else { // Key_Down
                if (row < view->model()->rowCount() - 1)
                    row += 1;
            }

            QModelIndex newIndex = view->model()->index(row, 0);
            view->setCurrentIndex(newIndex);
        }
        event->accept();
    } else if (key == Qt::Key_S) {
        QWidget *w = m_tabLayout->widget(tabIndex);
        if (ElaListView *view = qobject_cast<ElaListView *>(w)) {
            QModelIndex current = view->currentIndex();
            if (!current.isValid())
                return;
            int row = current.row();
            if (!m_enableFuncsPerTab[tabIndex].contains(row)) {
                emit GM_INSTANCE->m_signalMan->sigFuncUnavaliable(
                    view->model()->data(current, Qt::DisplayRole).toString());
            } else {
                LOG_INFO().noquote() << "请求功能:" << view->model()->data(current, Qt::DisplayRole).toString();
                emit GM_INSTANCE->m_signalMan->sigMenuRequest(current.row());
            }
        }

        event->accept();
    } else if (key == Qt::Key_X) {
        hide();
        event->accept();
    } else {
        LOG_WARNING().noquote() << "无效按键";
        QWidget::keyPressEvent(event); // 默认处理其他键
    }
}

void MgsMenu::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    setFocus();
}

void MgsMenu::focusTab(int index)
{
    if (index < 0 || index >= m_tabLayout->count())
        return;

    QWidget *w = m_tabLayout->widget(index);
    if (!w)
        return;

    if (ElaListView *view = qobject_cast<ElaListView *>(w)) {
        if (view->model() && view->model()->rowCount() > 0) {
            QModelIndex firstIndex = view->model()->index(0, 0);
            view->setCurrentIndex(firstIndex);
        }
    }
}
