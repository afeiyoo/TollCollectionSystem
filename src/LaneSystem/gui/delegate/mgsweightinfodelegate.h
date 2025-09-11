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
    QPixmap getAxisTypePixmap(uint axisType, uint status) const;
};
