#include "mgsweightinfodelegate.h"

#include "gui/component/mgsweightinfopanel.h"
#include <QPainter>
#include <QPainterPath>

MgsWeightInfoDelegate::MgsWeightInfoDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

MgsWeightInfoDelegate::~MgsWeightInfoDelegate() {}

void MgsWeightInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter->save();

    QRect rect = option.rect;

    // 背景（纯透明），边框浅灰色
    QColor borderColor = QColor("#c8c8c8");

    QPainterPath path;
    path.addRoundedRect(rect.adjusted(1, 1, -1, -1), 6, 6);
    painter->setPen(QPen(borderColor, 1));
    painter->drawPath(path);

    // 获取数据
    QString plate = index.data(WeightInfoModel::PlateRole).toString();
    uint axisType = index.data(WeightInfoModel::AxisTypeRole).toUInt();
    uint axisNum = index.data(WeightInfoModel::AxisNumRole).toUInt();
    qreal weight = index.data(WeightInfoModel::WeightRole).toReal();
    uint status = index.data(WeightInfoModel::StatusRole).toUInt();
    QPixmap pixmap = getAxisTypePixmap(axisType, status);

    QString axisInfo = QString("%1(%2轴)").arg(axisType).arg(axisNum);
    QString weightInfo = QString("%1吨").arg(weight);

    // 布局参数
    int margin = 5;
    int spacing = 5;

    // 图片区域（右侧）
    int iconWidth = 100;
    int iconHeight = 70;
    QRect iconRect(rect.right() - margin - iconWidth, rect.center().y() - iconHeight / 2, iconWidth, iconHeight);

    // 绘制圆角图片
    QPainterPath imgPath;
    imgPath.addRoundedRect(iconRect, 0, 0);
    painter->setClipPath(imgPath);
    painter->drawPixmap(iconRect, pixmap);
    painter->setClipping(false);

    // 文本区域（左侧）
    int textWidth = iconRect.left() - rect.left() - margin;
    int lineHeight = 18;
    int textX = rect.left() + margin;
    int textY = rect.top() + (rect.height() - (lineHeight * 3 + spacing * 2)) / 2;

    QFont font = option.font;
    font.setPixelSize(15);
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawText(QRect(textX, textY, textWidth, lineHeight), Qt::AlignLeft | Qt::AlignVCenter, plate);
    painter->drawText(QRect(textX, textY + lineHeight + spacing, textWidth, lineHeight),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      axisInfo);
    painter->drawText(QRect(textX, textY + (lineHeight + spacing) * 2, textWidth, lineHeight),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      weightInfo);

    painter->restore();
}

QSize MgsWeightInfoDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(190, 85);
}

QPixmap MgsWeightInfoDelegate::getAxisTypePixmap(uint axisType, uint status) const
{
    if (axisType == 0) {
        return status == 0 ? QPixmap(":/static/images/axistype_unknown_waiting.png")
                           : QPixmap(":/static/images/axistype_unknown_pass.png");
    }

    QString strStatus;
    if (status == 0)
        strStatus = "waiting";
    else
        strStatus = "pass";

    QString str = QString(":/static/images/axistype_%1_%2.png").arg(axisType).arg(strStatus);
    return QPixmap(str);
}
