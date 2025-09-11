#include "windows.h"
#pragma once
struct EtcExitInfo {
    /*
    车辆车牌号码+颜色.
    车牌号码+间隔符+车牌颜色
    间隔符：“_”,车牌颜色2位数字:
     0-蓝色，1-黄色，2-黑色，3-白色，4- 渐变绿色,5- 黄绿双拼色,6- 蓝白渐变色,7- 临时牌照,11-绿色,12-红色
    例：京A12345_1
    */
    char* vehicleId;

    /* 入口收费站编号 */
    char* enStationId;

    /* 出口收费站编号 */
    char* exStationId;

    /* 入口重量.车货总重，单位：KG */
    int enWeight;

    /* 出口重量，若没有请填-1.车货总重，单位：KG */
    int exWeight;

    /* 通行介质  1位数字 1-OBU,2-CPC卡,3-纸券,9-无通行介质*/
    int mediaType;

    /* 出口交易编号*/
    char* transactionId;

    /* 收费系统中产生的车辆通行ID.通行ID=通行介质ID+入口时间 */
    char* passId;

    /* 出口交易时间 YYYY-MM-DDTHH:mm:ss*/
    char* exTime;

    /* 交易支付方式 ETC交易时必填，其他交易可为空，但仍需要保留分隔符1位数字,1-出口ETC通行,2-出口ETC刷卡通行*/
    int transPayType;

    /* 总交易金额 用于通行省份拆分的金额*/
    long fee;

    /* 应收金额 其他交易时必填，ETC交易可为空,单位：分。应收金额=交易金额+优惠金额+省中心优惠金额*/
    long payFee;

    /* 车辆状态标识. 1位数字. 0x02-绿通车，0x03-联合收割机，0xFF-默认值 */
    char* vehicleSign;

	char* provinceCount;
};

/*
生成查验端APP需要扫描的出口信息二维码，返回HBITMAP-Windows位图对象的句柄
@param text gbk格式编码的字符串，程序会自动将其转换为utf-8编码
@width 二维码的宽度
*/
// dll中generateQrCodeAndReturnHBITMAP函数的指针
typedef HBITMAP (__stdcall* function_generateQrCodeAndReturnHBITMAP)(EtcExitInfo etcExitInfo, int width);

