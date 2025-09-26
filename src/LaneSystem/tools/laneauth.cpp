#include "laneauth.h"
#include "Logger.h"
#include "global/globalmanager.h"
#include "utils/datadealutils.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"

using namespace Utils;

LaneAuth::LaneAuth(QObject *parent)
    : QObject{parent}
{}

bool LaneAuth::getAuthInfo(const QString &fileName,
                           QString &stationID,
                           QString &laneID,
                           QString &laneIP,
                           QString &licenseCode,
                           QString &cnLaneID)
{
    FileName file = FileName::fromString(FileUtils::curApplicationDirPath() + fileName);
    if (!file.exists()) {
        LOG_ERROR().noquote() << "车道授权失败: 找不到授权文件 " << file.fileName(0);
        return false;
    }

    bool ok = false;
    FileReader reader;
    ok = reader.fetch(file.toString());
    if (!ok) {
        LOG_ERROR().noquote() << "车道授权失败: 读取授权文件失败 " << reader.errorString();
        return false;
    }

    QByteArray data = reader.data();
    QVariantMap oneMap = GM_INSTANCE->m_jsonParser->parse(data, &ok).toMap();
    if (!ok) {
        LOG_ERROR().noquote() << "车道授权失败: 解析授权文件内容错误 " << GM_INSTANCE->m_jsonParser->errorString();
        return false;
    }
    stationID = oneMap["stationID"].toString();
    laneID = oneMap["laneID"].toString();
    laneIP = oneMap["laneIP"].toString();
    licenseCode = oneMap["authCode"].toString();
    cnLaneID = oneMap["cnLaneID"].toString();

    LOG_INFO().noquote() << "授权文件内参数 "
                         << QString("站代码: %1, 车道号: %2, 车道IP: %3, 授权码: %4, CNLaneID: %5")
                                .arg(stationID, laneID, laneIP, licenseCode, cnLaneID);
    return true;
}

bool LaneAuth::authCheck(QString stationID, int laneID, QString laneIP, QString cnLaneID)
{
    LOG_INFO().noquote() << "车道授权检查开始";
    LOG_INFO().noquote() << "传入参数: "
                         << QString("站代码: %1, 车道号: %2, 车道IP: %3, CNLaneID: %4")
                                .arg(stationID)
                                .arg(laneID)
                                .arg(laneIP, cnLaneID);

    if (stationID.isEmpty() || laneID >= 100 || laneID <= 0 || laneIP.isEmpty() || cnLaneID.isEmpty()) {
        LOG_ERROR().noquote() << "车道授权失败: 传入参数不合规";
        return false;
    }

    QString fName = QString("/LaneInfo_%1%2.json").arg(stationID).arg(DataDealUtils::padValue(laneID, 2));
    QString fStationID = "";
    QString fLaneID = "";
    QString fLaneIP = "";
    QString fLicenseCode = "";
    QString fCnLaneID = "";
    if (!getAuthInfo(fName, fStationID, fLaneID, fLaneIP, fLicenseCode, fCnLaneID)) {
        return false;
    }
    if (fStationID.isEmpty() || fLaneID.isEmpty() || fLaneIP.isEmpty() || fLicenseCode.isEmpty()
        || fCnLaneID.isEmpty()) {
        LOG_ERROR().noquote() << "车道授权失败: 授权文件内参数不合规";
        return false;
    }
    QStringList ipList = NetworkUtils::getLocalAddresses();
    LOG_INFO().noquote() << "本机IP地址 " << ipList;
    if (!ipList.contains(fLaneIP)) {
        LOG_ERROR().noquote() << fLaneIP << " 非本机IP";
        return false;
    }
    if (fStationID != stationID || fLaneID != DataDealUtils::padValue(laneID, 2) || fLaneIP != laneIP
        || fCnLaneID != cnLaneID) {
        LOG_ERROR().noquote() << "车道授权失败: 传入参数与授权文件内参数不一致";
        return false;
    }
    QString cID = fCnLaneID + fLaneIP + "00000000000000000000000000000000";
    cID = cID.mid(0, 32);

    QString licenseFromFile = fLicenseCode;
    QString key = fStationID + fLaneID + "00";
    QString licenseFromParam = "";
    licenseFromParam = computeID2License(cID, key);
    if (licenseFromFile.isEmpty() || cID.isEmpty() || licenseFromFile != licenseFromParam) {
        LOG_ERROR().noquote() << "车道授权失败: 授权码验证失败";
        return false;
    }
    LOG_INFO().noquote() << "车道授权检查结束：成功";
    return true;
}

QString LaneAuth::computeID2License(QString cID, QString orgID)
{
    QByteArray key = QString("fjeit123" + orgID).toUtf8();
    key = key.leftJustified(16, '\0', true);

    QByteArray inpad = cID.toUtf8();

    int outLen = ((inpad.size() + 15) / 16) * 16;
    QByteArray out(outLen, 0);

    sm4_setkey_enc(&m_ctx, reinterpret_cast<unsigned char *>(key.data()));
    sm4_crypt_ecb(&m_ctx,
                  SM4_ENCRYPT,
                  outLen,
                  reinterpret_cast<unsigned char *>(inpad.data()),
                  reinterpret_cast<unsigned char *>(out.data()));

    return DataDealUtils::byteArrayToHexStr(out, false);
}
