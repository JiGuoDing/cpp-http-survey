#include <drogon/drogon.h>
#include <string>
#include <iostream>
#include <atomic>

using namespace drogon;

int main()
{
    std::atomic request_count{0};
    std::atomic<size_t> total_received_bytes{0};

    app().registerHandler(
        "/receive_data",
        [&request_count, &total_received_bytes](const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback) {

            int count = ++request_count;

            size_t body_size = req->getBody().size();
            total_received_bytes += body_size;

            std::cout << "\n=== 收到请求 ===\n";
            std::cout << "请求 #" << count << "\n";
            std::cout << "客户端IP: " << req->getPeerAddr().toIp() << "\n";
            std::cout << "请求方法: " << req->getMethodString() << "\n";
            std::cout << "请求路径: " << req->getPath() << "\n";
            std::cout << "请求头:\n";
            for (const auto &header : req->getHeaders()) {
                std::cout << "  " << header.first << ": " << header.second << "\n";
            }
            std::cout << "请求体大小: " << body_size << " 字节\n";
            std::cout << "累计接收数据: " << total_received_bytes / (1024 * 1024) << " MB\n";

            auto resp = HttpResponse::newHttpResponse();

            std::string response_body = "Received " + std::to_string(body_size) + " bytes";
            resp->setBody(response_body);

            resp->setContentTypeCode(CT_TEXT_PLAIN);

   
            std::cout << "\n=== 发送响应 ===\n";
            std::cout << "响应状态: " << resp->getStatusCode() << "\n";
            std::cout << "响应体: " << response_body << "\n";

            callback(resp);
        },
        {Post});


    app().registerHandler(
        "/request_stats",
        [&request_count, &total_received_bytes](const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback) {

            std::cout << "\n=== 收到统计请求 ===\n";
            std::cout << "客户端IP: " << req->getPeerAddr().toIp() << "\n";
            std::cout << "请求方法: " << req->getMethodString() << "\n";

            auto resp = HttpResponse::newHttpResponse();
            std::string stats = "总请求数: " + std::to_string(request_count) +
                              "\n总接收数据量: " +
                              std::to_string(total_received_bytes / (1024 * 1024)) + " MB";
            resp->setBody(stats);
            resp->setContentTypeCode(CT_TEXT_PLAIN);


            std::cout << "=== 发送统计响应 ===\n";
            std::cout << "响应内容:\n" << stats << "\n";

            callback(resp);
        },
        {Get});


    std::cout << "Server running at http://127.0.0.1:8848\n";
    std::cout << "数据接收端点: /receive_data (POST)\n";
    std::cout << "统计端点: /request_stats (GET)\n";

    app()
        .setLogLevel(trantor::Logger::kInfo)
        .addListener("0.0.0.0", 8848)
        .setThreadNum(16)
        .setClientMaxBodySize(32 * 1024 * 1024)
        .setClientMaxMemoryBodySize(32 * 1024 * 1024)
        .run();

    return 0;
}