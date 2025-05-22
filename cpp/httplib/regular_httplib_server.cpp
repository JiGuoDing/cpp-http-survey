#include <iostream>
#include <atomic>
#include <chrono>
#include "../../headers/httplib.h"

constexpr size_t DATA_SIZE = 30 * 1024 * 1024; // 30MB
std::atomic<int> request_count{0};
std::atomic<size_t> total_received_bytes{0};

int main() {
    httplib::Server svr;

    // 准备30MB测试数据
    const std::string large_data(DATA_SIZE, 'A');

    // 数据接收端点 (POST)
    svr.Post("/receive_data", [&](const httplib::Request& req, httplib::Response& res) {
        int count = ++request_count;
        size_t body_size = req.body.size();
        total_received_bytes += body_size;

        // 打印请求详情
        std::cout << "\n=== Received Request #" << count << " ==="
                  << "\nClient: " << req.remote_addr << ":" << req.remote_port
                  << "\nSize: " << body_size / (1024 * 1024) << " MB"
                  << "\nTotal Received: " << total_received_bytes / (1024 * 1024) << " MB\n";

        res.set_content("Received " + std::to_string(body_size) + " bytes", "text/plain");
    });

    // 统计信息端点 (GET)
    svr.Get("/request_stats", [&](const httplib::Request&, httplib::Response& res) {
        std::string stats = "Total requests: " + std::to_string(request_count) +
                          "\nTotal received: " + 
                          std::to_string(total_received_bytes / (1024 * 1024)) + " MB";
        res.set_content(stats, "text/plain");
    });

    // 大文件下载端点 (GET)
    svr.Get("/large_data", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(large_data, "text/plain");
    });

    std::cout << "Server running at http://localhost:8848\n"
              << "Endpoints:\n"
              << "  POST /receive_data\n"
              << "  GET  /request_stats\n"
              << "  GET  /large_data (30MB)\n";

    svr.listen("0.0.0.0", 8848);
    return 0;

}