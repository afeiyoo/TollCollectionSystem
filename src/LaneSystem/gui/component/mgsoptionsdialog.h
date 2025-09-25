#pragma once

#include <QStandardItemModel>
#include <QWidget>

#include "gui/component/mgsbasedialog.h"

class ElaText;
class ElaListView;
class ElaPushButton;
class MgsOptionsDialog : public MgsBaseDialog
{
    Q_OBJECT
public:
    explicit MgsOptionsDialog(QWidget *parent = nullptr);
    ~MgsOptionsDialog() override;

    void initUi();

    void setOptions(int dlgID, const QString &title, const QStringList &options);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    // 对话框标题
    ElaText *m_title = nullptr;
    // 选项视图
    ElaListView *m_optionsView = nullptr;
    QStandardItemModel *m_model = nullptr;
    // 按钮组
    ElaPushButton *m_yesButton = nullptr;
    ElaPushButton *m_noButton = nullptr;
    // 对话框ID
    int m_dlgID;
};
