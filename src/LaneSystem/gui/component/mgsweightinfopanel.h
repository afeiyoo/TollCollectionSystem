#pragma once

#include "utils/defs.h"
#include <QAbstractListModel>
#include <QWidget>

class WeightInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeightInfoModel(QObject *parent = nullptr);

    ~WeightInfoModel() override;

    enum WeightInfoRoles { PlateRole = Qt::UserRole + 1, AxisTypeRole, AxisNumRole, WeightRole, StatusRole };

    void setItems(const QList<ST_WeightInfoItem> &items);
    void appendItem(const ST_WeightInfoItem &item);
    void removeItem(const QModelIndex &index);
    void updateItem(const QModelIndex &index, uint status);

protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<ST_WeightInfoItem> m_dataList;
};

class ElaListView;
class MgsWeightInfoPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MgsWeightInfoPanel(QWidget *parent = nullptr);

    ~MgsWeightInfoPanel() override;

    void appendItem(const ST_WeightInfoItem &item);
    void updateItem(int row, uint status);

private:
    void initUi();

private:
    ElaListView *m_weightInfoView = nullptr;
    WeightInfoModel *m_weightInfoModel = nullptr;
};
