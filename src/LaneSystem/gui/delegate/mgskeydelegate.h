#pragma once

#include <QStyledItemDelegate>

class MgsKeyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MgsKeyDelegate(QObject *parent = nullptr);
    ~MgsKeyDelegate() override;

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
