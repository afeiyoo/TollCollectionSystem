#pragma once

#include <QFlags>
#include <QObject>

#define Q_BEGIN_ENUM_CREATE(CLASS) \
    class CLASS : public QObject \
    { \
        Q_OBJECT \
    public:

#define Q_END_ENUM_CREATE(CLASS) \
private: \
    Q_DISABLE_COPY(CLASS) \
    } \
    ;

#define Q_ENUM_CREATE(CLASS) Q_ENUM(CLASS)

// 车牌颜色
Q_BEGIN_ENUM_CREATE(PlateColorType)
enum PlateColor {
    Blue = 0x0000,
    Yellow = 0x0001,
    Black = 0x0002,
    White = 0x0003,
    Green = 0x0004,
    Plain = 0x0005,    // 拼
    Gradient = 0x0006, // 渐
    Temporty = 0x0007, // 临
    Unknown = 0x0009
};
Q_ENUM_CREATE(PlateColor)
Q_END_ENUM_CREATE(PlateColorType)
