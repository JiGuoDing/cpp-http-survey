#include <iostream>
#include <atomic>
#include <chrono>
#include <iomanip>
#include "../../headers/httplib.h"

std::atomic<size_t> total_bytes_sent{0};
std::atomic<size_t> completed_requests{0};
std::atomic<size_t> failed_requests{0};

void perform_test(const std::string& host, int port, int request_count, bool upload) {
    httplib::Client cli(host, port);
    constexpr  size_t data_size = 30 * 1024 * 1024; // 30MB
    const std::string large_data(data_size, 'A');

    for (int i = 0; i < request_count; ++i) {

        if (upload) {
            // 上传测试 (POST)
            auto res = cli.Post("/receive_data", large_data, "text/plain");
            if (res && res->status == 200) {
                total_bytes_sent += data_size;
                std::cout << "Upload #" << i+1 << " - " << res->body << "\n";
            } else {
                failed_requests++;
                std::cerr << "Upload failed: " << (res ? res->status : -1) << "\n";
            }
        } else {
            // 下载测试 (GET)
            std::string received_data;
            auto res = cli.Get("/large_data", [&](const char* data, size_t len) {
                received_data.append(data, len);
                return true;
            });

            if (res && res->status == 200) {
                total_bytes_sent += received_data.size();
                std::cout << "Download #" << i+1 << " - Received "
                          << received_data.size() / (1024 * 1024) << " MB\n";
            } else {
                failed_requests++;
                std::cerr << "Download failed: " << (res ? res->status : -1) << "\n";
            }
        }

        completed_requests++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <host> <port> <request_count> <upload|download>\n"
                  << "Example: " << argv[0] << " localhost 8848 10 upload\n";
        return 1;
    }

    const std::string host = argv[1];
    const int port = std::stoi(argv[2]);
    const int request_count = std::stoi(argv[3]);
    const bool upload = (std::string(argv[4]) == "upload");

    auto start = std::chrono::high_resolution_clock::now();

    // 单线程测试 (可改为多线程)
    perform_test(host, port, request_count, upload);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 打印结果
    std::cout << "\n=== Test Results ===\n"
              << "Server: " << host << ":" << port << "\n"
              << "Mode: " << (upload ? "Upload" : "Download") << "\n"
              << "Total requests: " << request_count << "\n"
              << "Successful: " << completed_requests << "\n"
              << "Failed: " << failed_requests << "\n"
              << "Total data: " << std::fixed << std::setprecision(2)
              << total_bytes_sent / (1024.0 * 1024.0) << " MB\n"
              << "Total time: " << duration << " ms\n"
              << "Throughput: " << std::setprecision(2)
              << (total_bytes_sent / (1024.0 * 1024.0)) / (duration / 1000.0) << " MB/s\n";

    return 0;
}