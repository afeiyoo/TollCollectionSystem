#pragma once

#include "defs.h"
#include <QObject>

namespace Utils {

class BizUtils : public QObject
{
    Q_OBJECT
public:
    explicit BizUtils(QObject *parent = nullptr);

    ~BizUtils() override;

    /*******************************************************/
    /****                   项目业务相关                 ****/
    /*******************************************************/
    // 获取不带颜色的车牌
    static QString getPlateNoColor(const QString &fullPlate);

    // 从车牌中获取并返回颜色编码
    static PlateColorType::PlateColor getColorCodeFromPlate(const QString &fullPlate);

    // 根据颜色编码获取车牌颜色 0-蓝 1-黄 2-黑 3-白 4-绿 5-拼 6-渐 7-临 9-未知
    static QString getColorFormColorCode(PlateColorType::PlateColor colorCode);

    /*******************************************************/
    /****                   DTP传输相关                  ****/
    /*******************************************************/
    // 依据对象属性，制作用于DTP传输的报文内容
    static QString makeDtpContentFromObj(const QObject &obj);

    // 依据QString，制作用于Dtp传输的报文内容
    static QString makeDtpContentFromStr(const QStringList &strList);

    // 生成完整的DTP传输报文
    static QString makeDtpXml(const QString &content,
                              const QString &businessId,
                              const QString &fromNode,
                              const QString &toNode,
                              int recordCount);
};

} // namespace Utils
