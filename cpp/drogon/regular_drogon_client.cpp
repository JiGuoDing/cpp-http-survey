#include <drogon/drogon.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include <iomanip>

using namespace drogon;
using namespace std::chrono;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_address> <port> <request_count>\n";
        std::cerr << "Example: " << argv[0] << " 127.0.0.1 8848 100\n";
        return 1;
    }

    const std::string server_address = argv[1];
    const uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));
    const int request_count = std::stoi(argv[3]);

    // 创建30MB的测试数据
    const size_t data_size = 30 * 1024 * 1024;
    std::string large_data(data_size, 'A');

    std::cout << "Testing http://" << server_address << ":" << port
              << " with " << request_count << " requests ("
              << data_size / (1024 * 1024) << "MB each)\n";

    // 统计变量
    std::atomic<size_t> total_bytes_sent{0};
    std::atomic<size_t> completed_requests{0};
    std::atomic<size_t> failed_requests{0};

    // 创建客户端
    auto client = HttpClient::newHttpClient(server_address, port);
    client->setUserAgent("DrogonBenchmark/1.0");

    auto start = high_resolution_clock::now();

    for (int i = 0; i < request_count; ++i) {
        auto req = HttpRequest::newHttpRequest();
        req->setPath("/receive_data");
        req->setMethod(Post);
        req->setContentTypeCode(CT_TEXT_PLAIN);
        req->setBody(large_data);

        // 打印发送请求详情
        std::cout << "\n=== 发送请求 #" << i+1 << " ===\n";
        std::cout << "目标URL: http://" << server_address << ":" << port << "/receive_data\n";
        std::cout << "请求方法: POST\n";
        std::cout << "请求体大小: " << large_data.size() / (1024 * 1024) << " MB\n";

        client->sendRequest(req, [&, i](ReqResult result, const HttpResponsePtr& resp) {
            // 打印响应详情
            std::cout << "\n=== 收到请求 #" << i+1 << " 的响应 ===\n";

            if (result == ReqResult::Ok && resp) {
                if (resp->getStatusCode() == k200OK) {
                    total_bytes_sent += data_size;
                    std::cout << "状态: 成功 (200 OK)\n";
                    std::cout << "响应体: " << resp->getBody() << "\n";
                } else {
                    ++failed_requests;
                    std::cout << "状态: 失败 (" << resp->getStatusCode() << ")\n";
                }
            } else {
                ++failed_requests;
                std::cout << "状态: 错误 (" << result << ")\n";
            }

            completed_requests++;
            std::cout << "进度: " << completed_requests << "/" << request_count << "\n";

            if (completed_requests == request_count) {
                app().getLoop()->quit();
            }
        });
    }

    app().run();

    auto end = high_resolution_clock::now();
    auto total_duration = duration_cast<milliseconds>(end - start).count();

    // 输出最终结果
    std::cout << "\n\n=== 最终测试结果 ===\n";
    std::cout << "服务器: http://" << server_address << ":" << port << "\n";
    std::cout << "总请求数: " << request_count << "\n";
    std::cout << "成功请求: " << request_count - failed_requests << "\n";
    std::cout << "失败请求: " << failed_requests << "\n";
    std::cout << "总发送数据: " << std::fixed << std::setprecision(2)
              << total_bytes_sent / (1024.0 * 1024.0) << " MB\n";
    std::cout << "总耗时: " << total_duration << " ms\n";
    std::cout << "平均每个请求耗时: " << total_duration / request_count << " ms\n";
    std::cout << "上传吞吐量: " << std::fixed << std::setprecision(2)
              << (total_bytes_sent / (1024.0 * 1024.0)) / (total_duration / 1000.0)
              << " MB/s\n";

    return 0;
}