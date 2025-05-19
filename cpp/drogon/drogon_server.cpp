//
// Created by jgd on 25-5-19.
//

#include <drogon/drogon.h>

int main()
{
    // 设置日志级别
    drogon::app().setLogLevel(trantor::Logger::kInfo);

    // 添加路由
    drogon::app().registerHandler(
        "/hello",
        [](const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback)
        {
            auto resp = drogon::HttpResponse::newHttpResponse();
            LOG_INFO << "Received a message from client.";
            resp->setBody("Hello from Drogon Server.");
            resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
            callback(resp);
        },
        // 只允许 GET 方法
        {drogon::Get});

    LOG_INFO << "Server running at 9873";
    drogon::app().addListener("0.0.0.0", 9873).setThreadNum(4).run();

    return 0;
}