#include "mgsoptionsdialog.h"

#include <QDebug>
#include <QKeyEvent>
#include <QPainter>

#include "ElaWidgetTools/ElaListView.h"
#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaText.h"
#include "global/constant.h"
#include "global/globalmanager.h"
#include "global/signalmanager.h"
#include "utils/uiutils.h"

using namespace Utils;

MgsOptionsDialog::MgsOptionsDialog(QWidget *parent)
    : MgsBaseDialog{parent}
{
    setMinimumWidth(350);
    setMaximumWidth(400);

    m_dlgID = Constant::DialogID::INVALID_DLG;

    initUi();

    UiUtils::disableMouseEvents(this);
}

MgsOptionsDialog::~MgsOptionsDialog() {}

void MgsOptionsDialog::initUi()
{
    m_title = new ElaText(this);
    QFont titleFont = m_title->font();
    titleFont.setPixelSize(Constant::FontSize::DIALOG_TITLE_SIZE);
    titleFont.setBold(true);
    m_title->setFont(titleFont);

    m_model = new QStandardItemModel(this);

    m_optionsView = new ElaListView(this);
    m_optionsView->setModel(m_model);
    m_optionsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_optionsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_optionsView->setIsTransparent(true);
    QFont itemFont = m_optionsView->font();
    itemFont.setPixelSize(Constant::FontSize::DIALOG_BODY_SIZE);
    m_optionsView->setFont(itemFont);
    m_optionsView->setFocusPolicy(Qt::NoFocus);

    m_yesButton = new ElaPushButton("确认", this);
    m_yesButton->setLightDefaultColor(QColor(Constant::Color::CONFIRM_BUTTON_BG));
    m_yesButton->setLightTextColor(QColor(Constant::Color::CONFIRM_BUTTON_TEXT));

    m_noButton = new ElaPushButton("返回", this);
    m_noButton->setLightDefaultColor(Constant::Color::CANCEL_BUTTON_BG);
    QList<ElaPushButton *> btnList = {m_yesButton, m_noButton};
    for (auto &btn : btnList) {
        btn->setContentsMargins(0, 0, 0, 0);
        btn->setBorderRadius(5);
        QFont btnFont = btn->font();
        btnFont.setPixelSize(Constant::FontSize::DIALOG_BUTTON_SIZE);
        btn->setFont(btnFont);
        btn->setFixedSize(80, 30);
    }

    QWidget *buttonWidget = new QWidget(this);
    buttonWidget->setFixedHeight(35);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->setContentsMargins(0, 0, 5, 0);
    buttonLayout->setSpacing(5);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_yesButton);
    buttonLayout->addWidget(m_noButton);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralVLayout = new QVBoxLayout(centralWidget);
    centralVLayout->setContentsMargins(0, 0, 0, 0);
    centralVLayout->setSpacing(0);
    centralVLayout->addWidget(m_title);
    centralVLayout->addWidget(m_optionsView, 1);
    centralVLayout->addWidget(buttonWidget);

    setCentralWidget(centralWidget);
}

void MgsOptionsDialog::setOptions(int dlgID, const QString &title, const QStringList &options)
{
    m_dlgID = dlgID;
    m_title->setText(title);
    m_model->clear();

    if (options.isEmpty()) {
        m_optionsView->setFixedHeight(m_optionsView->getItemHeight() + 5);
        adjustSize();
        setFixedSize(size());
        return;
    }

    for (const auto &option : options) {
        QStandardItem *item = new QStandardItem(option);

        QFont font = item->font();
        font.setPixelSize(Constant::FontSize::DIALOG_BODY_SIZE);
        item->setFont(font);

        m_model->appendRow(item);
    }
    // 焦点置于第一项
    QModelIndex firstIndex = m_optionsView->model()->index(0, 0);
    m_optionsView->setCurrentIndex(firstIndex);

    int totalHeight = m_model->rowCount() * m_optionsView->getItemHeight() + 5; // 5是padding
    int maxHeight = 600;
    // 限制一下最大高度，避免选项太多撑满屏幕
    totalHeight = qMin(totalHeight, maxHeight);
    m_optionsView->setFixedHeight(totalHeight);
    adjustSize();
    setFixedSize(size());
}

void MgsOptionsDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Up || key == Qt::Key_Down) {
        QModelIndex current = m_optionsView->currentIndex();
        int row = current.isValid() ? current.row() : 0;

        if (key == Qt::Key_Up) {
            if (row > 0)
                row -= 1;
        } else { // Key_Down
            if (row < m_optionsView->model()->rowCount() - 1)
                row += 1;
        }
        QModelIndex newIndex = m_optionsView->model()->index(row, 0);
        m_optionsView->setCurrentIndex(newIndex);

        event->accept();
    } else if (key == Qt::Key_S) {
        QModelIndex current = m_optionsView->currentIndex();
        if (!current.isValid())
            return;
        int row = current.row();
        emit GM_INSTANCE->m_signalMan->sigOptionSelected(m_dlgID, row);
        hide();

        event->accept();
    } else if (key == Qt::Key_X) {
        hide();
        event->accept();
    } else if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        int num = key - Qt::Key_0;
        if (num < m_optionsView->model()->rowCount()) {
            QModelIndex newIndex = m_optionsView->model()->index(num, 0);
            if (newIndex.isValid()) {
                m_optionsView->setCurrentIndex(newIndex);
                emit GM_INSTANCE->m_signalMan->sigOptionSelected(m_dlgID, num);
            }
        }
        hide();

        event->accept();
    } else {
        QDialog::keyPressEvent(event); // 默认处理其他键
    }
}

void MgsOptionsDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(Constant::Color::DIALOG_BG));
    // 背景绘制
    int layoutAreaHeight = m_title->height() + m_optionsView->height() + 5;
    painter.drawRect(QRectF(0, 0, width(), layoutAreaHeight));
    painter.restore();

    QDialog::paintEvent(event);
}

void MgsOptionsDialog::showEvent(QShowEvent *event)
{
    UiUtils::moveToCenter(this);
    QDialog::showEvent(event);
}
