#include "mgscolordelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "global/constant.h"

MgsColorDelegate::MgsColorDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

MgsColorDelegate::~MgsColorDelegate() {}

void MgsColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->save();

    QRect rect = option.rect;
    int padding = 5;
    int colorBoxWidth = rect.height();
    int colorRectWidth = rect.width() - colorBoxWidth - 3 * padding;
    QRect textRect(rect.left() + padding, rect.top(), colorBoxWidth, rect.height());
    QRect colorRect(textRect.right() + padding, rect.top() + padding, colorRectWidth, rect.height() - 2 * padding);

    bool isSelected = option.state.testFlag(QStyle::State_Selected);

    // 始终绘制边框
    painter->setPen(QPen(QColor(Constant::Color::GRAY_COLOR)));
    painter->drawRect(rect.adjusted(0, 0, -1, -1));

    // 绘制选中背景
    if (isSelected) {
        QColor bgColor = QColor(Constant::Color::ALICE_BULE_COLOR);
        painter->fillRect(rect, bgColor);
    }

    // 获取颜色字符串
    QString colorName = index.data(Qt::DisplayRole).toString();
    QFont font = painter->font();
    font.setPixelSize(18);
    painter->setFont(font);

    QColor textColor = isSelected ? QColor(Constant::Color::NAVY_BLUE_COLOR) : option.palette.color(QPalette::Text);
    painter->setPen(textColor);
    painter->drawText(textRect, Qt::AlignCenter, colorName);

    // 仅在文字下方绘制蓝色横线
    if (isSelected) {
        QRect underlineRect = textRect;
        underlineRect.setTop(underlineRect.bottom() - 3);
        underlineRect.setHeight(3);
        painter->fillRect(underlineRect, QColor(Constant::Color::NAVY_BLUE_COLOR));
    }

    // 根据颜色名称设置填充
    QColor borderColor = Qt::black;
    QBrush backgroundBrush = Qt::gray;
    if (colorName == "蓝") {
        backgroundBrush = QColor("#0d2ad6");
    } else if (colorName == "黄") {
        backgroundBrush = QColor("#ebc14f");
    } else if (colorName == "黑") {
        backgroundBrush = Qt::black;
    } else if (colorName == "白") {
        backgroundBrush = Qt::white;
    } else if (colorName == "临") {
        backgroundBrush = QColor("#cdcdcd");
    } else if (colorName == "绿") {
        QLinearGradient gradient(0, 1, 0, 0);
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient.setColorAt(0, QColor("#7fbf6e"));
        gradient.setColorAt(1, Qt::white);
        backgroundBrush = QBrush(gradient);
    } else if (colorName == "渐") {
        QLinearGradient gradient(0, 1, 0, 0);
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient.setColorAt(0, QColor("#47c6f3"));
        gradient.setColorAt(0.5, Qt::white);
        backgroundBrush = QBrush(gradient);
    } else if (colorName == "拼") {
        QColor leftColor("#ebc14f");
        QColor rightColor("#7fbf6e");
        int leftWidth = colorRect.width() / 4;
        QRect leftPart(colorRect.left(), colorRect.top(), leftWidth, colorRect.height());
        QRect rightPart(colorRect.left() + leftWidth,
                        colorRect.top(),
                        colorRect.width() - leftWidth,
                        colorRect.height());
        painter->setPen(Qt::NoPen);
        painter->fillRect(leftPart, leftColor);
        painter->fillRect(rightPart, rightColor);
        QPen pen(borderColor);
        pen.setWidthF(1.5);
        painter->setPen(pen);
        painter->drawRect(colorRect);
        painter->restore();
        return;
    }

    painter->setBrush(backgroundBrush);
    QPen pen(borderColor);
    pen.setWidthF(1.5);
    painter->setPen(pen);
    painter->drawRect(colorRect);

    painter->restore();
}

QSize MgsColorDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(180, 60);
}
