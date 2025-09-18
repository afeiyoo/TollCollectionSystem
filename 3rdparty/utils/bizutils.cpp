#include "bizutils.h"
#include "utils/datadealutils.h"

#include <QDate>
#include <QDebug>
#include <QMetaProperty>

using namespace Utils;

BizUtils::BizUtils(QObject *parent)
    : QObject{parent}
{}

Utils::BizUtils::~BizUtils() {}

QString BizUtils::getPlateNoColor(const QString &fullPlate)
{
    int pos = -1;
    QString checkData = fullPlate.mid(0, 1);

    if (DataDealUtils::getChineseCountFromString(fullPlate, 4) <= 1) {
        if ((pos = checkData.indexOf("白")) >= 0)
            return fullPlate.mid(pos + 1, fullPlate.length() - pos - 1);
        if ((pos = checkData.indexOf("黑")) >= 0)
            return fullPlate;
    }

    if ((pos = checkData.indexOf("蓝")) < 0 && (pos = checkData.indexOf("黄")) < 0
        && (pos = checkData.indexOf("白")) < 0 && (pos = checkData.indexOf("绿")) < 0
        && (pos = checkData.indexOf("拼")) < 0 && (pos = checkData.indexOf("渐")) < 0
        && (pos = checkData.indexOf("黑")) < 0 && (pos = checkData.indexOf("临")) < 0) {
        // 所有都没匹配到
        return fullPlate;
    }

    // 匹配到字符
    return fullPlate.mid(pos + 1, fullPlate.length() - pos - 1);
}

EM_PlateColor::PlateColor BizUtils::getColorCodeFromPlate(const QString &fullPlate)
{
    if (fullPlate.length() < 5)
        return EM_PlateColor::Unknown;
    if (DataDealUtils::getChineseCountFromString(fullPlate, 4) <= 1) {
        QString prefix = fullPlate.left(2);
        if (prefix.contains(QStringLiteral("白")))
            return EM_PlateColor::White;
        else
            return EM_PlateColor::Unknown;
    }
    QString checkData = fullPlate.mid(0, 1);
    if (checkData.contains(QStringLiteral("蓝")))
        return EM_PlateColor::Blue;
    else if (checkData.contains(QStringLiteral("黄")))
        return EM_PlateColor::Yellow;
    else if (checkData.contains(QStringLiteral("黑")))
        return EM_PlateColor::Black;
    else if (checkData.contains(QStringLiteral("白")))
        return EM_PlateColor::White;
    else if (checkData.contains(QStringLiteral("绿")))
        return EM_PlateColor::Green;
    else if (checkData.contains(QStringLiteral("拼")))
        return EM_PlateColor::Plain;
    else if (checkData.contains(QStringLiteral("渐")))
        return EM_PlateColor::Gradient;
    else if (checkData.contains(QStringLiteral("临")))
        return EM_PlateColor::Temporty;
    else
        return EM_PlateColor::Unknown;
}

QString BizUtils::getColorFormColorCode(EM_PlateColor::PlateColor colorCode)
{
    QStringList colors;
    colors << QStringLiteral("蓝") << QStringLiteral("黄") << QStringLiteral("黑") << QStringLiteral("白")
           << QStringLiteral("绿") << QStringLiteral("拼") << QStringLiteral("渐") << QStringLiteral("临");

    if (colorCode >= 8)
        return "";

    return colors.at(colorCode);
}

QString BizUtils::getKeyName(const QVariantMap &keyboard, uint keyCode)
{
    if (keyboard.isEmpty())
        return "";
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
    QString keyStr = metaEnum.valueToKey(keyCode);
    QVariantMap keyInfo = keyboard[keyStr].toMap();
    return keyInfo.value("Name", "").toString();
}

QString BizUtils::getKeyDesc(const QVariantMap &keyboard, uint keyCode)
{
    if (keyboard.isEmpty())
        return "";
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
    QString keyStr = metaEnum.valueToKey(keyCode);
    QVariantMap keyInfo = keyboard[keyStr].toMap();
    return keyInfo.value("Desc", "无效按键").toString();
}

int BizUtils::getKeyCode(const QVariantMap &keyboard, const QString &keyName)
{
    if (keyboard.isEmpty())
        return -1;
    QString enumName = keyboard.key(keyName);
    if (enumName.isEmpty())
        return -1;
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
    return metaEnum.keyToValue(enumName.toLocal8Bit().data());
}

QString BizUtils::makeDtpContentFromObj(const QObject &obj)
{
    QString content = "<record>";
    // 遍历 QObject 的属性，拼 content
    const QMetaObject *mo = obj.metaObject();
    int propCount = mo->propertyCount();
    for (int i = 0; i < propCount; ++i) {
        QMetaProperty prop = mo->property(i);
        const char *cname = prop.name();
        QString name = QString::fromLatin1(cname).toLower();

        // 跳过 Qt 自带的 objectName 属性
        if (name == QLatin1String("objectname"))
            continue;
        // 只要可读就取
        if (!prop.isReadable())
            continue;
        QVariant v = obj.property(cname);

        QString sValue;
        // 按类型格式化
        if (v.type() == QVariant::Date) {
            sValue = v.toDate().toString("yyyy-MM-dd");
        } else if (v.type() == QVariant::DateTime) {
            sValue = v.toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        } else {
            sValue = v.toString();
        }
        // 对日期/时间类型加花括号
        if (v.type() == QVariant::Date || v.type() == QVariant::DateTime) {
            sValue = "{" + sValue + "}";
        }

        // 拼成 <name>value</name>
        content += "<" + name + ">" + sValue + "</" + name + ">";
    }
    content += "</record>";

    return content;
}

QString BizUtils::makeDtpContentFromStr(const QStringList &strs)
{
    QString content;

    foreach (auto str, strs) {
        QString record = "<record>";
        QString val = QString("<![CDATA[%1]]>").arg(str);
        record += val;
        record += "</record>";

        content += record;
    }

    return content;
}

QString BizUtils::makeDtpXml(
    const QString &content, const QString &businessId, const QString &fromNode, const QString &toNode, int recordCount)
{
    QString result;

    result += QString(R"(<?xml version="1.0" encoding="gbk" ?><transfer><head>)");
    result += "<businessid>" + businessId + "</businessid>";
    result += "<fromnode>" + fromNode + "</fromnode>";
    result += "<tonode>" + toNode + "</tonode>";
    result += "<recordcount>" + QString::number(recordCount) + "</recordcount>";
    result += "</head><content>";
    result += content;
    result += "</content></transfer>";

    return result;
}
