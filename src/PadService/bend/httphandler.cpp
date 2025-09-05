#include "httphandler.h"

#include "Logger.h"
#include "bend/bizhandler.h"
#include "config/baseexception.h"
#include "global/globalmanager.h"

using namespace stefanfrings;

HttpHandler::HttpHandler(QObject *parent)
    : HttpRequestHandler{parent}
{}

HttpHandler::~HttpHandler() {}

void HttpHandler::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray reqBody = request.getBody();
    LOG_INFO().noquote() << QString("request size: %1 ==>\n%2")
                                .arg(reqBody.size())
                                .arg(QString::fromUtf8(reqBody.left(1024)));

    if (reqBody.size() <= 0)
        return;

    bool ok;
    QVariantMap aMap = GM_INSTANCE->m_jsonParser->parse(reqBody, &ok).toMap();
    if (!ok) {
        throw BaseException(1, "响应失败: Json解析异常(整体)");
    }

    int cmdType = aMap["cmdType"].toInt();
    QVariantMap dataMap = aMap["data"].toMap();
    if (dataMap.isEmpty())
        throw BaseException(1, "响应失败: Json解析异常(data字段内容为空)");

    try {
        BizHandler handler;
        QString dealtData = handler.doMainDeal(cmdType, dataMap, reqBody);

        aMap.remove("data");
        aMap.insert("data", dealtData);
        QByteArray sendData = GM_INSTANCE->m_jsonSerializer->serialize(aMap);

        LOG_INFO().noquote() << QString("response size: %1 <==\n%2")
                                    .arg(sendData.size())
                                    .arg(QString::fromUtf8(sendData.left(1024)));
        response.write(sendData, true);
    } catch (const BaseException &e) {
        QString dealtData = QString::fromUtf8(R"({"status":"%1","desc":"%2"})").arg(e.status()).arg(e.desc());

        aMap.remove("data");
        aMap.insert("data", dealtData);
        QByteArray sendData = GM_INSTANCE->m_jsonSerializer->serialize(aMap);

        LOG_INFO().noquote() << QString("response size: %1 <==\n%2")
                                    .arg(sendData.size())
                                    .arg(QString::fromUtf8(sendData.left(1024)));
        response.write(sendData, true);
    }
}
