#include "mgskeydelegate.h"
#include "global/constant.h"

#include <QPainter>

MgsKeyDelegate::MgsKeyDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

MgsKeyDelegate::~MgsKeyDelegate() {}

void MgsKeyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->save();

    bool isSelected = option.state & QStyle::State_Selected;
    QColor textColor = isSelected ? QColor(Constant::Color::NAVY_BLUE_COLOR) : Qt::black;

    // 仅在选中时绘制背景
    if (isSelected) {
        QColor bgColor = QColor(Constant::Color::ALICE_BULE_COLOR);
        painter->fillRect(option.rect, bgColor);
    }

    // 始终绘制边框
    painter->setPen(QPen(QColor(Constant::Color::GRAY_COLOR)));
    painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    // 绘制文字
    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(textColor);
    QFont font = painter->font();
    font.setPixelSize(18);
    painter->setFont(font);
    painter->drawText(option.rect, Qt::AlignCenter, text);

    // 如果选中，绘制下方蓝色横线
    if (isSelected) {
        QRect underlineRect = option.rect;
        underlineRect.setTop(underlineRect.bottom() - 3);
        underlineRect.setHeight(3);

        painter->fillRect(underlineRect, QColor(Constant::Color::NAVY_BLUE_COLOR));
    }

    painter->restore();
}

QSize MgsKeyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    if (text.size() > 1) {
        return QSize(50, 35);
    }
    return QSize(35, 35);
}
