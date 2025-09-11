#pragma once

#include <QObject>
#include <QSqlQuery>

namespace Utils {

class DataDealUtils : public QObject
{
    Q_OBJECT
public:
    explicit DataDealUtils(QObject *parent = nullptr);
    ~DataDealUtils() override;

    /*******************************************************/
    /****                   数据加密相关                 ****/
    /*******************************************************/
    // MD5编码
    static QString cryptoMD5(const QString &s, bool bUtf8 = true);

    // Modbus CRC16校验
    static quint16 getModbus16(quint8 *data, int len);

    // XModem CRC16校验
    static quint16 getXModem16(quint8 *data, int len);

    // X25 CRC16校验
    static quint16 getX2516(quint8 *data, int len);

    // CRC16校验, type默认为3(X25 CRC16校验), 2(XModem CRC16校验), 1(Modbus CRC16校验)
    static QByteArray getCRCCode(const QByteArray &data, int type = 3);

    // 对字符串str进行异或加密
    static QString getXorEncryptDecrypt(const QString &str, char key);

    // 异或校验码
    static uchar getOrCode(const QByteArray &data);

    // 累加和校验码
    static uchar getCheckCode(const QByteArray &data);

    /*******************************************************/
    /****                   日期时间相关                 ****/
    /*******************************************************/
    // 获取当前时间字符串 默认格式: yyyy-MM-dd hh:mm:ss
    static QString curDateTimeStr(const QString &format = "yyyy-MM-dd hh:mm:ss");

    // 获取当前日期字符串 默认格式: yyyy-MM-dd hh:mm:ss
    static QString curDateStr(const QString &format = "yyyy-MM-dd hh:mm:ss");

    // 获取当前时间 格式为: yyyy-MM-dd hh:mm:ss
    static QDateTime curDateTime();

    // 获取当前日期 格式为: yyyy-MM-dd 00:00:00
    static QDateTime curDate();

    /*******************************************************/
    /****                   内容输出相关                 ****/
    /*******************************************************/

    // 数字转字符串，并在左侧补0，可指定数字最小宽度
    static QString padValue(quint32 value, int width = -1);

    // 16进制字符串转10进制
    static int strHexToDecimal(const QString &strHex);

    // 10进制字符串转10进制
    static int strDecimalToDecimal(const QString &strDecimal);

    // 2进制字符串转10进制
    static int strBinToDecimal(const QString &strBin);

    // 16进制字符串转2进制字符串
    static QString strHexToStrBin(const QString &strHex);

    // 10进制转2进制字符串一个字节
    static QString decimalToStrBin1(int decimal);

    // 10进制转2进制字符串两个字节
    static QString decimalToStrBin2(int decimal);

    // 10进制转16进制字符串,补零.
    static QString decimalToStrHex(int decimal);

    // int转字节数组((大端字节序)
    static QByteArray intToByte(int i);

    // int转字节数组(小端字节序)
    static QByteArray intToByteRec(int i);

    // 字节数组(大端字节序)转int
    static int byteToInt(const QByteArray &data);

    // 字节数组(小端字节序)转int
    static int byteToIntRec(const QByteArray &data);

    // 字节数组(大端字节序)转quint32
    static quint32 byteToUInt(const QByteArray &data);

    // 字节数组(小端字节序)转quint32
    static quint32 byteToUIntRec(const QByteArray &data);

    // ushort转字节数组
    static QByteArray ushortToByte(ushort i);

    // ushort转字节数组(小端字节序)
    static QByteArray ushortToByteRec(ushort i);

    // 字节数组转ushort
    static int byteToUShort(const QByteArray &data);

    // 字节数组(小端字节序)转ushort
    static int byteToUShortRec(const QByteArray &data);

    // 字节数组转Ascii字符串
    static QString byteArrayToAsciiStr(const QByteArray &data);

    // 16进制字符串转字节数组
    static QByteArray hexStrToByteArray(const QString &str);
    static char convertHexChar(char ch);

    // Ascii字符串转字节数组
    static QByteArray asciiStrToByteArray(const QString &str);

    // 字节数组转16进制字符串
    static QString byteArrayToHexStr(const QByteArray &data);

    // 去除字符串中的空格 type: -1-移除左侧空格 0-移除所有空格 1-移除右侧空格 2-移除首尾空格
    // 3-移除首尾空格,中间空格保留1个
    static QString trimmed(const QString &text, short type);

    // 获取字符串中的中文个数
    static int getChineseCountFromString(const QString &data, int checkLen);

    // 生成指定范围(0~doundary)的随机数
    // Qt5.10前的版本，在获取随机数前需要先调用 qsrand(seed), 生成随机数种子
    static int getRandomNum(quint32 boundary);

    /*******************************************************/
    /****                   SQL 相关                    ****/
    /*******************************************************/
    // 解析obj对象，获取对应的插入Sql
    static QString getInsertSql(QObject *obj);

    // 解析obj对象，获取对应的更新Sql
    static QString getUpdateSql(QObject *obj);

    // 输出完整的SQL语句（不含占位符）
    static QString fullExecutedQuery(const QSqlQuery &query);
};

} // namespace Utils
