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

    void setOptions(const QString &title, const QStringList &options);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    ElaText *m_title = nullptr;
    ElaListView *m_optionsView = nullptr;
    QStandardItemModel *m_model = nullptr;
    ElaPushButton *m_yesButton = nullptr;
    ElaPushButton *m_noButton = nullptr;
};
