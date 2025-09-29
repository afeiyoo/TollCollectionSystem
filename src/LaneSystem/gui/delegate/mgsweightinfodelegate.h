#pragma once

#include <QStyledItemDelegate>

class MgsWeightInfoDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MgsWeightInfoDelegate(QObject *parent = nullptr);

    ~MgsWeightInfoDelegate() override;

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    // 获取轴型图片 status：0, 等待过车; 1, 允许过车; 2, 超限
    QPixmap getAxisTypePixmap(uint axisType, uint status) const;
};
