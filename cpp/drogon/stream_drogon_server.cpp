#include <drogon/drogon.h>
#include <string>
#include <iostream>
#include <atomic>
#include <chrono>

using namespace drogon;
using namespace std::chrono_literals;

int main()
{
    // 统计变量
    std::atomic<int> upload_count{0};
    std::atomic<size_t> total_bytes_received{0};
    std::atomic<size_t> active_uploads{0};

    // 设置上传路由
    app().registerHandler(
        "/upload",
        [&upload_count, &total_bytes_received, &active_uploads](
            const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback) {

            // 记录活跃上传数
            active_uploads++;

            // 设置流式请求处理
            req->setBodyCallback([&](const char *data, size_t len) {
                total_bytes_received += len;
                return len;
            });

            // 请求完成回调
            req->setCompleteCallback([&]() {
                upload_count++;
                active_uploads--;

                // 每10次上传打印一次日志
                if (upload_count % 10 == 0) {
                    LOG_INFO << "已处理上传请求: " << upload_count
                            << ", 总接收数据: " << total_bytes_received / (1024 * 1024) << "MB";
                }
            });

            // 创建响应
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("Upload received");
            callback(resp);
        },
        {Post});

    // 添加统计路由
    app().registerHandler(
        "/upload_stats",
        [&upload_count, &total_bytes_received, &active_uploads](
            const HttpRequestPtr &,
            std::function<void(const HttpResponsePtr &)> &&callback) {

            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(
                "活跃上传数: " + std::to_string(active_uploads) + "\n" +
                "总上传请求数: " + std::to_string(upload_count) + "\n" +
                "总接收数据: " + std::to_string(total_bytes_received / (1024 * 1024)) + "MB");
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            callback(resp);
        },
        {Get});

    // 启动服务器
    std::cout << "Server running at http://127.0.0.1:8848" << std::endl;
    std::cout << "上传端点: POST /upload" << std::endl;
    std::cout << "统计端点: GET /upload_stats" << std::endl;

    app()
        .setLogLevel(trantor::Logger::kInfo)
        .addListener("0.0.0.0", 8848)
        .setThreadNum(16) // 根据CPU核心数调整
        .setClientMaxBodySize(1024 * 1024 * 1024) // 允许最大1GB上传
        .run();

    return 0;
}