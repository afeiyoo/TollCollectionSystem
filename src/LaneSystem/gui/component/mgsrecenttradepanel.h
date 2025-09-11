#pragma once

#include <QAbstractTableModel>
#include <QWidget>

class RecentTradeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RecentTradeModel(QObject *parent = nullptr);
    ~RecentTradeModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setHeader(QStringList header);
    void appendTrade(QStringList trade);

private:
    QStringList m_header;
    QList<QStringList> m_tradeList;
};

class ElaTableView;
class MgsRecentTradePanel : public QWidget
{
    Q_OBJECT
public:
    explicit MgsRecentTradePanel(QStringList header, QWidget *parent = nullptr);

    ~MgsRecentTradePanel() override;

    void appendTrade(QStringList trade);

    ElaTableView *getRecentTradeView() const;

private:
    void initUi();

private:
    ElaTableView *m_recentTradeView = nullptr;
    RecentTradeModel *m_recentTradeModel = nullptr;
};
