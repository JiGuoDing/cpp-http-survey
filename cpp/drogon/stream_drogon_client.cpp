#include <drogon/drogon.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <fstream>
#include <random>
#include <memory>

using namespace drogon;
using namespace std::chrono;

// 全局统计变量
struct Stats {
    std::atomic<size_t> total_bytes_sent{0};
    std::atomic<size_t> completed_uploads{0};
    std::atomic<size_t> failed_uploads{0};
    std::atomic<size_t> active_connections{0};
    size_t min_latency = SIZE_MAX;
    size_t max_latency = 0;
    size_t total_latency = 0;
    std::mutex stats_mutex;
};

// 自定义数据源类
class RandomDataSource : public drogon::DataSource
{
public:
    RandomDataSource(size_t totalSize, size_t chunkSize = 1024 * 1024)
        : totalSize_(totalSize), chunkSize_(chunkSize)
    {
        // 初始化随机数生成器
        std::random_device rd;
        gen_.reset(new std::mt19937(rd()));
        dis_.reset(new std::uniform_int_distribution<>(0, 255));
    }

    size_t read(char *buffer, size_t bufSize) override
    {
        size_t toRead = std::min(bufSize, chunkSize_);
        toRead = std::min(toRead, totalSize_ - bytesSent_);

        if (toRead == 0)
            return 0;

        // 生成随机数据
        for (size_t i = 0; i < toRead; ++i) {
            buffer[i] = static_cast<char>((*dis_)(*gen_));
        }

        bytesSent_ += toRead;
        return toRead;
    }

private:
    size_t totalSize_;
    size_t chunkSize_;
    size_t bytesSent_{0};
    std::unique_ptr<std::mt19937> gen_;
    std::unique_ptr<std::uniform_int_distribution<>> dis_;
};

int main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <server_address> <port> <duration_sec> <concurrency>\n";
        std::cerr << "Example: " << argv[0] << " 127.0.0.1 8848 30 10\n";
        return 1;
    }

    const std::string server_address = argv[1];
    const uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));
    const int test_duration = std::stoi(argv[3]);
    const int concurrency = std::stoi(argv[4]);

    std::cout << "Testing upload performance to http://" << server_address << ":" << port
              << " for " << test_duration << " seconds with " << concurrency
              << " concurrent connections\n";

    Stats stats;
    auto start_time = high_resolution_clock::now();
    auto end_time = start_time + seconds(test_duration);

    // 创建客户端
    auto client = HttpClient::newHttpClient(server_address, port);
    client->setUserAgent("DrogonUploadBenchmark/1.0");

    // 上传函数
    auto do_upload = [&]() {
        while (high_resolution_clock::now() < end_time) {
            // 每次上传1MB数据
            const size_t upload_size = 1 * 1024 * 1024;

            // 创建数据源
            auto dataSource = std::make_shared<RandomDataSource>(upload_size);

            // 创建请求
            auto req = HttpRequest::newHttpRequest();
            req->setPath("/upload");
            req->setMethod(Post);
            req->setContentTypeCode(CT_TEXT_PLAIN);

            // 设置数据源
            req->setBody(dataSource);

            auto start = high_resolution_clock::now();
            stats.active_connections++;

            try {
                client->sendRequest(req, [&, start](ReqResult result, const HttpResponsePtr& resp) {
                    auto end = high_resolution_clock::now();
                    auto latency = duration_cast<milliseconds>(end - start).count();

                    {
                        std::lock_guard<std::mutex> lock(stats.stats_mutex);
                        stats.total_latency += latency;
                        if (latency < stats.min_latency) stats.min_latency = latency;
                        if (latency > stats.max_latency) stats.max_latency = latency;
                    }

                    if (result == ReqResult::Ok && resp && resp->getStatusCode() == k200OK) {
                        stats.completed_uploads++;
                        stats.total_bytes_sent += upload_size;
                    } else {
                        stats.failed_uploads++;
                        std::cerr << "Upload failed: " << result << "\n";
                    }

                    stats.active_connections--;

                    // 每发送10MB打印一次
                    if (stats.total_bytes_sent % (10 * 1024 * 1024) == 0) {
                        std::cout << "Sent " << (stats.total_bytes_sent / (1024 * 1024))
                                  << "MB, active connections: " << stats.active_connections
                                  << "\n";
                    }
                });
            } catch (const std::exception &e) {
                std::cerr << "Exception: " << e.what() << "\n";
                stats.failed_uploads++;
                stats.active_connections--;
            }

            // 控制上传速率 (每秒约10个请求)
            std::this_thread::sleep_for(100ms);
        }
    };

    // 创建多个上传线程
    std::vector<std::thread> threads;
    for (int i = 0; i < concurrency; ++i) {
        threads.emplace_back(do_upload);
    }

    // 定期打印统计信息
    auto stats_timer = app().getLoop()->runEvery(1.0, [&]() {
        auto elapsed = duration_cast<seconds>(high_resolution_clock::now() - start_time).count();
        if (elapsed == 0) return;

        std::cout << "\n=== Current Stats ===\n";
        std::cout << "Elapsed time: " << elapsed << "s\n";
        std::cout << "Active connections: " << stats.active_connections << "\n";
        std::cout << "Completed uploads: " << stats.completed_uploads << "\n";
        std::cout << "Failed uploads: " << stats.failed_uploads << "\n";
        std::cout << "Data sent: " << std::fixed << std::setprecision(2)
                  << stats.total_bytes_sent / (1024.0 * 1024.0) << " MB\n";
        std::cout << "Upload speed: " << std::fixed << std::setprecision(2)
                  << (stats.total_bytes_sent / (1024.0 * 1024.0)) / elapsed << " MB/s\n";
        if (stats.completed_uploads > 0) {
            std::cout << "Avg latency: " << stats.total_latency / stats.completed_uploads << "ms\n";
            std::cout << "Min latency: " << stats.min_latency << "ms\n";
            std::cout << "Max latency: " << stats.max_latency << "ms\n";
        }
    });

    // 运行事件循环
    app().run();

    // 等待所有上传线程完成
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // 测试结束，输出最终结果
    auto total_duration = duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count();
    double total_seconds = total_duration / 1000.0;

    std::cout << "\n\n=== Final Test Results ===\n";
    std::cout << "Server: http://" << server_address << ":" << port << "\n";
    std::cout << "Test duration: " << std::fixed << std::setprecision(2) << total_seconds << " seconds\n";
    std::cout << "Concurrency level: " << concurrency << "\n";
    std::cout << "Completed uploads: " << stats.completed_uploads << "\n";
    std::cout << "Failed uploads: " << stats.failed_uploads << "\n";
    std::cout << "Total data sent: " << std::fixed << std::setprecision(2)
              << stats.total_bytes_sent / (1024.0 * 1024.0) << " MB\n";
    std::cout << "Average upload speed: " << std::fixed << std::setprecision(2)
              << (stats.total_bytes_sent / (1024.0 * 1024.0)) / total_seconds << " MB/s\n";
    if (stats.completed_uploads > 0) {
        std::cout << "Average upload latency: " << stats.total_latency / stats.completed_uploads << "ms\n";
        std::cout << "Minimum latency: " << stats.min_latency << "ms\n";
        std::cout << "Maximum latency: " << stats.max_latency << "ms\n";
    }

    // 保存结果到文件
    std::ofstream outfile("upload_test_results.csv");
    if (outfile.is_open()) {
        outfile << "Metric,Value\n";
        outfile << "Duration (s)," << total_seconds << "\n";
        outfile << "Concurrency," << concurrency << "\n";
        outfile << "Completed Uploads," << stats.completed_uploads << "\n";
        outfile << "Failed Uploads," << stats.failed_uploads << "\n";
        outfile << "Total Data (MB)," << std::fixed << std::setprecision(2)
                << stats.total_bytes_sent / (1024.0 * 1024.0) << "\n";
        outfile << "Upload Speed (MB/s)," << std::fixed << std::setprecision(2)
                << (stats.total_bytes_sent / (1024.0 * 1024.0)) / total_seconds << "\n";
        if (stats.completed_uploads > 0) {
            outfile << "Avg Latency (ms)," << stats.total_latency / stats.completed_uploads << "\n";
            outfile << "Min Latency (ms)," << stats.min_latency << "\n";
            outfile << "Max Latency (ms)," << stats.max_latency << "\n";
        }
        outfile.close();
        std::cout << "Results saved to upload_test_results.csv\n";
    }

    return 0;
}