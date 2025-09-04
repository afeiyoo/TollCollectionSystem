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
    QByteArray logReqBody = reqBody.left(1024);
    LOG_INFO("request size: %d ==>\n%s", reqBody.size(), logReqBody.constData());

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

        QByteArray logSendData = sendData.left(1024);
        LOG_INFO("response size: %d <==\n%s", sendData.size(), logSendData.constData());
        response.write(sendData, true);
    } catch (const BaseException &e) {
        QString dealtData = QString::fromUtf8(R"({"status":"%1","desc":"%2"})").arg(e.status()).arg(e.desc());

        aMap.remove("data");
        aMap.insert("data", dealtData);
        QByteArray sendData = GM_INSTANCE->m_jsonSerializer->serialize(aMap);

        QByteArray logSendData = sendData.left(1024);
        LOG_INFO("response size: %d <==\n%s", sendData.size(), logSendData.constData());
        response.write(sendData, true);
    }
}
