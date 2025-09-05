#include "bizhandler.h"

#include "HttpClient/src/http.h"
#include "Logger.h"
#include "QJson/include/qobjecthelper.h"
#include "bean/t_etcout.h"
#include "bean/t_laneinputshift.h"
#include "bean/t_mtcout.h"
#include "bean/t_specialcards.h"
#include "bean/t_splitout.h"
#include "bend/dtpsender.h"
#include "config/baseexception.h"
#include "config/config.h"
#include "global/globalmanager.h"
#include "utils/bizutils.h"
#include "utils/datadealutils.h"
#include "utils/fileutils.h"

QMap<QString, ST_AuditInfo> BizHandler::m_auditInfos;

BizHandler::BizHandler(QObject *parent)
    : QObject{parent}
{
    // 每次有新请求到来时，清除30天前的图片文件
    Utils::FileUtils::autoDeleteFiles(GM_INSTANCE->m_pictureDir.toString(), ".jpg", 30);
    // Dtp发送对象初始化
    m_dtpSender = new DtpSender(this);
    m_dtpSender->initDtp("./libDtp-Client.so");
}

BizHandler::~BizHandler() {}

QString BizHandler::doMainDeal(int cmdType, const QVariantMap &dataMap, const QByteArray &reqBody)
{
    QString dealtData;
    switch (cmdType) {
    case 1:
        dealtData = doDealCmd01(dataMap); // 稽核登录
        break;
    case 18:
        dealtData = doDealCmd18(dataMap); // 路径展示
        break;
    case 19:
        dealtData = doDealCmd19(dataMap); // 绿通查验
        break;
    case 20:
        dealtData = doDealCmd20(dataMap); // 获取图片
        break;
    case 24:
        dealtData = doDealCmd24(reqBody); // 车牌识别
        break;
    case 25:
        dealtData = doDealCmd25(dataMap); // 获取状态名单
        break;
    case 26:
        dealtData = doDealCmd26(dataMap); // 交接班数据
        break;
    case 27:
        dealtData = doDealCmd27(dataMap); // 特情记录
        break;
    case 28:
        dealtData = doDealCmd28(dataMap); // 查询工号
        break;
    case 30:
        dealtData = doDealCmd30(dataMap); // 查询逃费车辆欠费信息
        break;
    case 39:
        dealtData = doDealCmd39(dataMap); // 本站绿通流水查询
        break;
    default:
        break;
    }
    return dealtData;
}

QString BizHandler::doDealCmd01(const QVariantMap &aMap)
{
    QString stationID;
    int laneID = 0;
    QString operatorID;

    if (aMap.contains("stationID"))
        stationID = aMap["stationID"].toString();
    if (aMap.contains("laneID"))
        laneID = aMap["laneID"].toInt();
    if (aMap.contains("operatorID"))
        operatorID = aMap["operatorID"].toString();

    if (stationID.isEmpty())
        throw BaseException(1, "响应失败: 站代码为空");

    QString stationName = m_ds.getStationName("3501" + stationID);

    QString msg = QString("%1(%2), 车道号: %3, 工号: %4").arg(stationName, stationID).arg(laneID).arg(operatorID);
    LOG_INFO().noquote() << QString("%1 稽核业务登录成功").arg(msg);

    QVariantMap resMap;
    resMap["loginStatus"] = 0;
    resMap["desc"] = msg;
    resMap["loginCode"] = QString("%1%2").arg(stationID, Utils::DataDealUtils::padValue(laneID, 2));

    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(resMap);
    return dealtData;
}

QString BizHandler::doDealCmd18(const QVariantMap &aMap)
{
    QString qrCodeMsg;
    QString plateNumber;

    if (aMap.contains("qrCodeMsg"))
        qrCodeMsg = aMap["qrCodeMsg"].toString();
    if (aMap.contains("plateNumber"))
        plateNumber = aMap["plateNumber"].toString();

    QString dealtData;
    if (!qrCodeMsg.isEmpty()) {
        dealtData = getVehicleWayFromScan(qrCodeMsg);
    } else if (!plateNumber.isEmpty()) {
        dealtData = getVehicleWayFromPlate(plateNumber);
    } else {
        throw BaseException(1, "响应失败: 车牌号或二维码为空");
    }

    return dealtData;
}

QString BizHandler::getVehicleWayFromPlate(const QString &plateNumber)
{
    T_EtcOut objEtc;
    T_MtcOut objMtc;

    QString passID;
    QString tradeID;
    QString enTime;
    QString enNetHex;
    QString enHex;
    QString enName;
    int exitFeeType = 0, mediaType = 3, vehClass = 0, userType = 0, axisCount = 0;
    int shouldPay = 0, discount = 0, factPay = 0;

    ST_Node exNode;
    exNode.nodeType = 1;
    if (m_ds.getLatestOutTrade(plateNumber, &objEtc, 0)) {
        // ETC出口记录
        if (objEtc.ExShiftDate <= QDateTime::fromString("20200101", "yyyyMMdd")) {
            LOG_INFO().noquote() << QString("取消省界站前数据无效 %1").arg(objEtc.ExShiftDate.toString("yyyy-MM-dd"));
            throw BaseException(1, "响应失败: 数据无效(取消省界站前数据)");
        }
        passID = objEtc.PassID;
        tradeID = objEtc.TradeID;
        exitFeeType = objEtc.ExitFeeType;
        vehClass = objEtc.ExVehClass;
        userType = objEtc.UserType;
        axisCount = objEtc.Axises;
        shouldPay = objEtc.ShouldPay * 100;
        factPay = objEtc.FactPay * 100;
        discount = objEtc.Discount * 100;
        enTime = objEtc.EnTime.toString("yyyy-MM-dd HH:mm:ss");
        enHex = objEtc.EnStation;
        enNetHex = QString::number(objEtc.EnNetID);
        enName = objEtc.EnStationName;
        if (passID.left(2) == "01") {
            mediaType = 0;
        } else if (passID.left(2) == "02") {
            mediaType = 1;
        }

        // 出口门架节点信息
        exNode.captureId = objEtc.TradeID;
        exNode.nodeHex = objEtc.ExStation;
        exNode.nodeId = objEtc.CNLaneID.left(14);
        exNode.nodeName = objEtc.ExStationName;
        exNode.passTime = objEtc.ExTime.toString("yyyy-MM-dd hh:mm:ss");
    } else if (m_ds.getLatestOutTrade(plateNumber, &objMtc, 1)) {
        // MTC出口记录
        if (objMtc.ExShiftDate <= QDateTime::fromString("20200101", "yyyyMMdd")) {
            LOG_INFO().noquote() << QString("取消省界站前数据无效 %1").arg(objMtc.ExShiftDate.toString("yyyy-MM-dd"));
            throw BaseException(1, "响应失败: 数据无效(取消省界站前数据)");
        }
        passID = objMtc.PassID;
        tradeID = objMtc.TradeID;
        exitFeeType = objMtc.ExitFeeType;
        vehClass = objMtc.ExVehClass;
        userType = objMtc.UserType;
        axisCount = objMtc.Axises;
        shouldPay = objMtc.ShouldPay * 100;
        factPay = objMtc.FactPay * 100;
        discount = objMtc.Discount * 100;
        enTime = objMtc.EnTime.toString("yyyy-MM-dd HH:mm:ss");
        enHex = objMtc.EnStation;
        enNetHex = QString::number(objMtc.EnNetID);
        enName = objMtc.EnStationName;
        if (passID.left(2) == "01") {
            mediaType = 0;
        } else if (passID.left(2) == "02") {
            mediaType = 1;
        }

        // 出口门架节点信息
        exNode.captureId = objMtc.TradeID;
        exNode.nodeHex = objMtc.ExStation;
        exNode.nodeId = objMtc.CNLaneID.left(14);
        exNode.nodeName = objMtc.ExStationName;
        exNode.passTime = objMtc.ExTime.toString("yyyy-MM-dd hh:mm:ss");
    } else {
        throw BaseException(1, "响应失败：未查询到相关记录");
    }

    if (passID.isEmpty())
        throw BaseException(1, "响应失败: 获取出口信息失败(passid为空)");

    LOG_INFO().noquote() << QString("查询出口信息结果返回: exitFeeType %1, passID %2, name %3, tradeId %4")
                                .arg(exitFeeType)
                                .arg(passID, exNode.nodeName, tradeID);

    // 查询入口信息
    ST_Node enNode;
    enNode.nodeType = 1;
    QVariantMap enInfo = m_ds.getEnInfo(passID);
    if (!enInfo.isEmpty()) {
        enNode.captureId = enInfo["TRADEID"].toString();
        enNode.passTime = enInfo["ENTIME"].toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        enNode.nodeHex = enInfo["ENSTATION"].toString();
        enNode.nodeName = enInfo["STATIONNAME"].toString();
        enNode.nodeId = enInfo["CNLANEID"].toString().left(14);
    }

    if (enInfo.isEmpty() || enNode.captureId.isEmpty()) {
        LOG_INFO().noquote() << "查询入口信息为空，使用出口记录中的入口信息";
        enNode.captureId = "";
        enNode.passTime = enTime;
        enNode.nodeHex = enNetHex + enHex;
        enNode.nodeId = m_ds.getGantryNodeID(enNode.nodeHex);
        enNode.nodeName = enName;
    }
    LOG_INFO().noquote() << QString("查询入口信息结果返回 passID %1, name %2").arg(passID, enNode.nodeName);

    // 组装发送报文
    ST_PathFitting pathFitting;
    pathFitting.plateNumber = Utils::BizUtils::getPlateNoColor(plateNumber);
    pathFitting.plateColor = Utils::BizUtils::getColorCodeFromPlate(plateNumber);
    pathFitting.mediaType = mediaType;
    pathFitting.vehicleType = vehClass;
    pathFitting.vehicleClass = userType;
    pathFitting.receivableMoney = shouldPay;
    pathFitting.tradingMoney = factPay;
    pathFitting.discountsMoney = discount;
    pathFitting.axesCount = axisCount;
    pathFitting.enNode = packNode(enNode);
    pathFitting.exNode = packNode(exNode);
    if (exitFeeType == 1 || exitFeeType == 2 || exitFeeType == 3 || exitFeeType == 11) {
        getGantryNodes(passID, pathFitting.gantryUnits);
    } else if (exitFeeType == 4 || exitFeeType == 5) {
        getGantryNodeSplitOut(tradeID, pathFitting.tollIntervalIDs, pathFitting.tollIntervalIDsTime);
    }
    QString sendJson = packFeePath(pathFitting, exitFeeType);
    LOG_INFO().noquote() << QString("发送地图路径请求 %1").arg(sendJson);

    QUrl url(GM_INSTANCE->m_config->m_baseConfig.mapUrl);
    auto reply = Http().instance().post(url, sendJson.toUtf8(), "application/json");

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());
    LOG_INFO().noquote() << QString("接收地图路径完成 %1").arg(result.left(1024));

    QVariantMap resMap = GM_INSTANCE->m_jsonParser->parse(result.toUtf8()).toMap();
    QVariantMap bizContent = resMap.value("bizContent", {}).toMap();
    QByteArray sendData = GM_INSTANCE->m_jsonSerializer->serialize(bizContent);

    return sendData;
}

QString BizHandler::getVehicleWayFromScan(const QString &scan)
{
    QVariantMap tempMap;
    tempMap.insert("pathFittingData", scan);
    QVariantMap aMap;
    aMap.insert("method", "getPathFittingByPathId");
    aMap.insert("sign", "");
    aMap.insert("signType", "NONE");
    aMap.insert("version", "1.0");
    aMap.insert("timestamp", Utils::DataDealUtils::curDateTimeStr());
    aMap.insert("encryptType", "NONE");
    aMap.insert("bizContent", GM_INSTANCE->m_jsonSerializer->serialize(tempMap));

    QString sendJson = GM_INSTANCE->m_jsonSerializer->serialize(aMap);
    LOG_INFO().noquote() << QString("传扫描内容：%1").arg(sendJson);

    QUrl url(GM_INSTANCE->m_config->m_baseConfig.mapUrl);
    auto reply = Http().instance().post(url, sendJson.toUtf8(), "application/json");

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());
    LOG_INFO().noquote() << QString("返回扫描结果: %1").arg(result);

    QVariantMap resMap = GM_INSTANCE->m_jsonParser->parse(result.toUtf8()).toMap();
    QVariant bizContent;
    if (resMap.contains("bizContent"))
        bizContent = resMap["bizContent"];

    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(bizContent);
    return dealtData;
}

QString BizHandler::packFeePath(const ST_PathFitting &pathFitting, int exitFeeType)
{
    QVariantMap map;

    map["plateNumber"] = pathFitting.plateNumber.toStdString().c_str();
    map["plateColor"] = pathFitting.plateColor;
    map["mediaType"] = pathFitting.mediaType;
    map["vehicleType"] = pathFitting.vehicleType;
    map["vehicleClass"] = pathFitting.vehicleClass;
    map["receivableMoney"] = pathFitting.receivableMoney;
    map["tradingMoney"] = pathFitting.tradingMoney;
    map["discountsMoney"] = pathFitting.discountsMoney;
    map["axesCount"] = pathFitting.axesCount;
    map["enNode"] = GM_INSTANCE->m_jsonParser->parse(pathFitting.enNode.toUtf8()).toMap();
    map["exNode"] = GM_INSTANCE->m_jsonParser->parse(pathFitting.exNode.toUtf8()).toMap();

    QString gantrys;
    if (exitFeeType == 3 || exitFeeType == 2 || exitFeeType == 1 || exitFeeType == 11) { // 卡内计费
        QVariantList unitsList;
        QVariantMap unitMap;
        for (const auto &unit : pathFitting.gantryUnits) {
            if (gantrys.isEmpty())
                gantrys += ",";
            gantrys += unit;

            unitMap = GM_INSTANCE->m_jsonParser->parse(unit.toUtf8()).toMap();
            unitsList.append(unitMap);
        }

        if (!gantrys.isEmpty())
            gantrys = "[" + gantrys + "]";
        map.insert("gantryUnits", unitsList);

    } else if (exitFeeType == 4 || exitFeeType == 5) { // 在线计费
        if (!pathFitting.tollIntervalIDs.isEmpty())
            map["tollIntervalIDs"] = pathFitting.tollIntervalIDs;
        if (!pathFitting.tollIntervalIDsTime.isEmpty())
            map["tollIntervalIDsTime"] = pathFitting.tollIntervalIDsTime;
    }

    QString var = QString("axesCount=%1&discountsMoney=%2&enNode=%3&exNode=%4&gantryUnits=%5&mediaType=%6&"
                          "plateColor=%7&plateNumber=%8&receivableMoney=%9&tradingMoney=%10&vehicleClass=%11"
                          "&vehicleType=%12")
                      .arg(pathFitting.axesCount)
                      .arg(pathFitting.discountsMoney)
                      .arg(pathFitting.enNode)
                      .arg(pathFitting.exNode)
                      .arg(gantrys)
                      .arg(pathFitting.mediaType)
                      .arg(pathFitting.plateColor)
                      .arg(pathFitting.plateNumber)
                      .arg(pathFitting.receivableMoney)
                      .arg(pathFitting.tradingMoney)
                      .arg(pathFitting.vehicleClass)
                      .arg(pathFitting.vehicleType);
    QString signMd5 = Utils::DataDealUtils::cryptoMD5(var).toLower();

    QVariantMap sendMap;
    sendMap["method"] = "pathFitting2";
    sendMap["sign"] = signMd5;
    sendMap["signType"] = "NONE";
    sendMap["version"] = "1.0";
    sendMap["timestamp"] = Utils::DataDealUtils::curDateTimeStr();
    sendMap["encryptType"] = "NONE";
    sendMap["bizContent"] = GM_INSTANCE->m_jsonSerializer->serialize(map);

    QByteArray sendJson = GM_INSTANCE->m_jsonSerializer->serialize(sendMap);
    return sendJson;
}

QString BizHandler::packNode(const ST_Node &node)
{
    QVariantMap temp;
    temp["captureId"] = node.captureId;
    temp["nodeHex"] = node.nodeHex;
    temp["nodeId"] = node.nodeId;
    temp["nodeName"] = node.nodeName;
    temp["nodeType"] = node.nodeType;
    temp["passTime"] = node.passTime;
    QString data = GM_INSTANCE->m_jsonSerializer->serialize(temp);

    return data;
}

void BizHandler::getGantryNodes(const QString &passID, QStringList &nodeList)
{
    nodeList.clear();

    auto records = m_ds.getGantryInfos(passID);
    if (records.isEmpty())
        return;

    ST_Node node;
    for (const auto &record : records) {
        if (record["ERRORCODE"].toString() != "")
            continue;
        node.nodeType = 0;
        node.captureId = "";
        node.nodeHex = record["FLAGID"].toString();
        node.nodeId = record["GANTRYID"].toString();
        node.nodeName = record["FLAGNAME"].toString();
        node.passTime = record["TRADETIME"].toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        nodeList.append(packNode(node));
    }
}

void BizHandler::getGantryNodeSplitOut(const QString &sTradeID, QString &tollIntervalsGroup, QString &transTimeGroup)
{
    // 查t_SplitOut
    T_SplitOut splitOut;
    if (!m_ds.getSplitOut(sTradeID, &splitOut)) {
        LOG_INFO().noquote() << "未查询到门架信息";
        return;
    }

    tollIntervalsGroup = splitOut.TollIntervalsGroup;
    transTimeGroup = splitOut.TransTimeGroup;
    LOG_INFO().noquote() << QString("门架信息：%1, %2").arg(tollIntervalsGroup, transTimeGroup);
}

QString BizHandler::doDealCmd19(const QVariantMap &aMap)
{
    QString qrCodeMsg;
    QString plateNumber;

    if (aMap.contains("qrCodeMsg"))
        qrCodeMsg = aMap["qrCodeMsg"].toString();
    if (aMap.contains("plateNumber"))
        plateNumber = aMap["plateNumber"].toString();

    QString dealtData;
    if (!qrCodeMsg.isEmpty()) {
        dealtData = getGreenVehicleWayFromScan(qrCodeMsg);
    } else if (!plateNumber.isEmpty()) {
        dealtData = getGreenVehicleInfoFromPlate(plateNumber);
    } else {
        throw BaseException(1, "响应失败: 车牌号或二维码为空");
    }

    return dealtData;
}

QString BizHandler::getGreenVehicleInfoFromPlate(const QString &plateNumber)
{
    T_EtcOut objEtc;
    T_MtcOut objMtc;

    QString passID;
    QString tradeID;
    QString enTime;
    QString enNetHex;
    QString enHex;
    QString enName;
    int exitFeeType = 0, mediaType = 3, vehClass = 0, userType = 0, axisCount = 0, shouldPay = 0, discount = 0,
        factPay = 0, enWeight = 0, exWeight = 0;

    ST_Node exNode;
    exNode.nodeType = 1;
    if (m_ds.getLatestOutTrade(plateNumber, &objEtc, 0)) {
        // ETC出口记录
        passID = objEtc.PassID;
        tradeID = objEtc.TradeID;
        exitFeeType = objEtc.ExitFeeType;
        if (objEtc.CardType == 15)
            mediaType = 1;
        else if (objEtc.CardType == 22 || objEtc.CardType == 23)
            mediaType = 0;
        vehClass = objEtc.ExVehClass;
        userType = objEtc.UserType;
        axisCount = objEtc.Axises;
        shouldPay = objEtc.ShouldPay * 100;
        factPay = objEtc.FactPay * 100;
        discount = objEtc.Discount * 100;
        enWeight = objEtc.EnTotalWeight;
        exWeight = objEtc.TotalWeight;
        enTime = objEtc.EnTime.toString("yyyy-MM-dd HH:mm:ss");
        enHex = objEtc.EnStation;
        enNetHex = QString::number(objEtc.EnNetID);
        enName = objEtc.EnStationName;

        // 出口门架节点信息
        exNode.captureId = objEtc.TradeID;
        exNode.nodeHex = objEtc.ExStation;
        exNode.nodeId = objEtc.CNLaneID.left(14);
        exNode.nodeName = objEtc.ExStationName;
        exNode.passTime = objEtc.ExTime.toString("yyyy-MM-dd hh:mm:ss");
    } else if (m_ds.getLatestOutTrade(plateNumber, &objMtc, 1)) {
        // MTC出口记录
        passID = objMtc.PassID;
        tradeID = objMtc.TradeID;
        exitFeeType = objMtc.ExitFeeType;
        if (objMtc.CardType == 15)
            mediaType = 1;
        vehClass = objMtc.ExVehClass;
        userType = objMtc.UserType;
        axisCount = objMtc.Axises;
        shouldPay = objMtc.ShouldPay * 100;
        factPay = objMtc.FactPay * 100;
        discount = objMtc.Discount * 100;
        enWeight = objMtc.EnTotalWeight;
        exWeight = objMtc.TotalWeight;
        enTime = objMtc.EnTime.toString("yyyy-MM-dd HH:mm:ss");
        enHex = objMtc.EnStation;
        enNetHex = QString::number(objMtc.EnNetID);
        enName = objMtc.EnStationName;

        // 出口门架节点信息
        exNode.captureId = objMtc.TradeID;
        exNode.nodeHex = objMtc.ExStation;
        exNode.nodeId = objMtc.CNLaneID.left(14);
        exNode.nodeName = objMtc.ExStationName;
        exNode.passTime = objMtc.ExTime.toString("yyyy-MM-dd hh:mm:ss");
    } else {
        throw BaseException(1, "响应失败：未查询到相关记录");
    }

    if (passID.isEmpty()) {
        throw BaseException(1, "响应失败: 获取出口信息失败(passid为空)");
    }

    LOG_INFO().noquote() << QString("查询出口信息结果返回: exitFeeType %1, passID %2, name %3, tradeId %4")
                                .arg(exitFeeType)
                                .arg(passID, exNode.nodeName, tradeID);

    // 查询入口信息
    ST_Node enNode;
    enNode.nodeType = 1;
    QVariantMap enInfo = m_ds.getEnInfo(passID);
    if (!enInfo.isEmpty()) {
        enNode.captureId = enInfo["TRADEID"].toString();
        enNode.passTime = enInfo["ENTIME"].toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        enNode.nodeHex = enInfo["ENSTATION"].toString();
        enNode.nodeName = enInfo["STATIONNAME"].toString();
        enNode.nodeId = enInfo["CNLANEID"].toString().left(14);
    }

    if (enInfo.isEmpty() || enNode.captureId.isEmpty()) {
        LOG_INFO().noquote() << "查询入口信息为空，使用出口记录中的入口信息";
        enNode.captureId = "";
        enNode.passTime = enTime;
        enNode.nodeHex = enNetHex + enHex;
        enNode.nodeId = m_ds.getGantryNodeID(enNode.nodeHex);
        enNode.nodeName = enName;
    }
    LOG_INFO().noquote() << QString("查询入口信息结果返回 passID %1, name %2").arg(passID, enNode.nodeName);

    // 判断是否允许绿通通行
    QString greenPassMsg;
    int banType = m_ds.getGreenPassBanType(plateNumber);
    if (banType == 1) {
        greenPassMsg = "存在多次绿通混装";
    } else if (banType == 2) {
        greenPassMsg = "存在多次假冒绿通";
    } else {
        greenPassMsg = m_ds.getGreenPassAppointment(plateNumber) ? "存在绿通预约信息" : "";
    }

    QVariantMap sendMap;
    sendMap["status"] = 0;
    sendMap["desc"] = "";
    sendMap["plateNumber"] = Utils::BizUtils::getPlateNoColor(plateNumber);
    sendMap["plateColor"] = Utils::BizUtils::getColorCodeFromPlate(plateNumber);
    sendMap["enWeight"] = enWeight;
    sendMap["exWeight"] = exWeight;
    sendMap["greenPassStatus"] = (banType > 0 ? 1 : 0);
    sendMap["greenPassMsg"] = greenPassMsg;
    sendMap["enNode"] = GM_INSTANCE->m_jsonParser->parse(packNode(enNode).toUtf8());
    sendMap["exNode"] = GM_INSTANCE->m_jsonParser->parse(packNode(exNode).toUtf8());

    QStringList nodesList;
    getGantryNodes(passID, nodesList);
    QVariantList units;
    foreach (auto node, nodesList) {
        QVariantMap var = GM_INSTANCE->m_jsonParser->parse(node.toUtf8()).toMap();
        units.append(var);
    }
    sendMap["units"] = units;

    QByteArray data = GM_INSTANCE->m_jsonSerializer->serialize(sendMap);
    return data;
}

QString BizHandler::getGreenVehicleWayFromScan(const QString &scan)
{
    ST_GreenScanInfo scanInfo = getGreenPassScanInfo(scan);

    QString passID;
    QString tradeID;
    int exitFeeType = 0, mediaType = 3, vehClass = 0, userType = 0, axisCount = 0;
    int shouldPay = 0, discount = 0, factPay = 0;

    // 获取出口信息
    passID = scanInfo.passId;
    if (passID.left(2) == "01") {
        mediaType = 0;
    } else if (passID.left(2) == "02") {
        mediaType = 1;
    }
    shouldPay = scanInfo.payFee;
    factPay = scanInfo.fee;
    discount = shouldPay - factPay;

    ST_Node exNode;
    exNode.nodeType = 1;
    exNode.captureId = "";
    exNode.nodeHex = m_ds.getGantryHexNode(scanInfo.exStationId);
    exNode.nodeId = scanInfo.exStationId;
    exNode.nodeName = m_ds.getGantryNodeName(scanInfo.exStationId);
    exNode.passTime = scanInfo.exTime;

    if (passID.isEmpty()) {
        throw BaseException(1, "响应失败: 解析二维码信息失败(passid为空)");
    }

    // 查询入口信息
    ST_Node enNode;
    enNode.nodeType = 1;
    QVariantMap enInfo = m_ds.getEnInfo(passID);
    if (!enInfo.isEmpty()) {
        enNode.captureId = enInfo["TRADEID"].toString();
        enNode.passTime = enInfo["ENTIME"].toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        enNode.nodeHex = enInfo["ENSTATION"].toString();
        enNode.nodeName = enInfo["STATIONNAME"].toString();
        enNode.nodeId = enInfo["CNLANEID"].toString().left(14);
    }

    if (enInfo.isEmpty() || enNode.captureId.isEmpty()) {
        LOG_INFO().noquote() << "查询入口信息为空，使用出口记录中的入口信息";
        enNode.captureId = "";
        enNode.passTime = QDateTime::fromString(scanInfo.passId.right(14), "yyyyMMddhhmmss")
                              .toString("yyyy-MM-dd hh:mm:ss");
        enNode.nodeHex = m_ds.getGantryHexNode(scanInfo.enStationId);
        enNode.nodeId = m_ds.getGantryNodeID(enNode.nodeHex);
        enNode.nodeName = m_ds.getGantryNodeName(scanInfo.enStationId);
    }
    LOG_INFO().noquote() << QString("查询入口信息结果返回 passID %1, name %2").arg(passID, enNode.nodeName);

    // 组装发送报文
    ST_PathFitting pathFitting;
    pathFitting.plateNumber = scanInfo.plateNumber;
    pathFitting.plateColor = scanInfo.plateColor;
    pathFitting.mediaType = mediaType;
    pathFitting.vehicleType = vehClass;
    pathFitting.vehicleClass = userType;
    pathFitting.receivableMoney = shouldPay;
    pathFitting.tradingMoney = factPay;
    pathFitting.discountsMoney = discount;
    pathFitting.axesCount = axisCount;

    pathFitting.enNode = packNode(enNode);
    pathFitting.exNode = packNode(exNode);
    getGantryNodes(passID, pathFitting.gantryUnits);
    QString sendJson = packFeePath(pathFitting, exitFeeType);

    LOG_INFO().noquote() << QString("发送地图路径请求: %1").arg(sendJson);
    QUrl url(GM_INSTANCE->m_config->m_baseConfig.mapUrl);
    auto reply = Http().instance().post(url, sendJson.toUtf8(), "application/json");

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());
    LOG_INFO().noquote() << QString("接收地图路径完成 %1").arg(result.left(1024));

    QVariantMap resMap = GM_INSTANCE->m_jsonParser->parse(result.toUtf8()).toMap();
    QVariantMap bizMap = resMap.value("bizContent", {}).toMap();
    QByteArray sendData = GM_INSTANCE->m_jsonSerializer->serialize(bizMap);

    return sendData;
}

ST_GreenScanInfo BizHandler::getGreenPassScanInfo(const QString &scan)
{
    ST_GreenScanInfo var;
    QStringList fields = scan.split('|');

    QString fullVehplate = fields[0];
    var.plateNumber = fullVehplate.section('_', 0, 0);
    var.plateColor = fullVehplate.section('_', 1, 1).toInt();

    var.enStationId = fields[1];
    var.exStationId = fields[2];
    var.enWeight = fields[3].toInt();
    var.exWeight = fields[4].toInt();
    var.mediaType = fields[5].toInt();
    var.transactionId = fields[6];
    var.passId = fields[7];

    var.exTime = fields[8].replace("T", " ");
    var.transPayType = fields[9].toInt();
    var.fee = fields[10].toLong();
    var.payFee = fields[11].toLong();
    var.vehicleSign = fields[12];
    var.provinceNum = fields[13].toInt();

    return var;
}

QString BizHandler::doDealCmd20(const QVariantMap &aMap)
{
    QString captureId;
    int nodeType = -1;
    QString plateNumber;
    QString gantryId;
    QString tradeTime;
    QString picId;

    if (aMap.contains("captureId"))
        captureId = aMap["captureId"].toString();
    if (aMap.contains("nodeType"))
        nodeType = aMap["nodeType"].toInt();
    if (aMap.contains("plateNumber"))
        plateNumber = aMap["plateNumber"].toString();
    if (aMap.contains("gantryId"))
        gantryId = aMap["gantryId"].toString();
    if (aMap.contains("tradeTime"))
        tradeTime = aMap["tradeTime"].toString();
    if (aMap.contains("picId"))
        picId = aMap["picId"].toString();

    QString imageData;
    if (nodeType == 0) { // 获取门架图片
        imageData = getGantryImage(plateNumber, gantryId, tradeTime);
        int pos = imageData.indexOf(",");
        if (pos > 0)
            imageData = imageData.left(pos);
    } else if (nodeType == 1) { // 获取车道图片
        imageData = getLaneImage(captureId);
    } else if (nodeType == 2) { // 获取本地图片
        imageData = getLocalImage(picId);
    } else {
        throw BaseException(1, QString("响应失败：节点类型错误(nodeType %1)").arg(nodeType));
    }

    QVariantMap map;
    map["status"] = "0";
    map["desc"] = "成功获取到图片";
    map["picData"] = imageData;
    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(map);

    return dealtData;
}

QString BizHandler::getGantryImage(const QString &plateNumber, const QString &gantryId, const QString &tradeTime)
{
    if (plateNumber.isEmpty())
        throw BaseException(1, "响应失败: 车牌号为空");
    if (gantryId.isEmpty())
        throw BaseException(1, "响应失败: 门架全网编号为空");
    if (tradeTime.isEmpty())
        throw BaseException(1, "响应失败: 交易时间为空");

    QVariantMap sendMap;
    sendMap["gantryId"] = gantryId;
    sendMap["tradeTime"] = tradeTime;
    sendMap["vehPlate"] = plateNumber;
    QString sendData = GM_INSTANCE->m_jsonSerializer->serialize(sendMap);
    LOG_INFO().noquote() << QString("获取门架图片请求 %1").arg(sendData);

    QUrl url(GM_INSTANCE->m_config->m_baseConfig.gantryPicUrl);
    auto reply = Http().instance().post(url, sendData.toUtf8(), "application/json");

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());
    LOG_INFO().noquote() << QString("返回门架图片数据: %1").arg(result.left(1024));

    return result;
}

QString BizHandler::getLaneImage(const QString &captureId)
{
    if (captureId.isEmpty())
        throw BaseException(1, "响应失败: 牌识Id为空");

    QVariantMap sendMap;
    sendMap["tradeId"] = captureId;
    sendMap["orgId"] = captureId.left(4);
    QString sendData = GM_INSTANCE->m_jsonSerializer->serialize(sendMap);
    LOG_INFO().noquote() << QString("获取车道图片请求: %1").arg(sendData);

    QUrl url(GM_INSTANCE->m_config->m_baseConfig.lanePicUrl);
    auto reply = Http().instance().post(url, sendData.toUtf8(), "application/json");

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());
    LOG_INFO().noquote() << QString("返回车道图片数据: %1").arg(result.left(1024));

    return result;
}

QString BizHandler::getLocalImage(const QString &picName)
{
    if (picName.isEmpty())
        throw BaseException(1, "响应失败: 图片ID为空");

    Utils::FileName picturePath = GM_INSTANCE->m_pictureDir + QString("/%1").arg(picName);

    if (!picturePath.exists()) {
        LOG_WARNING().noquote() << QString("图片文件不存在 %1.jgp").arg(picName);
        throw BaseException(1, "响应失败: 图片文件不存在");
    }

    Utils::FileReader fileReader;
    bool ok = fileReader.fetch(picturePath.toString());
    if (!ok) {
        LOG_ERROR().noquote() << QString("读取图片文件 %1.jgp 失败").arg(picName);
        throw BaseException(1, "响应失败: 读取图片文件失败");
    }

    QByteArray imageData = fileReader.data();

    QString result = imageData.toBase64();
    LOG_INFO().noquote() << QString("返回本地图片数据: %1").arg(result.left(1024));
    return result;
}

QString BizHandler::doDealCmd24(const QByteArray &reqBody)
{
    QByteArray sendData = reqBody;
    LOG_INFO().noquote() << QString("车牌识别获取请求: %1").arg(QString::fromUtf8(sendData.left(1024)));

    QUrl url(GM_INSTANCE->m_config->m_baseConfig.plateOcrUrl);
    auto reply = Http().instance().post(url, sendData, "application/json");
    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());

    bool ok = false;
    QVariantMap resMap = GM_INSTANCE->m_jsonParser->parse(result.toUtf8(), &ok).toMap();
    if (!ok)
        throw BaseException(1, "响应失败: 解析车牌识别返回内容异常");
    QString data = GM_INSTANCE->m_jsonSerializer->serialize(resMap["data"]);
    LOG_INFO().noquote() << QString("车牌识别内容返回: %1").arg(data);
    return data;
}

QString BizHandler::doDealCmd25(const QVariantMap &aMap)
{
    QString vehicleId;
    QString cardId;
    if (aMap.contains("vehicleId"))
        vehicleId = aMap["vehicleId"].toString();
    if (aMap.contains("cardId"))
        cardId = aMap["cardId"].toString();

    if (vehicleId.isEmpty() && cardId.isEmpty())
        throw BaseException(1, "响应失败: 车牌号与卡号为空");

    QVariantMap map;
    map["vehicleId"] = vehicleId;
    map["cardNum"] = cardId;
    QString sendData = GM_INSTANCE->m_jsonSerializer->serialize(map);
    LOG_INFO().noquote() << QString("状态名单信息查询请求: %1").arg(sendData);

    QString url(GM_INSTANCE->m_config->m_baseConfig.blackStatusUrl);
    auto reply = Http().instance().post(url, sendData.toUtf8(), "application/json");

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());
    LOG_INFO().noquote() << QString("返回状态名单信息查询结果: %1").arg(result);

    return result;
}

QString BizHandler::doDealCmd26(const QVariantMap &aMap)
{
    QString stationId;
    QString shiftDate;
    int shiftId = 0;
    int laneId = 0;
    QString userId;
    if (aMap.contains("stationId"))
        stationId = aMap["stationId"].toString();
    if (aMap.contains("shiftDate"))
        shiftDate = aMap["shiftDate"].toString();
    if (aMap.contains("shiftID"))
        shiftId = aMap["shiftID"].toInt();
    if (aMap.contains("laneID"))
        laneId = aMap["laneID"].toInt();
    if (aMap.contains("operator"))
        userId = aMap["operator"].toString();

    if (stationId.isEmpty())
        throw BaseException(1, "响应失败: 站代码为空");
    if (shiftDate.isEmpty())
        throw BaseException(1, "响应失败: 班次日期为空");
    if (shiftId == 0)
        throw BaseException(1, "响应失败: 班次号为空");
    if (laneId == 0)
        throw BaseException(1, "响应失败： 车道号为空");
    if (userId.isEmpty())
        throw BaseException(1, "响应失败: 员工号为空");

    T_LaneInputShift laneInputShift;
    QJson::QObjectHelper::qvariant2qobject(aMap, &laneInputShift);
    laneInputShift.UpdateTime = QDateTime::currentDateTime();
    LOG_INFO().noquote() << QString("交接班数据: %1").arg(laneInputShift.toString());

    QString dtpContent = Utils::BizUtils::makeDtpContentFromObj(laneInputShift);
    QString fromNode = stationId.rightJustified(4, QLatin1Char('0'))
                       + QString::number(laneId).rightJustified(2, QLatin1Char('0'));
    QString dtpXml = Utils::BizUtils::makeDtpXml(dtpContent, "531", fromNode, stationId, 1);
    LOG_INFO().noquote() << QString("交接班数据DTP报文: %1").arg(dtpXml);

    QString stationIP = m_ds.getStationIP(stationId);
    int res = m_dtpSender->sendMsgToDtp(stationIP, 13591, "TradeQ", "", dtpXml);

    if (res < 0)
        throw BaseException(1, "响应失败: 站级数据传输异常");

    QVariantMap map;
    map["status"] = "0";
    map["desc"] = "成功录入交接班数据";
    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(map);
    return dealtData;
}

QString BizHandler::doDealCmd27(const QVariantMap &aMap)
{
    QString stationID;
    QString shiftDate;
    int shiftID = -1;
    int laneID = -1;
    QString userID;
    int specialType = -1;
    int specialSubType = -1;
    QString specialMedium;
    QString desc;

    if (aMap.contains("stationId"))
        stationID = aMap["stationId"].toString();
    if (aMap.contains("shiftDate"))
        shiftDate = aMap["shiftDate"].toString();
    if (aMap.contains("shiftID"))
        shiftID = aMap["shiftID"].toInt();
    if (aMap.contains("laneID"))
        laneID = aMap["laneID"].toInt();
    if (aMap.contains("operator"))
        userID = aMap["operator"].toString();
    if (aMap.contains("specialType"))
        specialType = aMap["specialType"].toInt();
    if (aMap.contains("specialSubType"))
        specialSubType = aMap["specialSubType"].toInt();
    if (aMap.contains("specialMedium"))
        specialMedium = aMap["specialMedium"].toString();
    if (aMap.contains("desc"))
        desc = aMap["desc"].toString();

    if (stationID.isEmpty())
        throw BaseException(1, "响应失败: 收费站代码为空");
    if (shiftDate.isEmpty())
        throw BaseException(1, "响应失败: 班次日期为空");
    if (shiftID == -1)
        throw BaseException(1, "响应失败: 班次号为空");
    if (laneID == -1)
        throw BaseException(1, "响应失败: 车道号为空");
    if (userID.isEmpty())
        throw BaseException(1, "响应失败: 登记员工号为空");
    if (specialType == -1)
        throw BaseException(1, "响应失败: 特情类型为空");
    if (specialSubType == -1)
        throw BaseException(1, "响应失败: 特情子类型为空");
    if (specialMedium.isEmpty())
        throw BaseException(1, "响应失败: 特情介质号为空");

    // map对齐表结构
    QVariantMap resMap = aMap;
    resMap.insert("updateTime", QDateTime::currentDateTime());
    resMap.insert("dataID",
                  stationID + QString("%1").arg(laneID, 2, 10, QChar('0'))
                      + QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    resMap.insert("cardID", specialMedium);

    QString operatorName = m_ds.getUserName(userID, 1);
    resMap.insert("inputOperator", userID);
    resMap.insert("operatorName", operatorName);
    resMap.insert("inputOperatorName", operatorName);

    // DTP上传
    T_SpecialCards specialCard;

    resMap["specialType"] = specialSubType; // NOTE 特别处理
    QJson::QObjectHelper::qvariant2qobject(resMap, &specialCard);
    LOG_INFO().noquote() << QString("特情卡记录: %1").arg(specialCard.toString());

    QString dtpContent = Utils::BizUtils::makeDtpContentFromObj(specialCard);
    QString fromNode = stationID.rightJustified(4, QLatin1Char('0'))
                       + QString::number(specialCard.LaneID).rightJustified(2, QLatin1Char('0'));
    QString dtpXml = Utils::BizUtils::makeDtpXml(dtpContent, "532", fromNode, stationID, 1);
    LOG_INFO().noquote() << QString("特情卡记录DTP报文: %1").arg(dtpXml);

    QString stationIP = m_ds.getStationIP(stationID);
    int res = m_dtpSender->sendMsgToDtp(stationIP, 13591, "TradeQ", "", dtpXml);

    if (res < 0)
        throw BaseException(1, "响应失败: 站级数据传输异常");

    QVariantMap map;
    map["status"] = "0";
    map["desc"] = "成功录入特情卡数据";
    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(map);
    return dealtData;
}

QString BizHandler::doDealCmd28(const QVariantMap &aMap)
{
    QString cardId;
    if (aMap.contains("cardId"))
        cardId = aMap["cardId"].toString();
    if (cardId.isEmpty())
        throw BaseException(1, "响应失败: 身份卡号为空");

    QString userId = m_ds.getUserID(cardId);
    QString userName = m_ds.getUserName(cardId, 0);

    if (userId.isEmpty() && userName.isEmpty())
        throw BaseException(1, "响应失败: 未查询到相关用户信息");

    LOG_INFO().noquote() << QString("返回用户信息: cardId %1, userName %2, userId %3").arg(cardId, userName, userId);
    QVariantMap map;
    map["status"] = "0";
    map["desc"] = "";
    map["operator"] = userId;
    map["operatorName"] = userName;
    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(map);
    return dealtData;
}

QString BizHandler::doDealCmd30(QVariantMap aMap)
{
    QString vehicleId;
    if (aMap.contains("vehicleId"))
        vehicleId = aMap["vehicleId"].toString();

    if (vehicleId.isEmpty())
        throw BaseException(1, "响应失败: 车牌号为空");

    int colorIndex = Utils::BizUtils::getColorCodeFromPlate(vehicleId);
    QString plateNoColor = Utils::BizUtils::getPlateNoColor(vehicleId);

    LOG_INFO().noquote() << QString("查询逃漏费车辆 %1 欠费列表").arg(vehicleId);
    QString encodedVehicleId = QUrl::toPercentEncoding(QString("%1_%2").arg(plateNoColor).arg(colorIndex));
    QUrl url(GM_INSTANCE->m_config->m_baseConfig.arrearsUrl + "/" + encodedVehicleId);
    // 跳过证书校验
    Http().instance().setSkipVerify(true);
    auto reply = Http().instance().get(url);

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout() * 3);
    Http().instance().setSkipVerify(false);
    LOG_INFO().noquote() << QString("返回逃漏费车辆欠费列表: %1").arg(result.left(1024));

    QVariantMap resMap = GM_INSTANCE->m_jsonParser->parse(result.toUtf8()).toMap();
    bool success = resMap["success"].toBool();
    if (!success)
        throw BaseException(1, "响应失败: 欠费查询接口返回失败");

    QVariantList audits = resMap["data"].toList();
    if (audits.isEmpty())
        throw BaseException(1, "响应失败: 未查询到欠费信息");
    for (auto &var : audits) {
        QVariantMap auditMap = var.toMap();

        QString id = auditMap["id"].toString();
        QString enStationId = auditMap["realEnStationId"].toString();
        QString exStationId = auditMap["realExStationId"].toString();
        QString passId = auditMap["passId"].toString();
        QString escapeType = auditMap["confirmedEscapeTypeName"].toString();
        QString escapeTypeDesc = auditMap["remark"].toString();
        bool isPreBlack = auditMap["isPreBlack"].toBool();

        // 缓存稽核对象，补费上传时使用
        ST_AuditInfo auditInfo;
        auditInfo.isPreBlack = isPreBlack;
        auditInfo.enStationId = enStationId;
        auditInfo.exStationId = exStationId;
        auditInfo.passId = passId;
        auditInfo.escapeType = escapeType;
        auditInfo.escapeTypeDesc = escapeTypeDesc;
        m_auditInfos.insert(id, auditInfo);

        // 提高返回数据可读性
        auditMap["realEnStationName"] = enStationId.isEmpty() ? "" : m_ds.getStationName(enStationId);
        auditMap["realExStationName"] = exStationId.isEmpty() ? "" : m_ds.getStationName(exStationId);
        auditMap["isPreBlack"] = isPreBlack ? 1 : 0;

        var = auditMap;
    }

    QVariantMap map;
    map["status"] = "0";
    map["desc"] = "";
    map["audits"] = audits;

    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(map);
    return dealtData;
}

QString BizHandler::doDealCmd39(const QVariantMap &aMap)
{
    QString stationID;
    QString startTime;
    QString stopTime;

    if (aMap.contains("stationID"))
        stationID = aMap["stationID"].toString();
    if (aMap.contains("startTime"))
        startTime = aMap["startTime"].toString();
    if (aMap.contains("stopTime"))
        stopTime = aMap["stopTime"].toString();

    if (stationID.isEmpty())
        throw BaseException(1, "响应失败: 站代码为空");
    if (startTime.isEmpty())
        throw BaseException(1, "响应失败: 预约开始时间为空");
    if (stopTime.isEmpty())
        throw BaseException(1, "响应失败: 预约结束时间为空");

    QVariantMap sendMap;
    sendMap["queryType"] = "queryData";
    sendMap["queryAuth"] = "1";
    sendMap["querySql"]
        = QString("SELECT TradeID, PassID, ExVehPlate, EnNetID, EnStation, ExNetID, ExStation, ExTime, EnTotalWeight, "
                  "TotalWeight, CardType, ShouldPay, FactPay, ProvinceNum, CardBizType, Reserve, UserType, OBUPlate "
                  "FROM T_ETC_OUT WHERE "
                  "(UserType = 21 OR UserType = 22) AND ExTime >= '%1' AND ExTime < '%2' UNION ALL "
                  "SELECT TradeID, PassID, ExVehPlate, EnNetID, EnStation, ExNetID, ExStation, ExTime, EnTotalWeight, "
                  "TotalWeight, CardType, ShouldPay, FactPay, NULL AS ProvinceNum, CardBizType, Reserve, UserType, "
                  "NULL AS OBUPlate FROM T_MTC_OUT "
                  "WHERE (UserType = 21 OR UserType = 22) AND ExTime >= '%3' AND ExTime < '%4'")
              .arg(startTime, stopTime, startTime, stopTime);
    sendMap["dataType"] = 4;

    QString sendData = GM_INSTANCE->m_jsonSerializer->serialize(sendMap);
    LOG_INFO().noquote() << QString("本站绿通流水查询请求: %1").arg(sendData);

    QString stationIP = m_ds.getStationIP(stationID);
    QString stationServiceUrl = QString("http://%1:8082").arg(stationIP);
    QString url(stationServiceUrl);
    auto reply = Http().instance().post(url, sendData.toUtf8(), "application/json");

    QString result = blockUtilResponse(reply, Http().instance().getReadTimeout());
    LOG_INFO().noquote() << QString("返回本站绿通流水查询结果: %1").arg(result);

    QVariantMap resMap = GM_INSTANCE->m_jsonParser->parse(result.toUtf8()).toMap();
    if (resMap["errCode"].toInt() == 1) {
        QString errorMessage = resMap["errorMessage"].toString();
        LOG_ERROR().noquote() << QString("站级服务返回查询失败 %1").arg(errorMessage);
        throw BaseException(1, QString("响应失败: 站级服务返回查询失败 %1").arg(errorMessage));
    }

    QVariantList greenTradeList;
    QVariantList records = resMap["data"].toList();
    for (const auto &record : records) {
        QVariantMap recordMap = record.toMap();

        QVariantMap greenTrade;
        greenTrade["tradeId"] = recordMap["TradeID"].toString();
        greenTrade["passId"] = recordMap["PassID"].toString();
        greenTrade["vehPlate"] = recordMap["ExVehPlate"].toString();
        QString enHexNode = recordMap["EnNetID"].toString() + recordMap["EnStation"].toString();
        greenTrade["enStation"] = m_ds.getGantryNodeID(enHexNode);
        QString exHexNode = recordMap["ExNetID"].toString() + recordMap["ExStation"].toString();
        greenTrade["exStation"] = m_ds.getGantryNodeID(exHexNode);
        greenTrade["exTime"] = QDateTime::fromString(recordMap["ExTime"].toString(), "yyyy-MM-dd hh:mm:ss")
                                   .toString("yyyy-MM-ddThh:mm:ss");
        greenTrade["enWeight"] = recordMap["EnTotalWeight"].toString();
        greenTrade["exWeight"] = recordMap["TotalWeight"].toString();

        int cardType = recordMap["CardType"].toInt();
        if (cardType == 22 || cardType == 23) {
            greenTrade["mediaType"] = 1;
        } else if (cardType == 15) {
            greenTrade["mediaType"] = 2;
        } else if (cardType == 3) {
            greenTrade["mediaType"] = 3;
        } else {
            greenTrade["mediaType"] = 9;
        }

        int cardBizType = recordMap["CardBizType"].toInt();
        if (cardBizType == 33 || cardBizType == 34 || cardBizType == 35) {
            greenTrade["transPayType"] = 2;
        } else if (cardType == 22 || cardType == 23) {
            greenTrade["transPayType"] = 1;
        } else {
            greenTrade["transPayType"] = -1;
        }
        greenTrade["fee"] = recordMap["FactPay"].toInt();
        greenTrade["payFee"] = recordMap["ShouldPay"].toInt();

        int userType = recordMap["UserType"].toInt();
        QString reserve = recordMap["Reserve"].toString();
        if (reserve.contains("(Appointment:0)")) {
            greenTrade["vehicleSign"] = "0xFF";
        } else if (userType == 21) {
            greenTrade["vehicleSign"] = "0x02";
        } else if (userType == 22) {
            greenTrade["vehicleSign"] = "0x03";
        }

        int provinceNum = recordMap["ProvinceNum"].toInt();
        if (provinceNum == 0) {
            provinceNum = 1;
            QRegularExpression re(R"(\(ProvinceNum:(\d+)\))"); // 捕获数字
            QRegularExpressionMatch match = re.match(reserve);
            if (match.hasMatch())
                provinceNum = match.captured(1).toInt(); // 第1个括号里的内容
        }

        greenTrade["proviceCount"] = provinceNum;
        greenTrade["obuPlate"] = recordMap["OBUPlate"].toString();

        greenTradeList.append(greenTrade);
    }

    QVariantMap map;
    map["status"] = 0;
    map["desc"] = "查询成功";
    map["greenTradeList"] = greenTradeList;
    QString dealtData = GM_INSTANCE->m_jsonSerializer->serialize(map);
    return dealtData;
}

QString BizHandler::blockUtilResponse(HttpReply *reply, qint32 timeoutMs)
{
    bool isSuccessful = false;
    QString error;
    // 局部事件循环 + 超时保护
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, [&]() {
        error = QString("响应失败：网络等待超时(%1s)").arg(timeoutMs / 1000);
        loop.quit();
    });
    timer.start(timeoutMs);

    QString result;

    // 连接 finished 信号，一到就拿 body 并退出循环
    QObject::connect(reply, &HttpReply::finished, &loop, [&]() {
        if (reply->isSuccessful()) {
            isSuccessful = true;
            result = reply->body();
        } else {
            error = QString("响应失败: 网络错误(%1, %2)").arg(reply->statusCode()).arg(reply->reasonPhrase());
        }
        loop.quit();
    });

    // 阻塞到 finished() 或者超时
    loop.exec();

    // 清理
    if (timer.isActive())
        timer.stop();
    reply->deleteLater();
    if (!isSuccessful) {
        throw BaseException(1, error);
    }
    return result;
}
