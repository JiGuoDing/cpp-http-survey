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

    std::cout << "Testing http://" << server_address << ":" << port
              << " with " << request_count << " requests\n";

    // 统计变量
    std::atomic<size_t> total_bytes{0};
    std::atomic<size_t> completed_requests{0};
    std::atomic<size_t> failed_requests{0};
    size_t min_size = SIZE_MAX;
    size_t max_size = 0;

    // 创建带端口号的客户端
    auto client = HttpClient::newHttpClient(server_address, port);
    client->setUserAgent("DrogonBenchmark/1.0");

    auto req = HttpRequest::newHttpRequest();
    req->setPath("/large_data");
    req->setMethod(Get);
    req->setContentTypeCode(CT_TEXT_PLAIN);

    // 开始测试
    auto start = high_resolution_clock::now();

    for (int i = 0; i < request_count; ++i) {
        std::cout << "Sending request " << i << "\n";
        client->sendRequest(req, [&](ReqResult result, const HttpResponsePtr& resp) {
            if (result == ReqResult::Ok && resp) {
                if (resp->getStatusCode() == k200OK) {
                    auto body = resp->getBody();
                    size_t body_size = body.length();

                    total_bytes += body_size;
                    if (body_size < min_size) min_size = body_size;
                    if (body_size > max_size) max_size = body_size;
                } else {
                    ++failed_requests;
                    std::cerr << "Request failed with status: "
                              << resp->getStatusCode() << "\n";
                }
            } else {
                ++failed_requests;
                std::cerr << "Request failed: " << result << "\n";
            }

            if (++completed_requests % 10 == 0) {
                std::cout << "Progress: " << completed_requests << "/"
                          << request_count << " requests\r" << std::flush;
            }

            // 当所有请求完成时停止事件循环
            if (completed_requests == request_count) {
                app().getLoop()->quit();
            }
        });
    }

    // 运行事件循环
    app().run();

    auto end = high_resolution_clock::now();
    auto total_duration = duration_cast<milliseconds>(end - start).count();

    // 输出结果
    std::cout << "\n\n=== Test Results ===\n";
    std::cout << "Server: http://" << server_address << ":" << port << "\n";
    std::cout << "Total requests: " << request_count << "\n";
    std::cout << "Successful requests: " << request_count - failed_requests << "\n";
    std::cout << "Failed requests: " << failed_requests << "\n";
    std::cout << "Total data transferred: "
              << std::fixed << std::setprecision(2)
              << total_bytes / (1024.0 * 1024.0) << " MB\n";
    std::cout << "Total time: " << total_duration << " ms\n";
    std::cout << "Average time per request: "
              << total_duration / request_count << " ms\n";
    std::cout << "Throughput: "
              << std::fixed << std::setprecision(2)
              << (total_bytes / (1024.0 * 1024.0)) / (total_duration / 1000.0)
              << " MB/s\n";
    std::cout << "Min response size: " << min_size << " bytes\n";
    std::cout << "Max response size: " << max_size << " bytes\n";

    return 0;
}