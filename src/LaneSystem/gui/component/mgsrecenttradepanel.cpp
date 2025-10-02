#include "mgsrecenttradepanel.h"

#include <QHeaderView>
#include <QVBoxLayout>

#include "ElaWidgetTools/ElaTableView.h"
#include "global/constant.h"

//==============================================================================
// 模型实现
//==============================================================================
RecentTradeModel::RecentTradeModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

RecentTradeModel::~RecentTradeModel() {}

int RecentTradeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tradeList.count();
}

int RecentTradeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_header.count();
}

QVariant RecentTradeModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_tradeList[index.row()][index.column()];
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant RecentTradeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_header[section];
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void RecentTradeModel::setHeader(QStringList header)
{
    m_header = header;
    if (!m_header.isEmpty()) {
        emit headerDataChanged(Qt::Horizontal, 0, m_header.count() - 1);
    }
}

void RecentTradeModel::appendTrade(QStringList trade)
{
    if (trade.size() != m_header.size())
        return;

    beginInsertRows(QModelIndex(), m_tradeList.count(), m_tradeList.count());
    m_tradeList.append(trade);
    endInsertRows();
}

//==============================================================================
// 视图实现
//==============================================================================
MgsRecentTradePanel::MgsRecentTradePanel(QStringList header, QWidget *parent)
    : QWidget{parent}
{
    initUi();
    m_recentTradeModel = new RecentTradeModel(this);
    m_recentTradeModel->setHeader(header);
    m_recentTradeView->setModel(m_recentTradeModel);
}

MgsRecentTradePanel::~MgsRecentTradePanel() {}

void MgsRecentTradePanel::appendTrade(QStringList trade)
{
    m_recentTradeModel->appendTrade(trade);
    // 自动滚动到最后一行
    int lastRow = m_recentTradeModel->rowCount() - 1;
    if (lastRow >= 0) {
        m_recentTradeView->scrollToBottom();
    }
}

void MgsRecentTradePanel::initUi()
{
    m_recentTradeView = new ElaTableView(this);

    // 表头字体
    QFont headerFont = m_recentTradeView->horizontalHeader()->font();
    headerFont.setWeight(QFont::DemiBold);
    headerFont.setPixelSize(Constant::FontSize::TRADE_VIEW_SIZE);
    m_recentTradeView->horizontalHeader()->setFont(headerFont);

    // 表头行为
    m_recentTradeView->horizontalHeader()->setStretchLastSection(true);
    m_recentTradeView->horizontalHeader()->setSectionsMovable(false);
    m_recentTradeView->horizontalHeader()->setSectionsClickable(false);
    m_recentTradeView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // 索引列行为
    m_recentTradeView->verticalHeader()->setVisible(false);

    // 视图行为
    m_recentTradeView->setSelectionMode(QAbstractItemView::NoSelection);
    m_recentTradeView->setFocusPolicy(Qt::NoFocus);
    m_recentTradeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_recentTradeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_recentTradeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_recentTradeView->setShowGrid(true);           // 启用表格线
    m_recentTradeView->setGridStyle(Qt::SolidLine); // 使用实线
    m_recentTradeView->setStyleSheet(R"(QTableView { gridline-color: #d1d1d1; })");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_recentTradeView);
}

ElaTableView *MgsRecentTradePanel::getRecentTradeView() const
{
    return m_recentTradeView;
}
