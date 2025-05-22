//
// Created by jgd on 25-5-22.
//
#include <iostream>
#include <atomic>
#include <chrono>
#include <mutex>
#include "../../headers/httplib.h"

constexpr size_t DATA_SIZE = 30 * 1024 * 1024; // 30MB
std::atomic<size_t> request_count{0};
std::atomic<size_t> total_received_bytes{0};
std::mutex stats_mutex;

int main() {
    httplib::Server svr;

    // 准备30MB测试数据
    const std::string large_data(DATA_SIZE, 'A');

    // 设置线程池 (建议设置为CPU核心数的2倍)
    svr.new_task_queue = [] { return new httplib::ThreadPool(32); };

    // 数据接收端点
    svr.Post("/receive_data", [&](const httplib::Request& req, httplib::Response& res) {
        const size_t body_size = req.body.size();
        const size_t count = ++request_count;
        total_received_bytes += body_size;

        {
            std::lock_guard<std::mutex> lock(stats_mutex);
            std::cout << "POST #" << count
                      << " from " << req.remote_addr << ":" << req.remote_port
                      << " - Size: " << body_size/(1024 * 1024) << "MB"
                      << " - Total: " << total_received_bytes/(1024 * 1024) << "MB\n";
        }

        res.set_content("Received " + std::to_string(body_size) + " bytes", "text/plain");
    });

    // 大文件下载端点
    svr.Get("/large_data", [&](const httplib::Request& req, httplib::Response& res) {
        const size_t count = ++request_count;

        {
            std::lock_guard<std::mutex> lock(stats_mutex);
            std::cout << "GET #" << count
                      << " from " << req.remote_addr << ":" << req.remote_port << "\n";
        }

        res.set_content(large_data, "text/plain");
    });

    // 统计信息端点
    svr.Get("/request_stats", [&](const httplib::Request&, httplib::Response& res) {
        std::string stats = "Total requests: " + std::to_string(request_count) +
                          "\nTotal received: " +
                          std::to_string(total_received_bytes/(1024 * 1024)) + " MB";
        res.set_content(stats, "text/plain");
    });

    std::cout << "Multi-threaded server running on http://0.0.0.0:8848\n"
              << "Endpoints:\n"
              << "  POST /receive_data\n"
              << "  GET  /large_data (30MB)\n"
              << "  GET  /request_stats\n";

    svr.listen("0.0.0.0", 8848);
    return 0;
}