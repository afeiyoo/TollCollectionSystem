#pragma once

#include <QWidget>

class MgsPlateEdit : public QWidget
{
    Q_OBJECT
public:
    explicit MgsPlateEdit(QWidget *parent = nullptr);
    ~MgsPlateEdit() override;

    // 获取车牌
    QString getPlate() const;
    // 设置车牌
    void setPlate(const QString &plate);
    // 光标移动
    void moveCursorTo(int index);
    // 获取当前光标位置
    int getCurrentIndex() const;
    // 删除当前光标所在字符
    void delCurChar();
    // 设置当前光标所在字符
    void setCurChar(const QString &value);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void doToggleCursor();

signals:
    void sigCursorPositionChanged(int index);

private:
    // 最大车牌长度
    const int m_maxPlateSize{9};
    // 当前光标位置
    int m_currentIndex;
    // 光标是否可见
    bool m_cursorVisible;
    // 光标闪烁计时器
    QTimer *m_blinkTimer{nullptr};
    // 车牌，带颜色，*表示空
    QStringList m_plate;
};
