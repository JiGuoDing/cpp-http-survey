#include <drogon/drogon.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <thread>
#include <vector>

using namespace drogon;
using namespace std::chrono;

static std::atomic<int> thread_count{1};

void sendRequests(const std::string& server_address, uint16_t port, int requests_per_thread,
                 size_t data_size, std::atomic<size_t>& total_bytes_sent,
                 std::atomic<size_t>& completed_requests, std::atomic<size_t>& failed_requests) {
    
    auto client = HttpClient::newHttpClient(server_address, port);
    client->setUserAgent("DrogonBenchmark/1.0");

    
    std::string large_data(data_size, 'A');

    for (int i = 0; i < requests_per_thread; ++i) {
        auto req = HttpRequest::newHttpRequest();
        req->setPath("/receive_data");
        req->setMethod(Post);
        req->setContentTypeCode(CT_TEXT_PLAIN);
        req->setBody(large_data);

        
        std::cout << "\n=== 发送请求 #" << i+1 << " ===\n";
        std::cout << "目标URL: http://" << server_address << ":" << port << "/receive_data\n";
        std::cout << "请求方法: POST\n";
        std::cout << "请求体大小: " << large_data.size() / 1024.0 << " KB\n";

        client->sendRequest(req, [&, i, requests_per_thread](ReqResult result, const HttpResponsePtr& resp) {
            
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
            std::cout << "进度: " << completed_requests << "/"
                      << (requests_per_thread * thread_count) << "\n";

            if (completed_requests == requests_per_thread * thread_count) {
                app().getLoop()->quit();
            }
        });
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <server_address> <port> <requests_per_thread> <thread_count>\n";
        std::cerr << "Example: " << argv[0] << " 127.0.0.1 8848 100 4\n";
        return 1;
    }

    const std::string server_address = argv[1];
    const uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));
    const int requests_per_thread = std::stoi(argv[3]);
    thread_count = std::stoi(argv[4]);

    // 创建30KB的测试数据
    const size_t data_size = 30 * 1024;

    std::cout << "Testing http://" << server_address << ":" << port
              << " with " << requests_per_thread << " requests per thread ("
              << thread_count << " threads, "
              << data_size / 1024.0 << "KB each)\n";

    std::atomic<size_t> total_bytes_sent{0};
    std::atomic<size_t> completed_requests{0};
    std::atomic<size_t> failed_requests{0};

    auto start = high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&]() {
            sendRequests(server_address, port, requests_per_thread, data_size,
                       total_bytes_sent, completed_requests, failed_requests);
        });
    }

    app().run();

    for (auto& t : threads) {
        t.join();
    }

    auto end = high_resolution_clock::now();
    auto total_duration = duration_cast<milliseconds>(end - start).count();

    const int total_requests = requests_per_thread * thread_count;
    const double total_seconds = total_duration / 1000.0;
    const double avg_time_per_request = total_duration / static_cast<double>(total_requests);
    const double throughput_mbps = (total_bytes_sent / (1024.0 * 1024.0)) / total_seconds;

    std::cout << "\n\n=== 最终测试结果 ===\n";
    std::cout << "服务器: http://" << server_address << ":" << port << "\n";
    std::cout << "线程数: " << thread_count << "\n";
    std::cout << "每线程请求数: " << requests_per_thread << "\n";
    std::cout << "总请求数: " << total_requests << "\n";
    std::cout << "成功请求: " << total_requests - failed_requests << "\n";
    std::cout << "失败请求: " << failed_requests << "\n";
    std::cout << "总发送数据: " << std::fixed << std::setprecision(2)
              << total_bytes_sent / 1024.0 << " KB\n";
    std::cout << "总耗时: " << total_duration << " ms\n";
    std::cout << "平均每个请求耗时: " << std::fixed << std::setprecision(2)
              << avg_time_per_request << " ms\n";
    std::cout << "上传吞吐量: " << std::fixed << std::setprecision(2)
              << throughput_mbps << " MB/s\n";

    return 0;
}