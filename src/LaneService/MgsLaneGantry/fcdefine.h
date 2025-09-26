#ifndef FCDEFINE_H
#define FCDEFINE_H
/*
 * 路网中心定义的动态库接口及数据格式
 * 注意1：如果路网中心接口有变化，则FC_DATA_VERSION必须要变掉
 * 注意2：计费模块有更新，则FC_LIB_VERSION必须变掉
 */
#include "stdio.h"

//路网中心定义的数据结构版本号
#define  FC_DATA_VERSION   "20200320001"
//计费模块版本号（是否可以自已获取so文件名称来获取版本号？）
#define  FC_LIB_VERSION    "3520200915001"

#pragma pack(push,1)
struct TradingInfo
{
    char curTollGantry[20];   	//当前收费门架编码，当初始化函数中的当前门架编码为null时，应支持全省所有门架的费用查询（含代收）；初始化函数中的当前门架编码不为null时，该字段与初始化函数中的当前门架编码相等。
    char curPassTime[20];	    //当前收费门架经过时间，如：“2019-08-17T15:32:58“
    int laneStatus;	            //卡片或标签上获得的出入口车道状态，0x00-保留，0x 01-封闭E/M混合入口，0x02-封闭E/M混合出口，0x03-封闭ETC入口，0x04-封闭ETC出口，0x05-E/M混合开放式，0x06-ETC开放式，0x07～0F自定义，0x10-自助发卡，0x11～FF保留给未来使用，十六进制编码。
    char enTollStationHex[9];	//卡片或标签上获得的入口站Hex编码，如：“1101ABCD”
    char enPassTime[20];	    //入口时间
    char lastGantryHex[7];      //卡片或标签上获得的上一收费门架的HEX编码，如“1101AB”。（同一个收费单元的前后两个门架的HEX编码是相同的，所以实际上HEX编码与收费单元一一对应。）
    char lastPasstime[20];      //卡片或标签上获得的经过上一收费门架的时间
	int lastGantryFee;    //CPC 卡 EF02 文件中上个门架的计费金额，
                     	 //单位：分，ETC 无定义
	int lastFeeMileage;  //CPC 卡 EF02 文件中上个门架的计费里程，
	                     //单位：M（米），ETC 无定义
    char issuerId[7];           //发行服务机构编码，如四川工行：“510102”,CPC卡填“0”。
    char cardNetwork[5];		//卡网络编号：如“4401”，CPC卡填“0”。
    int vehicleUserType;		//车辆用户类型（CPU卡0015文件或单片式标签EF01） 0-普通车；6-公务车；8-军警车；10-紧急车；12-免费；14-车队；0～20内其他：自定义；对于道路运输证经营范围为“货物专用运输（集装箱）”且不含“普通货运”的牵引车、集装箱车定义为：Ox18，其他牵引车定义为：OxlB；应急救援车辆定义为： OxlA。
    int axleCount;		        //车辆轴数，CPC卡从DF01目录下EF01获取；电子标签从EF01获取（货车电子标签EF01无车辆轴数，填0）。
    int enAxleCount;	       //入口车辆轴数，入口写入的车辆轴数。CPC卡从DF01目录EF01获取；单片式标签从EF02获取，双片式标签从CPU卡0019文件获取。
    int tagType;		       //0-单片式标签 1-双片式ETC卡 2-CPC卡
    int cardType;		       //卡片类型，见CPU卡0015文件第9字节。非双片式或双片式标签无卡时填0。如：储值卡为22。
    int cardVer;	           //卡片版本号，见CPU卡0015文件第10字节。非双片式时填0。如：0x04。
    char obuSn[17];	            //OBU/CPC序号编码。OBU的EF01系统信息文件的合同序列号；CPC的EF01系统信息文件的CPC卡ID。如OBU ID号：4401123456789012。
    char cpuCardId[21];	       //双片式ETC卡号，如CPU卡：44011234567890123456
    char plateHex[25];	       //车牌号（16进制字符串格式）。卡签中读得的12字节车牌号扩展成24字节的16进制字符串。如：卡签中读得的GB2312编码的车牌“京A12345”时该字段填“BEA941313233343500000000”。
    unsigned short plateColor; //车牌颜色：高字节：0x00
    //低字节：0x00 –蓝色；0x01–黄色；0x02 –黑色；
    //0x03 –白色；0x04-渐变绿色；0x05-黄绿双拼色； 0x06-蓝白渐变；0x07～0xFF保留。如0x0001表示黄色。
    int vehicleType; 	//车型，若为CPC卡读取DF01目录下的EF01车型；若为电子标签读取EF01车型。
    //EF01文件车型相关描述：
    //①客车车型：
    //0x01–一类客车；0x02–二类客车；0x03–三类客车；0x04–四类客车；0x05–五类客车；0x06–六类客车；0x07～0x0A–自定义
    //②货车车型：0x0B–一类货车；0x0C–二类货车；0x0D–三类货车；0x0E–四类货车；0x0F–五类货车；0x10–六类货车；0x11～0x14–自定义
    //③专项作业车车型：0x15–一类专项作业车；0x16–二类专项作业车；0x17–三类专项作业车；0x18–四类专项作业车；0x19–五类专项作业车；0x1A–六类专项作业车；0x1B～0xFF–保留。注：若为牵引车，该字段写入牵引车车型。
    int enVehicleType;	//入口信息中的车型，双片式标签从0019文件获得，单片式标签从EF02文件获得，CPC从DF01目录下的EF01文件获得。
    int vehicleClass; 	//车种（仅CPC卡有效） 0-普通车；6-公务车；8-军警车；10-紧急车；12-免费；14-车队；0~20内其他：自定义应用； 21-绿通车；22-联合收割机 23-抢险救灾24-集装箱车 25-大件运输 26-应急保障车 27-货车列车或半挂汽车列车；28~255保留。
    int feeSumLocal;	//ETC储值卡当前卡面余额；CPC卡本省费用累计，单位：分
    int vehicleWeightLimits; //车辆限定载质量/座位数/准牵引总质量。由客车电子标签EFO1中车辆限定载重/座位数获得，由货车电子标签EF01中车辆核定载质量／准牵引总质量获得，质量单位为kg。CPC卡填0。
    int totalWeight;         //货车总重，单位：kg。总重信息双片式从0019文件获得，单片式标签从EF02文件获得，CPC从DF01目录下的EF01文件获得。
    int vehicleStatusFlag;   //车辆状态标识，从入口写入的文件中获得。车辆本次通行状态标识，OxOO -本次通行用于大件运输；OxOl -非优惠车辆； Ox02 -绿通车辆； Ox03 -联合收割机车辆； Ox04 -集装箱车辆。Ox05-0xFE -保留 注：可选填，默认写入OxFF。
	char feeProvBeginHex[9];// 本省入口 ETC 门架信息，（省界入 口 ETC 门架或省内第一个 ETC 门架的 6 位 HEX 编码，
	                        //收费站入口格式为“000000”），默认值为空表示此字
	                        //	段无效。
	int feeProvMileage;   //ETC 标签 EF04 文件或者 CPC EF02 文件
	                        //中的本省累计计费里程，单位：M（米），默认值-1 表
	                         //	示此字段无效。
	int feeProvGantryNum; //ETC 标签 EF04 文件中读出的本省累计
	                      //交易成功次数；CPC EF02 文件中读出的本省门架个
	                      //	数，默认值-1 表示此字段无效。
	int payFeeSumLocal; //ETC 标签 EF04 文件中读出的本省累计应
	                     //收金额；CPC 卡 EF02 文件中读出的本省累计金额，单
	                     //	位：分，默认填-1 表示此字段无效。 
	int realFeeSumLocal; //ETC 标签 EF04 文件中读出的本省累计实
	                      //收金额，单位：分，默认填-1 表示此字段无效。

    int gantryPassNum;   //CPC 卡 EF02 文件中过站信息中写入的门
	                      //架数量，ETC 默认为 0 
	char gantryPassHex[145];//CPC 卡 EF02 文件中过站信息，如本
	                      //门架之前经过 295F03，295F04 两个门架，则内容为：
	                     //	“295F03295F04”，ETC 默认为空
	int Spare1;          //计费模块保留输入字段 1
	int Spare2;          //计费模块保留输入字段 2
	int Spare3;           //计费模块保留输入字段 3
	char Spare4[50];      //计费模块保留输入字段 4,不支持汉字
	char Spare5[200];     //计费模块保留输入字段 5,不支持汉字
};
#pragma pack(pop)



#pragma pack(push,1)
struct FeeInfo
{
    int  payFee;	//应收金额（优惠前金额），单位：分
    int  discountFee; //优惠金额，单位：分
	int realFee; //实收金额，单位：分
	int cardFee; // ETC 卡面扣款金额（ETC 用户卡电子钱包扣款
	             //金额）；CPC 计费金额（含本门架计费金额、代
	             //	收门架金额和拟合前序漏收门架金额，同时用于
	             //	累计到 CPC 卡 EF02 文件中本省累计金额），单
	             //	位：分（注：旧版本字段名为 fee，仅进行字段
	             //	名修改但用途未变，避免与交易流水中字段 fee
	             //	混肴）。
	int feeProvSumLocal;//ETC 表示本省累计实收金额（含本门架
                     	//实收金额，即标签 EF04 文件中本省累计实收金
                     	//额）；CPC 表示本省累计拆分金额（含本门架计
  	                    //费金额，即 EF04 省份计费信息的本省累计计费
                     	//金额），单位：分。
	char paramVersion[14]; //计费参数版本，格式为“2位省份编码+yyyymmdd+3位顺序号”
	char feeSpecial[50]; //计费特情组合，无特情值时为空，详细
	                     //定义见“9.费率信息特情值定义” 多种特情以
                     	//	“|”分隔。
	int fitResult;     //本门架拟合结果 0-未拟合或拟合成功 1-拟合
	                    //失败。
    char tollIntervalIDs[1080];	//收费单元编号+处理标识，用于ETC复合消费或者CPC计费，若有代收或者拟合用“|”间隔，其中处理标识定义为 0-默认 1-本门架 2-没有前序门架的代收（代收门架为未建设或维护抢修） 3-拟合前序门架的代收（“上个门架-代收门架-本门架”的路径可确定为唯一路径）4-最短路径处理 5-最小费额处理，如：“G0015320030015201|G0015320030015203”
    char payFeeGroup[401];      // 收费单元应收金额（优惠前金额）组合，用于ETC复合消费或者CPC计费，单位：分，若有代收或者拟合用“|”分隔。如 ：“1000|2000”表示按照tollIntervalIDs中先后出现的两个收费单元分别应收费（优惠前）10元和20元。
    char discountFeeGroup[401]; //收费单元优惠金额组合，用于ETC复合消费或者CPC计费，单位：分，若有代收或者拟合用“|”分隔。如 ：“200|700”表示按照tollIntervalIDs中先后出现的两个收费单元分别应优惠2元和7元。
    char feeGroup[401];      //交易金额（应收金额减去优惠金额）组合，用于ETC复合消费或者CPC计费，单位：分，若有代收或者拟合用“|”分隔。如 ：“800|1300”表示按照tollIntervalIDs中先后出现的两个收费单元交易金额分别为8元和13元。
    char feeInfo1[401];      //计费信息1，省份自定义，ASCII编码（不支持汉字）计费模块可以使用自定义字段(计费信息1、2、3)返回优惠率、优惠类型及说明、错误码或特情等信息，该信息会在交易流水中上传省中心，用于省中心的自定义分析。推荐参照前面字段的样式。
    char feeInfo2[401];      //计费信息2，省份自定义，ASCII编码（不支持汉字）
    char feeInfo3[401];      //计费信息3，省份自定义，ASCII编码（不支持字）
    char feeLogMsg[501];     //计费模块中不写日志，若有需要通过该字段返回信息，门架系统写日志，ASCII编码（不支持汉字）
    int  feeVehicleType; 	//计费模块实际使用的车型，计费模	块可根据此字段返回本次交易使用的车型，车型	相关描述：
    //①客车车型：
    //0x01–一类客车；0x02–二类客车；0x03–三类	客车；0x04–四类客车；0x05–五类客车；	0x06–六类客车；0x07～0x0A–自定义
    //②货车车型：0x0B–一类货车；0x0C–二类货	车；0x0D–三类货车；0x0E–四类货车；0x0F–	五类货车；0x10–六类货车；0x11～0x14–自定	义
    //③专项作业车车型：0x15–一类专项作业车；	0x16–二类专项作业车；0x17–三类专项作业	车；0x18–四类专项作业车；0x19–五类专项作	业车；0x1A–六类专项作业车；0x1B～0xFF–保	留。注：若为牵引车，该字段写入牵引车车型。
    int feeMileage;  //本次计费里程，单位：1M（米）， 默认写0
    char provMinFee[10]; //省界出口计费模块可根据本省计费起
                         //点（见 7.2 费率信息的 feeProvBeginHex 字段）
                          //按最短路径或者最小费额等方式输出用于参考的
                          //累计通行金额，供异常分析使用。格式为：计算
                           //方式+“|”+累加通行金额；计算方式：1-最短路
                           //径 2-最小费额 3-其他，累加通行金额单位：分。 例:1|88888，默认为空
	char feeProvBeginHexFit[9];//拟合后的本省省界入口 ETC 门架
	                           //HEX 码。本省省界入口漏交易或交易失败的情况
	                          //	下，满足条件的省份可以拟合出本省省界入
	                           //	口，门架将其写入标签 EF04 或 CPC 卡 EF02 文
	                           //	件中的本省入口门架信息中，无需拟合或拟合
	                         //	失败填空。
	                            //	14
	char feeProvBeginTimeFit[20];//拟合后的本省省界入口 ETC 门 架的通行时间，此时间需在上个门架的通行时
	                             //间（输入参数 lastPasstime）与当前时间之
	                             //	间，如：“2020-03-20T15:32:58”，门架将其
	                                //	写入 CPC 卡 EF02 文件中的本省入口门架信息
	                              //	中，无需拟合或拟合失败填空。
	int fitProvFlag;//CPC 卡是否进行全省路径拟合的标识，0-未
                     //	拟合，大于 0 的值省内自行定义拟合方式。此
                	//	值为 0 时，以下 gantryPassNumFit ，
	                //	gantryPassHexFit，2 个字段均视为无效。
	int gantryPassNumFit;// CPC 卡进行全省路径拟合后的过站信
	                    //息 EF02 文件的过站信息中写入的门架数量。最 多 24 个门架。门架软件将此值写入到 EF02 文
	                   //	件中的第 29 字节。
	char gantryPassHexFit[145];// CPC 卡进行全省路径拟合后的
                             	//过站信息 EF02 文件中过站信息，如经过处理后
	                           //	还原出 295F03，295F05 两个门架，则内容为：
	                          //	“295F03295F05”。门架软件将此过站信息覆
	                          //	盖写入到 EF02 文件中的 30-101 字节。
	int feeSpare1;//计费模块保留输出字段 1
	int feeSpare2;//计费模块保留输出字段 2
	char feeSpare3[50];////扩展字段，，记录拟合相关信息，格式：全程最少费额路径拟合标志|拟合起始点|拟合结束点|拟合点数|拟合里程|拟合路径金额|速度|输入累计应收|输入累计实收
                       //全程最小费额路径拟合标志：
                       //0：正常拟合，非省内全程
                       //1：省内全程最少费额拟合，起点为入口收费站
                       //2：省内全程最少费额拟合，起点为省内第一个门架
                       //3：省内全程最少费额拟合，拟合到费额最少的入口省界门架
                       //4：省界出口，省内全程最少费额大于原有省内累计金额，替换
};
#pragma pack(pop)

#pragma pack(push,1)
struct FlagTradingInfo
{
    char curTollStationHex[20];	//当前收费站hex编码
    char curPassTime[20];	//当前收费门架经过时间，如：“2019-08-17T15:32:58“
    char enTollStationHex[9];	 //卡片或标签上获得的入口站Hex编码，如：“1101ABCD”
    char enPassTime[20];	//入口时间
    char lastGantryHex[7];//卡片或标签上获得的上一收费门架的HEX编码，如“1101AB”。（同一个收费单元的前后两个门架的HEX编码是相同的，所以实际上HEX编码与收费单元一一对应。）
    char lastPasstime[20];//卡片或标签上获得的经过上一收费门架的时间
    char issuerId[7];//发行服务机构编码，如四川工行：“510102”,CPC卡填“0”。
    int vehicleUserType;		//车辆用户类型（CPU卡0015文件或单片式标签EF01） 0-普通车；6-公务车；8-军警车；10-紧急车；12-免费；14-车队；0～20内其他：自定义；对于道路运输证经营范围为“货物专用运输（集装箱）”且不含“普通货运”的牵引车、集装箱车定义为：Ox18，其他牵引车定义为：OxlB；应急救援车辆定义为： OxlA。
    int axleCount;		//车辆轴数，CPC卡从DF01目录下EF01获取；电子标签从EF01获取（货车电子标签EF01无车辆轴数，填0）。
    int enAxleCount;	//入口车辆轴数，入口写入的车辆轴数。CPC卡从DF01目录EF01获取；单片式标签从EF02获取，双片式标签从CPU卡0019文件获取。
    int tagType;		//0-单片式标签 1-双片式ETC卡 2-CPC卡
    int cardType;		//卡片类型，见CPU卡0015文件第9字节。非双片式或双片式标签无卡时填0。如：储值卡为22。
    int cardVer;	//卡片版本号，见CPU卡0015文件第10字节。非双片式时填0。如：0x04。
    char plateHex[25];	//车牌号（16进制字符串格式）。卡签中读得的12字节车牌号扩展成24字节的16进制字符串。如：卡签中读得的GB2312编码的车牌“京A12345”时该字段填“BEA941313233343500000000”。
    unsigned short plateColor; //车牌颜色：高字节：0x00
    //低字节：0x00 –蓝色；0x01–黄色；0x02 –黑色；
    //0x03 –白色；0x04-渐变绿色；0x05-黄绿双拼色； 0x06-蓝白渐变；0x07～0xFF保留。如0x0001表示黄色。
    int vehicleType; 	//车型，若为CPC卡读取DF01目录下的EF01车型；若为电子标签读取EF01车型。
    //EF01文件车型相关描述：
    //①客车车型：
    //0x01–一类客车；0x02–二类客车；0x03–三类客车；0x04–四类客车；0x05–五类客车；0x06–六类客车；0x07～0x0A–自定义
    //②货车车型：0x0B–一类货车；0x0C–二类货车；0x0D–三类货车；0x0E–四类货车；0x0F–五类货车；0x10–六类货车；0x11～0x14–自定义
    //③专项作业车车型：0x15–一类专项作业车；0x16–二类专项作业车；0x17–三类专项作业车；0x18–四类专项作业车；0x19–五类专项作业车；0x1A–六类专项作业车；0x1B～0xFF–保留。注：若为牵引车，该字段写入牵引车车型。
    int enVehicleType;	//入口信息中的车型，双片式标签从0019文件获得，单片式标签从EF02文件获得，CPC从DF01目录下的EF01文件获得。
    int vehicleClass; 	//车种（仅CPC卡有效） 0-普通车；6-公务车；8-军警车；10-紧急车；12-免费；14-车队；0~20内其他：自定义应用； 21-绿通车；22-联合收割机 23-抢险救灾24-集装箱车 25-大件运输 26-应急保障车 27-货车列车或半挂汽车列车；28~255保留。
    int feeSumLocal;	//ETC储值卡当前卡面余额；CPC卡本省费用累计，单位：分
    int vehicleStatusFlag;//车辆状态标识，从入口写入的文件中获得。车辆本次通行状态标识，OxOO -本次通行用于大件运输；OxOl -非优惠车辆； Ox02 -绿通车辆； Ox03 -联合收割机车辆； Ox04 -集装箱车辆。Ox05-0xFE -保留 注：可选填，默认写入OxFF。

	char feeProvBeginHex[9];// 本省入口 ETC 门架信息，（省界入 口 ETC 门架或省内第一个 ETC 门架的 6 位 HEX 编码，
	//收费站入口格式为“000000”），默认值为空表示此字
	//	段无效。
	int feeProvMileage;   //ETC 标签 EF04 文件或者 CPC EF02 文件
	//中的本省累计计费里程，单位：M（米），默认值-1 表
	//	示此字段无效。
	int payFeeSumLocal; //ETC 标签 EF04 文件中读出的本省累计应
	//收金额；CPC 卡 EF02 文件中读出的本省累计金额，单
	//	位：分，默认填-1 表示此字段无效。 
	int realFeeSumLocal; //ETC 标签 EF04 文件中读出的本省累计实
	//收金额，单位：分，默认填-1 表示此字段无效。
    int gantryPassNum;   //CPC 卡 EF02 文件中过站信息中写入的门
	                      //架数量，ETC 默认为 0 
    char gantryPassHex[145];//CPC 卡 EF02 文件中过站信息，如本
	                      //门架之前经过 295F03，295F04 两个门架，则内容为：
	                     //	“295F03295F04”，ETC 默认为空


};
#pragma pack(pop)

#pragma pack(push,1)
struct FlagFeeInfo
{
    int FFShouldPay;	//应收金额（优惠前金额），单位：分
    int FFDiscountFee; //优惠金额，单位：分
    int FFTransFee;	//ETC储值卡卡面实际扣款金额（余额不足时赋值余额部分金额数值）；CPC实际计费金额，单位：分
    char paramVersion[14];//计费参数版本，格式为“2位省份编码+yyyymmdd+3位顺序号”
    char feeSpecial[50]; //计费特情组合，无特情值时为空，详细
    char ffIntervalIDs[851];	//收费单元编号+处理标识，用于ETC复合消费或者CPC计费，若有代收或者拟合用“|”间隔，其中处理标识定义为 0-默认 1-本门架 2-没有前序门架的代收（代收门架为未建设或维护抢修） 3-拟合前序门架的代收（“上个门架-代收门架-本门架”的路径可确定为唯一路径）4-最短路径处理 5-最小费额处理，如：“G0015320030015201|G0015320030015203”
    char ffIntervalPayFees[401];// 收费单元应收金额（优惠前金额）组合，用于ETC复合消费或者CPC计费，单位：分，若有代收或者拟合用“|”分隔。如 ：“1000|2000”表示按照tollIntervalIDs中先后出现的两个收费单元分别应收费（优惠前）10元和20元。
    char ffIntervalDiscounts[401];//收费单元优惠金额组合，用于ETC复合消费或者CPC计费，单位：分，若有代收或者拟合用“|”分隔。如 ：“200|700”表示按照tollIntervalIDs中先后出现的两个收费单元分别应优惠2元和7元。
    char ffIntervalFees[401];//交易金额（应收金额减去优惠金额）组合，用于ETC复合消费或者CPC计费，单位：分，若有代收或者拟合用“|”分隔。如 ：“800|1300”表示按照tollIntervalIDs中先后出现的两个收费单元交易金额分别为8元和13元。
    char feeInfo1[401];//计费信息1，省份自定义，ASCII编码（不支持汉字）计费模块可以使用自定义字段(计费信息1、2、3)返回优惠率、优惠类型及说明、错误码或特情等信息，该信息会在交易流水中上传省中心，用于省中心的自定义分析。推荐参照前面字段的样式。
    char feeInfo2[401];//计费信息2，省份自定义，ASCII编码（不支持汉字）
    char feeInfo3[401];//计费信息3，省份自定义，ASCII编码（不支持字）
    char feeLogMsg[501];//计费模块中不写日志，若有需要通过该字段返回信息，门架系统写日志，ASCII编码（不支持汉字）
    int FFVehClass; 	//计费模块实际使用的车型，计费模	块可根据此字段返回本次交易使用的车型，车型	相关描述：
    //①客车车型：
    //0x01–一类客车；0x02–二类客车；0x03–三类	客车；0x04–四类客车；0x05–五类客车；	0x06–六类客车；0x07～0x0A–自定义
    //②货车车型：0x0B–一类货车；0x0C–二类货	车；0x0D–三类货车；0x0E–四类货车；0x0F–	五类货车；0x10–六类货车；0x11～0x14–自定	义
    //③专项作业车车型：0x15–一类专项作业车；	0x16–二类专项作业车；0x17–三类专项作业	车；0x18–四类专项作业车；0x19–五类专项作	业车；0x1A–六类专项作业车；0x1B～0xFF–保	留。注：若为牵引车，该字段写入牵引车车型。
    int feeMileage;  //本次计费里程，单位：1M（米）， 默认写0

    int FFFactPay;  //实收金额
    int rateCompute;  //计费处理模式，1-无拟合路径，2-拟合路径，默认0
    int rateFitCount;  //计费拟合点数，无拟合填0，拟合则按拟合实际点数
    int obuFeeType;   //OBU计费方式，0-保留，1-无代收，2-门架代收，无二义//路径，3-门架代收且最短路径计费，4-门架代收且最小//费额计费
    char ffIntervalNames[1401];   //收费单元名称,多个收费单元用“|”间隔
    char flagIDs[401];          //收费门架hex编码,多个收费门架用“|”间隔
    char flagNames[1401];        //收费门架名称,多个收费门架用“|”间隔
	char flagPayFees[401];      // 收费门架应收金额组合
	char flagDiscounts[401];   //收费门架优惠金额组合
	char flagFees[401];         //收费门架交易金额（应收金额减去优惠金额）组合

	int fitResult;     //本门架拟合结果 0-未拟合或拟合成功 1-拟合
	int feeProvRealSum;//ETC 表示本省累计实收金额，即标签 EF04 文件中本省累计实收金
	                   //额）；CPC 表示本省累计拆分金额,即 EF04 省份计费信息的本省累计计费//金额），单位：分。

	int feeProvPaySum;//表示本省累计应收金额
	int feeProvMileage;//表示本省累计里程
	char feeSpare3[50];//扩展字段，//扩展字段，记录拟合相关信息，格式：全程最少费额路径拟合标志|拟合起始点|拟合结束点|拟合点数|拟合里程|拟合路径金额|速度|输入累计应收|输入累计实收
                       //全程最小费额路径拟合标志：
                       //0：正常拟合，非省内全程
                       //1：省内全程最少费额拟合，起点为入口收费站
                       //2：省内全程最少费额拟合，起点为省内第一个门架
                       //3：省内全程最少费额拟合，拟合到费额最少的入口省界门架
                       //4：省界出口，省内全程最少费额大于原有省内累计金额，替换

    int gantryPassNumFit;// CPC 卡进行全省路径拟合后的过站信
	                    //息 EF02 文件的过站信息中写入的门架数量。最 多 24 个门架。门架软件将此值写入到 EF02 文
	                   //	件中的第 29 字节。
	char gantryPassHexFit[145];// CPC 卡进行全省路径拟合后的
                             	//过站信息 EF02 文件中过站信息，如经过处理后
	                           //	还原出 295F03，295F05 两个门架，则内容为：
	                          //	“295F03295F05”。门架软件将此过站信息覆
	                          //	盖写入到 EF02 文件中的 30-101 字节。
};
#pragma pack(pop)


#pragma pack(push,1)
struct T_ParamVersion
{
    int ParamType;
    int Version;
    char VersionStr[50];
    char EnableTime[20];
    char DisableTime[20];
    int IsUsed;
};
#pragma pack(pop)

#pragma pack(push,1)
struct T_ETCFlag
{
    char FlagID[10];
    char GantryID[30];
    int FlagIndex;
    char FlagName[50];
    char FlagPlate[30];
    int FlagType;
    int RoadWay;
    int IsUsed;
    int IsVirtual;
};
#pragma pack(pop)

#pragma pack(push,1)
struct T_FlagFee
{
    int Version;
    char IntervalID[30];
    char IntervalName[80];
    char FlagID[10];
    int Fee1;
    int Fee2;
    int Fee3;
    int Fee4;
    int Fee5;
    int Fee6;
    int Fee11;
    int Fee12;
    int Fee13;
    int Fee14;
    int Fee15;
    int Fee16;
    int Fee17;
    int Fee21;
    int Fee22;
    int Fee23;
    int Fee24;
    int Fee25;
    int Fee26;
    int Fee27;
    int ChkPoint;
    int Distance;
};
#pragma pack(pop)

#pragma pack(push,1)
struct T_HolidayFree
{
    int HFID;
    char StartTime[20];
    char StopTime[20];
    int Vehclass;
};
#pragma pack(pop)

#pragma pack(push,1)
struct T_FF_Node
{
    char NodeID[10];
    char NodeName[100];
    int NodeType;
    int IsUsed;
};
#pragma pack(pop)

#pragma pack(push,1)
struct T_FF_NodeRelation
{
    char EnNodeID[10];
    char ExNodeID[10];
};
#pragma pack(pop)


#pragma pack(push,1)
struct T_ParamAxisRatio
{
    int StartAxises;
    int StopAxises;
    double AxisRatio;
};
#pragma pack(pop)


#pragma pack(push,1)
struct T_DiscountFee
{
    int DiscountType;
    char DiscountName[30];
    double Discount;
};
#pragma pack(pop)



#define NO_ERROR            0  //正常
#define PARMFILE_ABSENT   -10  //省份计费参数缺失
#define PARMFILE_INVALID  -11  //省份计费参数存在但无效
#define INVALID_IN_PARM   -12  //输入参数不正常
#define APPLY_MEM_FAILED  -13  //申请内存失败

//-20~-99 省份计费模块错误码
#define ERR_SYSTEM        -20  //模块系统错,不具备访问条件
#define ERR_BASEFEE       -21  //基础费率参数错
#define ERR_FLAGMAP       -22  //门架基础信息映射错
#define ERR_WHITELIST     -23  //WHITELIST无效
#define ERR_VEHICLETYPE   -24  //无效车型
#define ERR_GANTRYFEE     -25  //找不到门架对应的费率
#define ERR_AXLECOUNT     -26

extern "C" {
int  init(const char* curTollGantry, const char* soPath);  //初始化
int  deInit();
int  getLibVersion(char* version,int verLen,char* dataVersion, int dataVerLen);
int  getFeeDataVersion(char* version,int verLen);
int  getFee(const TradingInfo &tradingInfo,FeeInfo &feeInfo);
int  loadFlagFee(const T_ParamVersion *p1=NULL,int p1cnt=0,
                 const T_ETCFlag *p2=NULL,int p2cnt=0,
                 const T_FlagFee *p3=NULL,int p3cnt=0,
                 const T_HolidayFree *p4=NULL,int p4cnt=0,
                 const T_FF_Node *p5=NULL,int  p5cnt=0,
                 const T_FF_NodeRelation *p6=NULL,int  p6cnt=0,
                 const T_ParamAxisRatio *p7=NULL,int  p7cnt=0,
                 const T_DiscountFee *p8=NULL,int  p8cnt=0);
int getFlagFee(const FlagTradingInfo &tradingInfo, FlagFeeInfo &feeInfo);
}



#endif // FCDEFINE_H
