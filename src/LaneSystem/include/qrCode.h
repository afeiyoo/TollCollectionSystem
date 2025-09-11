#ifndef QRCODE_H
#define QRCODE_H


#if defined(__cplusplus)
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
typedef struct  {
    /*
    车辆车牌号码+颜色.
    车牌号码+间隔符+车牌颜色
    间隔符：“_”,车牌颜色2位数字:
     0-蓝色，1-黄色，2-黑色，3-白色，4- 渐变绿色,5- 黄绿双拼色,6- 蓝白渐变色,7- 临时牌照,11-绿色,12-红色
    例：京A12345_1
    */
    char vehicleId[30];
    /* 入口收费站编号 */
    char enStationId[30];
    /* 出口收费站编号 */
    char exStationId[30];
    /* 入口重量.车货总重，单位：KG */
    int enWeight;
    /* 出口重量，若没有请填-1.车货总重，单位：KG */
    int exWeight;
    /* 通行介质  1位数字 1-OBU,2-CPC卡,3-纸券,9-无通行介质*/
    int mediaType;
    /* 出口交易编号*/
    char transactionId[50];
    /* 收费系统中产生的车辆通行ID.通行ID=通行介质ID+入口时间 */
    char passId[50];
    /* 出口交易时间 YYYY-MM-DDTHH:mm:ss*/
    char exTime[30];
    /* 交易支付方式 ETC交易时必填，其他交易可为空，但仍需要保留分隔符1位数字,
     * 1-出口ETC通行,2-出口ETC刷卡通行-1-空*/
    int transPayType;
    /* 总交易金额 用于通行省份拆分的金额*/
    long fee;
    /* 应收金额 其他交易时必填，ETC交易可为空,单位：分。应收金额=交易金额+优惠金额+
     * 省中心优惠金额*/
    long payFee;
    /* 车辆状态标识. 1位数字. 0x02-绿通车，0x03-联合收割机，0xFF-默认值 */
    char vehicleSign[20];
    /* 通行省份个数,大于等于 1 */
    char provinceCount[20];
    //图片路径
    char imgPath[50];
    //输出类型 0=数据流,1=指定路径存放图片,2=两种模式都有
    int type;
    //二维码宽度
    int width;
   }EtcExitInfo;
typedef struct {
    //处理结果 //1=分配内存错误,2=保存图片错误,3=生存二维码数据失败
    int result;
    //输出类型0=数据,1=指定路径存放图片,2=两种模式都有
    int type;
    //输出数据
    char imgData[1024*1024];
    //输出数据长度
    unsigned int imgDataLen;
}EtcExitInfoRes;

/*
*生成bmp格式的二维码图片和数据
*/
extern void generateQrCode(EtcExitInfo* etcExitInfo,EtcExitInfoRes  *etcExitInfoRes);

#if defined(__cplusplus)
}
#endif



#endif // QRCODE_H
