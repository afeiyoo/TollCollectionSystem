#include "mgsoptionsdialog.h"

#include <QDebug>
#include <QKeyEvent>
#include <QPainter>

#include "ElaWidgetTools/ElaListView.h"
#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaText.h"
#include "global/constant.h"
#include "utils/uiutils.h"

using namespace Utils;

MgsOptionsDialog::MgsOptionsDialog(QWidget *parent)
    : MgsBaseDialog{parent}
{
    setMinimumWidth(350);
    setMaximumWidth(400);

    initUi();

    UiUtils::disableMouseEvents(this);
}

MgsOptionsDialog::~MgsOptionsDialog() {}

void MgsOptionsDialog::initUi()
{
    m_title = new ElaText(this);
    QFont titleFont = m_title->font();
    titleFont.setPixelSize(25);
    titleFont.setBold(true);
    m_title->setFont(titleFont);

    m_model = new QStandardItemModel(this);

    m_optionsView = new ElaListView(this);
    m_optionsView->setModel(m_model);
    m_optionsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_optionsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_optionsView->setIsTransparent(true);
    QFont itemFont = m_optionsView->font();
    itemFont.setPixelSize(17);
    m_optionsView->setFont(itemFont);
    m_optionsView->setFocusPolicy(Qt::NoFocus);

    m_yesButton = new ElaPushButton("确定", this);
    m_yesButton->setLightDefaultColor(QColor(Constant::Color::BLUE_COLOR));
    m_yesButton->setLightTextColor(QColor(Constant::Color::WHITE_COLOR));

    m_noButton = new ElaPushButton("取消", this);
    m_noButton->setLightDefaultColor(Constant::Color::GRAY_COLOR);
    QList<ElaPushButton *> btnList = {m_yesButton, m_noButton};
    for (auto &btn : btnList) {
        btn->setContentsMargins(0, 0, 0, 0);
        btn->setBorderRadius(5);
        QFont btnFont = btn->font();
        btnFont.setPixelSize(13);
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

void MgsOptionsDialog::setOptions(const QString &title, const QStringList &options)
{
    m_title->setText(title);
    m_model->clear();

    if (options.isEmpty()) {
        m_optionsView->setFixedHeight(m_optionsView->getItemHeight() + 10);
        adjustSize();
        setFixedSize(size());
        return;
    }

    for (const auto &option : options) {
        QStandardItem *item = new QStandardItem(option);

        QFont font = item->font();
        font.setPixelSize(17);
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
    event->accept();
}

void MgsOptionsDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(Constant::Color::WHITE_COLOR));
    // 背景绘制
    int layoutAreaHeight = m_title->height() + m_optionsView->height() + 5;
    painter.drawRect(QRectF(0, 0, width(), layoutAreaHeight));
    painter.restore();

    QDialog::paintEvent(event);
}
