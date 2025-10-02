#include "mgsauthdialog.h"

#include "ElaWidgetTools/ElaImageCard.h"
#include "ElaWidgetTools/ElaLineEdit.h"
#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaText.h"
#include "global/constant.h"
#include "utils/uiutils.h"

#include <QCoreApplication>
#include <QEvent>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

using namespace Utils;

MgsAuthDialog::MgsAuthDialog(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("身份认证");
    setWindowModality(Qt::ApplicationModal);
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    setFixedSize(300, 350);

    initUi();
}

MgsAuthDialog::~MgsAuthDialog() {}

void MgsAuthDialog::setID(const QString &id)
{
    if (!m_idEdit)
        return;
    m_idEdit->setText(id);
}

void MgsAuthDialog::setName(const QString &name)
{
    if (!m_nameEdit)
        return;
    m_nameEdit->setText(name);
}

void MgsAuthDialog::initUi()
{
    m_mainWidget = new QWidget(this);
    m_mainWidget->setStyleSheet(QString("background-color: %1;").arg(Constant::Color::DIALOG_BG));

    ElaImageCard *banner = new ElaImageCard(m_mainWidget);
    banner->setFixedSize(240, 70);
    banner->setIsPreserveAspectCrop(false);
    banner->setCardImage(QImage(Constant::Path::APP_BIG_ICON));

    ElaText *icon1 = new ElaText(m_mainWidget);
    icon1->setElaIcon(ElaIconType::IdCardClip);
    ElaText *icon2 = new ElaText(m_mainWidget);
    icon2->setElaIcon(ElaIconType::ChalkboardUser);
    ElaText *icon3 = new ElaText(m_mainWidget);
    icon3->setElaIcon(ElaIconType::LockKeyhole);
    QList<ElaText *> icons = {icon1, icon2, icon3};
    for (auto *icon : icons) {
        icon->setTextPixelSize(24);
        icon->setFixedSize(30, 30);
    }

    m_idEdit = new ElaLineEdit(m_mainWidget);
    m_idEdit->setPlaceholderText("工号");
    m_idEdit->setDisabled(true);
    m_nameEdit = new ElaLineEdit(m_mainWidget);
    m_nameEdit->setPlaceholderText("姓名");
    m_nameEdit->setDisabled(true);
    m_passwordEdit = new ElaLineEdit(m_mainWidget);
    m_passwordEdit->setPlaceholderText("密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    QList<ElaLineEdit *> inputs = {m_idEdit, m_nameEdit, m_passwordEdit};
    for (auto *input : inputs) {
        QFont font = input->font();
        font.setPixelSize(Constant::FontSize::DIALOG_BODY_SIZE);
        input->setFont(font);
        input->setFixedHeight(35);
        input->setIsClearButtonEnable(false);
        input->installEventFilter(this);
    }

    m_button = new ElaPushButton(m_mainWidget);
    m_button->setText("确认");
    QFont font = m_button->font();
    font.setPixelSize(17);
    m_button->setFont(font);
    m_button->setLightDefaultColor(Constant::Color::CONFIRM_BUTTON_BG);
    m_button->setLightTextColor(Constant::Color::CONFIRM_BUTTON_TEXT);
    m_button->setFixedHeight(45);
    m_button->setBorderRadius(5);
    UiUtils::disableMouseEvents(m_button);

    QGridLayout *contentLayout = new QGridLayout(m_mainWidget);
    contentLayout->setContentsMargins(25, 20, 25, 0);
    contentLayout->setHorizontalSpacing(10);
    contentLayout->setVerticalSpacing(15);

    contentLayout->addWidget(banner, 0, 0, 1, 2, Qt::AlignCenter);
    contentLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed), 1, 0, 1, 2);

    contentLayout->addWidget(icon1, 2, 0);
    contentLayout->addWidget(m_idEdit, 2, 1);

    contentLayout->addWidget(icon2, 3, 0);
    contentLayout->addWidget(m_nameEdit, 3, 1);

    contentLayout->addWidget(icon3, 4, 0);
    contentLayout->addWidget(m_passwordEdit, 4, 1);

    contentLayout->addWidget(m_button, 5, 0, 1, 2);
    contentLayout->setRowStretch(6, 1); // bottom stretch

    // 提示
    QHBoxLayout *tipLayout = UiUtils::createTipWidget("输入密码后，按【确认】键登录系统");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_mainWidget);
    mainLayout->addLayout(tipLayout);
}

bool MgsAuthDialog::eventFilter(QObject *obj, QEvent *event)
{
    // 屏蔽右键菜单
    if ((obj == m_idEdit || obj == m_nameEdit || obj == m_passwordEdit) && event->type() == QEvent::ContextMenu) {
        return true;
    }

    // 密码框特殊处理
    if (obj == m_passwordEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();

        bool isDigit = (key >= Qt::Key_0 && key <= Qt::Key_9);
        bool isControlKey = key == Qt::Key_Backspace || key == Qt::Key_Delete || key == Qt::Key_Left || key == Qt::Key_Right;

        if (!(isDigit || isControlKey)) {
            // 交给父控件处理
            QCoreApplication::sendEvent(this, event);
            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}

void MgsAuthDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key == Qt::Key_S) {
        event->accept();
    } else if (key == Qt::Key_X) {
        hide();
        event->accept();
    }

    QDialog::keyPressEvent(event);
}

void MgsAuthDialog::showEvent(QShowEvent *event)
{
    m_idEdit->clear();
    m_nameEdit->clear();
    m_passwordEdit->clear();
    m_passwordEdit->setFocus();

    UiUtils::moveToCenter(this);
    QDialog::showEvent(event);
}
