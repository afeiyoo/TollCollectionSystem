#include "mgsweightinfopanel.h"

#include "ElaWidgetTools/ElaListView.h"
#include "gui/delegate/mgsweightinfodelegate.h"

#include <QVBoxLayout>

WeightInfoModel::WeightInfoModel(QObject *parent)
    : QAbstractListModel(parent)
{}

WeightInfoModel::~WeightInfoModel() {}

void WeightInfoModel::setItems(const QList<WeightInfoItem> &items)
{
    beginResetModel();
    m_dataList = items;
    endResetModel();
}

void WeightInfoModel::appendItem(const WeightInfoItem &item)
{
    beginInsertRows(QModelIndex(), m_dataList.size(), m_dataList.size());
    m_dataList.append(item);
    endResetModel();
}

void WeightInfoModel::removeItem(const QModelIndex &index)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_dataList.size())
        return;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_dataList.removeAt(index.row());
    endRemoveRows();
}

void WeightInfoModel::updateItem(const QModelIndex &index, uint status)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_dataList.size())
        return;

    m_dataList[index.row()].status = status;
    emit dataChanged(index, index, {StatusRole});
}

int WeightInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dataList.size();
}

QVariant WeightInfoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_dataList.size())
        return QVariant();

    const WeightInfoItem &item = m_dataList[index.row()];
    switch (role) {
    case PlateRole:
        return item.plate;
    case AxisTypeRole:
        return item.axisType;
    case AxisNumRole:
        return item.axisNum;
    case WeightRole:
        return item.weight;
    case StatusRole:
        return item.status;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> WeightInfoModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PlateRole] = "plate";
    roles[AxisTypeRole] = "axisType";
    roles[AxisNumRole] = "axisNum";
    roles[WeightRole] = "weight";
    roles[StatusRole] = "status";

    return roles;
}

// -------------------------------------------------------

MgsWeightInfoPanel::MgsWeightInfoPanel(QWidget *parent)
    : QWidget(parent)
{
    initUi();
}

MgsWeightInfoPanel::~MgsWeightInfoPanel() {}

void MgsWeightInfoPanel::appendItem(const WeightInfoItem &item)
{
    if (!m_weightInfoModel)
        return;

    m_weightInfoModel->appendItem(item);
}

void MgsWeightInfoPanel::updateItem(int row, uint status)
{
    if (!m_weightInfoModel)
        return;

    QModelIndex index = m_weightInfoModel->index(row, 0);
    m_weightInfoModel->updateItem(index, status);
}

void MgsWeightInfoPanel::initUi()
{
    m_weightInfoView = new ElaListView(this);
    m_weightInfoModel = new WeightInfoModel(this);

    m_weightInfoView->setModel(m_weightInfoModel);
    m_weightInfoView->setItemDelegate(new MgsWeightInfoDelegate(this));

    m_weightInfoView->setSpacing(5);
    m_weightInfoView->setViewMode(QListView::IconMode);
    m_weightInfoView->setFlow(QListView::LeftToRight);
    m_weightInfoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_weightInfoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_weightInfoView->setFocusPolicy(Qt::NoFocus);
    m_weightInfoView->setResizeMode(QListView::Adjust);
    m_weightInfoView->setIsTransparent(true);
    m_weightInfoView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_weightInfoView->setDragEnabled(false);
    m_weightInfoView->setWrapping(false); // 禁止自动换行

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_weightInfoView);
}
