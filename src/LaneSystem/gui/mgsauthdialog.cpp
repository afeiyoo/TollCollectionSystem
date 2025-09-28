#include "mgsauthdialog.h"

#include "ElaWidgetTools/ElaImageCard.h"
#include "ElaWidgetTools/ElaLineEdit.h"
#include "ElaWidgetTools/ElaPushButton.h"
#include "ElaWidgetTools/ElaText.h"
#include "global/constant.h"
#include "utils/uiutils.h"

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

MgsAuthDialog::MgsAuthDialog(QWidget *parent)
    : QWidget{parent}
{
    setWindowTitle("身份认证");
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(300, 380);
    setWindowIcon(QIcon(Constant::Path::APP_ICON));
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);

    initUi();

    Utils::UiUtils::moveToCenter(this);
}

MgsAuthDialog::~MgsAuthDialog() {}

void MgsAuthDialog::setTipInfo(const QString &info)
{
    m_tipInfo->setText(info);
}

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
    ElaImageCard *banner = new ElaImageCard(this);
    banner->setFixedSize(240, 70);
    banner->setIsPreserveAspectCrop(false);
    banner->setCardImage(QImage(Constant::Path::APP_BIG_ICON));

    ElaText *icon1 = new ElaText(this);
    icon1->setElaIcon(ElaIconType::IdCardClip);
    ElaText *icon2 = new ElaText(this);
    icon2->setElaIcon(ElaIconType::ChalkboardUser);
    ElaText *icon3 = new ElaText(this);
    icon3->setElaIcon(ElaIconType::LockKeyhole);
    QList<ElaText *> icons = {icon1, icon2, icon3};
    for (auto *icon : icons) {
        icon->setTextPixelSize(24);
        icon->setFixedSize(30, 30);
    }

    m_idEdit = new ElaLineEdit(this);
    m_idEdit->setPlaceholderText("工号");
    m_idEdit->setDisabled(true);
    m_nameEdit = new ElaLineEdit(this);
    m_nameEdit->setPlaceholderText("姓名");
    m_nameEdit->setDisabled(true);
    m_passwordEdit = new ElaLineEdit(this);
    m_passwordEdit->setPlaceholderText("密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    QList<ElaLineEdit *> inputs = {m_idEdit, m_nameEdit, m_passwordEdit};
    for (auto *input : inputs) {
        QFont font = input->font();
        font.setPixelSize(17);
        input->setFont(font);
        input->setFixedHeight(35);
        input->setIsClearButtonEnable(false);
        input->installEventFilter(this);
    }

    m_button = new ElaPushButton(this);
    m_button->setText("确认");
    QFont font = m_button->font();
    font.setPixelSize(17);
    m_button->setFont(font);
    m_button->setLightDefaultColor(QColor(0, 123, 255));
    m_button->setLightTextColor(Qt::white);
    m_button->setFixedHeight(45);
    m_button->setBorderRadius(5);
    m_button->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_button->setDisabled(true);

    QGridLayout *contentLayout = new QGridLayout();
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
    QHBoxLayout *tipLayout = Utils::UiUtils::createTipWidget("输入密码后，按【确认】键登录系统");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(contentLayout, 1);
    mainLayout->addLayout(tipLayout);
}

bool MgsAuthDialog::eventFilter(QObject *obj, QEvent *event)
{
    // 检查三个输入框是否都非空，更新按钮状态
    if (event->type() == QEvent::KeyRelease || event->type() == QEvent::FocusOut) {
        bool allFilled = !m_idEdit->text().isEmpty() && !m_nameEdit->text().isEmpty()
                         && !m_passwordEdit->text().isEmpty();
        m_button->setEnabled(allFilled);
    }

    // 屏蔽右键菜单
    if ((obj == m_idEdit || obj == m_nameEdit || obj == m_passwordEdit) && event->type() == QEvent::ContextMenu) {
        return true;
    }

    // 密码框特殊处理
    if (obj == m_passwordEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();

        bool isDigit = (key >= Qt::Key_0 && key <= Qt::Key_9);
        bool isControlKey = key == Qt::Key_Backspace || key == Qt::Key_Delete || key == Qt::Key_Left
                            || key == Qt::Key_Right || key == Qt::Key_Tab;

        if (!(isDigit || isControlKey)) {
            // 交给父控件处理
            QCoreApplication::sendEvent(this, event);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void MgsAuthDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    qDebug() << "按键: " << QKeySequence(key).toString();

    if (key == Qt::Key_S) {
        event->accept();
    } else if (key == Qt::Key_X) {
        hide();
        event->accept();
    }

    QWidget::keyPressEvent(event);
}

void MgsAuthDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    m_idEdit->clear();
    m_nameEdit->clear();
    m_passwordEdit->clear();
    m_passwordEdit->setFocus();
}
