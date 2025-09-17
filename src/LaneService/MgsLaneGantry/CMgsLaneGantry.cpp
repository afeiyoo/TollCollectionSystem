#include "CMgsLaneGantry.h"
#include <QSqlQuery>
#include <QSqlRecord>

extern "C" Q_DECL_EXPORT IMgsLaneGantry* GetMgsLaneGantry()
{
    return new CMgsLaneGantry();
}


CMgsLaneGantry::CMgsLaneGantry()
{

    _handle = nullptr;
    m_fileAppender = nullptr;
}

bool CMgsLaneGantry::MgsLaneGantry_GetLastErr(char *errmsg)
{
    bool ret = false;
    try {
        strcpy(errmsg, m_ErrMsg.toStdString().c_str());
        ret = true;
    } catch (...) {
    }
    return ret;
}

bool CMgsLaneGantry::MgsLaneGantry_Init(char *dbHost,char *dbName,char *dbUser,char *dbPwd,int laneId,int index)
{
    bool ret = false;
    int dllret = -1;
    m_index = index;
    m_ErrMsg = "";
    if(m_fileAppender == nullptr){
        QString logFile = QCoreApplication::applicationDirPath() + "/log/" +
                QString::number(laneId)+ "/gantry"+QString::number(index)+".log";
        m_fileAppender = new RollingFileAppender(logFile);
        m_fileAppender->setLogFilesLimit(90);
        m_fileAppender->setFormat("%{time}{yyyy-MM-dd HH:mm:ss.zzz} [%{type}:%{threadid}] %{message}\n\n");
        m_fileAppender->setFlushOnWrite(true);
        m_fileAppender->setDatePattern(RollingFileAppender::DatePattern::DailyRollover);
        m_fileAppender->setDetailsLevel("debug");
    }
    cuteLogger->registerCategoryAppender("gantry", m_fileAppender);

    LOG_CDEBUG("gantry") << "MgsLaneGantry_Init begin.";
    QString ProgPath = QCoreApplication::applicationDirPath();

    ProgPath += "/";
    try {
        if (_handle) {
            _handle->unload();
            delete _handle;
            _handle = nullptr;
        }
        QString dllpath = ProgPath + "libfeecalc_province35_x86";
        if (_handle == nullptr)
            _handle = new QLibrary(dllpath);

        if (_handle->load()) {
            LOG_CDEBUG("gantry").noquote() << "MgsLaneGantry load " << "'" << _handle->fileName() << "'";
            _libfeecalc_init = (libfeecalc_init)_handle->resolve("init");
            _libfeecalc_deInit = (libfeecalc_deInit)_handle->resolve("deInit");
            _libfeecalc_getLibVersion =
                    (libfeecalc_getLibVersion)_handle->resolve("getLibVersion");
            _libfeecalc_getFeeDataVersion =
                    (libfeecalc_getFeeDataVersion)_handle->resolve("getFeeDataVersion");
            _libfeecalc_getFee = (libfeecalc_getFee)_handle->resolve("getFee");
            _libfeecalc_loadFlagFee =
                    (libfeecalc_loadFlagFee)_handle->resolve("loadFlagFee");
            _libfeecalc_getFlagFee =
                    (libfeecalc_getFlagFee)_handle->resolve("getFlagFee");

            if (_libfeecalc_init == NULL || _libfeecalc_deInit == NULL ||
                    _libfeecalc_getLibVersion == NULL ||
                    _libfeecalc_getFeeDataVersion == NULL || _libfeecalc_getFee == NULL ||
                    _libfeecalc_loadFlagFee == NULL || _libfeecalc_getFlagFee == NULL) {
                throw QString("加载计费函数失败");
            }
        } else
            throw QString("载入 'libfeecalc_province35_x86' 失败");
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_init begin." << "'" << ProgPath << "'";
        dllret = _libfeecalc_init(NULL, ProgPath.toUtf8().data());
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_init end. return " << dllret;
        if (ret != 0)
            throw QString(QString("").sprintf("libfeecalc_init返回%d", dllret));
        char version[128] = {0};
        int verLen = 120;
        char dataVersion[128] = {0};
        int dataVerLen = 120;
        dllret = -1;
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_getLibVersion begin.";
        dllret =
                _libfeecalc_getLibVersion(version, verLen, dataVersion, dataVerLen);
        LOG_CDEBUG("gantry").noquote() <<QString("libfeecalc_getLibVersion end. return %1 '%2' '%3'").arg(dllret)
                        .arg(version).arg(dataVersion);
        if (dllret)
            throw QString("计费模块函数调用失败");
        char feeDataVersion[128] = {0};
        int feeDataVerLen = 120;
        dllret = -1;
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_getFeeDataVersion begin.";
        dllret = _libfeecalc_getFeeDataVersion(feeDataVersion, feeDataVerLen);
        LOG_CDEBUG("gantry").noquote() << QString("libfeecalc_getFeeDataVersion end. return %1 '%2'").arg(dllret)
                        .arg(feeDataVersion);
        if (dllret)
            throw QString("计费模块函数调用失败");

        //        con1 = new TADOConnection(NULL);
        TGrantryDBCfg dbCfg;
        dbCfg.dbName = QString::fromStdString(dbName);
        dbCfg.dbPwd = QString::fromStdString(dbPwd);
        dbCfg.dbUser = QString::fromStdString(dbUser);
        dbCfg.dbHost = QString::fromStdString(dbHost);
        dbCfg.LanedID = laneId;
        ConnectDB(dbCfg);

        if (dllret == 0)
            ret = true;
    } catch (QString &e) {
        m_ErrMsg = e.toUtf8().data();
    } catch (...) {
        m_ErrMsg = "...";
    }
    LOG_CDEBUG("gantry").noquote() << "MgsLaneGantry_Init end. " << m_ErrMsg;
    return ret;
}

bool CMgsLaneGantry::MgsLaneGantry_LoadParam()
{
    bool ret = false;
    int dllret = -1;
    m_ErrMsg = "";
    if (gan_log == NULL) {
        return false;
    }
    LOG_CDEBUG("gantry").noquote() << "MgsLaneGantry_LoadParam begin.";
    T_ParamVersion *p1 = NULL;
    int p1cnt = 0;
    T_ETCFlag *p2 = NULL;
    int p2cnt = 0;
    T_FlagFee *p3 = NULL;
    int p3cnt = 0;
    T_HolidayFree *p4 = NULL;
    int p4cnt = 0;
    T_FF_Node *p5 = NULL;
    int p5cnt = 0;
    T_FF_NodeRelation *p6 = NULL;
    int p6cnt = 0;
    T_ParamAxisRatio *p7 = NULL;
    int p7cnt = 0;
    T_DiscountFee *p8 = NULL;
    int p8cnt = 0;

    try {
        CheckDB();
        int p18v = 0;
        p18v = GetParamVersion(18);
        int p20v = 0;
        p20v = GetParamVersion(20);
        if (_libfeecalc_loadFlagFee == NULL) {
            throw QString("计费模块函数未加载");
        }
        p1 = Load_T_ParamVersion(&p1cnt);
        p2 = Load_T_ETCFlag(&p2cnt);
        p3 = Load_T_FlagFee(&p3cnt, p20v);
        p4 = Load_T_HolidayFree(&p4cnt);
        p5 = Load_T_FF_Node(&p5cnt);
        p6 = Load_T_FF_NodeRelation(&p6cnt, p20v);
        p7 = Load_T_ParamAxisRatio(&p7cnt, p20v);
        p8 = Load_T_DiscountFee(&p8cnt, p18v);
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_loadFlagFee begin.";
        dllret =
                _libfeecalc_loadFlagFee(p1, p1cnt, p2, p2cnt, p3, p3cnt, p4, p4cnt, p5,
                                        p5cnt, p6, p6cnt, p7, p7cnt, p8, p8cnt);
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_loadFlagFee end. return " << dllret;

        if (dllret)
            ret=false;// QString("计费模块函数调用失败");
        if (dllret == 0)
            ret = true;
    } catch (QString &e) {

        m_ErrMsg = e.toUtf8().data();
    } catch (...) {
        m_ErrMsg = "...";
    }

    try {
        if (p1)
            delete[] p1;
        if (p2)
            delete[] p2;
        if (p3)
            delete[] p3;
        if (p4)
            delete[] p4;
        if (p5)
            delete[] p5;
        if (p6)
            delete[] p6;
        if (p7)
            delete[] p7;
        if (p8)
            delete[] p8;
    } catch (...) {
    }
    LOG_CDEBUG("gantry").noquote() << "MgsLaneGantry_LoadParam end. " << m_ErrMsg;
    return ret;
}

void CMgsLaneGantry::LogGantryTradeInfo(GantryTradeInfo *info)
{
    if (!info)
        return;
    QString s = "";
    s.sprintf("[libfeecalc交易信息] curTollStationHex:%s curPassTime:%s "
              "enTollStationHex:%s enPassTime:%s lastGantryHex:%s "
              "lastPasstime:%s issuerId:%s vehicleUserType:%d axleCount:%d "
              "enAxleCount:%d tagType:%d cardType:%d "
              "cardVer:%d plateHex:%s plateColor:%d vehicleType:%d "
              "enVehicleType:%d vehicleClass:%d "
              "feeSumLocal:%d vehicleStatusFlag:%d feeProvBeginHex:%s "
              "feeProvMileage:%d payFeeSumLocal:%d realFeeSumLocal:%d "
              "gantryPassNum:%d gantryPassHex:%s ",
              info->curTollStationHex, info->curPassTime, info->enTollStationHex,
              info->enPassTime, info->lastGantryHex, info->lastPasstime,
              info->issuerId, info->vehicleUserType, info->axleCount,
              info->enAxleCount, info->tagType, info->cardType, info->cardVer,
              info->plateHex, info->plateColor, info->vehicleType,
              info->enVehicleType, info->vehicleClass, info->feeSumLocal,
              info->vehicleStatusFlag, info->feeProvBeginHex,
              info->feeProvMileage, info->payFeeSumLocal, info->realFeeSumLocal,
              info->gantryPassNum, info->gantryPassHex);
    LOG_CDEBUG("gantry").noquote() << s;
    LOG_CDEBUG("gantry").noquote()<< "size=" << sizeof(GantryTradeInfo);
}

void CMgsLaneGantry::LogGantryFeeInfo(GantryFeeInfo *info) {
    if (!info)
        return;
    QString s = "";
    s.sprintf(
                "[libfeecalc计费信息] FFShouldPay:%d FFDiscountFee:%d FFTransFee:%d "
                "paramVersion:%s feeSpecial:%s "
                "ffIntervalIDs:%s ffIntervalPayFees:%s ffIntervalDiscounts:%s "
                "ffIntervalFees:%s feeInfo1:%s feeInfo2:%s feeInfo3:%s feeLogMsg:%s "
                "FFVehClass:%d feeMileage:%d FFFactPay:%d rateCompute:%d rateFitCount:%d "
                "obuFeeType:%d ffIntervalNames:%s flagIDs:%s flagNames:%s "
                "flagPayFees:%s flagDiscounts:%s flagFees:%s fitResult:%d "
                "feeProvRealSum:%d feeProvPaySum:%d feeProvMileage:%d feeSpare3:%s "
                "gantryPassNumFit:%d gantryPassHexFit:%s",
                info->FFShouldPay, info->FFDiscountFee, info->FFTransFee,
                info->paramVersion, info->feeSpecial, info->ffIntervalIDs,
                info->ffIntervalPayFees, info->ffIntervalDiscounts, info->ffIntervalFees,
                info->feeInfo1, info->feeInfo2, info->feeInfo3, info->feeLogMsg,
                info->FFVehClass, info->feeMileage, info->FFFactPay, info->rateCompute,
                info->rateFitCount, info->obuFeeType,
                GBKArray2String(info->ffIntervalNames).toUtf8().data(), info->flagIDs,
                GBKArray2String(info->flagNames).toUtf8().data(), info->flagPayFees,
                info->flagDiscounts, info->flagFees, info->fitResult,
                info->feeProvRealSum, info->feeProvPaySum, info->feeProvMileage,
                info->feeSpare3, info->gantryPassNumFit, info->gantryPassHexFit);
    LOG_CDEBUG("gantry").noquote() << s;
    LOG_CDEBUG("gantry").noquote() << "size=" << sizeof(GantryFeeInfo);
}

bool CMgsLaneGantry::is_str_utf8(const char *str) {
    unsigned int nBytes = 0; // UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr = *str;
    bool bAllAscii = true;
    for (unsigned int i = 0; str[i] != '\0'; ++i) {
        chr = *(str + i);
        //判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0) {

            bAllAscii = false;
        }
        if (nBytes == 0) {
            //如果不是ASCII码,应该是多字节符,计算字节数
            if (chr >= 0x80) {
                if (chr >= 0xFC && chr <= 0xFD) {
                    nBytes = 6;
                } else if (chr >= 0xF8) {
                    nBytes = 5;
                } else if (chr >= 0xF0) {
                    nBytes = 4;
                } else if (chr >= 0xE0) {
                    nBytes = 3;
                } else if (chr >= 0xC0) {
                    nBytes = 2;
                } else {
                    return false;
                }
                nBytes--;
            }
        } else {
            //多字节符的非首字节,应为 10xxxxxx
            if ((chr & 0xC0) != 0x80) {
                return false;
            }
            //减到为零为止
            nBytes--;
        }
    }
    //违返UTF8编码规则
    if (nBytes != 0) {
        return false;
    }
    if (bAllAscii) {
        return false;
    }
    return true;
}

QString CMgsLaneGantry::NormalizeIntervalID(char *itrvid) {
    QString ret = "";
    LOG_CDEBUG("gantry").noquote() << "收费单元处理前:"<< itrvid;
    QStringList sl = QString(itrvid).split("|");

    try {
        for (int i = 0; i < sl.count(); i++) {
            QString id = sl.at(i).toUpper();
            int l = id.length();
            if (l > 1)
                id = id.mid(0, l - 1);

            if (i == 0) {
                ret = id;
            } else {
                ret += "|" + id;
            }
        }
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << "NormalizeIntervalID:" << e;
    }
    LOG_CDEBUG("gantry").noquote() << "收费单元处理后:" << ret;
    return ret;
}

QString CMgsLaneGantry::NormalizeFlagID(char *itrvid) {
    QString ret = "";
    LOG_CDEBUG("gantry").noquote() << "门架号处理前:" << itrvid;
    try {
        ret = QString(itrvid).toUpper();
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << "NormalizeFlagID:" << e;
    }
    LOG_CDEBUG("gantry").noquote() << "门架号处理后:" << ret;
    return ret;
}

QString CMgsLaneGantry::NormalizeSpecialType(char *sp, int rateCompute, int fitResult) {
    // 0无特情 1节假日免费 2反向干扰 3余额不足 4拟合补漏的门架数大于阀值个数
    // 5剔除前序错误门架
    /*
    计费模块初始化失败	计费模块初始化异常导致计费失败	出口	61
    获取计费模块版本失败	计费模块获取版本号异常导致计费失败	出口
    62 计费模块查询费率返回失败	调用计费模块计费接口返回报错	出口	63

    路径不可通达	计费模块返回路径不可达的特情值	出口	66
    拟合门架数过多	计费模块返回拟合补漏的门架数量超出阀值（省内自行定义）
    出口	67 计费金额异常 计费模块返回的金额异常（省内自行定义异常阈值）
    出口	68
  */

    LOG_CDEBUG("gantry").noquote() << "特情处理前:" << sp;
    QStringList sl = QString(sp).split("W");
    QString ret = "";

    try {
        for (int i = 0; i < sl.count(); i++) {
            QString id = sl.at(i).toUpper();
            if (id == "4")
                id = "67";
            else
                id = "";
            if (id == "")
                continue;
            if (ret.length() == 0) {
                ret = id;
            } else {
                ret += "|" + id;
            }
        }
        if (rateCompute == 2) {
            if (ret.length() == 0) {
                ret = "500";
            } else {
                ret += "|500";
            }
        } else if (rateCompute == 1) {
            if (fitResult == 1) {
                if (ret.length() == 0) {
                    ret = "68";
                } else {
                    ret += "|68";
                }
            }
        }
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << "NormalizeSpecialType:" << e;
    }
    LOG_CDEBUG("gantry").noquote() << "特情处理后:" << ret;
    return ret;
}

bool CMgsLaneGantry::MgsLaneGantry_GetFlagFee(GantryTradeInfo *tradeingInfo,
                                              GantryFeeInfo *feeInfo) {
    bool ret = false;
    int dllret = -1;
    m_ErrMsg = "";
    if (gan_log == NULL) {
        //    gan_log = GetLaneLog("gantry");
        //    gan_log->level(0);
        return false;
    }
    if (tradeingInfo == NULL || feeInfo == NULL)
        return dllret;

    LOG_CDEBUG("gantry").noquote() << "MgsLaneGantry_GetFlagFee begin.";
    try {
        if (_libfeecalc_getFlagFee == NULL) {
            throw QString("计费模块函数未加载");
        }
        FlagTradingInfo tinfo;
        FlagFeeInfo finfo;
        memset(&tinfo, 0, sizeof(FlagTradingInfo));
        memset(&finfo, 0, sizeof(FlagFeeInfo));
        memcpy((GantryTradeInfo *)&tinfo, tradeingInfo, sizeof(GantryTradeInfo));

        LogGantryTradeInfo((GantryTradeInfo *)&tinfo);
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_getFlagFee begin.";
        dllret = _libfeecalc_getFlagFee(tinfo, finfo);
        LOG_CDEBUG("gantry").noquote() << "libfeecalc_getFlagFee end. return " << dllret;

        if (is_str_utf8(finfo.ffIntervalNames)) {
            QByteArray ba = String2GBKArray(finfo.ffIntervalNames);
            snprintf(finfo.ffIntervalNames, sizeof(finfo.ffIntervalNames), "%s",
                     ba.data());
        }
        if (is_str_utf8(finfo.flagNames)) {
            QByteArray ba = String2GBKArray(finfo.flagNames);
            snprintf(finfo.flagNames, sizeof(finfo.flagNames), "%s", ba.data());
        }
        QString intevalid = NormalizeIntervalID(finfo.ffIntervalIDs);
        snprintf(finfo.ffIntervalIDs, sizeof(finfo.ffIntervalIDs), "%s",
                 intevalid.toUtf8().data());

        QString flagids = NormalizeFlagID(finfo.flagIDs);
        snprintf(finfo.flagIDs, sizeof(finfo.flagIDs), "%s",
                 flagids.toUtf8().data());

        QString specialtype = NormalizeSpecialType(
                    finfo.feeSpecial, finfo.rateCompute, finfo.fitResult);
        snprintf(finfo.feeSpecial, sizeof(finfo.feeSpecial), "%s",
                 specialtype.toUtf8().data());
        LogGantryFeeInfo((GantryFeeInfo *)&finfo);
        memcpy(feeInfo, &finfo, sizeof(FlagFeeInfo));
        if (dllret) {
            strcpy(finfo.feeSpecial, "63");
            throw QString("计费模块函数调用失败");
        }
        ret = true;
    } catch (QString &e) {
        m_ErrMsg = e.toUtf8().data();
    } catch (...) {
        m_ErrMsg = "...";
    }
    LOG_CDEBUG("gantry").noquote() << "MgsLaneGantry_GetFlagFee end. " << m_ErrMsg;

    return ret;
}

bool CMgsLaneGantry::MgsLaneGantry_LoadNewParam(int paramtype, int newver)
{
    return MgsLaneGantry_LoadParam();

//    bool ret = false;
//    int dllret = -1;
//    m_ErrMsg = "";
//    if (gan_log == NULL) {
//        //    gan_log = GetLaneLog("gantry");
//        //    gan_log->level(0);
//        return false;
//    }

//    gan_log->debug("MgsLaneGantry_LoadNewParam begin.");
//    T_ParamVersion *p1 = NULL;
//    int p1cnt = 0;
//    T_ETCFlag *p2 = NULL;
//    int p2cnt = 0;
//    T_FlagFee *p3 = NULL;
//    int p3cnt = 0;
//    T_HolidayFree *p4 = NULL;
//    int p4cnt = 0;
//    T_FF_Node *p5 = NULL;
//    int p5cnt = 0;
//    T_FF_NodeRelation *p6 = NULL;
//    int p6cnt = 0;
//    T_ParamAxisRatio *p7 = NULL;
//    int p7cnt = 0;
//    T_DiscountFee *p8 = NULL;
//    int p8cnt = 0;

//    try {
//        if (_libfeecalc_loadFlagFee == NULL) {
//            throw QString("计费模块函数未加载");
//        }
//        FlagTradingInfo tradingInfo;
//        FlagFeeInfo feeInfo;
//        memset(&tradingInfo, 0, sizeof(tradingInfo));
//        memset(&feeInfo, 0, sizeof(feeInfo));

//        gan_log->debug("libfeecalc_loadFlagFee begin.");
//        p1 = Load_T_ParamVersion(&p1cnt);
//        p4 = Load_T_HolidayFree(&p4cnt);
//        if (paramtype == 18) {
//            p8 = Load_T_DiscountFee(&p8cnt, newver);
//        } else if (paramtype == 20) {
//            p2 = Load_T_ETCFlag(&p2cnt);
//            p3 = Load_T_FlagFee(&p3cnt, newver);
//            p5 = Load_T_FF_Node(&p5cnt);
//            p6 = Load_T_FF_NodeRelation(&p6cnt, newver);
//            p7 = Load_T_ParamAxisRatio(&p7cnt, newver);
//        }
//        dllret =
//                _libfeecalc_loadFlagFee(p1, p1cnt, p2, p2cnt, p3, p3cnt, p4, p4cnt, p5,
//                                        p5cnt, p6, p6cnt, p7, p7cnt, p8, p8cnt);
//        gan_log->debug("libfeecalc_loadFlagFee end. return %d", dllret);
//        if (dllret)
//            throw QString("计费模块函数调用失败");
//        ret = true;
//    } catch (QString &e) {
//        m_ErrMsg = e.toUtf8().data();
//    } catch (...) {
//        m_ErrMsg = "...";
//    }

//    try {
//        if (p1)
//            delete[] p1;
//        if (p2)
//            delete[] p2;
//        if (p3)
//            delete[] p3;
//        if (p4)
//            delete[] p4;
//        if (p5)
//            delete[] p5;
//        if (p6)
//            delete[] p6;
//        if (p7)
//            delete[] p7;
//        if (p8)
//            delete[] p8;
//    } catch (...) {
//    }

//    gan_log->debug("MgsLaneGantry_LoadNewParam end. %s", m_ErrMsg.toStdString().c_str());
//    return ret;
}

void CMgsLaneGantry::ConnectDB(TGrantryDBCfg dbCfg)
{
    try {
        if (QSqlDatabase::contains("mysql"))
            db = QSqlDatabase::database("mysql");
        else
            db = QSqlDatabase::addDatabase("QMYSQL", "gantry"+QString::number(dbCfg.LanedID)+"_"+QString::number(m_index));
        //    db = QSqlDatabase::addDatabase("QMYSQL");
        db.setPort(3306);
        if(dbCfg.dbHost.isEmpty()){
            db.setHostName("127.0.0.1");
            db.setDatabaseName("tolllanedb");
            db.setUserName("tlman");
            db.setPassword("ds18fjeit");
            gantryDB.dbName = "tolllanedb";
            gantryDB.dbPwd = "ds18fjeit";
            gantryDB.dbUser = "tlman";
            gantryDB.dbHost = "127.0.0.1";
        }else{
            db.setHostName(dbCfg.dbHost);
            db.setDatabaseName(dbCfg.dbName);
            db.setUserName(dbCfg.dbUser);
            db.setPassword(dbCfg.dbPwd);
            gantryDB = dbCfg;
        }
        if (!db.isOpen()) {
            bool ok = db.open();
            if (ok) {
                LOG_CDEBUG("gantry").noquote() << "open db success";
            } else {
                LOG_CDEBUG("gantry").noquote() << "open db error";
            }
            LOG_CDEBUG("gantry").noquote() << "ConnectDB";
        }
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << "ConnectDB exception: " << e;
    } catch (...) {
        LOG_CDEBUG("gantry").noquote() << "ConnectDB exception: ...";
    }
}

void CMgsLaneGantry::CheckDB()
{
    QSqlQuery query(db);
    if (query.exec("select CURRENT_TIMESTAMP")) {
        QSqlRecord rec = query.record();
    } else {
        db.close();
        QThread::sleep(2);
        ConnectDB(gantryDB);
        LOG_CDEBUG("gantry").noquote() << "CheckDB";
    }
}

int CMgsLaneGantry::GetParamVersion(int paramtype)
{
    QSqlQuery query(db);
    QString sql;
    int version = 0;
    sql.sprintf("select version from t_paramversion where paramtype=%d and "
                "IsUsed=1 and enabletime <= CURRENT_TIMESTAMP and "
                "CURRENT_TIMESTAMP < disabletime order by version desc ",
                paramtype);
    if (query.exec(sql)) {
        QSqlRecord rec = query.record();
        if (query.next()) {
            rec = query.record();
            int snocol = rec.indexOf("version");
            version = query.value(snocol).toInt();
            LOG_CDEBUG("gantry").noquote() << QString("GetParamVersion paramtype:%1 当前版本:%2 ").arg(paramtype)
                           .arg(version);
        }
    }
    return version;
}

T_ParamVersion *CMgsLaneGantry::Load_T_ParamVersion(int *cnt)
{
    QSqlQuery query(db);
    T_ParamVersion *p = NULL;
    QString sql = "";
    QString strdt = "";

    sql = "select ParamType,Version,VersionStr,EnableTime,DisableTime,IsUsed "
          "from t_paramversion where current_timestamp < DisableTime ";
    try {
        if (query.exec(sql)) {
            QSqlRecord rec = query.record();
            p = new T_ParamVersion[query.size()];
            int i = 0;
            while (query.next()) {
                rec = query.record();
                p[i].ParamType = rec.value("ParamType").toInt();
                p[i].Version = rec.value("Version").toInt();
                snprintf(p[i].VersionStr, sizeof(p[i].VersionStr), "%s",
                         rec.value("VersionStr").toString().toUtf8().data());
                strdt = rec.value("EnableTime")
                        .toDateTime()
                        .toString("yyyy-MM-ddThh:mm:ss");
                snprintf(p[i].EnableTime, sizeof(p[i].EnableTime), "%s",
                         strdt.toUtf8().data());
                strdt = rec.value("DisableTime")
                        .toDateTime()
                        .toString("yyyy-MM-ddThh:mm:ss");
                snprintf(p[i].DisableTime, sizeof(p[i].DisableTime), "%s",
                         strdt.toUtf8().data());
                p[i].IsUsed = rec.value("IsUsed").toInt();
                //        log->debug("%d %d %s %s %s   %d", p[i].ParamType,
                //        p[i].Version, p[i].VersionStr, p[i].EnableTime,
                //        p[i].DisableTime, p[i].IsUsed);
                ++i;
            }
            *cnt = i;
        }
        LOG_CDEBUG("gantry").noquote() << QString("T_ParamVersion 载入记录条数:%1 %2").arg(*cnt).arg(p);
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << "Exception: " << e << " " << sql;
    }
    return p;
}

T_ETCFlag *CMgsLaneGantry::Load_T_ETCFlag(int *cnt)
{
    QSqlQuery query(db);
    T_ETCFlag *p = NULL;
    QString sql = "";
    QString strdt = "";

    sql = "select "
          "FlagID,GantryID,FlagIndex,FlagName,FlagPlate,FlagType,RoadWay,IsUsed,"
          "IsVirtual from t_etcflag";

    try {
        if (query.exec(sql)) {
            QSqlRecord rec = query.record();
            p = new T_ETCFlag[query.size()];
            int i = 0;
            while (query.next()) {
                rec = query.record();
                snprintf(p[i].FlagID, sizeof(p[i].FlagID), "%s",
                         rec.value("FlagID").toString().toUtf8().data());
                snprintf(p[i].GantryID, sizeof(p[i].GantryID), "%s",
                         rec.value("GantryID").toString().toUtf8().data());
                p[i].FlagIndex = rec.value("FlagID").toInt();
                snprintf(p[i].FlagName, sizeof(p[i].FlagName), "%s",
                         rec.value("FlagName").toString().toUtf8().data());
                snprintf(p[i].FlagPlate, sizeof(p[i].FlagPlate), "%s",
                         rec.value("FlagPlate").toString().toUtf8().data());
                p[i].FlagType = rec.value("FlagType").toInt();
                p[i].RoadWay = rec.value("RoadWay").toInt();
                p[i].IsUsed = rec.value("IsUsed").toInt();
                p[i].IsVirtual = rec.value("IsVirtual").toInt();
                ++i;
            }
            *cnt = i;
        }
        LOG_CDEBUG("gantry").noquote() << QString("T_ETCFlag 载入记录条数:%1 %2").arg(*cnt).arg(p);
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << QString("Exception: %1 %2").arg(e).arg(sql);
    }
    return p;
}

T_FlagFee *CMgsLaneGantry::Load_T_FlagFee(int *cnt, int ver)
{
    QSqlQuery query(db);
    T_FlagFee *p = NULL;
    QString sql = "";
    QString strdt = "";

    sql.sprintf(
                "select "
                "Version,IntervalID,IntervalName,FlagID,Fee1,Fee2,Fee3,Fee4,Fee5,Fee6,"
                "Fee11,Fee12,Fee13,Fee14,Fee15,Fee16,Fee17,Fee21,Fee22,Fee23,Fee24,Fee25,"
                "Fee26,Fee27,ChkPoint, Distance from t_flagfee where Version=%d ",
                ver);
    try {
        if (query.exec(sql)) {
            QSqlRecord rec = query.record();
            p = new T_FlagFee[query.size()];
            int i = 0;
            while (query.next()) {
                rec = query.record();
                p[i].Version = rec.value("Version").toInt();
                snprintf(p[i].IntervalID, sizeof(p[i].IntervalID), "%s",
                         rec.value("IntervalID").toString().toUtf8().data());
                snprintf(p[i].IntervalName, sizeof(p[i].IntervalName), "%s",
                         rec.value("IntervalName").toString().toUtf8().data());
                snprintf(p[i].FlagID, sizeof(p[i].FlagID), "%s",
                         rec.value("FlagID").toString().toUtf8().data());
                p[i].Fee1 = rec.value("Fee1").toInt();
                p[i].Fee2 = rec.value("Fee2").toInt();
                p[i].Fee3 = rec.value("Fee3").toInt();
                p[i].Fee4 = rec.value("Fee4").toInt();
                p[i].Fee5 = rec.value("Fee5").toInt();
                p[i].Fee6 = rec.value("Fee6").toInt();
                p[i].Fee11 = rec.value("Fee11").toInt();
                p[i].Fee12 = rec.value("Fee12").toInt();
                p[i].Fee13 = rec.value("Fee13").toInt();
                p[i].Fee14 = rec.value("Fee14").toInt();
                p[i].Fee15 = rec.value("Fee15").toInt();
                p[i].Fee16 = rec.value("Fee16").toInt();
                p[i].Fee17 = rec.value("Fee17").toInt();
                p[i].Fee21 = rec.value("Fee21").toInt();
                p[i].Fee22 = rec.value("Fee22").toInt();
                p[i].Fee23 = rec.value("Fee23").toInt();
                p[i].Fee24 = rec.value("Fee24").toInt();
                p[i].Fee25 = rec.value("Fee25").toInt();
                p[i].Fee26 = rec.value("Fee26").toInt();
                p[i].Fee27 = rec.value("Fee27").toInt();
                p[i].ChkPoint = rec.value("ChkPoint").toInt();
                p[i].Distance = rec.value("Distance").toInt();
                ++i;
            }
            *cnt = i;
        }
        LOG_CDEBUG("gantry").noquote() << QString("T_FlagFee 载入记录条数:%1 %2").arg(*cnt).arg(p);
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << QString("Exception: %1 %2").arg(e).arg(sql);
    }
    return p;
}

T_HolidayFree *CMgsLaneGantry::Load_T_HolidayFree(int *cnt)
{
    T_HolidayFree *p = NULL;
    return p;
}

T_FF_Node *CMgsLaneGantry::Load_T_FF_Node(int *cnt)
{
    QSqlQuery query(db);
    T_FF_Node *p = NULL;
    QString sql = "";
    QString strdt = "";
    sql = "select NodeID,NodeName,NodeType,IsUsed from t_ff_node ";
    try {
        if (query.exec(sql)) {
            QSqlRecord rec = query.record();
            p = new T_FF_Node[query.size()];
            int i = 0;
            while (query.next()) {
                rec = query.record();
                snprintf(p[i].NodeID, sizeof(p[i].NodeID), "%s",
                         rec.value("NodeID").toString().toUtf8().data());
                snprintf(p[i].NodeName, sizeof(p[i].NodeName), "%s",
                         rec.value("NodeName").toString().toUtf8().data());
                p[i].NodeType = rec.value("NodeType").toInt();
                p[i].IsUsed = rec.value("IsUsed").toInt();
                ++i;
            }
            *cnt = i;
        }
        LOG_CDEBUG("gantry").noquote() << QString("T_FF_Node 载入记录条数:%1 %2").arg(*cnt).arg(p);
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << QString("Exception: %1 %2").arg(e).arg(sql);
    }
    return p;
}

T_FF_NodeRelation *CMgsLaneGantry::Load_T_FF_NodeRelation(int *cnt, int ver)
{
    QSqlQuery query(db);
    T_FF_NodeRelation *p = NULL;
    QString sql = "";
    QString strdt = "";
    sql.sprintf("select EnNodeID,ExNodeID from t_ff_noderelation where "
                "startversion<=%d and %d<=stopversion",
                ver, ver);
    try {
        if (query.exec(sql)) {
            QSqlRecord rec = query.record();
            p = new T_FF_NodeRelation[query.size()];
            int i = 0;
            while (query.next()) {
                rec = query.record();
                snprintf(p[i].EnNodeID, sizeof(p[i].EnNodeID), "%s",
                         rec.value("EnNodeID").toString().toUtf8().data());
                snprintf(p[i].ExNodeID, sizeof(p[i].ExNodeID), "%s",
                         rec.value("ExNodeID").toString().toUtf8().data());
                ++i;
            }
            *cnt = i;
        }
        LOG_CDEBUG("gantry").noquote() << QString("T_FF_NodeRelation V%1 载入记录条数:%2 %3").arg(ver).arg(*cnt).arg(p);
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << QString("Exception: %1 %2").arg(e).arg(sql);
    }
    return p;
}

T_ParamAxisRatio *CMgsLaneGantry::Load_T_ParamAxisRatio(int *cnt, int ver)
{
    QSqlQuery query(db);
    T_ParamAxisRatio *p = NULL;
    QString sql = "";
    QString strdt = "";
    sql.sprintf("select StartAxises,StopAxises,AxisRatio from t_paramaxisratio "
                "where startversion<=%d and %d<=stopversion",
                ver, ver);
    try {
        if (query.exec(sql)) {
            QSqlRecord rec = query.record();
            p = new T_ParamAxisRatio[query.size()];
            int i = 0;
            while (query.next()) {
                rec = query.record();
                p[i].StartAxises = rec.value("StartAxises").toInt();
                p[i].StopAxises = rec.value("StopAxises").toInt();
                p[i].AxisRatio = rec.value("AxisRatio").toDouble();
                ++i;
            }
            *cnt = i;
        }
        LOG_CDEBUG("gantry").noquote() << QString("T_ParamAxisRatio V%1 载入记录条数:%2 %3").arg(ver).arg(*cnt).arg(p);
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << "Exception: " << e;
    }
    return p;
}

T_DiscountFee *CMgsLaneGantry::Load_T_DiscountFee(int *cnt, int ver)
{
    QSqlQuery query(db);
    T_DiscountFee *p = NULL;
    QString sql = "";
    QString strdt = "";
    sql.sprintf(
                "select DiscountType,DiscountName,Discount from t_discountfee where "
                "discounttype in (1,2) and startversion<=%d and %d<=stopversion",
                ver, ver);
    try {
        if (query.exec(sql)) {
            QSqlRecord rec = query.record();
            p = new T_DiscountFee[query.size()];
            int i = 0;
            while (query.next()) {
                rec = query.record();
                p[i].DiscountType = rec.value("DiscountType").toInt();
                snprintf(p[i].DiscountName, sizeof(p[i].DiscountName), "%s",
                         rec.value("DiscountName").toString().toUtf8().data());
                p[i].Discount = rec.value("Discount").toDouble();
                ++i;
            }
            *cnt = i;
        }
        LOG_CDEBUG("gantry").noquote() << QString("T_DiscountFee V%d 载入记录条数:%d %p").arg(ver).arg(*cnt).arg(p);
    } catch (QString &e) {
        LOG_CDEBUG("gantry").noquote() << "Exception: " << e << " " << sql;
    }
    return p;
}

QString CMgsLaneGantry::GBKArray2String(char *gbk) {
    QString s;
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    if (tc)
        s = tc->toUnicode(gbk);
    return s;
}

QByteArray CMgsLaneGantry::String2GBKArray(const QString &s) {
    QByteArray ba;
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    if (tc)
        ba = tc->fromUnicode(s);
    return ba;
}
