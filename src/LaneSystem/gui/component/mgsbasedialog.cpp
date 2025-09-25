#include "mgsbasedialog.h"

#include <QDebug>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "utils/uiutils.h"

using namespace Utils;

MgsBaseDialog::MgsBaseDialog(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("请选择");
    setWindowModality(Qt::ApplicationModal);
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);

    initUi();
}

MgsBaseDialog::~MgsBaseDialog() {}

void MgsBaseDialog::initUi()
{
    // 内容区域
    m_centralWidget = new QWidget(this);

    // 提示
    m_tipLayout = UiUtils::createTipWidget("请按数字键选择对应的功能");

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(0);

    m_mainLayout->addWidget(m_centralWidget, 1);
    m_mainLayout->addLayout(m_tipLayout);
}

void MgsBaseDialog::setCentralWidget(QWidget *centralWidget)
{
    m_mainLayout->takeAt(0);
    m_mainLayout->takeAt(0);
    delete m_centralWidget;
    m_centralWidget = centralWidget;
    m_mainLayout->addWidget(centralWidget, 1);
    m_mainLayout->addLayout(m_tipLayout);
}
