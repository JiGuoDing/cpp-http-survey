#include <drogon/drogon.h>
#include <string>
#include <iostream>
#include <atomic>

using namespace drogon;

int main()
{
    // 创建30MB的测试数据 (约3000万个字符)
    const size_t data_size = 30 * 1024 * 1024; // 30MB
    std::string large_data(data_size, 'A'); // 填充'A'字符

    // 请求计数器(原子操作，线程安全)
    std::atomic<int> request_count{0};

    // 设置HTTP路由
    app().registerHandler(
        "/large_data",
        // 一个lambda回调函数
        [large_data, &request_count](const HttpRequestPtr &,
            // lambda回调函数
                    std::function<void(const HttpResponsePtr &)> &&callback) {
            // 请求计数增加
            int count = ++request_count;

            // 每10个请求打印一次日志
            if (count % 10 == 0) {
                LOG_INFO << "已处理请求数量: " << count;
            }

            // 创建一个新的HTTP响应对象
            auto resp = HttpResponse::newHttpResponse();
            // 设置响应体内容
            resp->setBody(large_data);
            // 设置内容类型为纯文本
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            callback(resp);
        },
        {Get});

    // 添加一个显示总请求数的路由
    app().registerHandler(
        "/request_stats",
        [&request_count](const HttpRequestPtr &,
                    std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("总请求数: " + std::to_string(request_count));
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            callback(resp);
        },
        {Get});

    // 启动服务器
    std::cout << "Server running at http://127.0.0.1:8848" << std::endl;
    std::cout << "测试端点: /large_data (30MB数据)" << std::endl;
    std::cout << "统计端点: /request_stats (查看请求数)" << std::endl;

    app()
        .setLogLevel(trantor::Logger::kInfo) // 设置日志级别
        .addListener("0.0.0.0", 8848)
        .setThreadNum(16) // 根据CPU核心数调整
        .run();

    return 0;
}