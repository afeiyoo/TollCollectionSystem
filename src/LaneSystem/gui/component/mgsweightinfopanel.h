#pragma once

#include <QAbstractListModel>
#include <QWidget>

// 称重信息
struct WeightInfoItem
{
    QString plate;
    uint axisType;
    uint axisNum;
    qreal weight;
    uint status; // 0-等待中，1-已交易
};

// 模型类
class WeightInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeightInfoModel(QObject *parent = nullptr);

    ~WeightInfoModel() override;

    enum WeightInfoRoles { PlateRole = Qt::UserRole + 1, AxisTypeRole, AxisNumRole, WeightRole, StatusRole };

    void setItems(const QList<WeightInfoItem> &items);
    void appendItem(const WeightInfoItem &item);
    void removeItem(const QModelIndex &index);
    void updateItem(const QModelIndex &index, uint status);

protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<WeightInfoItem> m_dataList;
};

class ElaListView;
class MgsWeightInfoPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MgsWeightInfoPanel(QWidget *parent = nullptr);

    ~MgsWeightInfoPanel() override;

    void appendItem(const WeightInfoItem &item);
    void updateItem(int row, uint status);

private:
    void initUi();

private:
    ElaListView *m_weightInfoView = nullptr;
    WeightInfoModel *m_weightInfoModel = nullptr;
};
