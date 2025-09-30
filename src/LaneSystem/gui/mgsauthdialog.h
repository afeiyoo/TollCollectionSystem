#pragma once

#include <QDialog>

class ElaLineEdit;
class ElaPushButton;
class ElaText;
class MgsAuthDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MgsAuthDialog(QWidget *parent = nullptr);
    ~MgsAuthDialog() override;

    void setID(const QString &id);
    void setName(const QString &name);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void initUi();

private:
    QWidget *m_mainWidget = nullptr;
    // 工号输入
    ElaLineEdit *m_idEdit = nullptr;
    // 姓名输入
    ElaLineEdit *m_nameEdit = nullptr;
    // 密码输入
    ElaLineEdit *m_passwordEdit = nullptr;
    // 确定按钮
    ElaPushButton *m_button = nullptr;
};
