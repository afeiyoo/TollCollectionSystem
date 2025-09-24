#pragma once

#include <QDialog>
#include <QVBoxLayout>

class MgsBaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MgsBaseDialog(QWidget *parent = nullptr);
    ~MgsBaseDialog() override;

protected:
    void setCentralWidget(QWidget *centralWidget);

private:
    void initUi();

private:
    // 主布局
    QVBoxLayout *m_mainLayout = nullptr;
    // 内容区域
    QWidget *m_centralWidget = nullptr;
    // 提示组件
    QHBoxLayout *m_tipLayout = nullptr;
};
