#pragma once

#include <QStyledItemDelegate>

class MgsColorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MgsColorDelegate(QObject *parent = nullptr);
    ~MgsColorDelegate() override;

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
