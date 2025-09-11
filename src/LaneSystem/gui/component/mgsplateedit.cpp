#include "mgsplateedit.h"

#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

#include "global/constant.h"

MgsPlateEdit::MgsPlateEdit(QWidget *parent)
    : QWidget{parent}
    , m_currentIndex{0}
    , m_cursorVisible{true}
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 车牌内容长度最长是9位
    for (int i = 0; i < 9; i++)
        m_plate << Constant::Character::EMPTY_CHARACTER;

    // 光标闪烁
    m_blinkTimer = new QTimer(this);
    connect(m_blinkTimer, &QTimer::timeout, this, &MgsPlateEdit::doToggleCursor);
    m_blinkTimer->start(500);

    resize(300, 50);
}

MgsPlateEdit::~MgsPlateEdit() {}

QString MgsPlateEdit::getPlate() const
{
    QString var;
    var = m_plate.join("").trimmed().replace(Constant::Character::EMPTY_CHARACTER, "");
    return var;
}

void MgsPlateEdit::setPlate(const QString &plate)
{
    int plateSize = plate.size();
    if (plateSize > 9)
        plateSize = m_maxPlateSize;
    for (int i = 0; i < plateSize; ++i) {
        m_plate[i] = plate[i];
    }
    update();
}

void MgsPlateEdit::moveCursorTo(int index)
{
    if (index < 0)
        index = 0;
    if (index >= m_maxPlateSize)
        index = m_maxPlateSize - 1;
    if (index >= 0 && index < m_maxPlateSize) {
        m_currentIndex = index;
        emit sigCursorPositionChanged(m_currentIndex);
        m_cursorVisible = true;
        update();
    }
}

int MgsPlateEdit::getCurrentIndex() const
{
    return m_currentIndex;
}

void MgsPlateEdit::delCurChar()
{
    if (m_currentIndex == 0) {
        // 光标在第0位，只清空当前位置
        m_plate[m_currentIndex] = Constant::Character::EMPTY_CHARACTER;

    } else if (m_currentIndex == m_maxPlateSize - 1 && m_plate[m_currentIndex] != Constant::Character::EMPTY_CHARACTER) {
        // 光标在第8索引（第9位），且当前位有内容，先删除当前位，不移动光标
        m_plate[m_currentIndex] = Constant::Character::EMPTY_CHARACTER;
        moveCursorTo(m_currentIndex);
    } else {
        // 删除当前位置前一位，光标左移
        m_currentIndex--;
        m_plate[m_currentIndex] = Constant::Character::EMPTY_CHARACTER;
        moveCursorTo(m_currentIndex);
    }
}

void MgsPlateEdit::setCurChar(const QString &value)
{
    if (m_currentIndex >= 0 && m_currentIndex < m_maxPlateSize) {
        m_plate[m_currentIndex] = value;
        moveCursorTo(m_currentIndex + 1);
    }
}

void MgsPlateEdit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QColor fontColor = Qt::black;
    QColor cursorColor = Qt::white;

    // 根据首位字符选择背景图片
    QString color = m_plate.at(0);
    QString bgImagePath;

    if (color == QStringLiteral("黄")) {
        bgImagePath = Constant::Path::YELLOW_PLATE;
        fontColor = Qt::black;
    } else if (color == QStringLiteral("蓝")) {
        bgImagePath = Constant::Path::BLUE_PLATE;
        fontColor = Qt::white;
    } else if (color == QStringLiteral("黑")) {
        bgImagePath = Constant::Path::BLACK_PLATE;
        fontColor = Qt::white;
    } else if (color == QStringLiteral("白")) {
        bgImagePath = Constant::Path::WHITE_PLATE;
        fontColor = Qt::black;
        cursorColor = Qt::black;
    } else if (color == QStringLiteral("临")) {
        bgImagePath = Constant::Path::TEMP_PLATE;
        fontColor = Qt::black;
    } else if (color == QStringLiteral("渐")) {
        bgImagePath = Constant::Path::GRADIENT_PLATE;
        fontColor = Qt::black;
    } else if (color == QStringLiteral("绿")) {
        bgImagePath = Constant::Path::GREEN_PLATE;
        fontColor = Qt::black;
    } else if (color == QStringLiteral("拼")) {
        bgImagePath = Constant::Path::MIX_PLATE;
        fontColor = Qt::black;
    } else {
        bgImagePath = Constant::Path::WHITE_PLATE;
        fontColor = Qt::black;
        cursorColor = Qt::black;
    }

    // 绘制背景图片
    QPixmap bgPixmap(bgImagePath);
    if (!bgPixmap.isNull()) {
        painter.drawPixmap(rect(), bgPixmap);
    }

    // 绘制字体
    QFont font = painter.font();
    font.setPixelSize(23);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(fontColor);

    int charWidth = width() / m_maxPlateSize;
    int charHeight = height();

    for (int i = 0; i < m_maxPlateSize; ++i) {
        QRect charRect(i * charWidth, 0, charWidth, charHeight);
        if (m_plate.at(i).size() >= 2) {
            QFont tempFont = painter.font();
            tempFont.setPixelSize(15);
            painter.setFont(tempFont);
        } else {
            painter.setFont(font);
        }
        painter.drawText(charRect, Qt::AlignCenter, m_plate.at(i));

        // 光标绘制
        if (i == m_currentIndex && m_cursorVisible) {
            painter.setPen(QPen(cursorColor, 3));
            int cursorWidth = charWidth / 2;
            int cursorX = charRect.center().x() - cursorWidth / 2;
            int cursorY = charRect.bottom() - 10;
            painter.drawLine(cursorX, cursorY, cursorX + cursorWidth, cursorY);
            painter.setPen(fontColor);
        }
    }
}

void MgsPlateEdit::doToggleCursor()
{
    m_cursorVisible = !m_cursorVisible;
    update();
}
