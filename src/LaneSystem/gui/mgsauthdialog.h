#pragma once

#include <QWidget>

class ElaLineEdit;
class ElaPushButton;
class ElaText;
class MgsAuthDialog : public QWidget
{
    Q_OBJECT
public:
    explicit MgsAuthDialog(QWidget *parent = nullptr);
    ~MgsAuthDialog() override;

    void setTipInfo(const QString &info);
    void setID(const QString &id);
    void setName(const QString &name);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void initUi();

private:
    ElaLineEdit *m_idEdit = nullptr;
    ElaLineEdit *m_nameEdit = nullptr;
    ElaLineEdit *m_passwordEdit = nullptr;
    ElaPushButton *m_button = nullptr;
    ElaText *m_tipInfo = nullptr;
};
